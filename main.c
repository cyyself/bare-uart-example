#include "uart.h"
#include "trap.h"
#include "mmu.h"

volatile int hart_fin[2];

void test_zicond() {
    long x = 0xabcddeadbeef;
    long y_0 = 0;
    long y_1 = 1;
    long res0, res1, res2, res3;
    asm volatile(
        "czero.eqz %0, %1, %2"
        : "=r" (res0)
        : "r" (x), "r" (y_0)
    );
    asm volatile(
        "czero.eqz %0, %1, %2"
        : "=r" (res1)
        : "r" (x), "r" (y_1)
    );
    asm volatile(
        "czero.nez %0, %1, %2"
        : "=r" (res2)
        : "r" (x), "r" (y_0)
    );
    asm volatile(
        "czero.nez %0, %1, %2"
        : "=r" (res3)
        : "r" (x), "r" (y_1)
    );
    dump_hex(res0);
    dump_hex(res1);
    dump_hex(res2);
    dump_hex(res3);
}

int main(long hartid) {
    // setup_mtvec();
    // mmu_init();
    // enter_smode();
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
    test_zicond();
    return 0;
}
