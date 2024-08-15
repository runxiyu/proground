%option noinput nounput noyywrap

%{
	int lineno=0;
%}

%%

^(.*)\n	printf("%4d\t%s", ++lineno, yytext);

%%

int main(int argc, char *argv[]) {
	if (argc > 1) {
		yyin = fopen(argv[1], "r");
	}
	yylex();
	fclose(yyin);
}
