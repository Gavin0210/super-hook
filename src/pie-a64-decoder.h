#include <stdint.h>

enum reg
{             // +--------------+
    x0 = 0,   // | X0           |
    x1 = 1,   // | X1           |
    x2 = 2,   // | X2           |
    x3 = 3,   // | X3           |
    x4 = 4,   // | X4           |
    x5 = 5,   // | X5           |
    x6 = 6,   // | X6           |
    x7 = 7,   // | X7           |
    x8 = 8,   // | X8 (XR)      |
    x9 = 9,   // | X9           |
    x10 = 10, // | X10          |
    x11 = 11, // | X11          |
    x12 = 12, // | X12          |
    x13 = 13, // | X13          |
    x14 = 14, // | X14          |
    x15 = 15, // | X15          |
    x16 = 16, // | X16 (IP0)    |
    x17 = 17, // | X17 (IP1)    |
    x18 = 18, // | X18 (PR)     |
    x19 = 19, // | X19          |
    x20 = 20, // | X20          |
    x21 = 21, // | X21          |
    x22 = 22, // | X22          |
    x23 = 23, // | X23          |
    x24 = 24, // | X24          |
    x25 = 25, // | X25          |
    x26 = 26, // | X26          |
    x27 = 27, // | X27          |
    x28 = 28, // | X28          |
    x29 = 29, // | X29 (FP)     |
    x30 = 30, // | X30 (LR)     |
    x31 = 31, // | X31 (SP/XZR) |
    reg_invalid = 32
}; // +--------------+

enum reg_alt
{
    xr = x8,   // Designated Indirect Result Location Parameter
    ip0 = x16, // Intra-Procedure Call temporary registers
    ip1 = x17, // Intra-Procedure Call temporary registers
    pr = x18,  // Platform Register
    es = x19,  // the first calleE-Saved register - not a standard alias
    fp = x29,  // Frame Pointer
    lr = x30,  // Link register
    sp = x31,  // Stack Pointer
    xzr = x31, // Zero Register
};

typedef enum
{
    A64_CBZ_CBNZ,
    A64_B_COND,
    A64_SVC,
    A64_HVC,
    A64_SMC,
    A64_BRK,
    A64_HLT,
    A64_DCPS1,
    A64_DCPS2,
    A64_DCPS3,
    A64_MSR_IMMED,
    A64_HINT,
    A64_CLREX,
    A64_DSB,
    A64_DMB,
    A64_ISB,
    A64_SYS,
    A64_MRS_MSR_REG,
    A64_SYSL,
    A64_TBZ_TBNZ,
    A64_B_BL,
    A64_BR,
    A64_BLR,
    A64_RET,
    A64_ERET,
    A64_DRPS,
    A64_LDX_STX,
    A64_LDR_LIT,
    A64_LDP_STP,
    A64_LDR_STR_IMMED,
    A64_LDR_STR_REG,
    A64_LDR_STR_UNSIGNED_IMMED,
    A64_LDX_STX_MULTIPLE,
    A64_LDX_STX_MULTIPLE_POST,
    A64_LDX_STX_SINGLE,
    A64_LDX_STX_SINGLE_POST,
    A64_LDADD,
    A64_LDCLR,
    A64_LDEOR,
    A64_LDSMAX,
    A64_LDSMIN,
    A64_LDUMAX,
    A64_LDUMIN,
    A64_LDSET,
    A64_SWP,
    A64_ADD_SUB_IMMED,
    A64_BFM,
    A64_EXTR,
    A64_LOGICAL_IMMED,
    A64_MOV_WIDE,
    A64_ADR,
    A64_ADD_SUB_EXT_REG,
    A64_ADD_SUB_SHIFT_REG,
    A64_ADC_SBC,
    A64_CCMP_CCMN_IMMED,
    A64_CCMP_CCMN_REG,
    A64_COND_SELECT,
    A64_DATA_PROC_REG1,
    A64_DATA_PROC_REG2,
    A64_DATA_PROC_REG3,
    A64_LOGICAL_REG,
    A64_SIMD_ACROSS_LANE,
    A64_SIMD_COPY,
    A64_SIMD_EXTRACT,
    A64_SIMD_MODIFIED_IMMED,
    A64_SIMD_PERMUTE,
    A64_SIMD_SCALAR_COPY,
    A64_SIMD_SCALAR_PAIRWISE,
    A64_SIMD_SCALAR_SHIFT_IMMED,
    A64_SIMD_SCALAR_THREE_DIFF,
    A64_SIMD_SCALAR_THREE_SAME,
    A64_SIMD_SCALAR_TWO_REG,
    A64_SIMD_SCALAR_X_INDEXED,
    A64_SIMD_SHIFT_IMMED,
    A64_SIMD_TABLE_LOOKUP,
    A64_SIMD_THREE_DIFF,
    A64_SIMD_THREE_SAME,
    A64_SIMD_TWO_REG,
    A64_SIMD_X_INDEXED,
    A64_CRYPTO_AES,
    A64_CRYPTO_SHA_REG3,
    A64_CRYPTO_SHA_REG2,
    A64_FCMP,
    A64_FCCMP,
    A64_FCSEL,
    A64_FLOAT_REG1,
    A64_FLOAT_REG2,
    A64_FLOAT_REG3,
    A64_FMOV_IMMED,
    A64_FLOAT_CVT_FIXED,
    A64_FLOAT_CVT_INT,
    A64_INVALID
} a64_instruction;
a64_instruction a64_decode(uint32_t *address);