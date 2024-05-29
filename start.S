.text
.globl _start
_start:
    // turn on fs in sstatus
    li x1, 1
    slli x1, x1, 13
    csrs sstatus, x1
    // turn on vs in sstatus
    li x1, 1
    slli x1, x1, 9
    csrs sstatus, x1
    // clear all gpr
    li  x1, 0
    li  x2, 0
    li  x3, 0
    li  x4, 0
    li  x5, 0
    li  x6, 0
    li  x7, 0
    li  x8, 0
    li  x9, 0
    li  x10, 0
    li  x11, 0
    li  x12, 0
    li  x13, 0
    li  x14, 0
    li  x15, 0
    li  x16, 0
    li  x17, 0
    li  x18, 0
    li  x19, 0
    li  x20, 0
    li  x21, 0
    li  x22, 0
    li  x23, 0
    li  x24, 0
    li  x25, 0
    li  x26, 0
    li  x27, 0
    li  x28, 0
    li  x29, 0
    li  x30, 0
    li  x31, 0
    la  sp, bootstacktop
    jal main
poweroff:
    j poweroff
_stack:
.section .data
    .global bootstack
    .align 16
bootstack:
    .space 1024
    .global bootstacktop
bootstacktop:
    .space 64