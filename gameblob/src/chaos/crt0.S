/* Very, very, very minimal crt0.
   this CRT0 is only compatible with code
   which is compiled with -G0 and thus has no
   small data usage.
*/
#include <regdef.h>

.set noreorder
.set noat

.text

.global _start
_start:
	/* TODO: zerofill the .bss sections? EE Kernel seems to
	 do this for us already, but it might be wise to just do it anyways... */

	/* Save ra */
	addiu sp, sp, -0x8
	sw ra, (sp)
	sw gp, 4(sp)

	/* Now that .bss should hopefully be filled,
	   jump to the C++ entry point. */
	jal chaosMain
	nop

	/* Once the entry point is finished, return back to the game code */
	lw ra, (sp)
	lw gp, 4(sp)
	jr ra
	addiu sp, sp, 0x8
