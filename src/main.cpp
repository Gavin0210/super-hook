#include "inline-hook/shadowhook.h"
#include "jfile.h"
#include <cerrno>
#include <cstring>
#include <iostream>
using namespace std;

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

void target_func(char *data) { logd("{}", data); }
void proxy_func(char *data) { logd("from proxy {}", data); }

void hook_func(uint64_t target_func, uint64_t proxy_func) {

  if (mprotect((void *)ALIGN_TO_4KB(target_func), getpagesize() * 2,
               PROT_READ | PROT_EXEC)) {
    loge("mprotect errno!");
  }

  // 保存前五个字节
  uint32_t save_ins[5];
  memcpy((void *)save_ins, (void *)target_func, 5 * sizeof(uint32_t));

  // 保存寄存器
}

void *mymemcpy(void *dest, const void *src, size_t n) { logd("pwd"); }

int main(int argc, char **argv) {

  void *orig = NULL;
  void *stub = NULL;
  // uint8_t buff[10];
  // uint8_t buff[10];

  shadowhook_init(SHADOWHOOK_MODE_UNIQUE, 1);

  // stub = shadowhook_hook_func_addr((void *)memcpy, (void *)mymemcpy, &orig);
  stub = shadowhook_hook_sym_name("libc.so", "memcpy", (void *)mymemcpy, &orig);
  // logd("{:x}", (uint64_t)stub);
  // logd("{:x}", (uint64_t)orig);

  // if (stub == NULL) {
  //   int err_num = shadowhook_get_errno();
  //   const char *err_msg = shadowhook_to_errmsg(err_num);
  //   logd("hook error {}- {}", err_num, err_msg);
  // }

  return 0;
}
