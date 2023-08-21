#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#define PAGE_SIZE 4096
#define ALIGN_TO_4KB(address) (((uint64_t)address) & ~(0xFFF))
void a64_MOV_wide(uint32_t **address, unsigned int sf, unsigned int opc,
                  unsigned int hw, unsigned int imm16, unsigned int rd) {
  // abb100101ccddddddddddddddddeeeee
  uint32_t inst = 0x12800000 | ((sf & 0x1) << 31) | ((opc & 0x3) << 29) |
                  ((hw & 0x3) << 21) | ((imm16 & 0xffff) << 5) |
                  ((rd & 0x1f) << 0);
  **address = inst;
}
void a64_copy_to_reg_64bits(uint32_t **write_p, uint16_t reg, uint64_t value) {
  uint32_t first_half_word = value & 0xFFFF;
  uint32_t second_half_word = (value >> 16) & 0xFFFF;
  uint32_t third_half_word = (value >> 32) & 0xFFFF;
  uint32_t fourth_half_word = (value >> 48) & 0xFFFF;

  // MOVZ
  a64_MOV_wide(write_p, 1, 2, 0, first_half_word, reg);
  (*write_p)++;

  if (second_half_word > 0) { // MOVK
    a64_MOV_wide(write_p, 1, 3, 1, second_half_word, reg);
    (*write_p)++;
  }

  if (third_half_word > 0) { // MOVK
    a64_MOV_wide(write_p, 1, 3, 2, third_half_word, reg);
    (*write_p)++;
  }

  if (fourth_half_word > 0) { // MOVK
    a64_MOV_wide(write_p, 1, 3, 3, fourth_half_word, reg);
    (*write_p)++;
  }
}

void *get_libc_base() {

  void *libc_handle = dlopen("libc.so", RTLD_NOW | RTLD_GLOBAL);

  if (libc_handle) {
    Dl_info dl_info;
    int result = dladdr((void *)printf, &dl_info);

    if (result && dl_info.dli_fname) {
      return dl_info.dli_fbase;
    } else {
      return NULL;
    }

    dlclose(libc_handle);
  } else {
    std::cerr << "Failed to open libc.so." << std::endl;
  }
  return NULL;
}
void a64_B_BL(uint32_t **address, unsigned int op, unsigned int imm26) {
  // a00101bbbbbbbbbbbbbbbbbbbbbbbbbb
  uint32_t inst = 0x14000000 | ((op & 0x1) << 31) | ((imm26 & 0x3ffffff) << 0);
  **address = inst;
}

void a64_branch_helper(uint32_t *write_p, uint64_t target, bool link) {
  int64_t difference = target - (uint64_t)write_p;

  a64_B_BL(&write_p, link ? 1 : 0, difference >> 2);
}

void change_rxw(void *addr) {
  if (mprotect((void *)ALIGN_TO_4KB(addr), getpagesize() * 2,
               PROT_WRITE | PROT_READ | PROT_EXEC)) {
  }
}
