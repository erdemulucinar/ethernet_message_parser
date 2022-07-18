#include "emp_front_end.h"

extern FILE *yyin;
extern int yyparse(ListNode **rawMsgList);
extern int yylex(void);

//------------------------------------------------------------------------
//---------------------------USER INTERFACE FUNCTIONS---------------------
//------------------------------------------------------------------------

ListNode* collectRawMessages(char* file){
    FILE *fp;
    ListNode *rawMsgList;
    
    fp = fopen(file, "r");
    yyin = fp;
    rawMsgList = 0;
    yyparse(&rawMsgList);
    fclose(fp);
    
    return rawMsgList;
}

//------------------------------------------------------------------------
//-----------------------------LIST FUNCTIONS-----------------------------
//------------------------------------------------------------------------

void listAdd(ListNode *head, void *data){
    ListNode *newNode;
    
    newNode = malloc(sizeof(ListNode));
    newNode->data = data;
    newNode->next = 0;

    while(head->next){
        head = head->next;
    }
    head->next = newNode;
}

void listCombine(ListNode *head1, ListNode *head2){
    while(head1->next){
        head1 = head1->next;
    }
    
    head1->next = head2;
}

int listLen(ListNode *head){
    int len;
    
    len = 0;
    while(head){
        head = head->next;
        len = len + 1;
    }
    
    return len;
}

//------------------------------------------------------------------------
//---------------------------UTILITY FUNCTIONS----------------------------
//------------------------------------------------------------------------
char* copyStr(char* str){
    char *strCpy;
    
    strCpy = malloc(strlen(str)*sizeof(char));
    strcpy(strCpy,str);
    return strCpy;
}

BitField* collectBitField(char *fieldStr){
    BitField *bitField;
    char *rangeBeg, *rangeEnd, *rangeDelim;
    int i, begIdx, endIdx;
    
    bitField = malloc(sizeof(BitField));
    bitField->byteNo = 0;
    bitField->idx = 0;
    bitField->len = 0;
    
    rangeBeg = strchr(fieldStr,'(');
    rangeEnd = strchr(fieldStr,')');
    rangeDelim = strchr(fieldStr,':');
    
    //ByteNo case
    if(!rangeBeg){
        bitField->byteNo = char2dec(fieldStr, (int) strlen(fieldStr));
        bitField->len = 8;
        bitField->idx = malloc(bitField->len*sizeof(int));
        for(i = 0; i < bitField->len; i++){
            bitField->idx[i] = 7-i;
        }
        return bitField;
    }
    
    //ByteNo(BitNo) case
    if(!rangeDelim){
        bitField->byteNo = char2dec(fieldStr, (int)(rangeBeg-fieldStr));
        bitField->len = 1;
        bitField->idx = malloc(sizeof(int));
        bitField->idx[0] = char2dec(rangeBeg+1, 1);
        return bitField;
    }
    
    //ByteNo(BitBeg:BitEnd) case
    bitField->byteNo = char2dec(fieldStr, (int)(rangeBeg-fieldStr));
    begIdx = char2dec(rangeBeg+1,1);
    endIdx = char2dec(rangeEnd-1,1);
    if(begIdx > endIdx){
        bitField->len = begIdx-endIdx+1;
        bitField->idx = malloc(bitField->len*sizeof(int));
        for(i = 0; i < bitField->len; i++){
            bitField->idx[i] = begIdx-i;
        }
    } else{
        bitField->len = endIdx-begIdx+1;
        bitField->idx = malloc(bitField->len*sizeof(int));
        for(i = 0; i < bitField->len; i++){
            bitField->idx[i] = begIdx+i;
        }
    }
    
    return bitField;
}

int char2dec(char *str, int len){
    int base, val;
    size_t i;
    
    base = 1;
    val = 0;
    for(i = len; i > 0; i--){
        val = val + (str[i-1]-'0') * base;
        base = base * 10;
    }
    
    return val;
}

char* num2bin(char *str){
    char *numBeg, *bits, numType;
    int fieldLen, i, numLen;
    
    numBeg = strchr(str, '\'');
    fieldLen = char2dec(str, (int)(numBeg-str-1));
    numLen = (int) (strlen(str) - (numBeg-str) - 1);
    numType = *(numBeg-1);
    
    switch (numType) {
        case 'b':
            bits = malloc((fieldLen+1)*sizeof(char));
            for(i = 0; i < fieldLen; i++){
                bits[i] = '0';
            }
            for(i = 0; i < numLen; i++){
                bits[i+fieldLen-numLen] = numBeg[i+1];
            }
            bits[fieldLen] = 0;
            break;
        case 'h':
            bits = hex2bin(numBeg+1,fieldLen);
            break;
        case 'd':
            bits = dec2bin(char2dec(numBeg+1, (int)strlen(numBeg+1)), fieldLen);
            break;
        default:
            bits = 0;
            break;
    }
    
    return bits;
}

char* hex2bin(char *str, int bitLen){
    char* bits;
    int dec;
    
    dec = hex2dec(str);
    bits = dec2bin(dec, bitLen);
    
    return bits;
}

char* dec2bin(int num, int bitLen){
    char *bits, i;
    
    bits = malloc((bitLen+1)*sizeof(char));
    for(i = 0; i < bitLen; i++){
        if(num%2){
            bits[bitLen-1-i] = '1';
        } else{
            bits[bitLen-1-i] = '0';
        }
        num = num / 2;
    }
    
    bits[bitLen] = 0;
    return bits;
}

int hex2dec(char *str){
    int val, acc, i, base, len;
    char nibble;
    
    len = (int) strlen(str);
    val = 0;
    acc = 0;
    base = 1;
    for(i = 0; i < len; i++){
        nibble = str[len-1-i];
        if((nibble >= 'a') && (nibble <= 'f')){
            val = (nibble-'a')+10;
        }
        if((nibble >= 'A') && (nibble <= 'F')){
            val = (nibble-'A')+10;
        }
        if((nibble >= '0') && (nibble <= '9')){
            val = nibble-'0';
        }
        acc = acc + val * base;
        base = base * 16;
    }
    
    return acc;
}

//------------------------------------------------------------------------
//----------------------------DEBUG FUNCTIONS-----------------------------
//------------------------------------------------------------------------

void printBitField(BitField *bitField){
    int i;
    
    printf("%d(",bitField->byteNo);
    
    i = 0;
    for(i = 0; i < bitField->len-1; i++){
        printf("%d,", bitField->idx[i]);
    }
    printf("%d)",bitField->idx[bitField->len-1]);
}

void printBitFieldList(ListNode *node){
    while(node){
        printBitField((BitField*) node->data);
        printf(" ");
        node = node->next;
    }
}
    
void printLine(Line* line){
    MsgFieldDef *msgFieldDef;
    ReportFieldDef *reportDef;
    ControlFieldDef *controlDef;
    ReportInterface *reportInterface;
    
    switch (line->lineType) {
        case MSG_FIELD_DEF:
            msgFieldDef = (MsgFieldDef*) (line->data);
            printf("Message field definition: ");
            printf("%s => ", msgFieldDef->name);
            printBitFieldList(msgFieldDef->bitFields);
            printf("\n");
            break;
        case REPORT_FIELD_DEF:
            reportDef = (ReportFieldDef*) (line->data);
            printf("Report field definition: %s\n",reportDef->name);
            break;
        case CONTROL_FIELD_DEF:
            controlDef = (ControlFieldDef*) (line->data);
            printf("Control field definition: %s = %s\n", controlDef->name, controlDef->val);
            break;
        case REPORT_INTERFACE:
            reportInterface = (ReportInterface*) (line->data);
            printf("Report interface: %s ",reportInterface->name);
            if(reportInterface->reportType == REPORT_RAM)
                printf("(RAM)\n");
            else
                printf("(RECORD)\n");
            break;
        default:
            break;
    }
}
    
void printLineList(ListNode *lineList){
    while(lineList){
        printLine((Line*)(lineList->data));
        lineList = lineList->next;
    }
}
    
void printRawMsg(RawMsg *msg){
    printf("Msg name: %s\n", msg->name);
    printf("Msg parent: ");
    if(msg->parent)
        printf("%s", msg->parent);
    printf("\n");
    printLineList(msg->lineList);
}
