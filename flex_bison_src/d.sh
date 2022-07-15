rm emp.tab.*
rm emp.lex.c
bison -d emp.y
flex -oemp.lex.c emp.l
