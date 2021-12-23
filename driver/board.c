/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-16     bigmagic       first version
 */

#include <rthw.h>
#include <rtthread.h>
#include "rtconfig.h"
#include "board.h"
#include "drv_uart.h"
#include "drv_timer.h"
#include "drv_led.h"

#include "cp15.h"
#include "mmu.h"
#include "interrupt.h"
#include "gicv3.h"

int rt_hw_get_gtimer_frq(void);
void rt_hw_set_gtimer_val(rt_uint64_t value);
int rt_hw_get_gtimer_val(void);
int rt_hw_get_cntpct_val(void);
int rt_hw_get_cntpct_val(void);
void rt_hw_gtimer_enable(void);
void rt_hw_gtimer_clear(void);

void rt_tick_set_period(void)
{
    rt_hw_gtimer_clear();
    volatile rt_uint32_t timerFreq;
    volatile rt_uint64_t timerCnt;
    timerFreq = rt_hw_get_gtimer_frq();
    timerCnt =  timerFreq / RT_TICK_PER_SECOND;
    rt_hw_set_gtimer_val(timerCnt);
    rt_hw_gtimer_enable();
}

void rt_hw_timer_isr(int vector, void *parameter)
{
#ifndef RT_USING_SMP
    rt_ubase_t base = RK_TIMER3_BASE;
    RK_TIMER_INIT_STATUS(base) = 0x01;  //clrear interrupt
#else
    rt_tick_set_period();
#endif
    rt_tick_increase();
}

void rt_hw_timer_init(void)
{
#ifndef RT_USING_SMP
    rt_ubase_t base = RK_TIMER3_BASE;

    rt_hw_interrupt_install(RK_TIMER3_IRQ, rt_hw_timer_isr, RT_NULL, "tick");

    RK_TIMER_LOAD_COUNT0(base) = RK_TIMER_CLK / RT_TICK_PER_SECOND;
    RK_TIMER_CUR_VALUE0(base)  = RK_TIMER_CLK / RT_TICK_PER_SECOND;
    RK_TIMER_CTRL_REG(base)    = 0x05;          //auto reload & enable the timer

    rt_hw_interrupt_umask(RK_TIMER3_IRQ);
#else
    rt_hw_interrupt_install(30, rt_hw_timer_isr, RT_NULL, "tick");
    rt_tick_set_period();
    rt_hw_interrupt_umask(30);
#endif
}
void idle_wfi(void)
{
    asm volatile ("wfi");
}

#ifdef RT_USING_SMP
extern void rt_hw_ipi_handler_install(int ipi_vector, rt_isr_handler_t ipi_isr_handler);

void ipi_handler()
{
    rt_scheduler_ipi_handler(RT_SCHEDULE_IPI, RT_NULL);
}
#endif

/**
 *  Initialize the Hardware related stuffs. Called from rtthread_startup()
 *  after interrupt disabled.
 */
void rt_hw_board_init(void)
{
    rt_uint32_t  redist_base;
    mmu_init();
    armv8_map(0, 0, 0x80000000, MEM_ATTR_MEMORY);
    armv8_map(0xFF770000, 0xFF770000, 0x10000000,  MEM_ATTR_IO);         //GRF BASE
    armv8_map(0xFF180000, 0xFF180000, 0x2000000,   MEM_ATTR_IO);         //UART BASE
    armv8_map(0xFF850000, 0xFF850000, 0x1000000,   MEM_ATTR_IO);         //GRF BASE
    armv8_map(0xFEE00000, 0xFEE00000, 0x10000000,  MEM_ATTR_IO);         //GIC 
    mmu_enable(); 

    /* initialize hardware interrupt */
    rt_hw_vector_init();   

    redist_base = platform_get_gic_redist_base();

    arm_gic_redist_address_set(0, redist_base, 0);
    arm_gic_redist_address_set(0, redist_base + (2U << 16), 1);
    arm_gic_redist_address_set(0, redist_base + 2 * (2U << 16), 2);
    arm_gic_redist_address_set(0, redist_base + 3 * (2U << 16), 3);
    arm_gic_redist_address_set(0, redist_base + 4 * (2U << 16), 4);
    arm_gic_redist_address_set(0, redist_base + 5 * (2U << 16), 5);

    rt_hw_interrupt_init(); 

    /* initialize uart */
    rt_hw_uart_init();
   
#ifdef RT_USING_CONSOLE
    /* set console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif /* RT_USING_CONSOLE */


#ifdef RT_USING_HEAP
    /* initialize memory system */
    rt_kprintf("heap: 0x%08x - 0x%08x\n", RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
    rt_system_heap_init(RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
#endif

#ifdef RT_USING_SMP
    /* install IPI handle */
    rt_hw_interrupt_set_priority(RT_SCHEDULE_IPI, 16);
    rt_hw_ipi_handler_install(RT_SCHEDULE_IPI, rt_scheduler_ipi_handler);
    rt_hw_interrupt_umask(RT_SCHEDULE_IPI);
#endif
        /* initialize timer for os tick */
    rt_hw_timer_init();

    rt_thread_idle_sethook(idle_wfi);

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

}

void rk_show()
{
    rt_uint64_t timerStep;
    rt_uint64_t timerFreq;
    rt_uint64_t timerCnt;
    timerFreq = rt_hw_get_gtimer_frq();
    timerStep = rt_hw_get_cntpct_val();
    asm volatile("mrs %0, CNTP_CVAL_EL0" : "=r" (timerCnt));
    rt_kprintf("timerCnt: 0x%x timerStep: 0x%x timerFreq: 0x%x\n", timerCnt, timerStep, timerFreq);
}
MSH_CMD_EXPORT(rk_show, show timer status);
