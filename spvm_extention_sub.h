#ifndef SPVM_EXTENTION_SUB_H
#define SPVM_EXTENTION_SUB_H

#include "spvm_base.h"

void SPVM_EXTENTION_SUB_std__test_call1(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__test_call2(SPVM* spvm, SPVM_ENV* env);

void SPVM_EXTENTION_SUB_std__println(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__print_byte(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__print_short(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__print_int(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__print_long(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__print_float(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__print_double(SPVM* spvm, SPVM_ENV* env);
void SPVM_EXTENTION_SUB_std__sum_int(SPVM* spvm, SPVM_ENV* env);

#endif
