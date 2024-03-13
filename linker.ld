SECTIONS
{
    . = 0x20000000;
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    .sbss : { *(.sbss) }
    .sdata : { *(.sdata) }
    .note.gnu.build-id : { *(.note.gnu.build-id) }
}
