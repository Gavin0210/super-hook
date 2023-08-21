.global start_of_dispatcher_s
start_of_dispatcher_s:



  STP  X4,  X5, [SP, #-0x10]!
  STP  X6,  X7, [SP, #-0x10]!
  STP  X8,  X9, [SP, #-0x10]!
  STP X10, X11, [SP, #-0x10]!
  STP X12, X13, [SP, #-0x10]!
  STP X14, X15, [SP, #-0x10]!
  STP X16, X17, [SP, #-0x10]!
  STP X18, X19, [SP, #-0x10]!
  STP X20, X21, [SP, #-0x10]!
  STP X28, X29, [SP, #-0x10]!
  STP X30, X31, [SP, #-0x10]!

  STP  X0,  X1, [SP, #-0x20]!
  STP  X2,  X3, [SP,  #0x10]

  MRS X19, NZCV
  MRS X20, FPCR
  MRS X21, FPSR
  
// //  next_addr 指针 ，pop到x0上 sp+32 
  // ADD X0, SP, #0
  mov X0, SP

  BLR X16

  MSR NZCV, X19
  MSR FPCR, X20
  MSR FPSR, X21 

  LDP  X2,  X3, [SP, #0x10]
  LDP  X0,  X1, [SP],#0x20

  LDP X30, X31, [SP],#0x10
  LDP X28, X29, [SP],#0x10
  LDP X20, X21, [SP],#0x10
  LDP X18, X19, [SP],#0x10
  LDP X16, X17, [SP],#0x10
  LDP X14, X15, [SP],#0x10
  LDP X12, X13, [SP],#0x10
  LDP X10, X11, [SP],#0x10
  LDP  X8,  X9, [SP],#0x10
  LDP  X6,  X7, [SP],#0x10
  LDP  X4,  X5, [SP],#0x10


  // BR X0



.global end_of_dispatcher_s
end_of_dispatcher_s:

