#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_method_info.h"
#include "sperl_descripter.h"
#include "sperl_argument_info.h"
#include "sperl_class_info.h"
#include "sperl_parser.h"
#include "sperl_const_info.h"

SPerl_OP* SPerl_OP_newOP_CONST(SPerl_yy_parser* parser, SPerl_OP* op) {
  
  SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)op->uv.ptr_value;
  SPerl_ARRAY_push(parser->current_const_infos, const_info);
  
  switch(const_info->type) {
    case SPerl_CONST_INFO_BOOLEAN:
      printf("CONST(boolean %d) -> term\n", const_info->uv.boolean_value);
      break;
    case SPerl_CONST_INFO_CHAR:
      printf("CONST(char %c) -> term\n", const_info->uv.char_value);
      break;
    case SPerl_CONST_INFO_BYTE:
      printf("CONST(byte %d) -> term\n", const_info->uv.byte_value);
      break;
    case SPerl_CONST_INFO_SHORT:
      printf("CONST(short %d) -> term\n", const_info->uv.short_value);
      break;
    case SPerl_CONST_INFO_INT:
      printf("CONST(int %d) -> term\n", const_info->uv.int_value);
      break;
    case SPerl_CONST_INFO_LONG:
      printf("CONST(long %ld) -> term\n", const_info->uv.long_value);
      break;
    case SPerl_CONST_INFO_FLOAT:
      printf("CONST(float %f) -> term\n", const_info->uv.float_value);
      break;
    case SPerl_CONST_INFO_DOUBLE:
      printf("CONST(double %f) -> term\n", const_info->uv.double_value);
      break;
    case SPerl_CONST_INFO_STRING: {
      printf("CONST(string %s) -> term\n", const_info->uv.string_value);
      break;
    }
  }
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_PACKAGE(SPerl_yy_parser* parser, SPerl_OP* op_pkgname, SPerl_OP* op_block, SPerl_OP* op_descripters) {
  SPerl_OP* op_package = SPerl_OP_newOP(SPerl_OP_PACKAGE, op_pkgname, op_block);
  if (op_descripters) {
    SPerl_OP_sibling_splice(op_package, op_package->first, 0, op_descripters);
  }
  
  SPerl_CLASS_INFO* class_info = SPerl_CLASS_INFO_new();
  class_info->name = op_pkgname->uv.string_value;
  class_info->op_block = op_block;
  
  // Set method infomations
  class_info->method_infos = parser->current_method_infos;
  parser->current_method_infos = SPerl_ARRAY_new(0);
  
  // Set constant infos
  SPerl_ARRAY* const_infos = parser->current_const_infos;
  class_info->const_infos = const_infos;
  parser->current_const_infos = SPerl_ARRAY_new(0);
  
  // Create constant pool
  SPerl_int* const_pool = parser->const_pool;
  SPerl_int i;
  for (i = 0; i < const_infos->length; i++) {
    SPerl_CONST_INFO* const_info = SPerl_ARRAY_fetch(const_infos, i);
    const_info->pool_pos = parser->const_pool_pos;
    switch(const_info->type) {
      case SPerl_CONST_INFO_BOOLEAN:
        *(const_pool + parser->const_pool_pos) = (SPerl_int)const_info->uv.boolean_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_CHAR:
        *(const_pool + parser->const_pool_pos) = (SPerl_int)const_info->uv.char_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_BYTE:
        *(const_pool + parser->const_pool_pos) = (SPerl_int)const_info->uv.byte_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_SHORT:
        *(const_pool + parser->const_pool_pos) = (SPerl_int)const_info->uv.short_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_INT:
        *(const_pool + parser->const_pool_pos) = const_info->uv.int_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_LONG:
        *(SPerl_long*)(const_pool + parser->const_pool_pos) = const_info->uv.long_value;
        parser->const_pool_pos += 2;
        break;
      case SPerl_CONST_INFO_FLOAT:
        *(SPerl_float*)(const_pool + const_info->pool_pos) = const_info->uv.float_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_DOUBLE:
        *(SPerl_double*)(const_pool + const_info->pool_pos) = const_info->uv.double_value;
        parser->const_pool_pos += 2;
        break;
      case SPerl_CONST_INFO_STRING:
        strcpy((SPerl_char*)(const_pool + const_info->pool_pos), const_info->uv.string_value);
        parser->const_pool_pos += (SPerl_int)(((strlen(const_info->uv.string_value) + 1) + 3) / sizeof(SPerl_int));
        break;
    }
  }
  
  // Add class information
  SPerl_ARRAY_push(parser->class_infos, class_info);
  
  return op_package;
}

SPerl_char SPerl_OP_create_desc_flags(SPerl_OP* op_descripters) {
  
  if (!op_descripters) {
    return 0;
  }
  
  SPerl_char desc_flags = 0;
  
  // descripters
  // descripters is list of descripter
  if (op_descripters->type == SPerl_OP_LIST) {
    SPerl_OP* op_next = op_descripters->first;
    while (op_next = SPerl_OP_sibling(op_next)) {
      desc_flags |= SPerl_DESCRIPTER_get_flag(op_next->uv.string_value);
    }
  }
  // descripters is descripter
  else if (op_descripters->type == SPerl_OP_CONST) {
    desc_flags |= SPerl_DESCRIPTER_get_flag(op_descripters->uv.string_value);
  }
  
  return desc_flags;
}

SPerl_ARGUMENT_INFO* SPerl_OP_create_argument_info(SPerl_OP* op_subarg) {
  SPerl_ARGUMENT_INFO* argument_info = SPerl_ARGUMENT_INFO_new();
  
  // subarg
  // subarg is VAR, desctype
  argument_info->name = op_subarg->first->uv.string_value;
  SPerl_OP* op_desctype = op_subarg->last;
  
  // type
  argument_info->type = op_desctype->first->uv.string_value;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  argument_info->desc_flags |= SPerl_OP_create_desc_flags(op_descripters);
  
  return argument_info;
}


SPerl_OP* SPerl_OP_newOP_SUB(SPerl_yy_parser* parser, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block) {
  
  // Create OP_SUB
  if (!op_optsubargs) {
    op_optsubargs = SPerl_OP_newOP(SPerl_OP_NULL, NULL, NULL);
  }
  SPerl_OP* op = SPerl_OP_newOP(SPerl_OP_SUB, op_subname, op_optsubargs);
  SPerl_OP_sibling_splice(op, op_optsubargs, 0, op_desctype);
  SPerl_OP_sibling_splice(op, op_desctype, 0, op_block);
  
  // Create method infomation
  SPerl_METHOD_INFO* method_info = SPerl_METHOD_INFO_new();
  method_info->name = op_subname->uv.string_value;
  
  // subargs
  // subargs is NULL
  if (op_optsubargs->type == SPerl_OP_NULL) {
    method_info->argument_count = 0;
  }
  // subargs is subarg
  else if (op_optsubargs->type == SPerl_OP_SUBARG) {
    
    // Argument count
    method_info->argument_count = 1;
    
    // Create argument information
    SPerl_ARGUMENT_INFO* argument_info = SPerl_OP_create_argument_info(op_optsubargs);
    
    // Add arugment information
    SPerl_ARRAY_push(method_info->argument_infos, argument_info);
  }
  // subargs is list of subarg
  else if (op_optsubargs->type == SPerl_OP_LIST) {
    SPerl_int argument_count = 0;
    SPerl_OP* op_subarg = op_optsubargs->first;
    while (op_subarg = SPerl_OP_sibling(op_subarg)) {
      // Increment argument count
      argument_count++;
      
      // Creat argument information
      SPerl_ARGUMENT_INFO* argument_info = SPerl_OP_create_argument_info(op_subarg);

      // Add arugment information
      SPerl_ARRAY_push(method_info->argument_infos, argument_info);
    }
    method_info->argument_count = argument_count;
  }
  
  // return type
  method_info->return_type = op_desctype->first->uv.string_value;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  method_info->desc_flags |= SPerl_OP_create_desc_flags(op_descripters);
  
  // Save block
  method_info->op_block = op_block;
  
  // Add method information
  SPerl_ARRAY_push(parser->current_method_infos, method_info);
  
  return op;
}

void SPerl_OP_dump_ast(SPerl_OP* op, SPerl_int depth) {
  
  SPerl_int i;
  for (i = 0; i < depth; i++) {
    printf(" ");
  }
  SPerl_int type = op->type;
  printf("%s", SPerl_op_name[type]);
  if (type == SPerl_OP_CONST) {
    SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)op->uv.ptr_value;
    switch(const_info->type) {
      case SPerl_CONST_INFO_BOOLEAN:
        printf(" boolean %d", const_info->uv.boolean_value);
        break;
      case SPerl_CONST_INFO_CHAR:
        printf(" char %c", const_info->uv.char_value);
        break;
      case SPerl_CONST_INFO_BYTE:
        printf(" byte %d", const_info->uv.byte_value);
        break;
      case SPerl_CONST_INFO_SHORT:
        printf(" short %d", const_info->uv.short_value);
        break;
      case SPerl_CONST_INFO_INT:
        printf(" int %d", const_info->uv.int_value);
        break;
      case SPerl_CONST_INFO_LONG:
        printf(" long %ld", const_info->uv.long_value);
        break;
      case SPerl_CONST_INFO_FLOAT:
        printf(" float %f", const_info->uv.float_value);
        break;
      case SPerl_CONST_INFO_DOUBLE:
        printf(" double %f", const_info->uv.double_value);
        break;
      case SPerl_CONST_INFO_STRING:
        printf(" string \"%s\"", const_info->uv.string_value);
        break;
    }
  }
  else if (type == SPerl_OP_VAR) {
    printf(" \"%s\"", op->uv.string_value);
  }
  printf("\n");
  
  if (op->first) {
    depth++;
    SPerl_OP_dump_ast(op->first, depth);
    depth--;
  }
  
  if (op->moresib) {
    SPerl_OP_dump_ast(SPerl_OP_sibling(op), depth);
  }
}

SPerl_OP* SPerl_OP_sibling(SPerl_OP* op) {
  return op->moresib ? op->sibparent : NULL;
}

void SPerl_OP_moresib_set(SPerl_OP* op, SPerl_OP* sib) {
  op->moresib = 1;
  op->sibparent = sib;
}

void SPerl_OP_lastsib_set(SPerl_OP* op, SPerl_OP* parent) {
  op->moresib = 0;
  op->sibparent = parent;
}

void SPerl_OP_maybesib_set(SPerl_OP* op, SPerl_OP* sib, SPerl_OP* parent) {
  op->moresib = sib ? 1 : 0;
  op->sibparent = op->moresib ? sib : parent;
}

SPerl_OP* SPerl_OP_sibling_splice(SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP* insert) {
  SPerl_OP *first;
  SPerl_OP *rest;
  SPerl_OP *last_del = NULL;
  SPerl_OP *last_ins = NULL;

  if (start)
    first = SPerl_OP_sibling(start);
  else if (!parent)
    goto no_parent;
  else
    first = parent->first;

  if (del_count && first) {
    last_del = first;
    while (--del_count && last_del->moresib)
      last_del = SPerl_OP_sibling(last_del);
    rest = SPerl_OP_sibling(last_del);
    SPerl_OP_lastsib_set(last_del, NULL);
  }
  else
    rest = first;

  if (insert) {
    last_ins = insert;
    while (last_ins->moresib) {
      last_ins = SPerl_OP_sibling(last_ins);
    }
    SPerl_OP_maybesib_set(last_ins, rest, NULL);
  }
  else {
    insert = rest;
  }

  if (start) {
    SPerl_OP_maybesib_set(start, insert, NULL);
  }
  else {
    if (!parent)
      goto no_parent;
    parent->first = insert;
  }
  
  if (!rest) {
    /* update last etc */
    SPerl_OP *lastop;

    if (!parent)
      goto no_parent;

    lastop = last_ins ? last_ins : start ? start : NULL;
    parent->last = lastop;

    if (lastop)
      SPerl_OP_lastsib_set(lastop, parent);
  }
  return last_del ? first : NULL;

  no_parent:
    fprintf(stderr, "panic: op_sibling_splice(): NULL parent");
    exit(1);
}

SPerl_OP* SPerl_OP_append_elem(SPerl_OP *first, SPerl_OP *last) {
  if (!first)
    return last;

  if (!last)
    return first;
  
  if (first->type == SPerl_OP_LIST) {
    SPerl_OP_sibling_splice(first, first->last, 0, last);
    return first;
  }
  else {
    SPerl_OP* op_list = SPerl_OP_newOP_LIST();
    SPerl_OP_sibling_splice(op_list, op_list->first, 0, first);
    SPerl_OP_sibling_splice(op_list, first, 0, last);
    
    return op_list;
  }
}

SPerl_OP* SPerl_OP_newOP_LIST() {
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(SPerl_OP_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(SPerl_OP_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP(SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(type, first, last, 0, 0);
}

SPerl_OP* SPerl_OP_newOP_flag(SPerl_char type, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op;

  op = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1);
  memset(op, 0, sizeof(SPerl_OP));
  
  op->type = type;
  op->first = first;
  op->flags = flags;
  op->private = private;
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1 );
      first->type = SPerl_OP_NULL;
    }
    
    op->last = last;
    SPerl_OP_moresib_set(first, last);
    if (op->last)
      SPerl_OP_lastsib_set(op->last, (SPerl_OP*)op);
  }
  else if (first) {
    SPerl_OP_lastsib_set(op->first, (SPerl_OP*)op);
  }

  return (SPerl_OP *)op;
}
