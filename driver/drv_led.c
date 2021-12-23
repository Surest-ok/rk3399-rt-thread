/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2021-10-16     gong.lee       first version
 */
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_led.h"

void rk_led_delay(void)
{
    volatile unsigned long int i, j;
    for(i = 20; i > 0; i--)
        for(j = 10000; j > 0; j--);
}

/***************************************************
WORK_LED     BLUE    GPIO2_D3   light with High level
DIY_LED      YELLOW  GPIO0_B5   light with High level
*****************************************************/
void rk_led_mode(int mode)
{
    rt_ubase_t gpio0_base = RK_GPIO0_BASE;
    rt_ubase_t gpio2_base = RK_GPIO2_BASE;  

    RK_GPIO_SWPORTA_DDR(gpio0_base) |=  (0x01 << (1 * 8 + 5));
    RK_GPIO_SWPORTA_DDR(gpio2_base )|=  (0x01 << (3 * 8 + 3));

    RK_GPIO_SWPORTA_DR(gpio0_base)  &= ~(0x01 << (1 * 8 + 5));
    RK_GPIO_SWPORTA_DR(gpio2_base)  &= ~(0x01 << (3 * 8 + 3));

    if(0 == mode)
    {
        RK_GPIO_SWPORTA_DR(gpio0_base)  &= ~(0x01 << (1 * 8 + 5));
        RK_GPIO_SWPORTA_DR(gpio2_base)  &= ~(0x01 << (3 * 8 + 3));
    }
    else if(1 == mode)
    {
        RK_GPIO_SWPORTA_DR(gpio0_base)  |=  (0x01 << (1 * 8 + 5));
    }
    else if(2 == mode)
    {
        RK_GPIO_SWPORTA_DR(gpio2_base)  |=  (0x01 << (3 * 8 + 3));
    }
    else if(3 == mode)
    {
        RK_GPIO_SWPORTA_DR(gpio0_base)  |=  (0x01 << (1 * 8 + 5));
        RK_GPIO_SWPORTA_DR(gpio2_base)  |=  (0x01 << (3 * 8 + 3));
    }

}








