
/* Include the api file */
INCLUDE bxapi/ld/@REGION@.ld
SECTIONS {
	BLOB_LOAD_ADDRESS = 0x000ab000;

	. = BLOB_LOAD_ADDRESS;
	_begin = BLOB_LOAD_ADDRESS;

	.text BLOB_LOAD_ADDRESS : {
		*(.text)
		*(.text.*)
    	*(.gnu.linkonce.t*)
	}

	.ctors : {
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
	    KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))	
	}
	
  	.dtors : {
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
  	}

	/DISCARD/ : {
		/* Ignore .reginfo. It is not needed and adds bloat */
		*(.reginfo)
	}

	.data : { 
		*(.data)
	    	*(.data.*)
    		*(.gnu.linkonce.d*)
	 }
	.rodata : {
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r*)
	}
	.rdata : { *(.rdata) }
	.lit8 : { *(.lit8) }
	.lit4 : { *(.lit4) }
	.sdata : { *(.sdata) }
	.sbss : { _fbss = .; *(.sbss) *(.scommon) }
	.bss : { *(.bss) }

	_end = ALIGN(4);


}
