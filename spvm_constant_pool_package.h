#ifndef SPVM_CONSTANT_POOL_PACKAGE_H
#define SPVM_CONSTANT_POOL_PACKAGE_H

#include "spvm_base.h"

// Array
struct SPVM_constant_pool_package {
  int32_t name_constant_pool_address;
  int32_t byte_size;
};

#endif