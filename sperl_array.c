#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity) {
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  SPerl_VALUE_T* values = SPerl_ALLOCATOR_UTIL_safe_malloc(array->capacity, sizeof(SPerl_VALUE_T));
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_maybe_extend(SPerl* sperl, SPerl_ARRAY* array) {
  int64_t length = array->length;
  assert(length >= 0);
  
  int64_t capacity = array->capacity;
  
  if (length >= capacity) {
    if (capacity > SIZE_MAX / 2) {
      SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
    }
    int64_t new_capacity = capacity * 2;
    array->values = SPerl_ALLOCATOR_UTIL_safe_realloc(array->values, new_capacity, sizeof(SPerl_VALUE_T));
    array->capacity = new_capacity;
  }
}

/* Function templates

This is function templates shared with array munipulate functions.
All functions must be follow bellow templates

%SUFFIX% : function name suffix
%TYPE% : type name

void SPerl_ARRAY_push_%SUFFIX%(SPerl* sperl, SPerl_ARRAY* array, %TYPE% value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int64_t length = array->length;
  
  *(%TYPE%*)&array->values[length] = value;
  array->length++;
}

%TYPE% SPerl_ARRAY_fetch_%SUFFIX%(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(%TYPE%*)&array->values[index];
}

void* SPerl_ARRAY_pop_%SUFFIX%(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length > 0);
  
  array->length--;
  
  return *(%TYPE%*)&array->values[array->length];
}

*/

// Start pointer implementation 
void SPerl_ARRAY_push_address(SPerl* sperl, SPerl_ARRAY* array, void* value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int64_t length = array->length;
  
  *(void**)&array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch_address(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(void**)&array->values[index];
}

void* SPerl_ARRAY_pop_address(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length > 0);
  
  array->length--;
  
  return *(void**)&array->values[array->length];
}
// End pointer implementation


void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array) {
  free(array->values);
  free(array);
}
