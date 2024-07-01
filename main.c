#include "uart.h"
#include "trap.h"
#include "mmu.h"

char s0[64] = "Hello World from UART!\r\n";
char s1[64];

void *v_memcpy(void* dest, const void* src, long n);

void enable_counter_smode() {
    asm volatile("csrs mcounteren, %0" : : "r" (0xffffffffu));
}

unsigned long rdcycle() {
    unsigned long cycle;
    asm volatile ("rdcycle %0" : "=r" (cycle));
    return cycle;
}

unsigned long rdtime() {
    unsigned long time;
    asm volatile ("rdtime %0" : "=r" (time));
    return time;
}

void check_freq() {
    const long timer_base_freq = 27000000;
    unsigned long start_cycle = rdcycle();
    unsigned long start_time = rdtime();
    unsigned long end_time = start_time;
    while (end_time - start_time < timer_base_freq) {
        end_time = rdtime();
    }
    unsigned long end_cycle = rdcycle();
    print_s("cpu freq = ");
    print_long(end_cycle - start_cycle);
    print_s("Hz\r\n");
}

unsigned long get_hartid() {
    unsigned long hartid;
    asm volatile ("csrr %0, mhartid" : "=r" (hartid));
    return hartid;
}

unsigned long get_mxstatus() {
    unsigned long mxstatus;
    asm volatile ("csrr %0, 0x7c0" : "=r" (mxstatus));
    return mxstatus;
}

void theadfix() {
    // disable MAEE in mxstatus
    asm volatile ("csrc 0x7c0, %0" : : "r" ((1<<21)));
}

void check_zicboz() {
    char a[127];
    char* ptr = a;

    // align ptr to 64B cache block
    while ( ( (long)ptr & ((1lu<<6)-1)) != 0) {
        ptr++;
    }

    // fill cache block with 'a'
    for (int i=0;i<64;i++) {
        ptr[i] = 'a';
    }

    // zero this block
    asm volatile ("cbo.zero 0(%0)" : : "r" ((void*)ptr));
    print_s("zeroed ");
    dump_hex((unsigned long)ptr);
    print_s("\r\n");

    // test if this block is zero
    int cnt_non_0 = 0;
    for (int i=0;i<64;i++) {
        if (ptr[i] != 0) {
            cnt_non_0++;
            print_s("non-0 at ");
            dump_hex((unsigned long)&ptr[i]);
            print_s("\r\n");
        }
    }

    if (cnt_non_0 == 0) {
        print_s("zicboz works\r\n");
    } else {
        print_s("zicboz does not work\r\n");
    }
}

void turn_on_cbze_s_mode() {
    asm volatile ("csrs menvcfg, %0" : : "r" ((1<<7)));
}

void test_rv32u() {
    long mstatus;
    asm volatile ("csrr %0, mstatus" : "=r" (mstatus));
    // set uxl to 32
    mstatus = mstatus & ~(0b11l << 32);
    mstatus = mstatus | (0b01l << 32);
    // set mpp to u mode
    mstatus = mstatus & ~(0b11 << 11);
    // enter u mode and test
    asm volatile ("csrw mstatus, %0" : : "r" (mstatus));
    long some_number = 0xdeadbeefdeadbeefl;
    long consumer = 0;
    asm volatile (
        "mv a1, %1\n"
        "lla a0, 2f\n"
        "csrw mtvec, a0\n"
        "lla a0, 1f\n"
        "csrw mepc, a0\n"
        "mret\n"
        "1:\n"
        ".short 0x582\n"
        "ebreak\n"
        ".p2align 2\n"
        "2:\n"
        "mv %0, a1\n"
        : "=r" (consumer)
        : "r" (some_number)
    );
    setup_mtvec();
    print_s("consumer = ");
    dump_hex(consumer);
}

void test_rv32u2() {
    long mstatus;
    asm volatile ("csrr %0, mstatus" : "=r" (mstatus));
    // set uxl to 32
    mstatus = mstatus & ~(0b11l << 32);
    mstatus = mstatus | (0b01l << 32);
    // set mpp to u mode
    mstatus = mstatus & ~(0b11 << 11);
    // enter u mode and test
    asm volatile ("csrw mstatus, %0" : : "r" (mstatus));
    long some_number = 0xdeadbeefdeadbeefl;
    long consumer = 0;
    asm volatile (
        "mv a1, %1\n"
        "lla a0, 2f\n"
        "csrw mtvec, a0\n"
        "lla a0, 1f\n"
        "csrw mepc, a0\n"
        "mret\n"
        "1:\n"
        "ebreak\n"
        ".p2align 2\n"
        "2:\n"
        "mv %0, a1\n"
        : "=r" (consumer)
        : "r" (some_number)
    );
    setup_mtvec();
    print_s("consumer = ");
    dump_hex(consumer);
}

void test_rv32u3() {
    long mstatus;
    asm volatile ("csrr %0, mstatus" : "=r" (mstatus));
    // set uxl to 32
    mstatus = mstatus & ~(0b11l << 32);
    mstatus = mstatus | (0b01l << 32);
    // set mpp to u mode
    mstatus = mstatus & ~(0b11 << 11);
    asm volatile ("csrw mstatus, %0" : : "r" (mstatus));
    // enter u mode and test
    long some_number = 0xdeadbeefdeadbeefl;
    long consumer = 0;
    asm volatile (
        "mv a1, %1\n"
        "lla a0, 2f\n"
        "csrw mtvec, a0\n"
        "lla a0, 1f\n"
        "csrw mepc, a0\n"
        "mret\n"
        "1:\n"
        "addi a1, a1, 0\n"
        "ebreak\n"
        ".p2align 2\n"
        "2:\n"
        "mv %0, a1\n"
        : "=r" (consumer)
        : "r" (some_number)
    );
    setup_mtvec();
    print_s("consumer = ");
    dump_hex(consumer);
}

int main() {
    unsigned long hartid = get_hartid();
    print_s("hart ");
    print_long(hartid);
    print_s(" is booting\r\n");
    theadfix();
    setup_mtvec();
    mmu_pmp_allow_all();
    test_rv32u();
    test_rv32u2();
    test_rv32u3();
    print_s("back\n");
    return 0;
}
