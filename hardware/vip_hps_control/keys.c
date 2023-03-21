#include "cpu.h"
#include "address_map_arm.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define HW_REGS_BASE (0xff200000)
#define HW_REGS_SPAN (0x00200000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)
unsigned short getKeyAH (volatile unsigned short * keys)
{
    return ~(*keys) & 0xF;
}
int main(void)
{
    int hps_f = open("/dev/mem", (O_RDWR | O_SYNC));
    printf("HPS FD: %d\n", hps_f);
    void * virtual_base_lw = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, HW_REGS_BASE);
    volatile unsigned short * KEY_BASE = (volatile unsigned short *) (virtual_base_lw + BUTTONS_PIO_BASE);
    while (1)
    {
        if (getKeyAH(KEY_BASE) & 8)
            printf("got key\n");
    }
    close(hps_f);
    return 0;
}
