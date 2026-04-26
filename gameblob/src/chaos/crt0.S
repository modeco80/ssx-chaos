#include <regdef.h>

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

	/* Now that .bss should hopefully be filled, call the C++ entry point. */
	jal chaosMain
	nop

	/* Restore registers which were clobbered from the stack */
	lw gp, 4(sp)
	lw ra, (sp)

	/* Return back to the game code. */
	jr ra
	addiu sp, sp, 0x8
