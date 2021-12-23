/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-09-15     Bernard      first version
 * 2019-07-28     zdzn         add smp support
 */

#include <rthw.h>
#include <rtthread.h>
#include <board.h>
#include "cp15.h"

int rt_hw_cpu_id(void)
{
    int cpu_id;
    rt_base_t value;

    __asm__ volatile (
            "mrs %0, mpidr_el1"
            :"=r"(value)
            );
    cpu_id = value & 0x1ff;

    switch (cpu_id) 
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
            return cpu_id;
        case 0x100:
            return 0x4;
        case 0x101:
            return 0x5;
        default:
            break;
    }
    return cpu_id;
};

#ifdef RT_USING_SMP

void rt_hw_spin_lock_init(rt_hw_spinlock_t *lock)
{
    lock->slock = 0;
}

void rt_hw_spin_lock(rt_hw_spinlock_t *lock)
{
    unsigned long newval;
    rt_hw_spinlock_t lockval;
    __asm__ __volatile__("prfm pstl1keep, %a0\n" : : "p" (&lock->slock));
    __asm__ __volatile__(
        "1: ldaxr    %w0,      %2   \n"
        "   add      %w1, %w0, %3   \n"
        "   stxr     %w1, %w1, %2   \n"
        "   cbnz     %w1,      1b"
        : "=&r" (lockval), "=&r" (newval),  "+Q" (lock->slock)
        : "I" (1 << 16)
        : "memory");

    while (lockval.tickets.next != lockval.tickets.owner) {
        __WFE();
        lockval.tickets.owner = *(volatile unsigned short *)(&lock->tickets.owner);
    }

    __DMB();
}

void rt_hw_spin_unlock(rt_hw_spinlock_t *lock)
{
    __DMB();
    lock->tickets.owner++;
    __DSB();
    __SEV();
}
#endif /*RT_USING_SMP*/

/**
 * @addtogroup ARM CPU
 */
/*@{*/

/** shutdown CPU */
RT_WEAK void rt_hw_cpu_shutdown()
{
    rt_uint32_t level;
    rt_kprintf("shutdown...\n");

    level = rt_hw_interrupt_disable();
    while (level)
    {
        RT_ASSERT(0);
    }
}

/*@}*/
