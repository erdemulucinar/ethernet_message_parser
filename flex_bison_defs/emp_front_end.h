#ifndef EMP_H
#define EMP_H

#include "string.h"
#include <stdlib.h>
#include <stdio.h>

//A simple data structure to collect lines & messages.
typedef struct ListNode{
    void *data;
    struct ListNode *next;
} ListNode;

//Line Declarations
typedef enum {MSG_FIELD_DEF, REPORT_FIELD_DEF, CONTROL_FIELD_DEF, REPORT_INTERFACE} LineType;
typedef enum {REPORT_RAM, REPORT_RECORD} ReportInterfaceType;

typedef struct BitField{
    int byteNo;
    int *idx;
    int len;
} BitField;

typedef struct MsgFieldDef{ //Any field defined for a message (byte no & bits).
    char *name;
    ListNode *bitFields;
} MsgFieldDef;

typedef struct ReportFieldDef{ //A field which is declared as output.
    char *name;
} ReportFieldDef;

typedef struct ControlFieldDef{ //A control statement for a message (field=value).
    char *name;
    char *val;
} ControlFieldDef;

typedef struct ReportInterface{ //Report interface type (ram or record & name).
    ReportInterfaceType reportType;
    char *name;
} ReportInterface;

typedef struct Line{ //Any of the above information about a message.
    LineType lineType;
    void *data;
} Line;

typedef struct RawMsg{ //Information about a message. No checks or operations are
                       //done at this point.
    char *name;
    char *parent;
    ListNode *lineList;
} RawMsg;

//User interface functions
ListNode* collectRawMessages(char* file);

//List functions
void listAdd(ListNode *head, void *data);
void listCombine(ListNode *head1, ListNode *head2);

//Utility Functions
char* copyStr(char* str);
BitField* collectBitField(char *str);
int char2dec(char *str, int len);
char* num2bin(char *str);
char* hex2bin(char *str, int bitLen);
char* dec2bin(int num, int bitLen);
int hex2dec(char *str);

//Debug Functions
void printBitField(BitField *bitField);
void printBitFieldList(ListNode *node);
void printLine(Line* line);
void printLineList(ListNode *lineList);
void printRawMsg(RawMsg *msg);

#endif
