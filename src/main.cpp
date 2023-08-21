#include "main.hpp"

using namespace std;

extern "C" void dispatcher_trampoline();
extern "C" void *start_of_dispatcher_s;
extern "C" void *end_of_dispatcher_s;

static uint64_t xx0 = 0;
static uint64_t xx1 = 0;
static uint64_t xx2 = 0;

struct ARGS {
  uint64_t x0;
  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
};

extern "C" void hnit(uint64_t x0, uint64_t x1, uint64_t x2) {
  if (x0 + x1 + x2 > 0) {
    printf("%lx", xx0);
    printf("%lx", xx1);
    printf("%lx", xx2);
  }
}

extern "C" void proxy_func(struct ARGS *args) {
  auto x0 = args->x0;
  auto x1 = args->x1;
  auto x2 = args->x2;

  if (x2 == 0x100) {
    hnit(x0, x1, x2);
  }
};

extern "C" int main() {

  auto libc_base = get_libc_base();
  change_rxw(libc_base);
  change_rxw((void *)memcpy);

  auto hook_addr = (uint32_t *)memcpy;

  auto write_p = hook_addr;
  // 保存指令
  uint32_t ins_save = *write_p;

  write_p = (uint32_t *)libc_base;
  // // 复制代理函数地址到x16
  a64_copy_to_reg_64bits(&write_p, 16, (uint64_t)proxy_func);

  auto tmp_size =
      (uint8_t *)&end_of_dispatcher_s - (uint8_t *)&start_of_dispatcher_s;
  memcpy(write_p, &start_of_dispatcher_s, tmp_size);
  write_p += (tmp_size / 4);

  *write_p++ = ins_save;
  a64_branch_helper(write_p, (uint64_t)(hook_addr + 1), false);
  __builtin___clear_cache((char *)libc_base, (char *)libc_base + tmp_size + 8);

  // 调到libc开头
  a64_branch_helper(hook_addr, (uint64_t)libc_base, false);
  __builtin___clear_cache((char *)hook_addr, (char *)hook_addr + 4);

  return 0;
}
