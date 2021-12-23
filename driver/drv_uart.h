/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2021-10-16     gong.lee       first version
 */

#ifndef DRV_UART_H__
#define DRV_UART_H__

#include "rk3399.h"

#define   RK_UART_RBR(BASE)          HWREG32(BASE + 0x0000)       //Receive Holding Register
#define   RK_UART_THR(BASE)          HWREG32(BASE + 0x0000)       //Transmit Holding Register
#define   RK_UART_DLL(BASE)        	 HWREG32(BASE + 0x0000)       //Divisor Latch (Low)
#define   RK_UART_DLH(BASE)        	 HWREG32(BASE + 0x0004)       //Divisor Latch (High)
#define   RK_UART_IER(BASE)        	 HWREG32(BASE + 0x0004)       //Interrupt Enable Register 
#define   RK_UART_IRR(BASE)        	 HWREG32(BASE + 0x0008)       //Interrupt Enable Register 
#define   RK_UART_FCR(BASE)        	 HWREG32(BASE + 0x0008)       //FIFO Control Register 
#define   RK_UART_LCR(BASE)        	 HWREG32(BASE + 0x000C)       //Line Control Register
#define   RK_UART_MCR(BASE)        	 HWREG32(BASE + 0x0010)       //Modem Control Register
#define   RK_UART_LSR(BASE)        	 HWREG32(BASE + 0x0014)       //Line Status Register
#define   RK_UART_SRR(BASE)        	 HWREG32(BASE + 0x0088)       //Software Reset Register
#define   RK_UART_USR(BASE)        	 HWREG32(BASE + 0x007C)       //UART Status Register
#define   RK_UART_SFE(BASE)        	 HWREG32(BASE + 0x0098)       //Shadow FIFO Enable
#define   RK_UART_SRT(BASE)        	 HWREG32(BASE + 0x009C)       //Shadow RCVR Trigger
#define   RK_UART_STET(BASE)         HWREG32(BASE + 0x00A0)       //Shadow TX Empty Trigger

#define   RK_GRF_GPIO4B_IOMUX        HWREG32(RK_GRF_BASE + 0xe024)

int rt_hw_uart_init(void);
int debug(char c);

#endif /* DRV_UART_H__ */
