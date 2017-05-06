#include "spvm_bytecode.h"







const char* const SPVM_BYTECODE_C_CODE_NAMES[] = {
  "NOP",
  "UNDEF",
  "CONSTANT_BYTE_0",
  "CONSTANT_BYTE_1",
  "CONSTANT_SHORT_0",
  "CONSTANT_SHORT_1",
  "CONSTANT_INT_M1",
  "CONSTANT_INT_0",
  "CONSTANT_INT_1",
  "CONSTANT_INT_2",
  "CONSTANT_INT_3",
  "CONSTANT_INT_4",
  "CONSTANT_INT_5",
  "CONSTANT_LONG_M1",
  "CONSTANT_LONG_0",
  "CONSTANT_LONG_1",
  "CONSTANT_LONG_2",
  "CONSTANT_LONG_3",
  "CONSTANT_LONG_4",
  "CONSTANT_LONG_5",
  "CONSTANT_FLOAT_0",
  "CONSTANT_FLOAT_1",
  "CONSTANT_FLOAT_2",
  "CONSTANT_DOUBLE_0",
  "CONSTANT_DOUBLE_1",
  "PUSH_BYTE",
  "PUSH_BYTE_TO_SHORT",
  "PUSH_SHORT",
  "PUSH_BYTE_TO_INT",
  "PUSH_SHORT_TO_INT",
  "PUSH_BYTE_TO_LONG",
  "PUSH_SHORT_TO_LONG",
  "LOAD_CONSTANT",
  "LOAD_CONSTANT_W",
  "ARRAY_LOAD_INT",
  "ARRAY_LOAD_LONG",
  "ARRAY_LOAD_FLOAT",
  "ARRAY_LOAD_DOUBLE",
  "ARRAY_LOAD_BYTE",
  "ARRAY_LOAD_SHORT",
  "ARRAY_STORE_BYTE",
  "ARRAY_STORE_SHORT",
  "ARRAY_STORE_INT",
  "ARRAY_STORE_LONG",
  "ARRAY_STORE_FLOAT",
  "ARRAY_STORE_DOUBLE",
  "POP",
  "APOP",
  "ADD_INT",
  "ADD_LONG",
  "ADD_FLOAT",
  "ADD_DOUBLE",
  "SUBTRACT_INT",
  "SUBTRACT_LONG",
  "SUBTRACT_FLOAT",
  "SUBTRACT_DOUBLE",
  "MULTIPLY_INT",
  "MULTIPLY_LONG",
  "MULTIPLY_FLOAT",
  "MULTIPLY_DOUBLE",
  "DIVIDE_INT",
  "DIVIDE_LONG",
  "DIVIDE_FLOAT",
  "DIVIDE_DOUBLE",
  "REMAINDER_INT",
  "REMAINDER_LONG",
  "REMAINDER_FLOAT",
  "REMAINDER_DOUBLE",
  "NEGATE_INT",
  "NEGATE_LONG",
  "NEGATE_FLOAT",
  "NEGATE_DOUBLE",
  "LEFT_SHIFT_INT",
  "LEFT_SHIFT_LONG",
  "RIGHT_SHIFT_INT",
  "RIGHT_SHIFT_LONG",
  "RIGHT_SHIFT_UNSIGNED_INT",
  "RIGHT_SHIFT_UNSIGNED_LONG",
  "BIT_AND_INT",
  "BIT_AND_LONG",
  "OR_INT",
  "OR_LEFT",
  "XOR_INT",
  "XOR_LONG",
  "CONVERT_INT_TO_LONG",
  "CONVERT_INT_TO_FLOAT",
  "CONVERT_INT_TO_DOUBLE",
  "CONVERT_LONG_TO_INT",
  "CONVERT_LONG_TO_FLOAT",
  "CONVERT_LONG_TO_DOUBLE",
  "CONVERT_FLOAT_TO_INT",
  "CONVERT_FLOAT_TO_LONG",
  "CONVERT_FLOAT_TO_DOUBLE",
  "CONVERT_DOUBLE_TO_INT",
  "CONVERT_DOUBLE_TO_LONG",
  "CONVERT_DOUBLE_TO_FLOAT",
  "CONVERT_INT_TO_BYTE",
  "CONVERT_INT_TO_SHORT",
  "ICMP",
  "FCMPL",
  "FCMPG",
  "DCMPL",
  "DCMPG",
  "IFEQ",
  "IFNE",
  "IFLT",
  "IFGE",
  "IFGT",
  "IFLE",
  "IF_LCMPEQ",
  "IF_LCMPNE",
  "IF_LCMPLT",
  "IF_LCMPGE",
  "IF_LCMPGT",
  "IF_LCMPLE",
  "IF_ACMPEQ",
  "IF_ACMPNE",
  "GOTO",
  "TABLESWITCH",
  "LOOKUPSWITCH",
  "ARRAYLENGTH",
  "ATHROW",
  "WIDE",
  "IFNULL",
  "IFNONNULL",
  "INVOKESTATIC_WW",
  "LOAD_CONSTANT_WW",
  "BGETFIELD",
  "SGETFIELD",
  "IGETFIELD",
  "LGETFIELD",
  "FGETFIELD",
  "DGETFIELD",
  "BPUTFIELD",
  "SPUTFIELD",
  "IPUTFIELD",
  "LPUTFIELD",
  "FPUTFIELD",
  "DPUTFIELD",
  "INC_BYTE",
  "INC_SHORT",
  "INC_INT",
  "INC_LONG",
  "CONVERT_BYTE_TO_INT",
  "CONVERT_SHORT_TO_INT",
  "CONVERT_BYTE_TO_LONG",
  "CONVERT_BYTE_TO_FLOAT",
  "CONVERT_BYTE_TO_DOUBLE",
  "CONVERT_SHORT_TO_BYTE",
  "CONVERT_SHORT_TO_LONG",
  "CONVERT_SHORT_TO_FLOAT",
  "CONVERT_SHORT_TO_DOUBLE",
  "CONVERT_LONG_TO_BYTE",
  "CONVERT_LONG_TO_SHORT",
  "CONVERT_FLOAT_TO_BYTE",
  "CONVERT_FLOAT_TO_SHORT",
  "CONVERT_DOUBLE_TO_BYTE",
  "CONVERT_DOUBLE_TO_SHORT",
  "CONVERT_BYTE_TO_SHORT",
  "MALLOC_OBJECT",
  "MALLOC_ARRAY",
  "MALLOC_STRING",
  "RETURN_VOID",
  "RETURN_VALUE",
  "LOAD",
  "LOAD_0",
  "LOAD_1",
  "LOAD_2",
  "LOAD_3",
  "STORE",
  "STORE_0",
  "STORE_1",
  "STORE_2",
  "STORE_3",
  "ADD_BYTE",
  "SUBTRACT_BYTE",
  "MULTIPLY_BYTE",
  "DIVIDE_BYTE",
  "REMAINDER_BYTE",
  "NEGATE_BYTE",
  "LEFT_SHIFT_BYTE",
  "RIGHT_SHIFT_BYTE",
  "RIGHT_SHIFT_UNSIGNED_BYTE",
  "BIT_AND_BYTE",
  "OR_BYTE ",
  "XOR_BYTE",
  "BCMP",
  "ADD_SHORT",
  "SUBTRACT_SHORT",
  "MULTIPLY_SHORT",
  "DIVIDE_SHORT",
  "REMAINDER_SHORT",
  "NEGATE_SHORT",
  "LEFT_SHIFT_SHORT",
  "RIGHT_SHIFT_SHORT",
  "RIGHT_SHIFT_UNSIGNED_SHORT",
  "BIT_AND_SHORT",
  "OR_SHORT ",
  "XOR_SHORT",
  "CMP_SHORT",
  "ASTORE",
  "AASTORE",
  "APUTFIELD",
  "INC_REF_COUNT",
  "DEC_REF_COUNT",
};
