#ifndef __RK3399_H__
#define __RK3399_H__

#define    ARM_GIC_NR_IRQS 256
#define    ARM_GIC_MAX_NR   1

#define    RK_UART_CLK                  (24000000)
#define    RK_TIMER_CLK                 (24000000)

#define    RK_TIMER3_IRQ                (116)
#define    RK_UART2_IRQ                 (132)

#define    RK_TIMER_BASE                0xFF850000
#define    RK_TIMER3_BASE               (RK_TIMER_BASE + 0x20 * 3)

#define    RK_GPIO0_BASE                0xFF720000
#define    RK_GPIO1_BASE                0xFF730000
#define    RK_GPIO2_BASE                0xFF780000
#define    RK_GPIO3_BASE                0xFF788000
#define    RK_GPIO4_BASE                0xFF790000

#define    RK_GRF_BASE                  0xFF770000

#define    RK_UART0_BASE                0xFF180000
#define    RK_UART1_BASE                0xFF190000
#define    RK_UART2_BASE                0xFF1A0000
#define    RK_UART3_BASE                0xFF1B0000
#define    RK_UART4_BASE                0xFF370000

#define    GIC_500_DISTRIBUTOR_PPTR     0xFEE00000
#define    GIC_500_REDISTRIBUTOR_PPTR   0xFEF00000
#define    GIC_500_CONTROLLER_PPTR      0xFFF00000

rt_inline rt_uint32_t platform_get_gic_redist_base(void)
{
    return GIC_500_REDISTRIBUTOR_PPTR;
}

rt_inline rt_uint32_t platform_get_gic_dist_base(void)
{
    return GIC_500_DISTRIBUTOR_PPTR;
}

rt_inline rt_uint32_t platform_get_gic_cpu_base(void)
{
    return GIC_500_CONTROLLER_PPTR;
}
#endif
