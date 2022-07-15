rm flex_bison/header/*
rm flex_bison/src/*
bison -d flex_bison/defs/emp.y
flex -oemp.lex.c flex_bison/defs/emp.l
mv *.tab.h flex_bison/header
mv *.{lex,tab}.c flex_bison/src
