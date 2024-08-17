%{
unsigned long long chars = 0;
unsigned long long words = 0;
unsigned long long lines = 0;
%}

%%

[^ \t\n\r\f\v]+ { words ++; chars += strlen(yytext); }
\n              { chars ++; lines ++;                }
.               { chars ++;                          }

%%

int main (void) {
	yylex();
	printf("%8llu%8llu%8llu\n", lines, words, chars);
}

int yywrap() {
	return 1;
}
