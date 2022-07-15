%option noyywrap
%{
#include "emp_front_end.h"
#include "emp.tab.h"
%}

%%
[ \t\r\n] {/*ignore*/}
"message"                   {return MESSAGE;}
"is"                        {return IS;}
"report"                    {return REPORT;}
"ram"                       {return RAM;}
"record"                    {return RECORD;}
"output"                    {return OUTPUT;}
":"                         {return DEF;}
"&"                         {return COMBINE;}
"="                         {return EQ;}
[a-zA-Z][a-zA-Z_0-9]*       {yylval.id = copyStr(yytext); return ID;}
[0-9]+h'[0-9A-Fa-f]+        {yylval.id = copyStr(yytext); return NUMBER;}
[0-9]+b'[01]+               {yylval.id = copyStr(yytext); return NUMBER;}
[0-9]+d'[0-9]+              {yylval.id = copyStr(yytext); return NUMBER;}
[0-9]+(\([0-9](:[0-9])?\))? {yylval.id = copyStr(yytext); return BIT_FIELD;}
.                           {printf("Unrecognized Symbol: %s",yytext);}
%%
