/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2021-10-16     gong.lee       first version
 */
#ifndef   __DRV_LED_H__
#define   __DRV_LED_H__

#include  "rk3399.h"

#define   RK_GPIO_SWPORTA_DR(BASE)           HWREG32(BASE + 0x0000)      //0x0000 Port A data register
#define   RK_GPIO_SWPORTA_DDR(BASE)          HWREG32(BASE + 0x0004)      //0x0004 Port A data direction register
#define   RK_GPIO_INT_ENABLE(BASE)        	 HWREG32(BASE + 0x0030)      //0x0030 Interrupt enable register
#define   RK_GPIO_INT_MASK(BASE)        	 HWREG32(BASE + 0x0034)      //0x0034 Interrupt mask register
#define   RK_GPIO_INT_TYPELEVEL(BASE)        HWREG32(BASE + 0x0038)      //0x0038 Interrupt level register
#define   RK_GPIO_INT_POLARITY(BASE)         HWREG32(BASE + 0x003C)      //0x003C Interrupt polarity register
#define   RK_GPIO_INT_STATUS(BASE)        	 HWREG32(BASE + 0x0040)      //0x0040 Interrupt status of port A
#define   RK_GPIO_INT_RAWSTATUS(BASE)        HWREG32(BASE + 0x0044)      //0x0044 Raw Interrupt status of port A
#define   RK_GPIO_DEBOUNCE(BASE)        	 HWREG32(BASE + 0x0048)      //0x0048 Debounce enable register
#define   RK_GPIO_PORTA_EOI(BASE)        	 HWREG32(BASE + 0x004C)      //0x004c Port A clear interrupt register
#define   RK_GPIO_EXT_PORTA(BASE)        	 HWREG32(BASE + 0x0050)      //0x0050 Debounce enable register
#define   RK_GPIO_LS_SYNC(BASE)        	     HWREG32(BASE + 0x0060)      //0x0060 Level_sensitive synchronization enable register

void rk_led_mode(int mode);
void rk_led_delay(void);

#endif

