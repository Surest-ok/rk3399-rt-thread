/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-06     ZhaoXiaowei    the first version
 */
#include "../rtconfig.h"
  /*
 *enable gtimer
 */
.globl rt_hw_gtimer_enable
rt_hw_gtimer_enable:
	MOV X0,#1
	MSR CNTP_CTL_EL0,X0
	RET

.globl rt_hw_gtimer_clear
rt_hw_gtimer_clear:
    MRS X0, CNTP_CTL_EL0
	BIC X0,x0,#0x1
    BIC X0,x0,#0x2
	MSR CNTP_CTL_EL0,X0
	RET

/*
 *set gtimer CNTP_TVAL_EL0 value
 */
.globl rt_hw_set_gtimer_val
rt_hw_set_gtimer_val:
	MSR CNTP_TVAL_EL0,X0
	RET

/*
 *get gtimer CNTP_TVAL_EL0 value
 */
.globl rt_hw_get_gtimer_val
rt_hw_get_gtimer_val:
	MRS X0,CNTP_TVAL_EL0
	RET


.globl rt_hw_get_cntpct_val
rt_hw_get_cntpct_val:
	MRS X0, CNTPCT_EL0
	RET

/*
 *get gtimer frq value
 */
.globl rt_hw_get_gtimer_frq
rt_hw_get_gtimer_frq:
	MRS X0,CNTFRQ_EL0
	RET

.macro SAVE_CONTEXT

    /* Switch to use the EL0 stack pointer. */
    MSR 	SPSEL, #0

    /* Save the entire context. */
    STP 	X0, X1, [SP, #-0x10]!
    STP 	X2, X3, [SP, #-0x10]!
    STP 	X4, X5, [SP, #-0x10]!
    STP 	X6, X7, [SP, #-0x10]!
    STP 	X8, X9, [SP, #-0x10]!
    STP 	X10, X11, [SP, #-0x10]!
    STP 	X12, X13, [SP, #-0x10]!
    STP 	X14, X15, [SP, #-0x10]!
    STP 	X16, X17, [SP, #-0x10]!
    STP 	X18, X19, [SP, #-0x10]!
    STP 	X20, X21, [SP, #-0x10]!
    STP 	X22, X23, [SP, #-0x10]!
    STP 	X24, X25, [SP, #-0x10]!
    STP 	X26, X27, [SP, #-0x10]!
    STP 	X28, X29, [SP, #-0x10]!
    STP 	X30, XZR, [SP, #-0x10]!

    MRS		X0, CurrentEL
    CMP		X0, 0xc
    B.EQ	3f
    CMP		X0, 0x8
    B.EQ	2f
    CMP		X0, 0x4
    B.EQ	1f
    B 		.
3:
    MRS		X3, SPSR_EL3
    /* Save the ELR. */
    MRS		X2, ELR_EL3
    B		0f
2:
    MRS		X3, SPSR_EL2
    /* Save the ELR. */
    MRS		X2, ELR_EL2
    B		0f
1:
    MRS		X3, SPSR_EL1
    MRS		X2, ELR_EL1
    B		0f
0:

    STP 	X2, X3, [SP, #-0x10]!

    MOV 	X0, SP   /* Move SP into X0 for saving. */

    /* Switch to use the ELx stack pointer. */
    MSR 	SPSEL, #1

    .endm

.macro SAVE_CONTEXT_T

    /* Switch to use the EL0 stack pointer. */
    MSR 	SPSEL, #0

    /* Save the entire context. */
    STP 	X0, X1, [SP, #-0x10]!
    STP 	X2, X3, [SP, #-0x10]!
    STP 	X4, X5, [SP, #-0x10]!
    STP 	X6, X7, [SP, #-0x10]!
    STP 	X8, X9, [SP, #-0x10]!
    STP 	X10, X11, [SP, #-0x10]!
    STP 	X12, X13, [SP, #-0x10]!
    STP 	X14, X15, [SP, #-0x10]!
    STP 	X16, X17, [SP, #-0x10]!
    STP 	X18, X19, [SP, #-0x10]!
    STP 	X20, X21, [SP, #-0x10]!
    STP 	X22, X23, [SP, #-0x10]!
    STP 	X24, X25, [SP, #-0x10]!
    STP 	X26, X27, [SP, #-0x10]!
    STP 	X28, X29, [SP, #-0x10]!
    STP 	X30, XZR, [SP, #-0x10]!

    MRS		X0, CurrentEL
    CMP		X0, 0xc
    B.EQ	3f
    CMP		X0, 0x8
    B.EQ	2f
    CMP		X0, 0x4
    B.EQ	1f
    B 		.
3:
    MRS		X3, SPSR_EL3
    MOV		X2, X30
    B		0f
2:
    MRS		X3, SPSR_EL2
    MOV		X2, X30
    B		0f
1:
    MRS		X3, SPSR_EL1
    MOV		X2, X30
    B		0f
0:

    STP 	X2, X3, [SP, #-0x10]!

    MOV 	X0, SP   /* Move SP into X0 for saving. */

    /* Switch to use the ELx stack pointer. */
    MSR 	SPSEL, #1

    .endm

.macro RESTORE_CONTEXT

    /* Switch to use the EL0 stack pointer. */
    MSR 	SPSEL, #0

    /* Set the SP to point to the stack of the task being restored. */
    MOV		SP, X0

    LDP 	X2, X3, [SP], #0x10  /* SPSR and ELR. */

    MRS		X0, CurrentEL
    CMP		X0, 0xc
    B.EQ	3f
    CMP		X0, 0x8
    B.EQ	2f
    CMP		X0, 0x4
    B.EQ	1f
    B 		.
3:
    MSR		SPSR_EL3, X3
    MSR		ELR_EL3, X2
    B		0f
2:
    MSR		SPSR_EL2, X3
    MSR		ELR_EL2, X2
    B		0f
1:
    MSR		SPSR_EL1, X3
    MSR		ELR_EL1, X2
    B		0f
0:

    LDP 	X30, XZR, [SP], #0x10
    LDP 	X28, X29, [SP], #0x10
    LDP 	X26, X27, [SP], #0x10
    LDP 	X24, X25, [SP], #0x10
    LDP 	X22, X23, [SP], #0x10
    LDP 	X20, X21, [SP], #0x10
    LDP 	X18, X19, [SP], #0x10
    LDP 	X16, X17, [SP], #0x10
    LDP 	X14, X15, [SP], #0x10
    LDP 	X12, X13, [SP], #0x10
    LDP 	X10, X11, [SP], #0x10
    LDP 	X8, X9, [SP], #0x10
    LDP 	X6, X7, [SP], #0x10
    LDP 	X4, X5, [SP], #0x10
    LDP 	X2, X3, [SP], #0x10
    LDP 	X0, X1, [SP], #0x10

    /* Switch to use the ELx stack pointer.  _RB_ Might not be required. */
    MSR 	SPSEL, #1

    ERET

    .endm

.text
/*
 * rt_base_t rt_hw_interrupt_disable();
 */
.globl rt_hw_interrupt_disable
rt_hw_interrupt_disable:
    MRS      X0, DAIF
    MSR      DAIFSet, #3
    DSB      SY
    RET

.globl rt_hw_local_irq_disable
rt_hw_local_irq_disable:
    MRS      X0, DAIF
    MSR      DAIFSet, #3
    DSB      SY
    RET

/*
 * void rt_hw_interrupt_enable(rt_base_t level);
 */
.globl rt_hw_interrupt_enable
rt_hw_interrupt_enable:
    DSB     SY
    MOV     X1, #0xC0
    ANDS    X0, X0, X1
    B.NE    rt_hw_interrupt_enable_exit
    MSR     DAIFClr, #3
rt_hw_interrupt_enable_exit:
    RET

.globl rt_hw_local_irq_enable
rt_hw_local_irq_enable:
    DSB     SY
    MOV     X1, #0xC0
    ANDS    X0, X0, X1
    B.NE    rt_hw_interrupt_enable_exit
    MSR     DAIFClr, #3
rt_hw_local_irq_enable_exit:
    RET

/*
 * void rt_hw_context_switch_to(rt_ubase_t to);
 * r0 --> to
 * r1 --> to thread's tcb
 */
.globl rt_hw_context_switch_to
rt_hw_context_switch_to:
#ifdef RT_USING_SMP
    STP 	X0, X1, [SP, #-0x10]!
    MOV     X0, X1
    bl      rt_cpus_lock_status_restore
    LDP 	X0, X1, [SP], #0x10
#endif /*RT_USING_SMP*/
    LDR		X0, [X0]
    RESTORE_CONTEXT

.text
/*
 * void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to);
 * r0 --> from
 * r1 --> to
 */
.globl rt_hw_context_switch
rt_hw_context_switch:
    MOV		X8,X0
    MOV		X19,X1
    MOV		X10,X2
    SAVE_CONTEXT_T
    STR     X0, [X8]             // store sp in preempted tasks TCB
#ifdef RT_USING_SMP
    MOV	    X0, X10
    bl      rt_cpus_lock_status_restore
#endif

    LDR     X0, [X19]             // store sp in preempted tasks TCB

    RESTORE_CONTEXT



/*
 * void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to);
 */
.globl rt_thread_switch_interrupt_flag
.globl rt_interrupt_from_thread
.globl rt_interrupt_to_thread
.globl rt_hw_context_switch_interrupt
rt_hw_context_switch_interrupt:
#ifdef RT_USING_SMP

    /* r0 :svc_mod context
     * r1 :addr of from_thread's sp
     * r2 :addr of to_thread's sp
     * r3 :to_thread's tcb
     */
    MOV  X20,  X0
    MOV  X21,  X1
    MOV  X22,  X2
    MOV  X23,  X3
//    SAVE_CONTEXT_T
    
    STR     X0, [X1]

    MOV     X0, X23
    bl      rt_cpus_lock_status_restore

    LDR		X0, [X22]            // get new task stack pointer
    RESTORE_CONTEXT
#else /*RT_USING_SMP*/
    ADR 	X2, rt_thread_switch_interrupt_flag
    LDR 	X3, [X2]
    CMP 	X3, #1
    B.EQ 	_reswitch
    ADR 	X4, rt_interrupt_from_thread   // set rt_interrupt_from_thread
    MOV 	X3, #1              // set rt_thread_switch_interrupt_flag to 1
    STR 	X0, [X4]
    STR 	X3, [X2]
_reswitch:
    ADR 	X2, rt_interrupt_to_thread     // set rt_interrupt_to_thread
    STR 	X1, [X2]
    RET
#endif /*RT_USING_SMP*/
.text

// -- Exception handlers ----------------------------------

    .align  8
.globl vector_fiq
vector_fiq:
#ifdef RT_USING_SMP
    SAVE_CONTEXT
    STP 	X0, X1, [SP, #-0x10]!
    BL      rt_interrupt_enter
    BL      rt_hw_trap_irq
    BL      rt_interrupt_leave

    LDP 	X0, X1, [SP], #0x10
    STP 	X0, X1, [SP, #-0x10]!
    BL      rt_scheduler_do_irq_switch
    LDP 	X0, X1, [SP], #0x10
    B       vector_fiq_exit
#else
    SAVE_CONTEXT
    STP 	X0, X1, [SP, #-0x10]!

    BL      rt_interrupt_enter
    BL      rt_hw_trap_irq
    BL      rt_interrupt_leave
    
    LDP 	X0, X1, [SP], #0x10

    // if rt_thread_switch_interrupt_flag set, jump to
    // rt_hw_context_switch_interrupt_do and don't return
    ADR 	X1, rt_thread_switch_interrupt_flag
    LDR     X2, [X1]
    CMP     X2, #1
    B.NE     vector_fiq_exit

    MOV     X2,  #0         // clear flag
    STR     X2,  [X1]

    ADR     X3,  rt_interrupt_from_thread
    LDR     X4,  [X3]
    STR     x0,  [X4]       // store sp in preempted tasks's TCB

    ADR     x3,  rt_interrupt_to_thread
    LDR     X4,  [X3]
    LDR     x0,  [X4]       // get new task's stack pointer
#endif 
vector_fiq_exit:	
    RESTORE_CONTEXT

.globl      rt_interrupt_enter
.globl      rt_interrupt_leave
.globl      rt_thread_switch_interrupt_flag
.globl      rt_interrupt_from_thread
.globl      rt_interrupt_to_thread


// -------------------------------------------------------------------

    .align  8
.globl vector_irq
vector_irq:
#ifdef RT_USING_SMP
    SAVE_CONTEXT
    STP 	X0, X1, [SP, #-0x10]!
    BL      rt_interrupt_enter
    BL      rt_hw_trap_irq
    BL      rt_interrupt_leave

    LDP 	X0, X1, [SP], #0x10
    STP 	X0, X1, [SP, #-0x10]!
    BL      rt_scheduler_do_irq_switch
    LDP 	X0, X1, [SP], #0x10
    B       vector_irq_exit
#else
    SAVE_CONTEXT
    STP 	X0, X1, [SP, #-0x10]!

    BL      rt_interrupt_enter
    BL      rt_hw_trap_irq
    BL      rt_interrupt_leave
    
    LDP 	X0, X1, [SP], #0x10

    // if rt_thread_switch_interrupt_flag set, jump to
    // rt_hw_context_switch_interrupt_do and don't return
    ADR 	X1, rt_thread_switch_interrupt_flag
    LDR     X2, [X1]
    CMP     X2, #1
    B.NE     vector_irq_exit

    MOV     X2,  #0         // clear flag
    STR     X2,  [X1]

    ADR     X3,  rt_interrupt_from_thread
    LDR     X4,  [X3]
    STR     x0,  [X4]       // store sp in preempted tasks's TCB

    ADR     x3,  rt_interrupt_to_thread
    LDR     X4,  [X3]
    LDR     x0,  [X4]       // get new task's stack pointer
#endif 
vector_irq_exit:	
    RESTORE_CONTEXT

// -------------------------------------------------

    .align  8
    .globl  vector_error
vector_error:
    SAVE_CONTEXT
    BL      rt_hw_trap_error
    B       .
