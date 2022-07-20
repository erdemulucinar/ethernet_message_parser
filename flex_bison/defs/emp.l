%option noyywrap
%{
#include "emp_data_structures.h"
#include "emp_util.h"
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
[a-zA-Z][a-zA-Z_0-9]*       {
                                char *str;
                                str = malloc(strlen(yytext)*sizeof(char));
                                strcpy(str,yytext);
                                yylval.id = str;
                                return ID;
                            }
[1-9]+[0-9]*h'[0-9A-Fa-f]+  {
                                yylval.val = hex2num(yytext);
                                return NUMBER;
                            }
[1-9]+[0-9]*b'[01]+         {
                                yylval.val = bin2num(yytext);
                                return NUMBER;
                            }
[1-9]+[0-9]*d'[0-9]+        {
                                yylval.val = dec2num(yytext);
                                return NUMBER;
                            }
[0-9]+(\([0-9](:[0-9])?\))? {
                                yylval.bitField = collectBitField(yytext);
                                return BIT_FIELD;
                            }
.                           {printf("Unrecognized Symbol: %s",yytext);}
%%
