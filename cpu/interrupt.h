/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-16     bigmagic       first version
 */

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <rthw.h>
#include <board.h>

#define INT_IRQ     0x00
#define INT_FIQ     0x01

void rt_hw_interrupt_init(void);
void rt_hw_interrupt_mask(int vector);
void rt_hw_interrupt_umask(int vector);
void rt_hw_interrupt_ack(int vector);
int rt_hw_interrupt_get_irq(void);
void rt_hw_interrupt_set_priority(int vector, unsigned int priority);
rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name);

#endif
