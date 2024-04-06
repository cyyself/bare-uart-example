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

int main() {
    unsigned long hartid = get_hartid();
    print_s("hart ");
    print_long(hartid);
    print_s(" is booting\r\n");
    theadfix();
    unsigned long mxstatus = get_mxstatus();
    print_s("mxstatus = ");
    dump_hex(mxstatus);
    setup_mtvec();
    mmu_init();
    print_s("returned from mmu_init\r\n");
    check_zicboz();
    turn_on_cbze_s_mode();
    enable_counter_smode();
    print_s("entering smode\r\n");
    enter_smode();
    print_s("now entered smode\r\n");
    check_zicboz();
    double x = 1.0;
    dump_hex(*(unsigned long*)(&x));
    v_memcpy(s1, s0, 64);
    print_s(s1);
    check_freq();
    return 0;
}
