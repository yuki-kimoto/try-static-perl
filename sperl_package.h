#ifndef SPERL_PACKAGE_H
#define SPERL_PACKAGE_H

#include "sperl_base.h"

struct SPerl_package {
  SPerl_OP* op_name;
  SPerl_OP* op_type;
  SPerl_ARRAY* op_fields;
  SPerl_ARRAY* op_subs;
  int32_t byte_size;
  int32_t constant_pool_address;
  int32_t name_constant_pool_address;
};

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl* sperl);

#endif
