#include "uart.h"
#include "trap.h"
#include "mmu.h"

int main(long hartid) {
    // setup_mtvec();
    // mmu_init();
    // enter_smode();
    print_s("Hello World from UART!\n");
    print_long(hartid);
    return 0;
}
