%code requires {
//#include "lex.h"
	extern char *yytext;
	extern int yylex (void);
#include "mystring.h"

static
void yyerror (char const *s) {
	fprintf (stderr, "%s\n", s);
}

static
bool for_init = false;

static
void* pool = false;
static
size_t pool_size = 0;
static
void* get_mem(size_t amt) {
	pool_size += amt;
	pool = realloc(pool, pool_size);
	return pool + pool_size - amt;
}

static
string make_string(const char* s, size_t len) {
	char* base = get_mem(len);
	memcpy(base, s, len);
	return (string) { .base = base, .len = len };
}

#include "output.h"

}

%define api.value.type union

%token RETURNS
%token CLOSURE
%token FOR_TYPES
%token INIT
%token TYPE
%token NAME
%token END_FOR_TYPES
%token OPEN_COMMENT
%token CLOSE_COMMENT
%token LINE_COMMENT
%token EOL
%token SPACE
%token <char> CHAR

%type <string> delimiter
%type <string> space

%define api.token.prefix {TOK_}
%define parse.error verbose
%define parse.lac full

%defines
%token-table

%%

input:
  %empty
| RETURNS input { output_return_type(); }
| CLOSURE input { output_closure_name(); }
| for_statement input 
| CHAR input { output_char($1); }
| space input { output_space($1); }
;

space:
  OPEN_COMMENT space CLOSE_COMMENT space { $$ = make_string(yytext, yylen); }
| LINE_COMMENT "[^\n]+" EOL space { $$ = make_string(yytext, yylen); }
| SPACE { $$ = make_string(yytext, yylen); }
;

for_statement:
  space FOR_TYPES maybe_init
;

maybe_init:
  space INIT { for_init = true; } space end_for_statement
| space { for_init = false; } end_for_statement
;

end_for_statement:
  space TYPE space NAME space delimiter END_FOR_TYPES
  { output_for_types(for_init,true,true,$6); }
| space NAME space delimiter  END_FOR_TYPES
  { output_for_types(for_init,false,true,$4); }
| space TYPE space delimiter  END_FOR_TYPES
  { output_for_types(for_init,true,false,$4); }
;

delimiter: "[^ \t\n]+" "[ \t\n]*" { $$ = make_string(yytext, yylen); };

%%
