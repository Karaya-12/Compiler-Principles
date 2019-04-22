    int num_lines = 0, num_chars = 0;

%%

\n ++num_lines; ++num_chars;
. ++num_chars;

%%

main()
{
	yylex();
	printf("Number of Lines: %d, Number of Chars: %d\n ", num_lines, num_chars);
}
