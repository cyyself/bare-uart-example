.text
.globl _start
_start:
    // turn on fs in mstatus
    li x1, 1
    slli x1, x1, 13
    csrs mstatus, x1
    // turn on vs in mstatus
    li x1, 1
    slli x1, x1, 9
    csrs mstatus, x1
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
    csrr a0, mhartid
    slli t0, a0, 12
    la sp, bootstack
    add sp, sp, t0
    jal main
poweroff:
    j poweroff
_stack:
.section .data
    .global bootstack
    .align 16
bootstack:
    .space 65536
    .global bootstacktop
bootstacktop:
    .space 64

.globl test_memcpy
# void *test_memcpy(void* dest, const void* src, size_t n)
# a0=dest, a1=src, a2=n
#
test_memcpy:
mv a3, a0 # Copy destination
srl a2, a2, 2 # Convert bytes to 32b words
vsetvli t0, a2, e32, m1, ta, ma # Vectors of 32b
loop:
vle32.v v0, (a1) # Load bytes
sh2add a1, t0, a1 # Bump pointer
sub a2, a2, t0 # Decrement count
vse32.v v0, (a3) # Store bytes
sh2add a3, t0, a3 # Bump pointer
bnez a2, loop # Any more?
ret # Return