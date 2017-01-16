#ifndef SPERL_VM_H
#define SPERL_VM_H

#include "sperl_base.h"

// Field information
struct SPerl_vm {
  int32_t* operand_stack;
  int32_t* call_stack;
  SPerl_FRAME* frame_stack;
  int32_t operand_stack_capacity;
  int32_t operand_stack_base;
  int32_t operand_stack_top;
  int32_t call_stack_capacity;
  int32_t call_stack_base;
  int32_t call_stack_next;
  int32_t frame_next;
};

SPerl_VM* SPerl_VM_new(SPerl* sperl);
void SPerl_VM_call_sub(SPerl* sperl, SPerl_VM* vm, const char* sub_base_name);
SPerl_VM* SPerl_VM_extend_call_stack(SPerl* sperl, SPerl_VM* vm, int32_t extend);

#endif
