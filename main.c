#include "uart.h"
#include "trap.h"
#include "mmu.h"

volatile int hart_fin[2];

void *test_memcpy(void* dest, const void* src, long n);

void nemu_signal(int a){
    asm volatile ("mv a0, %0\n\t"
                  ".insn r 0x6B, 0, 0, x0, x0, x0\n\t"
                  :
                  : "r"(a)
                  : "a0");
}

#define DISABLE_TIME_INTR 0x100
#define NOTIFY_PROFILER 0x101
#define GOOD_TRAP 0x0

int main(long hartid) {
    // setup_mtvec();
    // mmu_init();
    // enter_smode();
    /*
    int wait = 0;
    do {
        wait = 0;
        for (int i=0;i<hartid;i++) {
            if (hart_fin[i] == 0) {
                print_s("Waiting for hart ");
                print_long(i);
                print_s("\r\n");
                wait = 1;
                break;
            }
        }
    } while(wait);
    print_s("Hello World from hart ");
    print_long(hartid);
    print_s("\r\n");
    hart_fin[hartid] = 1;
     */
    /*
    long vlenb;
    asm volatile("csrr %0, vlenb" : "=r" (vlenb));
    print_long(vlenb);
    print_s("\r\n");
     */
    char *s = 0x90000000u;
    char *t = 0x90001000u;
    for (int i=0;i<100;i++) {
        test_memcpy(t + 4, s + 4, 512);
        // print_long(i);
        // print_s("\r\n");
    }
    nemu_signal(GOOD_TRAP);
    return 0;
}
