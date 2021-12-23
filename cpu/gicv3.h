/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-07-20     Bernard      first version
 */

#ifndef __GIC_V3_H__
#define __GIC_V3_H__

#include <rthw.h>
#include <board.h>

/* necessary for pre-processor */
#define STRINGIFY(s)    tostring(s)
#define tostring(s) #s

#define __get_gicv3_reg(CR, Rt)						\
({													\
	__asm__ volatile ("mrs %0, " STRINGIFY(CR)			\
			  : "=r" (Rt) :: "memory");			\
})

#define __set_gicv3_reg(CR, Rt)						\
({									\
	__asm__ volatile ("msr " STRINGIFY(CR) ", %0"			\
		         :: "r" (Rt) );			\
})

/* necessary for pre-processor */
#define stringify(s)    tostring(s)
#define tostring(s) #s

/* System register interface to GICv3 */
#define ICC_IAR0_EL1        S3_0_C12_C8_0
#define ICC_IAR1_EL1        S3_0_C12_C12_0
#define ICC_EOIR0_EL1       S3_0_C12_C8_1
#define ICC_EOIR1_EL1       S3_0_C12_C12_1
#define ICC_HPPIR0_EL1      S3_0_C12_C8_2
#define ICC_HPPIR1_EL1      S3_0_C12_C12_2
#define ICC_BPR0_EL1        S3_0_C12_C8_3
#define ICC_BPR1_EL1        S3_0_C12_C12_3
#define ICC_DIR_EL1         S3_0_C12_C11_1
#define ICC_PMR_EL1         S3_0_C4_C6_0
#define ICC_RPR_EL1         S3_0_C12_C11_3
#define ICC_CTLR_EL1        S3_0_C12_C12_4
#define ICC_CTLR_EL3        S3_6_C12_C12_4
#define ICC_SRE_EL1         S3_0_C12_C12_5
#define ICC_SRE_EL2         S3_4_C12_C9_5
#define ICC_SRE_EL3         S3_6_C12_C12_5
#define ICC_IGRPEN0_EL1     S3_0_C12_C12_6
#define ICC_IGRPEN1_EL1     S3_0_C12_C12_7
#define ICC_IGRPEN1_EL3     S3_6_C12_C12_7
#define ICC_SEIEN_EL1       S3_0_C12_C13_0
#define ICC_SGI0R_EL1       S3_0_C12_C11_7
#define ICC_SGI1R_EL1       S3_0_C12_C11_5
#define ICC_ASGI1R_EL1      S3_0_C12_C11_6

#define __REG32(x) (*((volatile unsigned int*)((rt_uint64_t)x)))



#define ROUTED_TO_ALL (1)
#define ROUTED_TO_SPEC (0)

/** Macro to access the Distributor Control Register (GICD_CTLR)
*/
#define GICD_CTLR_RWP (1<<31)
#define GICD_CTLR_E1NWF (1<<7)
#define GICD_CTLR_DS (1<<6)
#define GICD_CTLR_ARE_NS (1<<5)
#define GICD_CTLR_ARE_S (1<<4)
#define GICD_CTLR_ENGRP1S (1<<2)
#define GICD_CTLR_ENGRP1NS (1<<1)
#define GICD_CTLR_ENGRP0 (1<<0)

/** Macro to access the Redistributor Control Register (GICR_CTLR)
*/
#define GICR_CTLR_UWP (1<<31)
#define GICR_CTLR_DPG1S (1<<26)
#define GICR_CTLR_DPG1NS (1<<25)
#define GICR_CTLR_DPG0 (1<<24)
#define GICR_CTLR_RWP (1<<3)
#define GICR_CTLR_IR (1<<2)
#define GICR_CTLR_CES (1<<1)
#define GICR_CTLR_EnableLPI (1<<0)

/** Macro to access the Generic Interrupt Controller Interface (GICC)
*/
#define GIC_CPU_CTRL(hw_base)               __REG32((hw_base) + 0x00U)
#define GIC_CPU_PRIMASK(hw_base)            __REG32((hw_base) + 0x04U)
#define GIC_CPU_BINPOINT(hw_base)           __REG32((hw_base) + 0x08U)
#define GIC_CPU_INTACK(hw_base)             __REG32((hw_base) + 0x0cU)
#define GIC_CPU_EOI(hw_base)                __REG32((hw_base) + 0x10U)
#define GIC_CPU_RUNNINGPRI(hw_base)         __REG32((hw_base) + 0x14U)
#define GIC_CPU_HIGHPRI(hw_base)            __REG32((hw_base) + 0x18U)
#define GIC_CPU_IIDR(hw_base)               __REG32((hw_base) + 0xFCU)

/** Macro to access the Generic Interrupt Controller Distributor (GICD)
*/
#define GIC_DIST_CTRL(hw_base)              __REG32((hw_base) + 0x000U)
#define GIC_DIST_TYPE(hw_base)              __REG32((hw_base) + 0x004U)
#define GIC_DIST_IGROUP(hw_base, n)         __REG32((hw_base) + 0x080U + ((n)/32U) * 4U)
#define GIC_DIST_ENABLE_SET(hw_base, n)     __REG32((hw_base) + 0x100U + ((n)/32U) * 4U)
#define GIC_DIST_ENABLE_CLEAR(hw_base, n)   __REG32((hw_base) + 0x180U + ((n)/32U) * 4U)
#define GIC_DIST_PENDING_SET(hw_base, n)    __REG32((hw_base) + 0x200U + ((n)/32U) * 4U)
#define GIC_DIST_PENDING_CLEAR(hw_base, n)  __REG32((hw_base) + 0x280U + ((n)/32U) * 4U)
#define GIC_DIST_ACTIVE_SET(hw_base, n)     __REG32((hw_base) + 0x300U + ((n)/32U) * 4U)
#define GIC_DIST_ACTIVE_CLEAR(hw_base, n)   __REG32((hw_base) + 0x380U + ((n)/32U) * 4U)
#define GIC_DIST_PRI(hw_base, n)            __REG32((hw_base) + 0x400U + ((n)/4U) * 4U)
#define GIC_DIST_TARGET(hw_base, n)         __REG32((hw_base) + 0x800U + ((n)/4U) * 4U)
#define GIC_DIST_CONFIG(hw_base, n)         __REG32((hw_base) + 0xc00U + ((n)/16U) * 4U)
#define GIC_DIST_SOFTINT(hw_base)           __REG32((hw_base) + 0xf00U)
#define GIC_DIST_CPENDSGI(hw_base, n)       __REG32((hw_base) + 0xf10U + ((n)/4U) * 4U)
#define GIC_DIST_SPENDSGI(hw_base, n)       __REG32((hw_base) + 0xf20U + ((n)/4U) * 4U)
#define GIC_DIST_ICPIDR2(hw_base)           __REG32((hw_base) + 0xfe8U)
#define GIC_DIST_IROUTER_LOW(hw_base, n)    __REG32((hw_base) + 0x6000U + (n)*8U)
#define GIC_DIST_IROUTER_HIGH(hw_base, n)   __REG32((hw_base) + 0x6000U + (n)*8U + 4)

/* SGI base address  is at 64K offset from Redistributor base address */
#define GIC_RSGI_OFFSET 0x10000

/** Macro to access the Generic Interrupt Controller Redistributor (GICD)
*/
#define GIC_RDIST_CTRL(hw_base)              __REG32((hw_base) + 0x000U)
#define GIC_RDIST_IIDR(hw_base)              __REG32((hw_base) + 0x004U)
#define GIC_RDIST_TYPER(hw_base)             __REG32((hw_base) + 0x008U)
#define GIC_RDIST_TSTATUSR(hw_base)          __REG32((hw_base) + 0x010U)
#define GIC_RDIST_WAKER(hw_base)             __REG32((hw_base) + 0x014U)
#define GIC_RDIST_SETLPIR(hw_base)           __REG32((hw_base) + 0x040U)
#define GIC_RDIST_CLRLPIR(hw_base)           __REG32((hw_base) + 0x048U)
#define GIC_RDIST_PROPBASER(hw_base)         __REG32((hw_base) + 0x070U)
#define GIC_RDIST_PENDBASER(hw_base)         __REG32((hw_base) + 0x078U)
#define GIC_RDIST_INVLPIR(hw_base)           __REG32((hw_base) + 0x0A0U)
#define GIC_RDIST_INVALLR(hw_base)           __REG32((hw_base) + 0x0B0U)
#define GIC_RDIST_SYNCR(hw_base)             __REG32((hw_base) + 0x0C0U)

#define GIC_RDISTSGI_IGROUPR0(hw_base, n)    __REG32((hw_base) + GIC_RSGI_OFFSET + 0x080U + (n)*4U)
#define GIC_RDISTSGI_ISENABLER0(hw_base)     __REG32((hw_base) + GIC_RSGI_OFFSET + 0x100U)
#define GIC_RDISTSGI_ICENABLER0(hw_base)     __REG32((hw_base) + GIC_RSGI_OFFSET + 0x180U)
#define GIC_RDISTSGI_ISPENDR0(hw_base)       __REG32((hw_base) + GIC_RSGI_OFFSET + 0x200U)
#define GIC_RDISTSGI_ICPENDR0(hw_base)       __REG32((hw_base) + GIC_RSGI_OFFSET + 0x280U)
#define GIC_RDISTSGI_ISACTIVER0(hw_base)     __REG32((hw_base) + GIC_RSGI_OFFSET + 0x300U)
#define GIC_RDISTSGI_ICACTIVER0(hw_base)     __REG32((hw_base) + GIC_RSGI_OFFSET + 0x380U)
#define GIC_RDISTSGI_IPRIORITYR(hw_base, n)  __REG32((hw_base) + GIC_RSGI_OFFSET + 0x400U + ((n) / 4U) * 4U)
#define GIC_RDISTSGI_ICFGR0(hw_base)         __REG32((hw_base) + GIC_RSGI_OFFSET + 0xC00U)
#define GIC_RDISTSGI_ICFGR1(hw_base)         __REG32((hw_base) + GIC_RSGI_OFFSET + 0xC04U)
#define GIC_RDISTSGI_IGRPMODR0(hw_base, n)   __REG32((hw_base) + GIC_RSGI_OFFSET + 0xD00U + (n)*4)
#define GIC_RDISTSGI_NSACR(hw_base)          __REG32((hw_base) + GIC_RSGI_OFFSET + 0xE00U)

#define GIC_RSGI_AFF1_OFFSET    16
#define GIC_RSGI_AFF2_OFFSET    32
#define GIC_RSGI_AFF3_OFFSET    48

rt_uint32_t arm_gic_cpumask_to_affval(rt_uint32_t *cpu_mask, rt_uint32_t *cluster_id, rt_uint32_t *target_list);
rt_uint64_t get_main_cpu_affval(void);
int arm_gic_get_active_irq(rt_uint32_t index);
void arm_gic_ack(rt_uint32_t index, int irq);

void arm_gic_mask(rt_uint32_t index, int irq);
void arm_gic_umask(rt_uint32_t index, int irq);

rt_uint32_t arm_gic_get_pending_irq(rt_uint32_t index, int irq);
void arm_gic_set_pending_irq(rt_uint32_t index, int irq);
void arm_gic_clear_pending_irq(rt_uint32_t index, int irq);

void arm_gic_set_configuration(rt_uint32_t index, int irq, rt_uint32_t config);
rt_uint32_t arm_gic_get_configuration(rt_uint32_t index, int irq);

void arm_gic_clear_active(rt_uint32_t index, int irq);

void arm_gic_set_cpu(rt_uint32_t index, int irq, unsigned int cpumask);
rt_uint32_t arm_gic_get_target_cpu(rt_uint32_t index, int irq);

void arm_gic_set_priority(rt_uint32_t index, int irq, rt_uint32_t priority);
rt_uint32_t arm_gic_get_priority(rt_uint32_t index, int irq);

void arm_gic_set_interface_prior_mask(rt_uint32_t index, rt_uint32_t priority);
rt_uint32_t arm_gic_get_interface_prior_mask(rt_uint32_t index);

void arm_gic_set_binary_point(rt_uint32_t index, rt_uint32_t binary_point);
rt_uint32_t arm_gic_get_binary_point(rt_uint32_t index);

rt_uint32_t arm_gic_get_irq_status(rt_uint32_t index, int irq);

void arm_gic_send_affinity_sgi(rt_uint32_t index, int irq, rt_uint32_t cpu_mask, rt_uint32_t routing_mode);
rt_uint32_t arm_gic_get_high_pending_irq(rt_uint32_t index);

rt_uint32_t arm_gic_get_interface_id(rt_uint32_t index);

void arm_gic_set_group(rt_uint32_t index, int irq, rt_uint32_t group);
rt_uint32_t arm_gic_get_group(rt_uint32_t index, int irq);

int arm_gic_redist_address_set(rt_uint32_t index, rt_uint32_t redist_addr, rt_uint32_t cpu_id);
int arm_gic_cpu_interface_address_set(rt_uint32_t index, rt_uint32_t interface_addr, rt_uint32_t cpu_id);
int arm_gic_dist_init(rt_uint32_t index, rt_uint32_t dist_base, int irq_start);
int arm_gic_cpu_init(rt_uint32_t index);
int arm_gic_redist_init(rt_uint32_t index);

void arm_gic_dump_type(rt_uint32_t index);
void arm_gic_dump(rt_uint32_t index);

void arm_gic_set_system_register_enable_mask(rt_uint32_t index, rt_uint32_t value);
rt_uint32_t arm_gic_get_system_register_enable_mask(rt_uint32_t index);
void arm_gic_secondary_cpu_init(void);
#endif

