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

#include "drv_uart.h"

struct hw_uart_device
{
    rt_ubase_t  hw_base;
    rt_uint32_t irqno;
};

static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    volatile unsigned long rate;
    unsigned long baudrate = 1500000;

    rt_ubase_t base;
    struct hw_uart_device *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;
    
    base = uart->hw_base;
    
    switch (base)
    {
    case RK_UART2_BASE:
        RK_GRF_GPIO4B_IOMUX = (3 << 18) | (3 << 16) | (2 << 2) | (2 << 0);
    break;
    default:
        return (RT_EINVAL);
    }
   
    /* UART reset, rx fifo & tx fifo reset */
    RK_UART_SRR(base) = (0x01 << 1) | (0x01 << 1) | (0x01 << 2);

    RK_UART_LCR(base) |= (0x01 << 7);            // enable access LCR

    /* uart rate is div for 24M input clock */
    rate = RK_UART_CLK / 16 / baudrate;

    RK_UART_DLL(base) = (rate & 0xff);
    RK_UART_DLH(base) = ((rate >> 8) & 0xff);

    RK_UART_LCR(base) &= ~(0x03 << 0);
    RK_UART_LCR(base) |=  (0x03 << 0);           //8bits

    RK_UART_LCR(base) &= ~(0x01 << 3);           //parity disabled

    RK_UART_LCR(base) &= ~(0x01 << 2);           //1 stop bit

    RK_UART_FCR(base) = (0x2 << 6) | (0X1 << 0);

    RK_UART_LCR(base) &= ~(0x01 << 7);          // disable access LCR

    RK_UART_MCR(base) = 0x00;

    RK_UART_IER(base) = 0x01;                   // rx interrupt enable

    return RT_EOK;
}

static rt_err_t uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct hw_uart_device *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;

    switch (cmd)
    {
        case RT_DEVICE_CTRL_CLR_INT:
            /* disable rx irq */
            rt_hw_interrupt_mask(uart->irqno);
            break;

        case RT_DEVICE_CTRL_SET_INT:
            /* enable rx irq */
            rt_hw_interrupt_umask(uart->irqno);
            break;
    }

    return RT_EOK;
}

static int uart_putc(struct rt_serial_device *serial, char c)
{
    rt_ubase_t base;
    struct hw_uart_device *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;
    base = uart->hw_base;

    while((RK_UART_USR(base) & (0x01 << 1)) == 0);

    RK_UART_THR(base) = c;

    return RT_EOK;
}

static int uart_getc(struct rt_serial_device *serial)
{
    rt_int8_t  ch = -1;
    rt_ubase_t base;
    struct hw_uart_device *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;
    base = uart->hw_base;

    if((RK_UART_LSR(base) & (0x01 << 0))) {
        ch = RK_UART_RBR(base) & 0xff;
    }
    return ch;
}

static const struct rt_uart_ops _uart_ops =
{
    uart_configure,
    uart_control,
    uart_putc,
    uart_getc,
};

static void rt_hw_uart_isr(int irqno, void *param)
{
    rt_ubase_t base;
    struct hw_uart_device *uart;
    struct rt_serial_device *serial = (struct rt_serial_device*)param;

    RT_ASSERT(serial != RT_NULL);

    uart = (struct hw_uart_device *)serial->parent.user_data;
    base = uart->hw_base;
    rt_uint32_t pending = RK_UART_IRR(base);
    
    if((pending & 0x4) == 0x4) {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }
}


/* UART device driver structure */
static struct hw_uart_device _uart2_device =
{
    RK_UART2_BASE,
    RK_UART2_IRQ,
};

static struct rt_serial_device _serial;

int debug(char c)
{
    rt_ubase_t base = RK_UART2_BASE;

    while((RK_UART_USR(base) & (0x01 << 1)) == 0);

    RK_UART_THR(base) = c;

    return RT_EOK;
}

int rt_hw_uart_init(void)
{
    struct hw_uart_device  *uart;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    uart = &_uart2_device;

    _serial.ops    = &_uart_ops;
    _serial.config = config;

    /* register UART1 device */
    rt_hw_serial_register(&_serial, "uart",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                          uart);

    rt_hw_interrupt_install(uart->irqno, rt_hw_uart_isr, &_serial, "uart");

    return 0;
}