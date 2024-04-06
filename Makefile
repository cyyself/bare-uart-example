CROSS_COMPILE :=	riscv64-linux-gnu-
CC		:=	$(CROSS_COMPILE)gcc
OBJCOPY	:=	$(CROSS_COMPILE)objcopy
OBJDUMP :=  $(CROSS_COMPILE)objdump
EXTRA_CFLAGS := -DARCH_HAS_PMP
CFLAGS := -march=rv64gcv_zicbom_zicbop_zicboz -nostdlib -static -mcmodel=medany -fno-builtin-printf -O0 $(EXTRA_CFLAGS)
.PHONY: clean

all: start.bin dump.S

start.bin: start.elf
	$(OBJCOPY) -O binary $< $@

start.elf: linker.ld start.S main.c uart.c trap.c mmu.c
	$(CC) $(CFLAGS) -T $^ -lgcc -o $@

dump.S: start.elf
	$(OBJDUMP) -D $< > $@

clean:
	rm start.bin start.elf
