#include <regdef.h>

#define at $1

/*
 * Minimal startup code for the chaos mod.
 *
 * Where we are jumped into, we have a lot of stuff which has been initalized for us:
 * - a functional stack
 * - realmem MEM_* heap
 *
 * so this startup code doesn't actually need to do a whole lot.
 */

.set noat
.set noreorder

.text

.global _start
_start:
	/* TODO: zerofill .bss section. EE Kernel seems to
	 zerofill the area for us already, but it might be wise to just do it anyways... */

	/* Save registers which are clobbered on the stack */
	addiu sp, sp, -0x8
	sw ra, (sp)
	sw gp, 4(sp)

	/* Call C++ global constructors */
	jal __do_global_ctors
	nop

	/* Now that .bss should hopefully be filled, and
	 we've called global constructors, call the C++ entry point. */
	jal chaosMain
	nop

	slti at, v0, 0x0
	beq at, zero, .Lokay
	nop

	/* If we get here, chaosMain() > 0, so something went wrong.
		Call global destructors. */
	jal __do_global_dtors
	nop

.Lokay:
	/* Restore registers which were clobbered from the stack */
	lw gp, 4(sp)
	lw ra, (sp)

	/* Return back to the game code. */
	jr ra
	addiu sp, sp, 0x8
