#include "address_map_arm.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SW_SPAN 0x10

#define HW_REGS_BASE (0xff200000)
#define HW_REGS_SPAN (0x00200000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

int main(void)
{
    int hps_f = open("/dev/mem", (O_RDWR | O_SYNC));
    printf("HPS_F is %d\n", (int) hps_f);
    void * lw = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, HW_REGS_BASE);
    unsigned short * ptr = (unsigned short *) lw + SW_BASE;
    printf("%p %d\n", lw, (int) (*ptr));
    close(hps_f);
    return 0;
}
