%{
#include <stdio.h>
#include <stdlib.h>
#include "emp_data_structures.h"
#include "emp_util.h"
int yylex(void);
void yyerror(LinkedList **msgList, const char *s);
%}

//Directives
%parse-param {LinkedList **msgList}

%union {
    //Lexer
    char *id; //An ID's c-string representation.
    char *val; //Binary representation of a match condition's value.
    BitField *bitField;
    //Parser
    LinkedList *bitFieldList;
    MsgLine *msgLine;
    Msg *msg;
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
%token <val> NUMBER             //Any number (Lexer transforms hex&dec to binary)
%token <bitField> BIT_FIELD     //Any bitfield as defined in "emp_data_structure.h"

%type <bitFieldList> field_vector_indices
%type <msgLine> msg_line
%type <msg> msg_line_list
%type <msg> msg_def

%%

msg_def_list:
    msg_def{
        if(!(*msgList)){
            *msgList = createLinkedList(&copyRef, &deleteMsg, &compareMsg);
        }
        appendLinkedList(*msgList, (void*) $1);
    }
    | msg_def_list msg_def{
        appendLinkedList(*msgList, (void*) $2);
    }
    ;
    
msg_def:
    MESSAGE ID IS ID DEF msg_line_list
    {
        $6->name = $2;
        $6->parent = $4;
        $$ = $6;
    }
    | MESSAGE ID DEF msg_line_list
    {
        $4->name = $2;
        $$ = $4;
    }
    ;

msg_line_list:
    msg_line
    {
        $$ = addLine(0, $1);
    }
    | msg_line_list msg_line
    {
        $$ = addLine($1, $2);
    }
    ;

msg_line:
    REPORT RAM ID //Report RAM interface
    {
        $$ = createReportInterface(REPORT_RAM, $3);
    }
    | REPORT RECORD ID //Report record interface
    {
        $$ = createReportInterface(REPORT_RECORD, $3);
    }
    | ID DEF field_vector_indices //Message field definition
    {
        $$ = createMsgField($1, $3);
    }
    | ID EQ NUMBER //Match condition definition
    {
        $$ = createMatchCondition($1, $3);
    }
    | OUTPUT ID //Report field definition
    {
        $$ = createReportField($2);
    }
    ;

field_vector_indices:
    BIT_FIELD
    {
        $$ = createLinkedList(&copyRef, &deleteBitField, &compareBitField);
        appendLinkedList($$, (void*) $1);
    }
    | field_vector_indices COMBINE BIT_FIELD
    {
        appendLinkedList($1, (void*) $3);
        $$ = $1;
    }
    ;

%%

void yyerror(LinkedList **msgList, char const *s) {
    fprintf(stderr, "%s\n", s);
}
