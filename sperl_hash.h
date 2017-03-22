#ifndef SPERL_HASH_H
#define SPERL_HASH_H

#include "sperl_base.h"

// Hash table
struct SPerl_hash {
  SPerl_HASH_ENTRY** table;
  SPerl_HASH_ENTRY* entries;
  int64_t table_capacity;
  int64_t entries_capacity;
  int64_t entries_length;
};

// Hash function
SPerl_HASH* SPerl_HASH_new(SPerl* sperl, int64_t capacity);
void SPerl_HASH_free(SPerl* sperl, SPerl_HASH* hash);
void SPerl_HASH_maybe_extend_entries(SPerl* sperl, SPerl_HASH* hash);

int64_t SPerl_HASH_new_hash_entry_address(SPerl* sperl, SPerl_HASH* hash, const char* key, void* value);
void SPerl_HASH_rehash_address(SPerl* sperl, SPerl_HASH* hash, int64_t new_capacity);
void SPerl_HASH_insert_norehash_address(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, void* value);
void SPerl_HASH_insert_address(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, void* value);
void* SPerl_HASH_search_address(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length);

int64_t SPerl_HASH_new_hash_entry_long(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t value);
void SPerl_HASH_rehash_long(SPerl* sperl, SPerl_HASH* hash, int64_t new_table_capacity);
void SPerl_HASH_insert_norehash_long(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, int64_t value);
void SPerl_HASH_insert_long(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length, int64_t value);
int64_t SPerl_HASH_search_long(SPerl* sperl, SPerl_HASH* hash, const char* key, int64_t length);

#endif
