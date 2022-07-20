#ifndef EMP_UTIL_H
#define EMP_UTIL_H

#include "emp_data_structures.h"

//0.Common
char* copyStr(char* str);

//1.Data structures

//Returns the compare result of two bitIdx sets. When set1 spans a range
//which ends at a higher index, results is bigger than 0. When set2 spans
//a range which ends at a higher index, result is smaller than 0. When
//the ranges are the same, result is 0.
int compareBitIdx(int *set1, int *set2, int len);

//2.Lexer

//Converts a hexadecimal number <BITLEN_INTEGER_STRING>h'<INPUT_HEX_STRING>
//to its binary representation INPUT_BIT_STRING.
char* hex2num(char *hex);

//Converts a binary number <BITLEN_INTEGER_STRING>b'<INPUT_BIN_STRING>
//to its binary representation INPUT_BIT_STRING. Even though the number
//is already binary, conversion is done to cover cases where the input
//is longer/shorter than the <BITLEN_INTEGER>
char* bin2num(char *bin);

//Converts a decimal number <BITLEN_INTEGER_STRING>d'<INPUT_DEC_STRING>
//to its binary representation INPUT_BIT_STRING.
char* dec2num(char *dec);

//Converts a bit field string to a BitField structure as defined in
//emp_data_structure.h. There are three possible bit field expressions
//1. <BYTENO_INT_STRING>: Denotes all the bits from 7 to 0 of BYTENO_INT
//2. <BYTENO_INT_STRING>(BITIDX_INT_STRING): Denotes only one bit, which
//is BITIDX_INT of the byte BYTENO_INT.
//3. <BYTENO_INT_STRING>(BITIDXBEG_INT_STRING:BITIDXEND_INT_STRING):
//Denotes bit range of [BITIDXBEG_INT:BITIDXEND_INT] of byte BYTENO_INT.
//range may be ascending or descending.
BitField* collectBitField(char *str);

//3.Parser

#endif
