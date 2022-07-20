#include "emp_util.h"

//------------------------------------------------------------------------
//---------------------------Common Functions----------------------------
//------------------------------------------------------------------------
char* copyStr(char* str){
    char *copy;
    
    copy = malloc(strlen(str)*sizeof(char));
    strcpy(copy, str);
    
    return copy;
}

//------------------------------------------------------------------------
//-----------------------Data Structures Functions------------------------
//------------------------------------------------------------------------
int compareBitIdx(int *set1, int *set2, int len){
    int set1Ascending, set2Ascending, val1, val2;
    
    //Ascending check for the sets
    set1Ascending = set1[0] < set1[len-1];
    set2Ascending = set2[0] < set2[len-1];
    
    //Lowest values of the sets are chosen to compare.
    if(set1Ascending){
        val1 = set1[0];
    } else{
        val1 = set1[len-1];
    }
    
    if(set2Ascending){
        val2 = set2[0];
    } else{
        val2 = set2[len-1];
    }
    
    //If the lowest values are the same then the sets are same. If not,
    //set1's range ends at a higher index and vice versa.
    return val1-val2;
}

//------------------------------------------------------------------------
//----------------------------Lexer Functions-----------------------------
//------------------------------------------------------------------------

//Converts the decimal string <INTEGER_STRING> to the integer value INTEGER.
int str2dec(char *str, int len){
    int base, val, i;
    
    base = 1;
    val = 0;
    for(i = len-1; i >= 0; i--){
        val = val + (str[i]-'0') * base;
        base = base * 10;
    }
    
    return val;
}

//For <BITLEN_INTEGER_STRING>{b,h,d}'<INPUT_VAL_STRING>
//bitLen = BITLEN_INTEGER
//inputStr = <INPUT_VAL_STR>
//inputLen = length(<INPUT_VAL_STR>)
void getNumParameters(char *str, int *bitLen, char **inputStr, int *inputLen){
    char *apostrophe;
    
    apostrophe = strchr(str,'\'');
    *bitLen = str2dec(str, (int)(apostrophe-str-1));
    *inputStr = apostrophe+1;
    *inputLen = (int)strlen(apostrophe+1);
}

//Converts an integer to binary representation. A c-string is returned with
//len number of chars of '1's and '0's.
char* int2binStr(int num, int len){
    char *val;
    int i;
    
    val = malloc((len+1)*sizeof(char));
    num = num & ((1<<len)-1);
    for(i = len-1; i >= 0; i--){
        val[i] = '0' + (num % 2);
        num = num / 2;
    }
    val[len] = 0;
    
    return val;
}

char* hex2num(char *hex){
    char *inStr, inChar;
    int bitLen, inLen;
    int i, inInt, base;
    
    //Collecting parameters
    getNumParameters(hex, &bitLen, &inStr, &inLen);
    
    //Calculating input integer
    inInt = 0;
    base = 1;
    for(i = inLen-1; i >= 0; i--){
        inChar = inStr[i];
        if(inChar >= 'A' && inChar <= 'F'){
            inInt = inInt + ((inChar - 'A') + 10) * base;
        } else if(inChar >= 'a' && inChar <= 'f'){
            inInt = inInt + ((inChar - 'a') + 10) * base;
        } else{ //0-9
            inInt = inInt + (inChar - '0') * base;
        }
        base = base * 16;
    }
    
    //Converting the number to binary
    return int2binStr(inInt, bitLen);
}

char* bin2num(char *bin){
    char *inStr, inChar;
    int bitLen, inLen;
    int i, inInt, base;
    
    //Collecting parameters
    getNumParameters(bin, &bitLen, &inStr, &inLen);
    
    //Calculating input integer
    inInt = 0;
    base = 1;
    for(i = inLen-1; i >= 0; i--){
        inChar = inStr[i];
        inInt = inInt + (inChar - '0') * base;
        base = base * 2;
    }
    
    //Converting the number to binary
    return int2binStr(inInt, bitLen);
}

char* dec2num(char *dec){
    char *inStr, inChar;
    int bitLen, inLen;
    int i, inInt, base;
    
    //Collecting parameters
    getNumParameters(dec, &bitLen, &inStr, &inLen);
    
    //Calculating input integer
    inInt = 0;
    base = 1;
    for(i = inLen-1; i >= 0; i--){
        inChar = inStr[i];
        inInt = inInt + (inChar - '0') * base;
        base = base * 10;
    }
    
    //Converting the number to binary
    return int2binStr(inInt, bitLen);
}

BitField* collectBitField(char *str){
    BitField *bitField;
    char *rangeBeg, *rangeEnd, *rangeDelim;
    int i, begIdx, endIdx;
    
    bitField = malloc(sizeof(BitField));
    bitField->byteNo = 0;
    bitField->bitIdx = 0;
    bitField->len = 0;
    
    rangeBeg = strchr(str,'(');
    rangeEnd = strchr(str,')');
    rangeDelim = strchr(str,':');
    
    //1.ByteNo case: There are no paranthesis thus it must be
    //only a byte. Thus length is 8 bits and the idx is 7:0 which
    //is the default range for a byte.
    if(!rangeBeg){
        bitField->byteNo = str2dec(str, (int) strlen(str));
        bitField->len = 8;
        bitField->bitIdx = malloc(bitField->len*sizeof(int));
        for(i = 0; i < bitField->len; i++){
            bitField->bitIdx[i] = 7-i;
        }
        return bitField;
    }
    
    //2.ByteNo(BitNo) case: There are paranthesis but no colon in the
    //string thus it must be a single bit of a byte. In this case the
    //length is 1.
    if(!rangeDelim){
        bitField->byteNo = str2dec(str, (int)(rangeBeg-str));
        bitField->len = 1;
        bitField->bitIdx = malloc(sizeof(int));
        bitField->bitIdx[0] = str2dec(rangeBeg+1, 1);
        return bitField;
    }
    
    //3.ByteNo(BitBeg:BitEnd) case: There are paranthesis and there is
    //a colon in the string, indicating a bit range description of a
    //byte.
    bitField->byteNo = str2dec(str, (int)(rangeBeg-str));
    begIdx = str2dec(rangeBeg+1,1);
    endIdx = str2dec(rangeEnd-1,1);
    if(begIdx > endIdx){
        //For the case with bit indices descending in the range,
        //length is first idx - last idx and the bit indices are
        //descending from first idx until last idx.
        bitField->len = begIdx-endIdx+1;
        bitField->bitIdx = malloc(bitField->len*sizeof(int));
        for(i = 0; i < bitField->len; i++){
            bitField->bitIdx[i] = begIdx-i;
        }
    } else{
        //For the case with bit indices ascending in the range,
        //length is last idx - first idx and the bit indices are
        //ascending from first idx until last idx.
        bitField->len = endIdx-begIdx+1;
        bitField->bitIdx = malloc(bitField->len*sizeof(int));
        for(i = 0; i < bitField->len; i++){
            bitField->bitIdx[i] = begIdx+i;
        }
    }
    
    return bitField;
}
