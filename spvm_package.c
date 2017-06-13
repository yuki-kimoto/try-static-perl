#include "spvm_package.h"
#include "spvm_.h"
#include "spvm_parser_allocator.h"
#include "spvm_constant_pool.h"
#include "spvm_parser.h"

SPVM_PACKAGE* SPVM_PACKAGE_new(SPVM_PARSER* parser) {
  SPVM_PACKAGE* package = SPVM_PARSER_ALLOCATOR_alloc_memory_pool(parser, parser->allocator, sizeof(SPVM_PACKAGE));
  
  package->op_subs = SPVM_PARSER_ALLOCATOR_alloc_array(parser, parser->allocator, 0);
  package->op_fields = SPVM_PARSER_ALLOCATOR_alloc_array(parser, parser->allocator, 0);
  
  return package;
}
