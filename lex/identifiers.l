%option noyywrap
digit	[0-9]
letter	[A-Za-z]
%{
	int count;
%}
%%
{letter}({letter}|{digit})*	{fwrite(yytext, yyleng, 1, yyout); ++count;};
.
%%
int main(void) {
	yylex();
	printf("%d\n", count);
	return 0;
}
