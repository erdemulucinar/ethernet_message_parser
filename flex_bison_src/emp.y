%{
#include <stdio.h>
#include <stdlib.h>
#include "emp_front_end.h"
int yylex(void);
void yyerror(ListNode **msgList, const char *s);
%}

//Directives
%parse-param {ListNode **msgList}

%union {
    //Lexer
    char *id; //An ID's c-string representation.
    //Parser
    ListNode *bitFieldList;
    Line *line;
    ListNode *msgLineList;
    RawMsg *msg;
}

//Keywords
%token MESSAGE                  //"message"
%token IS                       //"is"
%token REPORT                   //"report"
%token RAM                      //"ram"
%token RECORD                   //"record"
%token OUTPUT                   //"output"
//Operators
%token DEF                      //":"
%token COMBINE                  //"&"
%token EQ                       //"="
//Variables
%token <id> ID                  //Any string which starts with a letter
%token <id> NUMBER              //Any number (Lexer transforms hex&dec to binary)
%token <id> BIT_FIELD           //Any bitfield defined as in "emp.h"

%type <bitFieldList> field_vector_indices
%type <line> msg_line
%type <msgLineList> msg_line_list
%type <msg> msg_def

%%

msg_def_list:
    msg_def{
        *msgList = malloc(sizeof(ListNode));
        (*msgList)->data = (void*) $1;
        (*msgList)->next = 0;
    }
    | msg_def_list msg_def{
        listAdd(*msgList,(void*) $2);
    }
    ;
    
msg_def:
    MESSAGE ID IS ID DEF msg_line_list
    {
        $$ = malloc(sizeof(RawMsg));
        $$->name = $2;
        $$->parent = $4;
        $$->lineList = $6;
    }
    | MESSAGE ID DEF msg_line_list
    {
        $$ = malloc(sizeof(RawMsg));
        $$->name = $2;
        $$->parent = 0;
        $$->lineList = $4;
    }
    ;

msg_line_list:
    msg_line
    {
        $$ = malloc(sizeof(ListNode));
        $$->data = (void*) $1;
        $$->next = 0;
    }
    | msg_line_list msg_line
    {
        listAdd($1,(void*) $2);
        $$ = $1;
    }
    ;

msg_line:
    REPORT RAM ID //Report RAM interface
    {
        ReportInterface *reportInterface;
        reportInterface = malloc(sizeof(ReportInterface));
        reportInterface->reportType = REPORT_RAM;
        reportInterface->name = $3;
        
        $$ = malloc(sizeof(Line));
        $$->lineType = REPORT_INTERFACE;
        $$->data = (void*) reportInterface;
    }
    | REPORT RECORD ID //Report record interface
    {
        ReportInterface *reportInterface;
        reportInterface = malloc(sizeof(ReportInterface));
        reportInterface->reportType = REPORT_RECORD;
        reportInterface->name = $3;
        
        $$ = malloc(sizeof(Line));
        $$->lineType = REPORT_INTERFACE;
        $$->data = (void*) reportInterface;
    }
    | ID EQ NUMBER //Control field definition
    {
        ControlFieldDef *controlDef;
        controlDef = malloc(sizeof(ControlFieldDef));
        controlDef->name = $1;
        controlDef->val = num2bin($3);
        
        $$ = malloc(sizeof(Line));
        $$->lineType = CONTROL_FIELD_DEF;
        $$->data = (void*) controlDef;
    }
    | OUTPUT ID //Report field definition
    {
        ReportFieldDef *reportDef;
        reportDef = malloc(sizeof(ReportFieldDef));
        reportDef->name = $2;
        
        $$ = malloc(sizeof(Line));
        $$->lineType = REPORT_FIELD_DEF;
        $$->data = (void*) reportDef;
    }
    | ID DEF field_vector_indices //Message field definition
    {
        MsgFieldDef *fieldDef;
        fieldDef = malloc(sizeof(MsgFieldDef));
        fieldDef->name = $1;
        fieldDef->bitFields = $3;
        
        $$ = malloc(sizeof(Line));
        $$->lineType = MSG_FIELD_DEF;
        $$->data = (void*) fieldDef;
    }
    ;

field_vector_indices:
    BIT_FIELD
    {
        BitField *bitField;
        bitField = collectBitField($1);
        $$ = malloc(sizeof(ListNode));
        $$->data = (void*) bitField;
        $$->next = 0;
    }
    | field_vector_indices COMBINE BIT_FIELD
    {
        BitField *bitField;
        bitField = collectBitField($3);
        listAdd($1, (void*) bitField);
        $$ = $1;
    }
    ;

%%

void yyerror(ListNode **msgList, char const *s) {
    fprintf(stderr, "%s\n", s);
}
