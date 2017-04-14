#include <stdlib.h>
#include <stdio.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_memory_pool.h"
#include "sperl_allocator_util.h"
#include "sperl_allocator_parser.h"
#include "sperl_parser.h"

SPerl_ALLOCATOR_PARSER* SPerl_ALLOCATOR_PARSER_new(SPerl* sperl) {
  SPerl_ALLOCATOR_PARSER* allocator = malloc(sizeof(SPerl_ALLOCATOR_PARSER));
  
  // Memory pool - memory pool save short strings and object, except array and hash
  // These datas are created at compile time
  allocator->memory_pool = SPerl_MEMORY_POOL_new(sperl, 0);
  
  // Arrays - these arrays are created at compile time
  allocator->arrays = SPerl_ARRAY_new(sperl, 0);
  
  // Hashed - these hashes are created at compile time
  allocator->hashes = SPerl_ARRAY_new(sperl, 0);
  
  // Long strings - these strings are created at compile time
  allocator->long_strings = SPerl_ARRAY_new(sperl, 0);
  
  return allocator;
}

void* SPerl_ALLOCATOR_PARSER_alloc_memory_pool(SPerl* sperl, SPerl_PARSER* parser, size_t size) {
  return SPerl_MEMORY_POOL_alloc(sperl, parser->memory_pool, size);
}

SPerl_ARRAY* SPerl_ALLOCATOR_PARSER_new_array(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator, size_t capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, allocator->arrays, array);
  
  return array;
}

SPerl_HASH* SPerl_ALLOCATOR_PARSER_new_hash(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator, size_t capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(sperl, capacity);
  
  SPerl_ARRAY_push(sperl, allocator->hashes, hash);
  
  return hash;
}

int32_t* SPerl_ALLOCATOR_PARSER_new_int(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator) {
  int32_t* value = SPerl_MEMORY_POOL_alloc(sperl, allocator->memory_pool, sizeof(int32_t));
  
  return value;
}

char* SPerl_ALLOCATOR_PARSER_new_string(SPerl* sperl,  SPerl_ALLOCATOR_PARSER* allocator, size_t length) {
  char* str;
  
  if (length < 0xFF) {
    str = (char*) SPerl_MEMORY_POOL_alloc(sperl, allocator->memory_pool, length + 1);
  }
  else {
    str = (char*)SPerl_ALLOCATOR_UTIL_safe_malloc_i32(length + 1, sizeof(char));
    SPerl_ARRAY_push(sperl, allocator->long_strings, str);
  }
  
  return str;
}

void SPerl_ALLOCATOR_PARSER_free(SPerl* sperl, SPerl_ALLOCATOR_PARSER* allocator) {
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl, allocator->memory_pool);
  
  // Free arrays
  for (int32_t i = 0, len = allocator->arrays->length; i < len; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(sperl, allocator->arrays, i);
    SPerl_ARRAY_free(sperl, array);
  }
  SPerl_ARRAY_free(sperl, allocator->arrays);
  
  // Free hashes
  for (int32_t i = 0, len = allocator->hashes->length; i < len; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(sperl, allocator->hashes, i);
    SPerl_HASH_free(sperl, hash);
  }
  SPerl_ARRAY_free(sperl, allocator->hashes);
  
  // Free long strings
  for (int32_t i = 0, len = allocator->long_strings->length; i < len; i++) {
    void* str = SPerl_ARRAY_fetch(sperl, allocator->long_strings, i);
    free(str);
  }
  SPerl_ARRAY_free(sperl, allocator->long_strings);
}
