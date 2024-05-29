#include "uart.h"
#include "trap.h"
#include "mmu.h"

int main() {
    setup_stvec();
    print_s("Hello World from UART!");
    return 0;
}
