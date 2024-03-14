#include <stdint.h>

const uint64_t kernelPage = 0xC0000000;

uint64_t pml4[512] __attribute__((aligned(32)));

uint64_t pageDirTab[512] __attribute__((aligned(32)));

uint64_t basePageDir[512] __attribute__((aligned(4096)));
uint64_t kernelPageDir[512] __attribute__((aligned(4096)));
//uint64_t MemoryPageDir[512];

uint64_t getAddress(uint64_t* addr) {
    return ((uint64_t)addr)-kernelPage;
}

void initPaging() {
    pml4[0] = getAddress(pageDirTab)|1;

    pageDirTab[0] = getAddress(basePageDir)|1;
    pageDirTab[3] = getAddress(kernelPageDir)|1;

    basePageDir[0] = (0<<21) | 0b10000011;

    for (uint16_t i=0; i<512; i++) {
        kernelPageDir[i] = (i << 21) | 0b10000011;
    }

    // Move paging to cr3
    asm("mov %0, %%rax\n\t"
        "mov %%rax, %%cr3" : : "a" (getAddress(pml4)) );
}

void addPage(uint64_t src, uint64_t dst) {

}