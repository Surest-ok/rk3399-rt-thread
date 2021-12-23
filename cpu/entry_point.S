.section ".text.entrypoint","ax"
.globl _start
#include "../rtconfig.h"
_start:
    mrs     x0, CurrentEL           /* CurrentEL Register. bit 2, 3. Others reserved */
    and     x0, x0, #12             /* Clear reserved bits */

    /* Running at EL3? */
    cmp     x0, #12                 /* EL3 value is 0b1100 */
    bne     cpu_not_in_el3

    /* Should never be executed, just for completeness. (EL3) */
    mov     x0, #(1 << 0)           /* EL0 and EL1 are in Non-Secure state */
    orr     x0, x0, #(1 << 4)       /* RES1 */
    orr     x0, x0, #(1 << 5)       /* RES1 */
    orr     x0, x0, #(1 << 7)       /* SMC instructions are undefined at EL1 and above */
    orr     x0, x0, #(1 << 8)       /* HVC instructions are enabled at EL1 and above */
    orr     x0, x0, #(1 << 10)      /* The next lower level is AArch64 */
    msr     scr_el3, x2

    mov     x2, #0x3c9
    msr     spsr_el3, x2            /* 0b1111001001 */
    adr     x2, cpu_not_in_el3
    msr     elr_el3, x2
    eret                            /* Exception Return: from EL3, continue from cpu_not_in_el3 */

cpu_not_in_el3:                     /* Running at EL2 or EL1 */
    cmp     x0, #4                  /* EL1 = 0100  */
    beq     cpu_in_el1              /* Halt this core if running in El1 */

cpu_in_el2:

    bl      __asm_flush_dcache_all
    bl      __asm_invalidate_icache_all

    mrs     x0, hcr_el2
    orr     x0, x0, #(1 << 31)          /* Enable AArch64 in EL1 */ 
    orr     x0, x0, #(1 << 41) 
    orr     x0, x0, #(1 << 40) 
    bic     x0, x0, #(1 << 27)   
    bic     x0, x0, #(1 << 3)   
    bic     x0, x0, #(1 << 4)   
    bic     x0, x0, #(1 << 5)  
  
    orr     x0, x0, #(1 << 1)  
    bic     x0, x0, #(1 << 42)      
    msr     hcr_el2, x0

    /* Enable CNTP for EL1 */
    mrs     x0, cnthctl_el2         /* Counter-timer Hypervisor Control register */
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr

    mrs     x0, midr_el1
    mrs     x2, mpidr_el1
    msr vpidr_el2, x0
    msr vmpidr_el2, x2

  	mov	x0, #0x0800			        // Set/clear RES{1,0} bits
    movk x0, #0x30d0, lsl #16	    // Clear EE and E0E on LE systems
	msr	sctlr_el1, x0  

    // Disable EL2 coprocessor traps.
	mov x9, #0x33ff
	msr cptr_el2, x9
    msr hstr_el2, xzr

	// Disable stage 2 translations.
	msr vttbr_el2, xzr

    /* Hypervisor stub */
	adrp	x0, system_vectors
	msr	vbar_el2, x0

    mrs     x2, sctlr_el2
    orr     x2, x2, #(1 << 12)
    bic     x2, x2, #(3 << 3)       /* Disable SP Alignment check */
    bic     x2, x2, #(1 << 1)       /* Disable Alignment check */
    bic	    x2, x2, #(1 << 25)
    msr     sctlr_el2, x2
  
    /* Avoid trap from SIMD or float point instruction */
    mov     x1, #(3 << 20)         /* Don't trap any SIMD/FP instructions in both EL0 and EL1 */
    msr     cpacr_el1, x1

    mrs     x1, sctlr_el1
    orr     x1, x1, #(1 << 12)      /* Enable Instruction */
    bic     x1, x1, #(3 << 3)       /* Disable SP Alignment check */
    bic     x1, x1, #(1 << 1)       /* Disable Alignment check */
    msr     sctlr_el1, x1

    /* Change execution level to EL1 */
    mov     x2, #0x3c5
    msr     spsr_el2, x2            /* 0b1111000100 */
    adr     x2, cpu_in_el1
    msr     elr_el2, x2
    isb

    msr     spsel, #1                                                            
    eret

cpu_in_el1:

    ldr x1, = _start
    /* Read cpu id */
    mrs     x0, mpidr_el1
    and     x0, x0, #0x1ff
    cmp     x0, #0x100
    b.ge    2f
    mov     x2, x0
    b       0f

2:  
    sub     x0, x0, #0x100
    add     x0, x0, #4
    mov     x2, x0

0:
    cmp     x0 , #0
    b.eq    1f

    and     x0, x0, #0xff
    sub     x1 , x1 , #0x20000

    cmp     x0 , #0
    b.eq    1f

    sub     x0 , x0 , #1
    b.ne    0b

1:
    bic     x1 , x1 , #0xf                                                                
    mov     sp, x1

    cmp     x2, #0
    b.ne    secondary_cpu_start

    ldr     x1,  =__bss_start
    ldr     w2, =__bss_size

clean_bss_loop:
    cbz     w2, jump_to_entry
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, clean_bss_loop

jump_to_entry:

    b       rtthread_startup
    b       cpu_idle                /* For failsafe, halt this core too */

.globl cpu_idle
cpu_idle:
    wfe
.globl diy_led_on

diy_led_on:
    mov x28,lr
	mov	x0, #0xff720000           
	mov	x1, #0xff720000           
	ldr	w1, [x1,#4]
	orr	w1, w1, #0x2000
	str	w1, [x0,#4]
	mov	x0, #0xff720000           
	mov	x1, #0xff720000           
	ldr	w1, [x1]
	orr	w1, w1, #0x2000
	str	w1, [x0]
	ret x28

.globl	work_led_on
work_led_on:
    mov x28,lr
	mov	x0, #0xff780000           
	mov	x1, #0xff780000           
	ldr w1, [x1,#4]
	orr	w1, w1, #0x8000000
	str	w1, [x0,#4]
	mov	x0, #0xff780000           
	mov	x1, #0xff780000           
	ldr	w1, [x1]
	orr	w1, w1, #0x8000000
	str	w1, [x0]
	ret x28

.macro SMCCC instr
.cfi_startproc
    \instr	#0
    ldr	x4, [sp]
    stp	x0, x1, [x4, #0]
    stp	x2, x3, [x4, #16]
    ret
.cfi_endproc
.endm

.globl arm_smccc_smc
arm_smccc_smc:
	SMCCC	smc

.global secondary_cpu_start
secondary_cpu_start:
#ifdef RT_USING_SMP
    b  secondary_cpu_c_start
#else
    b   .
#endif
