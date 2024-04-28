#include "uart.h"
#include "trap.h"
#include "mmu.h"

volatile int hart_fin[2];

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
    return 0;
}
