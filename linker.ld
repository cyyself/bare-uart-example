SECTIONS
{
    . = 0x200000;
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    .sbss : { *(.sbss) }
    .sdata : { *(.sdata) }
    .note.gnu.build-id : { *(.note.gnu.build-id) }
}
