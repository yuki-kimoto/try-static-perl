#include "sperl_vmcode.h"







SPerl_char* const SPerl_VMCODE_C_CODE_NAMES[] = {
  "IADD",
  "LADD",
  "FADD",
  "DADD",
  "AALOAD",
  "AASTORE",
  "ACONST_NULL",
  "ALOAD",
  "ALOAD_0",
  "ALOAD_1",
  "ALOAD_2",
  "ALOAD_3",
  "ANEWARRAY",
  "ARETURN",
  "ARRAYLENGTH",
  "ASTORE",
  "ASTORE_0",
  "ASTORE_1",
  "ASTORE_2",
  "ASTORE_3",
  "ATHROW",
  "BALOAD",
  "BASTORE",
  "BIPUSH",
  "CALOAD",
  "CASTORE",
  "CHECKCAST",
  "D2F",
  "D2I",
  "D2L",
  "DALOAD",
  "DASTORE",
  "DCMPG",
  "DCMPL",
  "DCONST_0",
  "DCONST_1",
  "DDIV",
  "DLOAD",
  "DLOAD_0",
  "DLOAD_1",
  "DLOAD_2",
  "DLOAD_3",
  "DMUL",
  "DNEG",
  "DREM",
  "DRETURN",
  "DSTORE",
  "DSTORE_0",
  "DSTORE_1",
  "DSTORE_2",
  "DSTORE_3",
  "DSUB",
  "DUP",
  "DUP_X1",
  "DUP_X2",
  "DUP2",
  "DUP2_X1",
  "DUP2_X2",
  "F2D",
  "F2I",
  "F2L",
  "FALOAD",
  "FASTORE",
  "FCMPG",
  "FCMPL",
  "FCONST_0",
  "FCONST_1",
  "FCONST_2",
  "FDIV",
  "FLOAD",
  "FLOAD_0",
  "FLOAD_1",
  "FLOAD_2",
  "FLOAD_3",
  "FMUL",
  "FNEG",
  "FREM",
  "FRETURN",
  "FSTORE",
  "FSTORE_0",
  "FSTORE_1",
  "FSTORE_2",
  "FSTORE_3",
  "FSUB",
  "GETFIELD",
  "GETSTATIC",
  "GOTO",
  "GOTO_W",
  "I2B",
  "I2C",
  "I2D",
  "I2F",
  "I2L",
  "I2S",
  "IALOAD",
  "IAND",
  "IASTORE",
  "ICONST_M1",
  "ICONST_0",
  "ICONST_1",
  "ICONST_2",
  "ICONST_3",
  "ICONST_4",
  "ICONST_5",
  "IDIV",
  "IF_ACMPEQ",
  "IF_ACMPNE",
  "IF_ICMPEQ",
  "IF_ICMPNE",
  "IF_ICMPLT",
  "IF_ICMPGE",
  "IF_ICMPGT",
  "IF_ICMPLE",
  "IFEQ",
  "IFNE",
  "IFLT",
  "IFGE",
  "IFGT",
  "IFLE",
  "IFNONNULL",
  "IFNULL",
  "IINC",
  "ILOAD",
  "ILOAD_0",
  "ILOAD_1",
  "ILOAD_2",
  "ILOAD_3",
  "IMUL",
  "INEG",
  "INSTANCEOF",
  "INVOKEDYNAMIC",
  "INVOKEINTERFACE",
  "INVOKESPECIAL",
  "INVOKESTATIC",
  "INVOKEVIRTUAL",
  "IOR",
  "IREM",
  "IRETURN",
  "ISHL",
  "ISHR",
  "ISTORE",
  "ISTORE_0",
  "ISTORE_1",
  "ISTORE_2",
  "ISTORE_3",
  "ISUB",
  "IUSHR",
  "IXOR",
  "JSR",
  "JSR_W",
  "L2D",
  "L2F",
  "L2I",
  "LALOAD",
  "LAND",
  "LASTORE",
  "LCMP",
  "LCONST_0",
  "LCONST_1",
  "LDC",
  "LDC_W",
  "LDC2_W",
  "LDIV",
  "LLOAD",
  "LLOAD_0",
  "LLOAD_1",
  "LLOAD_2",
  "LLOAD_3",
  "LMUL",
  "LNEG",
  "LOOKUPSWITCH",
  "LOR",
  "LREM",
  "LRETURN",
  "LSHL",
  "LSHR",
  "LSTORE",
  "LSTORE_0",
  "LSTORE_1",
  "LSTORE_2",
  "LSTORE_3",
  "LSUB",
  "LUSHR",
  "LXOR",
  "MONITORENTER",
  "MONITOREXIT",
  "MULTIANEWARRAY",
  "NEW",
  "NEWARRAY",
  "NOP",
  "POP",
  "POP2",
  "PUTFIELD",
  "PUTSTATIC",
  "RET",
  "RETURN",
  "SALOAD",
  "SASTORE",
  "SIPUSH",
  "SWAP",
  "TABLESWITCH",
  "WIDE"
};
