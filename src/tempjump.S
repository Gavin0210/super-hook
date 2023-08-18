.global start_of_dispatcher_s
start_of_dispatcher_s:

.global dbm_client_entry
dbm_client_entry:
    mov X16,X0
    LDP X0,X1,[X16,#0x00]
    LDP X2,X3,[X16,#0x10]
    LDP X4,X5,[X16,#0x20]
    LDP X6,X7,[X16,#0x30]
    LDP X8,X9,[X16,#0x40]
    LDP X10,X11,[X16,#0x50]
    LDP X12,X13,[X16,#0x60]
    LDP X14,X15,[X16,#0x70]
    // LDP X16,X17,[X16,#0x80]
    LDP X18,X19,[X16,#0x90]
    LDP X20,X21,[X16,#0xA0]
    LDP X22,X23,[X16,#0xB0]
    LDP X24,X25,[X16,#0xC0]
    LDP X26,X27,[X16,#0xD0]
    LDP X28,X29,[X16,#0xE0]
    LDR X30,[X16,#0xF0]
    LDR X17,[X16,#0xF8]
    mov SP,X17
    LDR X16,[X16,#0x100]
    BR  X16

push_x4_x21:
  STP  X4,  X5, [SP, #-144]!
  STP  X6,  X7, [SP, #16]
  STP  X8,  X9, [SP, #32]
  STP X10, X11, [SP, #48]
  STP X12, X13, [SP, #64]
  STP X14, X15, [SP, #80]
  STP X16, X17, [SP, #96]
  STP X18, X19, [SP, #112]
  STP X20, X21, [SP, #128]
  RET

pop_x4_x21:
  LDP  X6,  X7, [SP, #16]
  LDP  X8,  X9, [SP, #32]
  LDP X10, X11, [SP, #48]
  LDP X12, X13, [SP, #64]
  LDP X14, X15, [SP, #80]
  LDP X16, X17, [SP, #96]
  LDP X18, X19, [SP, #112]
  LDP X20, X21, [SP, #128]
  LDP  X4,  X5, [SP], #144
  RET

// 仿照scan函数内部 save_content
  STP  X0,  X1, [SP, #-16]!
.global dispatcher_trampoline
dispatcher_trampoline:
  // PUSH all general purpose registers but X0, X1
  // X0 and X1 are pushed by the exit stub
  STP  X2,  X3, [SP, #-48]!
  STP X29, X30, [SP, #16]
  STP  X2,  X3, [SP, #32]
  BL push_x4_x21

  MRS X19, NZCV
  MRS X20, FPCR
  MRS X21, FPSR
  
//  next_addr 指针 ，pop到x0上 sp+32 
  ADD X2, SP, #176
  LDR X3, disp_thread_data
  LDR X9, dispatcher_addr

  BLR X9

  MSR NZCV, X19
  MSR FPCR, X20
  MSR FPSR, X21 

  BL pop_x4_x21
  LDP X29, X30, [SP, #16]
  LDP  X0,  X1, [SP, #32]
  LDP  X2,  X3, [SP], #48

checked_cc_return:
  BR X0


dispatcher_addr: .quad 0

.global disp_thread_datal
disp_thread_datal:
disp_thread_data: .quad 0



.global end_of_dispatcher_s
end_of_dispatcher_s:

