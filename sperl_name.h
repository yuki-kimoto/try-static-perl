#ifndef SPERL_NAME_H
#define SPERL_NAME_H

#include "sperl_base.h"

enum {
  SPerl_NAME_C_CODE_SUB,
  SPerl_NAME_C_CODE_FIELD,
};

extern SPerl_char* const SPerl_NAME_C_CODE_NAMES[];

struct SPerl_name {
  SPerl_int code;
  SPerl_OP* op_var;
  SPerl_OP* op_name;
  SPerl_boolean anon;
  SPerl_char* abs_name;
};

SPerl_NAME* SPerl_NAME_new(SPerl_PARSER* parser);

#endif
