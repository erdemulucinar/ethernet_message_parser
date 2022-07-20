#include "emp_data_structures.h"
#include "emp_util.h"

#define safe_free(X) if(X) free(X)

//------------------------------------------------------------------------
//-----------------------------Msg Functions------------------------------
//------------------------------------------------------------------------
Msg* addLine(Msg *msg, MsgLine *line){
    ReportInterface *reportInterface;
    MsgField *msgField;
    MatchCondition *matchCondition;
    ReportField *reportField;
    
    //If input is null, create a new msg.
    if(!msg){
        msg = malloc(sizeof(Msg));
        msg->name = 0;
        msg->parent = 0;
        msg->reportInterface = 0;
        msg->msgFields = createLinkedList(&copyRef, &deleteMsgField, &compareMsgField);
        msg->matchConditions = createLinkedList(&copyRef, &deleteMatchCondition, &compareMatchCondition);
        msg->reportFields = createLinkedList(&copyRef, &deleteReportField, &compareReportField);
    }
    
    //Adding lines to corresponding fields of the msg struct.
    switch (line->lineType) {
        case REPORT_INTERFACE:
            reportInterface = (ReportInterface*)(line->data);
            msg->reportInterface = reportInterface;
            //TODO Add report interface overwrite check & error mechanism.
            break;
        case MESSAGE_FIELD:
            msgField = (MsgField*)(line->data);
            appendLinkedList(msg->msgFields, (void*) msgField);
            break;
        case MATCH_CONDITION:
            matchCondition = (MatchCondition*)(line->data);
            appendLinkedList(msg->matchConditions, (void*) matchCondition);
            break;
        case REPORT_FIELD:
            reportField = (ReportField*)(line->data);
            appendLinkedList(msg->reportFields, (void*) reportField);
            break;
        default:
            break;
    }
    
    return msg;
}

//------------------------------------------------------------------------
//-----------------------MsgLine Creation Functions-----------------------
//------------------------------------------------------------------------
MsgLine* createReportInterface(ReportInterfaceType type, char *name){
    ReportInterface *reportInterface;
    MsgLine *msgLine;
    
    reportInterface = malloc(sizeof(ReportInterface));
    reportInterface->interfaceType = type;
    reportInterface->interfaceName = name;
    
    msgLine = malloc(sizeof(MsgLine));
    msgLine->lineType = REPORT_INTERFACE;
    msgLine->data = (void*) reportInterface;
    
    return msgLine;
}

MsgLine* createMsgField(char *name, LinkedList *bitFields){
    MsgField *msgField;
    MsgLine *msgLine;
    
    msgField = malloc(sizeof(MsgField));
    msgField->name = name;
    msgField->bitFields = bitFields;
    
    msgLine = malloc(sizeof(MsgLine));
    msgLine->lineType = MESSAGE_FIELD;
    msgLine->data = (void*) msgField;
    
    return msgLine;
}

MsgLine* createMatchCondition(char *name, char *val){
    MatchCondition *matchCondition;
    MsgLine *msgLine;
    
    matchCondition = malloc(sizeof(MatchCondition));
    matchCondition->name = name;
    matchCondition->val = val;
    
    msgLine = malloc(sizeof(MsgLine));
    msgLine->lineType = MATCH_CONDITION;
    msgLine->data = (void*) matchCondition;
    
    return msgLine;
}

MsgLine* createReportField(char *name){
    ReportField *reportField;
    MsgLine *msgLine;
    
    reportField = malloc(sizeof(ReportField));
    reportField->name = name;
    
    msgLine = malloc(sizeof(MsgLine));
    msgLine->lineType = REPORT_FIELD;
    msgLine->data = (void*) reportField;
    
    return msgLine;
}

//------------------------------------------------------------------------
//---------------------Linked List Template Functions---------------------
//------------------------------------------------------------------------
void* copyRef(void *data){
    return data;
}

void deleteBitField(void *data){
    BitField *bitField;
    
    bitField = (BitField*) data;
    safe_free(bitField->bitIdx);
    free(bitField);
}

void deleteMsgField(void *data){
    MsgField *msgField;
    
    msgField = (MsgField*) data;
    safe_free(msgField->name);
    deleteBitField(msgField->bitFields);
    free(msgField);
}

void deleteMatchCondition(void *data){
    MatchCondition *matchCondition;
    
    matchCondition = (MatchCondition*) data;
    safe_free(matchCondition->name);
    safe_free(matchCondition->val);
    free(matchCondition);
}

void deleteReportField(void *data){
    ReportField *reportField;
    
    reportField = (ReportField*) data;
    safe_free(reportField->name);
    free(reportField);
}

void deleteMsg(void *data){
    Msg *msg;
    
    msg = (Msg*) data;
    
    safe_free(msg->name);
    safe_free(msg->parent);
    safe_free(msg->reportInterface->interfaceName);
    safe_free(msg->reportInterface);
    flushLinkedList(msg->msgFields);
    flushLinkedList(msg->matchConditions);
    flushLinkedList(msg->reportFields);
}

int compareBitField(void *data1, void *data2){
    BitField *bitField1, *bitField2;
    int compRes;
    
    bitField1 = (BitField*) data1;
    bitField2 = (BitField*) data2;
    
    //Byte no check
    compRes = bitField1->byteNo - bitField2->byteNo;
    if(!compRes)
        return compRes;
    
    //Length check
    compRes = bitField1->len - bitField2->len;
    if(!compRes)
        return compRes;
    
    //Set compare
    return compareBitIdx(bitField1->bitIdx, bitField2->bitIdx, bitField1->len);
}

int compareMsgField(void *data1, void *data2){
    MsgField *msgField1, *msgField2;
    
    msgField1 = (MsgField*) data1;
    msgField2 = (MsgField*) data2;
    
    return strcmp(msgField1->name, msgField2->name);
}

int compareMatchCondition(void *data1, void *data2){
    MatchCondition *matchCondition1, *matchCondition2;
    
    matchCondition1 = (MatchCondition*) data1;
    matchCondition2 = (MatchCondition*) data2;
    
    return strcmp(matchCondition1->name, matchCondition2->name);
}

int compareReportField(void *data1, void *data2){
    ReportField *reportField1, *reportField2;
    
    reportField1 = (ReportField*) data1;
    reportField2 = (ReportField*) data2;
    
    return strcmp(reportField1->name, reportField2->name);
}

int compareMsg(void *data1, void *data2){
    Msg *msg1, *msg2;
    
    msg1 = (Msg*) data1;
    msg2 = (Msg*) data2;
    
    return strcmp(msg1->name, msg2->name);
}

void printBitField(void *data){
    BitField *bitField;
    int i;
    
    bitField = (BitField*) data;
    printf("%d(", bitField->byteNo);
    for(i = 0; i < bitField->len-1; i++){
        printf("%d,",bitField->bitIdx[i]);
    }
    printf("%d) ",bitField->bitIdx[bitField->len-1]);
}

void printMsgField(void *data){
    MsgField *msgField;
    
    msgField = (MsgField*) data;
    printf("%s <= ", msgField->name);
    printLinkedList(msgField->bitFields, &printBitField);
}

void printMatchCondition(void *data){
    MatchCondition *matchCondition;
    
    matchCondition = (MatchCondition*) data;
    printf("%s == ", matchCondition->name);
    printf("%s\n",matchCondition->val);
}

void printReportField(void *data){
    ReportField *reportField;
    
    reportField = (ReportField*) data;
    printf("output %s\n", reportField->name);
}

void printMsg(void *data){
    Msg *msg;
    
    msg = (Msg*) data;
    
    printf("----MESSAGE: %s\n", msg->name);
    
    if(msg->parent){
        printf("Parent: %s\n\n", msg->parent);
    }
    
    if(msg->reportInterface){
        printf("%s (", msg->reportInterface->interfaceName);
        if(msg->reportInterface->interfaceType == REPORT_RAM){
            printf("RAM)\n\n");
        } else{
            printf("RECORD)\n\n");
        }
    }
    
    printLinkedList(msg->msgFields, &printMsgField);
    printLinkedList(msg->matchConditions, &printMatchCondition);
    printLinkedList(msg->reportFields, &printReportField);
}
