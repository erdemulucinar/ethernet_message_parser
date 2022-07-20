#ifndef EMP_DATA_STRUCTURES_H
#define EMP_DATA_STRUCTURES_H

#include "linked_list.h"
#include "string.h"

//Data Structures
typedef enum {REPORT_RAM, REPORT_RECORD} ReportInterfaceType;
typedef enum {REPORT_INTERFACE, MESSAGE_FIELD, MATCH_CONDITION, REPORT_FIELD} MsgLineType;

typedef struct BitField{
    int byteNo;
    int *bitIdx;
    int len;
} BitField;

typedef struct MsgField{
    char *name;
    LinkedList *bitFields; //BitField
} MsgField;

typedef struct MatchCondition{
    char *name;
    char *val;
} MatchCondition;

typedef struct ReportField{
    char *name;
} ReportField;

typedef struct ReportInterface{
    ReportInterfaceType interfaceType;
    char *interfaceName;
} ReportInterface;

typedef struct MsgLine{
    MsgLineType lineType;
    void *data;
} MsgLine;

typedef struct Msg{
    //ID of the message
    char *name;
    char *parent;
    //Report Interface of the message
    ReportInterface *reportInterface;
    //Message fields
    LinkedList *msgFields; //MsgField
    LinkedList *matchConditions; //MatchCondition
    LinkedList *reportFields; //ReportField
} Msg;

//Msg functions
Msg* addLine(Msg *msg, MsgLine *line);

//MsgLine creation functions
MsgLine* createReportInterface(ReportInterfaceType type, char *name);
MsgLine* createMsgField(char *name, LinkedList *bitFields);
MsgLine* createMatchCondition(char *name, char *val);
MsgLine* createReportField(char *name);

//Linked list template functions
void* copyRef(void *data); //Shallow copy will be used for all linked lists.

void deleteBitField(void *data);
void deleteMsgField(void *data);
void deleteMatchCondition(void *data);
void deleteReportField(void *data);
void deleteMsg(void *data);

//Comparing two bit fields occur in two steps. First "byte no"s are compared.
//If they are the same, then bitIdx must be compared. It is done as comparing
//two integer sets.
int compareBitField(void *data1, void *data2);
//Comparing message fields is done by comparing the name of them, since there
//cannot be two fields with the same name.
int compareMsgField(void *data1, void *data2);
//Only names are compared for match conditions since only names are important
//to determine if two conditions are same.
int compareMatchCondition(void *data1, void *data2);
//Only names are compared. It is the only member of the struct.
int compareReportField(void *data1, void *data2);
//Only the names are compared.
int compareMsg(void *data1, void *data2);

void printBitField(void *data);
void printMsgField(void *data);
void printMatchCondition(void *data);
void printReportField(void *data);
void printMsg(void *data);

#endif
