#!/bin/bash
cd src/observer/sql/parser/
flex lex_sql.l
bison -d -b yacc_sql yacc_sql.y