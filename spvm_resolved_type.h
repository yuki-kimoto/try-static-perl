#ifndef SPVM_RESOLVED_TYPE_H
#define SPVM_RESOLVED_TYPE_H

#include "spvm_base.h"

enum {
  SPVM_RESOLVED_TYPE_C_CORE_LENGTH = 14
};

enum {
  SPVM_RESOLVED_TYPE_C_ID_BYTE,
  SPVM_RESOLVED_TYPE_C_ID_SHORT,
  SPVM_RESOLVED_TYPE_C_ID_INT,
  SPVM_RESOLVED_TYPE_C_ID_LONG,
  SPVM_RESOLVED_TYPE_C_ID_FLOAT,
  SPVM_RESOLVED_TYPE_C_ID_DOUBLE,
  SPVM_RESOLVED_TYPE_C_ID_STRING,
  SPVM_RESOLVED_TYPE_C_ID_ARRAY_BYTE,
  SPVM_RESOLVED_TYPE_C_ID_ARRAY_SHORT,
  SPVM_RESOLVED_TYPE_C_ID_ARRAY_INT,
  SPVM_RESOLVED_TYPE_C_ID_ARRAY_LONG,
  SPVM_RESOLVED_TYPE_C_ID_ARRAY_FLOAT,
  SPVM_RESOLVED_TYPE_C_ID_ARRAY_DOUBLE,
  SPVM_RESOLVED_TYPE_C_ID_ARRAY_STRING,
};

extern const char* const SPVM_RESOLVED_TYPE_C_CORE_NAMES[];

struct SPVM_resolved_type {
  SPVM_ARRAY* part_names;
  const char* name;
  int32_t name_length;
  int32_t id;
  int32_t code;
};

SPVM_RESOLVED_TYPE* SPVM_RESOLVED_TYPE_new(SPVM* spvm);
_Bool SPVM_RESOLVED_TYPE_is_array(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type);
_Bool SPVM_RESOLVED_TYPE_contain_sub(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type);
_Bool SPVM_RESOLVED_TYPE_is_integral(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type);
_Bool SPVM_RESOLVED_TYPE_is_core_value_array(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type);
_Bool SPVM_RESOLVED_TYPE_is_numeric(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type);
int32_t SPVM_RESOLVED_TYPE_get_array_dimention(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type);

#endif
