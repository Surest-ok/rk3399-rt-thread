/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-16     bigmagic       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <board.h>
#include "drv_led.h"

static rt_hw_spinlock_t _print_lock;

void led_entry()
{
    int tick = 0;

    while (1) {
        tick ++;
        if (tick  > 2) {
            rk_led_mode(3);

        } else {
            rk_led_mode(0);
        }
        tick = tick > 4 ? 0 : tick;
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
 
}

void sio0_entry()
{
    while (1) {
        rt_hw_spin_lock(&_print_lock);
        rt_kprintf("this is sio_entry0 on cpu:%d\r\n", rt_hw_cpu_id());
        rt_hw_spin_unlock(&_print_lock);
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
 
}

void sio1_entry()
{
    while (1) {
        rt_hw_spin_lock(&_print_lock);
        rt_kprintf("this is sio_entry1 on cpu:%d\r\n", rt_hw_cpu_id());
        rt_hw_spin_unlock(&_print_lock);
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
 
}

void sio2_entry()
{
    while (1) {
        rt_hw_spin_lock(&_print_lock);
        rt_kprintf("this is sio_entry2 on cpu:%d\r\n", rt_hw_cpu_id());
        rt_hw_spin_unlock(&_print_lock);
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
 
}

void sio3_entry()
{
    while (1) {
        rt_hw_spin_lock(&_print_lock);
        rt_kprintf("this is sio_entry3 on cpu:%d\r\n", rt_hw_cpu_id());
        rt_hw_spin_unlock(&_print_lock);
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
 
}

int main(int argc, char** argv)
{
    rt_uint8_t  cpu;
    rt_thread_t tid;

    rt_kprintf("Hi, this is RT-Thread!!\n");
    rt_thread_delay(RT_TICK_PER_SECOND);

    rt_hw_spin_lock_init(&_print_lock);



    tid = rt_thread_create("led", led_entry, RT_NULL,
                           RT_MAIN_THREAD_STACK_SIZE, RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(tid != RT_NULL);

    cpu = 5;
    rt_thread_control(tid, RT_THREAD_CTRL_BIND_CPU, (void *)cpu);
    rt_thread_startup(tid);

    
    tid = rt_thread_create("test0", sio0_entry, RT_NULL,
                           RT_MAIN_THREAD_STACK_SIZE, 9, 20);
    RT_ASSERT(tid != RT_NULL);
    cpu = 4;
    rt_thread_control(tid, RT_THREAD_CTRL_BIND_CPU, (void *)cpu);

    rt_thread_startup(tid);

    tid = rt_thread_create("test1", sio1_entry, RT_NULL,
                           RT_MAIN_THREAD_STACK_SIZE, 13, 20);
    RT_ASSERT(tid != RT_NULL);

    cpu = 1;
    rt_thread_control(tid, RT_THREAD_CTRL_BIND_CPU, (void *)cpu);
    rt_thread_startup(tid);

    tid = rt_thread_create("test2", sio2_entry, RT_NULL,
                           RT_MAIN_THREAD_STACK_SIZE, 15, 20);
    RT_ASSERT(tid != RT_NULL);
    cpu = 2;
    rt_thread_control(tid, RT_THREAD_CTRL_BIND_CPU, (void *)cpu);
    rt_thread_startup(tid);
   
    tid = rt_thread_create("test3", sio3_entry, RT_NULL,
                           RT_MAIN_THREAD_STACK_SIZE, RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(tid != RT_NULL);
    cpu = 3;
    rt_thread_control(tid, RT_THREAD_CTRL_BIND_CPU, (void *)cpu);
    rt_thread_startup(tid);

    return 0;
}
