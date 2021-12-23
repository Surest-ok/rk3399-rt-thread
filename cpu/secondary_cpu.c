/*
 * @ : Copyright (c) 2020 Phytium Information Technology, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0.
 *
 * @Date: 2021-05-26 10:09:45
 * @LastEditTime: 2021-08-16 16:32:08
 * @Description:  This files is for
 *
 * @Modify History:
 *  Ver   Who        Date         Changes
 * ----- ------     --------    --------------------------------------
 */

#include <rtthread.h>
#include "board.h"
#include "gicv3.h"
#include "cp15.h"
#include "mmu.h"
#include "interrupt.h"

#ifdef RT_USING_SMP
volatile unsigned int cpu_on[RT_CPUS_NR] = { 0 }; 
extern void _start();
extern void __asm_flush_dcache_all();
extern void __asm_invalidate_icache_all();
struct arm_smccc_res {
    rt_uint64_t  SMCCC_ulArg0;
    rt_uint64_t  SMCCC_ulArg1;
    rt_uint64_t  SMCCC_ulArg2;
    rt_uint64_t  SMCCC_ulArg3;
};
void arm_smccc_smc(unsigned long a0, unsigned long a1,
			    unsigned long a2, unsigned long a3,
			    unsigned long a4, unsigned long a5,
			    unsigned long a6, unsigned long a7,
			    struct arm_smccc_res *res);
                
#define PSCI_CPUON_NUM 0x84000003
#define PSCI_RESET_NUM 0x84000009

extern int rt_hw_timer_init(void);
extern void secondary_cpu_start(void);
void rt_hw_secondary_cpu_up(void)
{

    rt_uint32_t i;
    rt_uint32_t cpu_mask = 0;
    struct arm_smccc_res res;
    rt_kprintf("rt_hw_secondary_cpu_up is processing \r\n");

    for (i = 1; i < 6; i++)
    {
        if (i == 1)
        {
            /* code */
            arm_smccc_smc(0x84000003, 0x1, (unsigned long) _start, 0, 0, 0, 0, 0, &res);
            cpu_mask = 0x02;
        }
        else if (i == 2)
        {
            arm_smccc_smc(0x84000003, 0x2, (unsigned long) _start, 0, 0, 0, 0, 0, &res);
            cpu_mask = 0x04;
        }
        else if (i == 3)
        {
            arm_smccc_smc(0x84000003, 0x3, (unsigned long) _start, 0, 0, 0, 0, 0, &res);
            cpu_mask = 0x08;
        } 
        else if (i == 4) 
        {
            arm_smccc_smc(0x84000003, 0x100, (unsigned long) _start, 0, 0, 0, 0, 0, &res);
            cpu_mask = 0x10;
        } 
        else if (i == 5) 
        {
            arm_smccc_smc(0x84000003, 0x101, (unsigned long) _start, 0, 0, 0, 0, 0, &res);
            cpu_mask = 0x11;
        }
        else
        {
            continue;
        }

        do {
              __WFE();

            } while (cpu_on[i] != 0xdeadbeef);
 
        __DSB();
        rt_hw_ipi_send(RT_SCHEDULE_IPI, cpu_mask);

    }
    rt_kprintf("rt_hw_secondary_cpu_up end \r\n");
}

void secondary_cpu_c_start(void)
{
    uint32_t id;
    mmu_init();
    mmu_enable();

    __asm_flush_dcache_all();

    id = rt_hw_cpu_id();
    rt_kprintf("cpu = 0x%08x\n",id);

    rt_hw_vector_init();

    rt_hw_spin_lock(&_cpus_lock);

    arm_gic_secondary_cpu_init();
    rt_hw_timer_init();
  
    rt_hw_interrupt_set_priority(RT_SCHEDULE_IPI, 16);
    rt_hw_interrupt_umask(RT_SCHEDULE_IPI);

    cpu_on[id] = 0xdeadbeef;

    __SEV();

    rt_kprintf("cpu %d startup.\n",id);

    rt_system_scheduler_start();

}

void rt_hw_secondary_cpu_idle_exec(void)
{
    __WFE();
}

#endif
