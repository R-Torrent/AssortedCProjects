/************************************************************************************************
    Performs the long division of two signed integers
    by Roger Torrent, April 18, 2015

    USAGE:
	ldivision < [?] | [<"dividend"> <"divisor"> [\b:##] [\d] [\i] [\l[:xx]] [\n[:##]] [\p]] >
    OPTIONS:
		? : Display help message
	"dividend": long [LONG_MIN + 1, LONG_MAX], expressed in base B (or decimal with \d)
	"divisor" : long [-|LONG_MAX/B| - 1, |LONG_MAX/B| + 1], expressed in base B,
		     (or decimal with \d) with the expression LONG_MAX/B truncated toward zero
	    \b:## : Sets B to ##, base of choice [opt.]
		    unsigned char [2, 36] (by default B=BDEF; decimal notation)
	       \d : Forces decimal notation on "dividend" and "divisor" [opt.]
	       \i : Display additional information [opt.]
	  \l[:xx] : Sets lc to the native locale, or "xx" if the field xx is specified [opt.]
		    The locale adjusts the decimal point displayed and the time format
		    (by default lc="C")
	  \n[:##] : Sets N to 0, or "##" if the field ## is specified, limit on fractional
		     base-B digits displayed [opt.]
		    long [0, |LONG_MAX/B|] (by default N is maximum; decimal notation),
	       \p : Prefixes a '+' sign to all positive numbers [opt.]
    RETURNS:
	To 'stdout':	** \t = 8 SPACES **
	    "dividend" / "divisor", string literal expressed in base B
		(##) encloses repeating digits
		"..." should the fractional sequence surpass the N limit and N > 0
		A decimal point marks the existence of a fractional remainder, even when N = 0
	    Additional information (option \i)
		"Integral digits displayed" message
		"Fractional digits displayed" message
		"(inside repeating parentheses)" message, should a repetition be found
		Result evaluation (EXACT SOLUTION or INEXACT SOLUTION displayed)
		Local date and time representation
		"Locale employed" message, if it has been modified from the default "C"
		Command executed
	To 'stderr':
	    Specific error string literal
	    Help message
	To system:
	    Macro EXIT_SUCCESS or EXIT_FAILURE (included in <stdlib.h>)

    ** THIS SOURCE FILE IS BEST VIEWED WHEN 1 HORIZONTAL TAB = 8 SPACES  **
    ** EXTENSIVE ERROR CONTROL ** THIS PROGRAM IS ANSI C (C89) COMPLIANT **
*************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <time.h>

/*  'SIZEl' of the char array used to store string literals of long integers
    = characters required to a binary representation of '+'/'-' + [LONG_MAX] + '\0'	*/
#define SIZEl sizeof(long) * CHAR_BIT + 1
#define SIZEc 81 /* 'SIZEc' of the char strings (non-numeric) serviced			*/

#if defined(_MSC_VER) || defined(__GNUC__) /*  Microsoft's Visual C or GNU C compilers  */
    #pragma pack(1)			   /*  Structures byte-aligned; no padding	*/
#endif

typedef struct {	 /*  Fractional digits record	      */
    long rem;
    char dig;
} digf_t;

typedef struct {	 /*  User's input record	      */
    struct {
        unsigned char n; /*  Numeric value		      */
        char arg[SIZEc]; /*  Stores user's optional argument  */
    } B; /*  Base  */
    struct {
        long n; 	 /*  Numeric value		      */
        char arg[SIZEc]; /*  Stores user's optional argument  */
    } N; /*  Fractional digits displayed  */
    struct {
        char lc[SIZEc];  /*  Name			      */
        char arg[SIZEc]; /*  Stores user's optional argument  */
    } L; /*  Locale  */
    unsigned char flags;
    long dvdnd; /*  dividend  */
    long divsr; /*  divisor   */
} opts_t;

/*  'Display digits' set  */
static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*  Flags for extra options  */
#define FL_B 001 /*  \b set  */
#define FL_D 002 /*  \d set  */
#define FL_I 004 /*  \i set  */
#define FL_L 010 /*  \l set  */
#define FL_N 020 /*  \n set  */
#define FL_P 040 /*  \p set  */

#define BDEF 10 			 /*  Default base of integer representation  */
static long NMAX = LONG_MAX/BDEF; /*  Default number of fractional digits displayed  */

#define MIN(A, B) ((A) < (B) ? (A) : (B))	       /*  Minimum of A and B macro  */
#define QUOTE(A) (sprintf(bufferc, "\"%s\"", A), bufferc) /*  string A -> "A" macro  */

/*  FORMAT_PRINT_INT macro prints integers to 'stdout' using the character 
    set of digits[], and inserts the '+' if option \p has been set		     */
#define FORMAT_PRINT_INT(INTGR, SZE)			\
{							\
    pargv1 = bufferl;					\
    if (INTGR < 0)					\
	*pargv1++ = '-';				\
    else if (INTGR && com.flags & FL_P) 		\
	*pargv1++ = '+';				\
    SZE = (long)dtoB(INTGR, (long)com.B.n, pargv1);	\
    printf("%s", bufferl);				\
}

/*  Error messages */
#define ERRM00 "Illegal argument count"
#define ERRM01 "Out of range"
#define ERRM02 "Expected \"\\\" in"
#define ERRM03 "Expected \":\" in"
#define ERRM04 "Illegal option"
#define ERRM10 "Illegal operand"
#define ERRM11 "Illegal field specification"
#define ERRM12 "Field redefinition"
#define ERRM13 "Argument repetition"
#define ERRM14 "Unknown locale"
#define ERRM20 "Division by zero"
#define ERRM21 "Insufficient memory"

/*  Error message ERRM## printed to 'stderr'; jump to err_exit  */
#define ERR(num1, num2, str)						\
{									\
    fprintf(stderr, "\nERROR(" #num1 #num2 ")_" ERRM ## num1 ## num2);	\
    if (str)								\
	fprintf(stderr, ": %s", str);					\
    fprintf(stderr, "\n");						\
    goto err_exit ## num1;						\
}

/**********************************************************
    size_t dtoB(const long d, const long base, char dest[])

    Converts a signed integer's notation from base-10 to
    a user-supplied base, dropping any '-' sign

    ARGUMENTS:
	   'd' : Integer to be converted
		 long [LONG_MIN, LONG_MAX]
	'base' : Base of conversion
		 long [2, 36]
	'dest' : Pointer to the destination string
		 char []
    RETURNS:
	In the char buffer addressed by pointer dest:
	    A string literal of the converted integer
	To the calling routine: size_t [1, SIZE_MAX - 1]
	    Number of characters written (excluding '\0')

    ** NO BUFFER OVERFLOW CONTROL ON 'dest' POINTER **
    ** REQUIRES 'dtoB_rec()' RECURSIVE AUX FUNCTION **
***********************************************************/
size_t dtoB(const long d, const long base, char dest[])
{
    char *dtoB_rec(const long, const long, char *);

/*  Passing -d as an argument for d < 0 would result in an error for d = LONG_MIN  */
    dtoB_rec(d, base, dest);

    return strlen(dest);
}

/***********************************************************************
    char *dtoB_rec(const long d, const long base, char *pc)

    Auxiliary recursive function to the 'dtoB' conversion function

    ARGUMENTS:
	   'd' : Integer dividend
		 long [LONG_MIN, LONG_MAX]
	'base' : Integer divisor
		 long [2, 36]
	  'pc' : Pointer to the destination byte in the char array for
		    the division remainder
		 char *
    RETURNS:
	The position of the next digit, preinitialized with '\0': char *
************************************************************************/
char *dtoB_rec(const long d, const long base, char *pc)
{
    ldiv_t op = ldiv(d, base);

    if (op.quot)
	pc = dtoB_rec (op.quot, base, pc);

    *pc++ = digits[labs(op.rem)];
    *pc = '\0';

    return pc;
}

/********************************************************************
    void syntax_disp(char *comm, unsigned char base)

    Prints the help message (? option or after a SYNTAX)

    ARGUMENTS:
	'comm' : Pointer to the system's command that executed main()
		 char *
	'base' : Numerical base
		 unsigned char [2, 36]
    RETURNS:
	To 'stderr': Detailed syntax description

    ** \t = 8 SPACES **
*********************************************************************/
void syntax_disp(char *comm, unsigned char base)
{
    char buffer[SIZEl];

    fprintf(stderr, "\nSYNTAX: %s < [?] | [<\"dividend\"> <\"divisor\"> [\\b:##] [\\d] [\\i] "
	"[\\l[:xx]] [\\n[:##]] [\\p]] >\n", comm);
    fprintf(stderr, "\t ? > Displays this message\n");
    fprintf(stderr, "\"dividend\" > %ld to %ld [expressed in choice base B]\n", LONG_MIN + 1, LONG_MAX);
    if (base != 10) {
	dtoB(LONG_MAX, (long)base, buffer);
	fprintf(stderr, "\t      [Base %02hu]: -%s to %s\n", base, buffer, buffer);
    }
    fprintf(stderr, " \"divisor\" > -(%ld/B+1) to %ld/B+1 [expressed in choice base B]\n", LONG_MAX, LONG_MAX);
    dtoB(NMAX + 1, (long)base, buffer);
    fprintf(stderr, "\t      [Base %02hu]: -%s to %s\n", base, buffer, buffer);
    fprintf(stderr, "     \\b:## > Base of the numerical representation\n");
    fprintf(stderr, "\t      2 to 36 [use decimal notation; %d by default]\n", BDEF);
    fprintf(stderr, "\t\\d > Forces decimal notation on the division operands\n");
    fprintf(stderr, "\t\\i > Displays additional information\n");
    fprintf(stderr, "   \\l[:xx] > Sets the locale to \"xx\"\n");
    fprintf(stderr, "\t      Default locale: C\n");
    fprintf(stderr, "\t      Native locale: %s\n", setlocale(0, ""));
    fprintf(stderr, "\t      [to the native locale if the field \"xx\" is unspecified]\n");
    fprintf(stderr, "   \\n[:##] > Limit on fractional base-B digits displayed\n");
    fprintf(stderr, "\t      0 to %ld/B [use decimal notation; maximum by default]\n", LONG_MAX);
    fprintf(stderr, "\t      [with base %02hu]: 0 to %ld\n", base, NMAX);
    fprintf(stderr, "\t      [0 if the field \"##\" is unspecified]\n");
    fprintf(stderr, "\t\\p > Prefixes a '+' to all positive numbers\n");
}

/*******  MAIN PROGRAM  *******/
int main(int argc, char *argv[])
{
/*  Declarations and initialization  */
    long tempL, i = 2, intBs, frcBs = 0, repBs = 0;
    char *pargv1, *pargv2, bufferl[SIZEl], bufferc[SIZEc];

    size_t mem; /*  Type 'size_t' defined in <stdlib.h>, <string.h>, <stdio.h>, and <time.h>  */
    ldiv_t op;  /*  Type 'ldiv_t' defined in <stdlib.h>  */

    opts_t com = {{BDEF}, {NMAX}, {"C"}}; /*  User's command  */
    digf_t *pdigf0 = NULL, *pdigf;

/*  Help request  */
    if (argc == 2 && !strcmp(argv[1], "?")) {
	syntax_disp(argv[0], com.B.n);

	return EXIT_SUCCESS;
    }
	
/*  Assignments and error control  */
    if (argc < 3) /*  Illegal argument count  */
	ERR(0, 0, NULL);

    while (++i < argc) { /*  Optional arguments  */
	pargv1 = argv[i];
	if (*pargv1++ != '\\')		 /*  Expected "\"	    */
	    ERR(0, 2, QUOTE(argv[i]));
	switch (*pargv1++) {
	case 'b': case 'B':
	    if (com.flags & FL_B) {	 /*  Base number redefined  */
		sprintf(bufferc, "\"%s\" to \"%s\"", com.B.arg, argv[i]);
		ERR(1, 2, bufferc);
	    }
	    strcpy(com.B.arg, argv[i]);
	    if (*pargv1++ != ':')	 /*  Expected ":"	    */
		ERR(0, 3, QUOTE(argv[i]));
	    tempL = strtol(pargv1, &pargv2, 10);
	    if (*pargv2)		 /*  Illegal base number    */
		ERR(1, 1, QUOTE(argv[i]));
	    if (tempL < 2 || tempL > 36) /*  B out of range	    */
		ERR(0, 1, QUOTE(argv[i]));
	    com.B.n = (unsigned char)tempL;
	    NMAX = LONG_MAX/tempL;
	    if (!(com.flags & FL_N))
		com.N.n = NMAX;
	    com.flags |= FL_B;
	    break;
	case 'n': case 'N':
	    if (com.flags & FL_N) {  /*  Fractional display number redefined  */
		sprintf(bufferc, "\"%s\" to \"%s\"", com.N.arg, argv[i]);
		ERR(1, 2, bufferc);
	    }
	    strcpy(com.N.arg, argv[i]);
	    if (!*pargv1)	      /*  Empty field -> N = 0	       */
		com.N.n = 0;
	    else {
		if (*pargv1++ != ':') /*  Expected ":"		       */
		    ERR(0, 3, QUOTE(argv[i]));
		com.N.n = strtol(pargv1, &pargv2, 10);
		if (*pargv2)	/*  Illegal fractional display number  */
		    ERR(1, 1, QUOTE(argv[i]));
	    }
	    com.flags |= FL_N;
	    break;
	case 'l': case 'L':
	    if (com.flags & FL_L) {	      /*  Locale redefined	 */
		sprintf(bufferc, "\"%s\" to \"%s\"", com.L.arg, argv[i]);
		ERR(1, 2, bufferc);
	    }
	    strcpy(com.L.arg, argv[i]);
	    if (!*pargv1)	  /*  Empty field -> Native locale	 */
		strcpy(com.L.lc, "");
	    else {
		if (*pargv1++ != ':')	      /*  Expected ":"		 */
		    ERR(0, 3, QUOTE(argv[i]));
		strcpy(com.L.lc, pargv1);     /*  User-specified locale  */
	    }
	    if (!setlocale(LC_ALL, com.L.lc)) /*  Unknown locale	 */
		ERR(1, 4, QUOTE(com.L.lc));
	    com.flags |= FL_L;
	    break;
	case 'd': case 'D':
	    if (com.flags & FL_D) /*  Decimal enforcement requested twice     */
		ERR(1, 3, QUOTE(argv[i]));
	    if (*pargv1)	  /*  Illegal option			      */
		ERR(0, 4, QUOTE(argv[i]));
	    com.flags |= FL_D;
	    break;
	case 'i': case 'I':
	    if (com.flags & FL_I) /*  Additional information requested twice  */
		ERR(1, 3, QUOTE(argv[i]));
	    if (*pargv1)	  /*  Illegal option			      */
		ERR(0, 4, QUOTE(argv[i]));
	    com.flags |= FL_I;
	    break;
	case 'p': case 'P':
	    if (com.flags & FL_P) /*  Positive sign requested twice	      */
		ERR(1, 3, QUOTE(argv[i]));
	    if (*pargv1)	  /*  Illegal option			      */
		ERR(0, 4, QUOTE(argv[i]));
	    com.flags |= FL_P;
	    break;
	default: /*  Illegal option  */
	    ERR(0, 4, QUOTE(argv[i]));
	    break;
	}
    }
    if (com.N.n < 0 || com.N.n > NMAX) /*  N out of range  */
	ERR(0, 1, QUOTE(com.N.arg));

    com.dvdnd = strtol(argv[1], &pargv2, com.flags & FL_D ? 10 : (int)com.B.n);
    if (*pargv2)    /*	Illegal dividend  */
	ERR(1, 0, QUOTE(argv[1]));
    if (errno == ERANGE || com.dvdnd == LONG_MIN) /*  Out of range; LONG_MIN/-1 would  */
	ERR(0, 1, QUOTE(argv[1]));		  /*  result in an overflow error      */

    com.divsr = strtol(argv[2], &pargv2, com.flags & FL_D ? 10 : (int)com.B.n);
    if (*pargv2)    /*	Illegal divisor   */
	ERR(1, 0, QUOTE(argv[2]));
    if (errno == ERANGE || labs(com.divsr) > NMAX + 1) /*  Out of range 	       */
	ERR(0, 1, QUOTE(argv[2]));

    if (!com.divsr) /*	Division by zero  */
	ERR(2, 0, NULL);

/*  Memory allocation; reserves minimum(N, (|divisor| - 1))·sizeof(digf_t) bytes  */
    if (mem = (size_t)MIN(com.N.n, labs(com.divsr) - 1) * sizeof(digf_t)) {
	pdigf = pdigf0 = malloc(mem);
	if (!pdigf0) { /*  Insufficient memory  */
	    sprintf(bufferc, "Requested %.2f MB -> Reduce \\n:##", (double)mem/1048576);
	    ERR(2, 1, bufferc);
	}
    }

/*  dividend = op.quot · divisor + op.rem  */
    op = ldiv(com.dvdnd, com.divsr);

/*  Division results printed to 'stdout'
    The operands are displayed converted to base-B, even when \d is set  */
    printf("\n");
    FORMAT_PRINT_INT(com.dvdnd, i);
    printf(" / ");
    FORMAT_PRINT_INT(com.divsr, i);
    printf(" =\n");
    FORMAT_PRINT_INT(op.quot, intBs); /*  intBs will store the number of repeating digits  */

    if (op.rem) {
/*  Fractional digits
    Stored in digf_t array pointed by pdigf
	Remainder in pdigf->rem
	First digit corresponding to the remainder in pdigf->dig

    B·rem[i] = digf[i]·divisor + rem[i+1]

    frcBs will store the number of fractional digits
    repBs will store the number of repeating digits  */
        for (frcBs = 0; op.rem && !repBs && frcBs < com.N.n; frcBs++, pdigf++) {
	    pdigf->rem = op.rem;		        /*  pdigf stores the remainder  */
	    op = ldiv(com.B.n * pdigf->rem, com.divsr); /*  This call limits the range of divisor to LONG_MAX/B+1  */
	    pdigf->dig = digits[labs(op.quot)]; 	/*  pdigf stores the character  */

	i = 0;
	do /*  Checks for remainder repetition  */
	    if ((pdigf-i)->rem == op.rem)
		repBs = i + 1;
	while (!repBs && i++ <= frcBs);
        }

	printf("%c", *localeconv()->decimal_point);	       /*  Prints locale decimal point	*/

        for (i = 0, pdigf = pdigf0; i < frcBs; i++, pdigf++) { /*  Prints fractional digits	*/
	    if ((i == frcBs - repBs) && repBs)
		printf("(");
	    printf("%c", pdigf->dig);
	}
	if (repBs)
	    printf(")");
	else if (com.N.n && op.rem)
	    printf("...");
    }

/*  Additional information (\i) to 'stdout'  */
    if (com.flags & FL_I) {
	time_t cal_time; /*  Type 'time_t' defined in <time.h>	*/

	printf("\n\n     Integral digits displayed: %ld", intBs);
	if (frcBs)
	    printf("\n   Fractional digits displayed: %ld", frcBs);
	if (repBs)
	    printf("\n(inside repeating parentheses): %ld", repBs);

	if (op.rem && !repBs)
	    printf("\n\t ** INEXACT SOLUTION -> Increase \\n:##");
	else
	    printf("\n\t   ** EXACT SOLUTION **");

	if ((cal_time = time(NULL)) != -1) {
	    strftime(bufferc, (size_t)SIZEc, "%c", localtime(&cal_time));
	    printf("\n\t   %s", bufferc);
	}

	if (com.flags & FL_L)
	    printf("\n Locale employed: %s", setlocale(0, NULL));

	printf("\nCommand executed:");
	do
	    printf(" %s", *argv++);
	while (--argc);
    }
    printf("\n");

/*  Memory release and successful exit	*/
    free(pdigf0);

    return EXIT_SUCCESS;

/*  Syntax error messages and exit	*/
err_exit0: err_exit1:
    syntax_disp(argv[0], com.B.n);

err_exit2:
    return EXIT_FAILURE;
}