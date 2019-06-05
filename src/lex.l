%{
#include "grammar.tab.h"
%}
%option noyywrap
%%
RETURNS { return TOK_RETURNS; }
CLOSURE { return TOK_CLOSURE; }
FOR_TYPES { return TOK_FOR_TYPES; }
INIT { return TOK_INIT; }
TYPE { return TOK_TYPE; }
NAME { return TOK_NAME; }
END_FOR_TYPES { return TOK_END_FOR_TYPES; }
"/*" { return TOK_OPEN_COMMENT; }
"*/" { return TOK_CLOSE_COMMENT; }
"//" { return TOK_LINE_COMMENT; }
\n { return TOK_EOL; }
[ \t\n] { return TOK_SPACE; }
. { return TOK_CHAR; }
