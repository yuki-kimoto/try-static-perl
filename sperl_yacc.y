%pure-parser
%parse-param	{ SPerl* sperl }
%lex-param	{ SPerl* sperl }

%{
  #include <stdio.h>
  
  #include "sperl.h"
  #include "sperl_parser.h"
  #include "sperl_yacc_util.h"
  #include "sperl_toke.h"
  #include "sperl_op.h"
  #include "sperl_dumper.h"
  #include "sperl_constant.h"
%}

%token <opval> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE MALLOC
%token <opval> LAST NEXT NAME VAR CONSTANT ENUM DESCRIPTOR CORETYPE UNDEF DIE
%token <opval> SWITCH CASE DEFAULT VOID TRY CATCH

%type <opval> grammar opt_statements statements statement my field if_statement else_statement
%type <opval> block enumeration_block package_block sub opt_declarations_in_package call_sub unop binop
%type <opval> opt_terms terms term args arg opt_args use declaration_in_package declarations_in_package
%type <opval> enumeration_values enumeration_value
%type <opval> type package_name field_name sub_name package declarations_in_grammar opt_enumeration_values type_array
%type <opval> for_statement while_statement expression opt_declarations_in_grammar opt_term throw_exception
%type <opval> call_field array_elem convert_type enumeration new_object type_name array_length declaration_in_grammar
%type <opval> switch_statement case_statement default_statement type_array_with_length
%type <opval> ';' opt_descriptors descriptors type_or_void normal_statement try_catch


%right <opval> ASSIGN
%left <opval> OR
%left <opval> AND
%left <opval> BIT_OR BIT_XOR
%left <opval> BIT_AND
%nonassoc <opval> REL
%left <opval> SHIFT
%left <opval> '+' '-'
%left <opval> MULTIPLY DIVIDE REMAINDER
%right <opval> NOT '~' ARRAY_LENGTH UMINUS
%nonassoc <opval> INC DEC
%left <opval> ARROW
%nonassoc <opval> ')'
%left <opval> '('
%left <opval> '[' '{'

%%

grammar
  : opt_declarations_in_grammar
    {
      $$ = SPerl_OP_build_grammar(sperl, $1);

      // Syntax error
      if (sperl->parser->error_count) {
        YYABORT;
      }
      else {
        // Dump sperl information
        SPerl_DUMPER_dump_sperl(sperl);
      }
    }

opt_declarations_in_grammar
  :	/* Empty */
    {
      $$ = SPerl_OP_new_op_list(sperl, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  |	declarations_in_grammar
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_new_op_list(sperl, $1->file, $1->line);
        SPerl_OP_sibling_splice(sperl, $$, $$->first, 0, $1);
      }
    }
  
declarations_in_grammar
  : declarations_in_grammar declaration_in_grammar
    {
      $$ = SPerl_OP_append_elem(sperl, $1, $2, $1->file, $1->line);
    }
  | declaration_in_grammar

declaration_in_grammar
  : use
  | package

use
  : USE package_name ';'
    {
      $$ = SPerl_OP_build_use(sperl, $1, $2);
    }

package
  : PACKAGE package_name package_block
    {
      $$ = SPerl_OP_build_package(sperl, $1, $2, $3);
      if (sperl->parser->fatal_error) {
        YYABORT;
      }
    }

opt_declarations_in_package
  :	/* Empty */
    {
      $$ = SPerl_OP_new_op_list(sperl, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  |	declarations_in_package
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_new_op_list(sperl, $1->file, $1->line);
        SPerl_OP_sibling_splice(sperl, $$, $$->first, 0, $1);
      }
    }

declarations_in_package
  : declarations_in_package declaration_in_package
    {
      $$ = SPerl_OP_append_elem(sperl, $1, $2, $1->file, $1->line);
    }
  | declaration_in_package

declaration_in_package
  : field
  | sub
  | enumeration

package_block
  : '{' opt_declarations_in_package '}'
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CLASS_BLOCK, $1->file, $1->line);
      SPerl_OP_sibling_splice(sperl, $$, NULL, 0, $2);
    }

enumeration_block 
  : '{' opt_enumeration_values '}'
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_ENUM_BLOCK, $1->file, $1->line);
      SPerl_OP_sibling_splice(sperl, $$, NULL, 0, $2);
    }

opt_enumeration_values
  :	/* Empty */
    {
      $$ = SPerl_OP_new_op_list(sperl, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  |	enumeration_values
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_new_op_list(sperl, $1->file, $1->line);
        SPerl_OP_sibling_splice(sperl, $$, $$->first, 0, $1);
      }
    }
    
enumeration_values
  : enumeration_values ',' enumeration_value 
    {
      $$ = SPerl_OP_append_elem(sperl, $1, $3, $1->file, $1->line);
    }
  | enumeration_value
  
enumeration_value
  : NAME
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_DECL_ENUMERATION_VALUE, $1->file, $1->line);
      SPerl_OP_sibling_splice(sperl, $$, NULL, 0, $1);
    }
  | NAME ASSIGN CONSTANT
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_DECL_ENUMERATION_VALUE, $1->file, $1->line);
      SPerl_OP_sibling_splice(sperl, $$, NULL, 0, $1);
      SPerl_OP_sibling_splice(sperl, $$, $1, 0, $3);
    }

throw_exception
  : DIE term
    {
      SPerl_OP_sibling_splice(sperl, $1, NULL, 0, $2);
      $$ = $1;
    }

opt_statements
  :	/* Empty */
    {
      $$ = SPerl_OP_new_op_list(sperl, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  |	statements
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_new_op_list(sperl, $1->file, $1->line);
        SPerl_OP_sibling_splice(sperl, $$, $$->first, 0, $1);
      }
    }
    
statements
  : statements statement 
    {
      $$ = SPerl_OP_append_elem(sperl, $1, $2, $1->file, $1->line);
    }
  | statement

statement
  : normal_statement
  | if_statement
  | for_statement
  | while_statement
  | block
  | switch_statement
  | case_statement
  | default_statement
  | try_catch

block 
  : '{' opt_statements '}'
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_BLOCK, $1->file, $1->line);
      SPerl_OP_sibling_splice(sperl, $$, NULL, 0, $2);
    }

normal_statement
  : term ';'
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_POP, $1->file, $1->line);
      SPerl_OP_sibling_splice(sperl, $$, NULL, 0, $1);
    }
  | expression ';'
  | ';'
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NULL, $1->file, $1->line);
    }

for_statement
  : FOR '(' normal_statement term ';' opt_term ')' block
    {
      $$ = SPerl_OP_build_for_statement(sperl, $1, $3, $4, $6, $8);
    }

while_statement
  : WHILE '(' term ')' block
    {
      $$ = SPerl_OP_build_while_statement(sperl, $1, $3, $5);
    }

switch_statement
  : SWITCH '(' term ')' block
    {
      $$ = SPerl_OP_build_switch_statement(sperl, $1, $3, $5);
    }

case_statement
  : CASE term ':'
    {
      $$ = SPerl_OP_build_case_statement(sperl, $1, $2);
    }

default_statement
  : DEFAULT ':'

if_statement
  : IF '(' term ')' block else_statement
    {
      $$ = SPerl_OP_build_if_statement(sperl, $1, $3, $5, $6);
    }

else_statement
  : /* NULL */
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NULL, sperl->parser->cur_module_path, sperl->parser->cur_line);
    };
  | ELSE block
    {
      $2->flag |= SPerl_OP_C_FLAG_BLOCK_ELSE;
      $$ = $2;
    }
  | ELSIF '(' term ')' block else_statement
    {
      $$ = SPerl_OP_build_if_statement(sperl, $1, $3, $5, $6);
    }

field
  : HAS field_name ':' type ';'
    {
      $$ = SPerl_OP_build_field(sperl, $1, $2, $4);
    }

sub
 : SUB sub_name '(' opt_args ')' ':' opt_descriptors type_or_void block
     {
       $$ = SPerl_OP_build_sub(sperl, $1, $2, $4, $7, $8, $9);
     }
 | SUB sub_name '(' opt_args ')' ':' opt_descriptors type_or_void ';'
     {
       $$ = SPerl_OP_build_sub(sperl, $1, $2, $4, $7, $8, NULL);
     }
enumeration
  : ENUM enumeration_block
    {
      $$ = SPerl_OP_build_enumeration(sperl, $1, $2);
    }

my
  : MY VAR ':' type
    {
      $$ = SPerl_OP_build_my_var(sperl, $1, $2, $4, NULL);
    }
  | MY VAR
    {
      $$ = SPerl_OP_build_my_var(sperl, $1, $2, NULL, NULL);
    }
  | MY VAR ':' type ASSIGN term
    {
      $$ = SPerl_OP_build_my_var(sperl, $1, $2, $4, $6);
    }
  | MY VAR ASSIGN term
    {
      $$ = SPerl_OP_build_my_var(sperl, $1, $2, NULL, $4);
    }

expression
  : LAST
  | NEXT
  | RETURN
  | RETURN term
    {
      $$ = $1;
      SPerl_OP_sibling_splice(sperl, $$, NULL, 0, $2);
    }
  | throw_exception
  | my

opt_terms
  :	/* Empty */
    {
      $$ = SPerl_OP_new_op_list(sperl, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  |	terms
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_new_op_list(sperl, $1->file, $1->line);
        SPerl_OP_sibling_splice(sperl, $$, $$->first, 0, $1);
      }
    }
    
terms
  : terms ',' term
    {
      $$ = SPerl_OP_append_elem(sperl, $1, $3, $1->file, $1->line);
    }
  | term

array_length
  : ARRAY_LENGTH term
    {
      $$ = SPerl_OP_build_array_length(sperl, $1, $2);
    }

opt_term
  : /* NULL */
    {
      $$ = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NULL, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  | term

term
  : VAR
  | CONSTANT
  | UNDEF
  | call_sub
  | call_field
  | unop
  | binop
  | array_elem
  | convert_type
  | new_object
  | array_length

new_object
  : MALLOC type_name
    {
      $$ = SPerl_OP_build_malloc_object(sperl, $1, $2);
    }
  | MALLOC type_array_with_length
    {
      $$ = SPerl_OP_build_malloc_object(sperl, $1, $2);
    }

convert_type
  : '(' type ')' term
    {
      $$ = SPerl_OP_build_convert_type(sperl, $2, $4);
    }

call_field
  : term '{' field_name '}'
    {
      $$ = SPerl_OP_build_call_field(sperl, $1, $3);
    }

unop
  : '+' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_PLUS, $1->file, $1->line);
      $$ = SPerl_OP_build_unop(sperl, op, $2);
    }
  | '-' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NEGATE, $1->file, $1->line);
      $$ = SPerl_OP_build_unop(sperl, op, $2);
    }
  | INC term
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_PRE_INC, $1->file, $1->line);
      $$ = SPerl_OP_build_unop(sperl, op, $2);
    }
  | term INC
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_POST_INC, $2->file, $2->line);
      $$ = SPerl_OP_build_unop(sperl, op, $1);
    }
  | DEC term
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_PRE_DEC, $1->file, $1->line);
      $$ = SPerl_OP_build_unop(sperl, op, $2);
    }
  | term DEC
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_POST_DEC, $2->file, $2->line);
      $$ = SPerl_OP_build_unop(sperl, op, $1);
    }
  | '~' term
    {
      $$ = SPerl_OP_build_unop(sperl, $1, $2);
    }
  | NOT term
    {
      $$ = SPerl_OP_build_unop(sperl, $1, $2);
    }

binop
  : term '+' term
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_ADD, $2->file, $2->line);
      $$ = SPerl_OP_build_binop(sperl, op, $1, $3);
    }
  | term '-' term
    {
      SPerl_OP* op = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_SUBTRACT, $2->file, $2->line);
      $$ = SPerl_OP_build_binop(sperl, op, $1, $3);
    }
  | term MULTIPLY term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term DIVIDE term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term REMAINDER term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term BIT_XOR term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term BIT_AND term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term BIT_OR term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term SHIFT term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term REL term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term ASSIGN term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | '(' term ')'
    {
      $$ = $2;
    }
  | term OR term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }
  | term AND term
    {
      $$ = SPerl_OP_build_binop(sperl, $2, $1, $3);
    }

array_elem
  : term '[' term ']'
    {
      $$ = SPerl_OP_build_array_elem(sperl, $1, $3);
    }

call_sub
  : sub_name '(' opt_terms  ')'
    {
      $$ = SPerl_OP_build_call_sub(sperl, SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NULL, $1->file, $1->line), $1, $3);
    }
  | term ARROW sub_name '(' opt_terms ')'
    {
      $$ = SPerl_OP_build_call_sub(sperl, $1, $3, $5);
    }
  | term ARROW sub_name
    {
      SPerl_OP* op_terms = SPerl_OP_new_op_list(sperl, $1->file, $2->line);
      $$ = SPerl_OP_build_call_sub(sperl, $1, $3, op_terms);
    }
  | package_name ARROW sub_name '(' opt_terms  ')'
    {
      $$ = SPerl_OP_build_call_sub(sperl, $1, $3, $5);
    }
  | package_name ARROW sub_name
    {
      SPerl_OP* op_terms = SPerl_OP_new_op_list(sperl, $1->file, $2->line);
      $$ = SPerl_OP_build_call_sub(sperl, $1, $3, op_terms);
    }

opt_args
  :	/* Empty */
    {
      $$ = SPerl_OP_new_op_list(sperl, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  |	args
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_new_op_list(sperl, $1->file, $1->line);
        SPerl_OP_sibling_splice(sperl, $$, $$->first, 0, $1);
      }
    }

args
  : args ',' arg
    {
      $$ = SPerl_OP_append_elem(sperl, $1, $3, $1->file, $1->line);
    }
  | arg

arg
  : VAR ':' type
    {
      $$ = SPerl_OP_build_my_var(sperl, SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_MY_VAR, $1->file, $1->line), $1, $3, NULL);
    }

opt_descriptors
  :	/* Empty */
    {
      $$ = SPerl_OP_new_op_list(sperl, sperl->parser->cur_module_path, sperl->parser->cur_line);
    }
  |	descriptors
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_new_op_list(sperl, $1->file, $1->line);
        SPerl_OP_sibling_splice(sperl, $$, $$->first, 0, $1);
      }
    }
    
descriptors
  : descriptors ',' DESCRIPTOR
    {
      $$ = SPerl_OP_append_elem(sperl, $1, $3, $1->file, $1->line);
    }
  | DESCRIPTOR

type
  : type_name
  | type_array


type_name
  : NAME
    {
      $$ = SPerl_OP_build_type_name(sperl, $1);
    }

try_catch
  : TRY block CATCH '(' arg ')' block
    {
      $$ = SPerl_OP_build_try_catch(sperl, $1, $2, $3, $5, $7);
    }

type_array
  : type_name '[' ']'
    {
      $$ = SPerl_OP_build_type_array(sperl, $1, NULL);
    }

type_array_with_length
  : type_name '[' term ']'
    {
      $$ = SPerl_OP_build_type_array(sperl, $1, $3);
    }

type_or_void
  : type
  | VOID

field_name : NAME
sub_name : NAME
package_name : NAME

%%

