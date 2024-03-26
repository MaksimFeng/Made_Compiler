#ifndef extern_
 #define extern_ extern
#endif

// Global variables
// Copyright (c) 2019 Warren Toomey, GPL3

extern_ int     	Line;
extern_ int		Putback;
extern_ FILE		*Infile;
extern_ struct token	Token;
extern_ FILE		*Outfile;
#define TEXTLEN         512             // Length of symbols in input
extern_ char Text[TEXTLEN + 1];         // Last identifier scanned