
# line 27 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
#ifdef GAWKDEBUG
#define YYDEBUG 12
#endif

#include "awk.h"

#define CAN_FREE	TRUE
#define DONT_FREE	FALSE

#if defined(HAVE_STDARG_H) && defined(__STDC__) && __STDC__
static void yyerror(const char *m, ...) ;
#else
static void yyerror(); /* va_alist */
#endif
static char *get_src_buf P((void));
static int yylex P((void));
static NODE *node_common P((NODETYPE op));
static NODE *snode P((NODE *subn, NODETYPE op, int sindex));
static NODE *mkrangenode P((NODE *cpair));
static NODE *make_for_loop P((NODE *init, NODE *cond, NODE *incr));
static NODE *append_right P((NODE *list, NODE *new));
static void func_install P((NODE *params, NODE *def));
static void pop_var P((NODE *np, int freeit));
static void pop_params P((NODE *params));
static NODE *make_param P((char *name));
static NODE *mk_rexp P((NODE *exp));
static int dup_parms P((NODE *func));
static void param_sanity P((NODE *arglist));
static void parms_shadow P((const char *fname, NODE *func));
static int isnoeffect P((NODETYPE t));
static int isassignable P((NODE *n));
static void dumpintlstr P((char *str, size_t len));
static void dumpintlstr2 P((char *str1, size_t len1, char *str2, size_t len2));
static void count_args P((NODE *n));

enum defref { FUNC_DEFINE, FUNC_USE };
static void func_use P((char *name, enum defref how));
static void check_funcs P((void));

static int want_assign;		/* lexical scanning kludge */
static int want_regexp;		/* lexical scanning kludge */
static int can_return;		/* lexical scanning kludge */
static int io_allowed = TRUE;	/* lexical scanning kludge */
static int parsing_end_rule = FALSE; /* for warnings */
static char *lexptr;		/* pointer to next char during parsing */
static char *lexend;
static char *lexptr_begin;	/* keep track of where we were for error msgs */
static char *lexeme;		/* beginning of lexeme for debugging */
static char *thisline = NULL;
#define YYDEBUG_LEXER_TEXT (lexeme)
static int param_counter;
static char *tokstart = NULL;
static char *tok = NULL;
static char *tokend;

static long func_count;		/* total number of functions */

#define HASHSIZE	1021	/* this constant only used here */
NODE *variables[HASHSIZE];
static int var_count;		/* total number of global variables */

extern char *source;
extern int sourceline;
extern struct src *srcfiles;
extern int numfiles;
extern int errcount;
extern NODE *begin_block;
extern NODE *end_block;

# line 97 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	long lval;
	AWKNUM fval;
	NODE *nodeval;
	NODETYPE nodetypeval;
	char *sval;
	NODE *(*ptrval)();
} YYSTYPE;
# define FUNC_CALL 257
# define NAME 258
# define REGEXP 259
# define ERROR 260
# define YNUMBER 261
# define YSTRING 262
# define RELOP 263
# define APPEND_OP 264
# define ASSIGNOP 265
# define MATCHOP 266
# define NEWLINE 267
# define CONCAT_OP 268
# define LEX_BEGIN 269
# define LEX_END 270
# define LEX_IF 271
# define LEX_ELSE 272
# define LEX_RETURN 273
# define LEX_DELETE 274
# define LEX_WHILE 275
# define LEX_DO 276
# define LEX_FOR 277
# define LEX_BREAK 278
# define LEX_CONTINUE 279
# define LEX_PRINT 280
# define LEX_PRINTF 281
# define LEX_NEXT 282
# define LEX_EXIT 283
# define LEX_FUNCTION 284
# define LEX_GETLINE 285
# define LEX_NEXTFILE 286
# define LEX_IN 287
# define LEX_AND 288
# define LEX_OR 289
# define INCREMENT 290
# define DECREMENT 291
# define LEX_BUILTIN 292
# define LEX_LENGTH 293
# define TWOWAYIO 294
# define UNARY 295

#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#define	YYCONST	const
#else
#include <malloc.h>
#include <memory.h>
#define	YYCONST
#endif

#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef yyerror
#if defined(__cplusplus)
	void yyerror(YYCONST char *);
#endif
#endif
#ifndef yylex
	int yylex(void);
#endif
	int yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#endif

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 919 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"


struct token {
	const char *operator;		/* text to match */
	NODETYPE value;		/* node type */
	int class;		/* lexical class */
	unsigned flags;		/* # of args. allowed and compatability */
#	define	ARGS	0xFF	/* 0, 1, 2, 3 args allowed (any combination */
#	define	A(n)	(1<<(n))
#	define	VERSION_MASK	0xFF00	/* old awk is zero */
#	define	NOT_OLD		0x0100	/* feature not in old awk */
#	define	NOT_POSIX	0x0200	/* feature not in POSIX */
#	define	GAWKX		0x0400	/* gawk extension */
#	define	RESX		0x0800	/* Bell Labs Research extension */
	NODE *(*ptr)();		/* function that implements this keyword */
};

/* Tokentab is sorted ascii ascending order, so it can be binary searched. */
/* Function pointers come from declarations in awk.h. */

static struct token tokentab[] = {
{"BEGIN",	Node_illegal,	 LEX_BEGIN,	0,		0},
{"END",		Node_illegal,	 LEX_END,	0,		0},
#ifdef ARRAYDEBUG
{"adump",	Node_builtin,    LEX_BUILTIN,	GAWKX|A(1),	do_adump},
#endif
{"and",		Node_builtin,    LEX_BUILTIN,	GAWKX|A(2),	do_and},
{"asort",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(1)|A(2),	do_asort},
{"atan2",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(2),	do_atan2},
{"bindtextdomain",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(1)|A(2),	do_bindtextdomain},
{"break",	Node_K_break,	 LEX_BREAK,	0,		0},
{"close",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(1)|A(2),	do_close},
{"compl",	Node_builtin,    LEX_BUILTIN,	GAWKX|A(1),	do_compl},
{"continue",	Node_K_continue, LEX_CONTINUE,	0,		0},
{"cos",		Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(1),	do_cos},
{"dcgettext",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(1)|A(2)|A(3),	do_dcgettext},
{"dcngettext",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(1)|A(2)|A(3)|A(4)|A(5),	do_dcngettext},
{"delete",	Node_K_delete,	 LEX_DELETE,	NOT_OLD,	0},
{"do",		Node_K_do,	 LEX_DO,	NOT_OLD,	0},
{"else",	Node_illegal,	 LEX_ELSE,	0,		0},
{"exit",	Node_K_exit,	 LEX_EXIT,	0,		0},
{"exp",		Node_builtin,	 LEX_BUILTIN,	A(1),		do_exp},
{"extension",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(2),	do_ext},
{"fflush",	Node_builtin,	 LEX_BUILTIN,	RESX|A(0)|A(1), do_fflush},
{"for",		Node_K_for,	 LEX_FOR,	0,		0},
{"func",	Node_K_function, LEX_FUNCTION,	NOT_POSIX|NOT_OLD,	0},
{"function",	Node_K_function, LEX_FUNCTION,	NOT_OLD,	0},
{"gensub",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(3)|A(4), do_gensub},
{"getline",	Node_K_getline,	 LEX_GETLINE,	NOT_OLD,	0},
{"gsub",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(2)|A(3), do_gsub},
{"if",		Node_K_if,	 LEX_IF,	0,		0},
{"in",		Node_illegal,	 LEX_IN,	0,		0},
{"index",	Node_builtin,	 LEX_BUILTIN,	A(2),		do_index},
{"int",		Node_builtin,	 LEX_BUILTIN,	A(1),		do_int},
{"length",	Node_builtin,	 LEX_LENGTH,	A(0)|A(1),	do_length},
{"log",		Node_builtin,	 LEX_BUILTIN,	A(1),		do_log},
{"lshift",	Node_builtin,    LEX_BUILTIN,	GAWKX|A(2),	do_lshift},
{"match",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(2)|A(3), do_match},
{"mktime",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(1),	do_mktime},
{"next",	Node_K_next,	 LEX_NEXT,	0,		0},
{"nextfile",	Node_K_nextfile, LEX_NEXTFILE,	GAWKX,		0},
{"or",		Node_builtin,    LEX_BUILTIN,	GAWKX|A(2),	do_or},
{"print",	Node_K_print,	 LEX_PRINT,	0,		0},
{"printf",	Node_K_printf,	 LEX_PRINTF,	0,		0},
{"rand",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(0),	do_rand},
{"return",	Node_K_return,	 LEX_RETURN,	NOT_OLD,	0},
{"rshift",	Node_builtin,    LEX_BUILTIN,	GAWKX|A(2),	do_rshift},
{"sin",		Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(1),	do_sin},
{"split",	Node_builtin,	 LEX_BUILTIN,	A(2)|A(3),	do_split},
{"sprintf",	Node_builtin,	 LEX_BUILTIN,	0,		do_sprintf},
{"sqrt",	Node_builtin,	 LEX_BUILTIN,	A(1),		do_sqrt},
{"srand",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(0)|A(1), do_srand},
#if defined(GAWKDEBUG) || defined(ARRAYDEBUG) /* || ... */
{"stopme",	Node_builtin,    LEX_BUILTIN,	GAWKX|A(0),	stopme},
#endif
{"strftime",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(0)|A(1)|A(2), do_strftime},
{"strtonum",	Node_builtin,    LEX_BUILTIN,	GAWKX|A(1),	do_strtonum},
{"sub",		Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(2)|A(3), do_sub},
{"substr",	Node_builtin,	 LEX_BUILTIN,	A(2)|A(3),	do_substr},
{"system",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(1),	do_system},
{"systime",	Node_builtin,	 LEX_BUILTIN,	GAWKX|A(0),	do_systime},
{"tolower",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(1),	do_tolower},
{"toupper",	Node_builtin,	 LEX_BUILTIN,	NOT_OLD|A(1),	do_toupper},
{"while",	Node_K_while,	 LEX_WHILE,	0,		0},
{"xor",		Node_builtin,    LEX_BUILTIN,	GAWKX|A(2),	do_xor},
};

#ifdef MBS_SUPPORT
/* Variable containing the current shift state.  */
static mbstate_t cur_mbstate;
/* Ring buffer containing current characters.  */
#define MAX_CHAR_IN_RING_BUFFER 8
#define RING_BUFFER_SIZE (MAX_CHAR_IN_RING_BUFFER * MB_LEN_MAX)
static char cur_char_ring[RING_BUFFER_SIZE];
/* Index for ring buffers.  */
static int cur_ring_idx;
/* This macro means that last nextc() return a singlebyte character
   or 1st byte of a multibyte character.  */
#define nextc_is_1stbyte (cur_char_ring[cur_ring_idx] == 1)
#endif /* MBS_SUPPORT */

/* getfname --- return name of a builtin function (for pretty printing) */

const char *
getfname(register NODE *(*fptr)())
{
	register int i, j;

	j = sizeof(tokentab) / sizeof(tokentab[0]);
	/* linear search, no other way to do it */
	for (i = 0; i < j; i++) 
		if (tokentab[i].ptr == fptr)
			return tokentab[i].operator;

	fatal(_("fptr %x not in tokentab\n"), fptr);
	return NULL;    /* to stop warnings */
}

/* yyerror --- print a syntax error message, show where */

/*
 * Function identifier purposely indented to avoid mangling
 * by ansi2knr.  Sigh.
 */

static void
#if defined(HAVE_STDARG_H) && defined(__STDC__) && __STDC__
  yyerror(const char *m, ...)
#else
/* VARARGS0 */
  yyerror(va_alist)
  va_dcl
#endif
{
	va_list args;
	const char *mesg = NULL;
	register char *bp, *cp;
	char *scan;
	char *buf;
	int count;
	static char end_of_file_line[] = "(END OF FILE)";

	errcount++;
	/* Find the current line in the input file */
	if (lexptr && lexeme) {
		if (thisline == NULL) {
			cp = lexeme;
			if (*cp == '\n') {
				cp--;
				mesg = _("unexpected newline");
			}
			for (; cp != lexptr_begin && *cp != '\n'; --cp)
				continue;
			if (*cp == '\n')
				cp++;
			thisline = cp;
		}
		/* NL isn't guaranteed */
		bp = lexeme;
		while (bp < lexend && *bp && *bp != '\n')
			bp++;
	} else {
		thisline = end_of_file_line;
		bp = thisline + strlen(thisline);
	}
	msg("%.*s", (int) (bp - thisline), thisline);

#if defined(HAVE_STDARG_H) && defined(__STDC__) && __STDC__
	va_start(args, m);
	if (mesg == NULL)
		mesg = m;
#else
	va_start(args);
	if (mesg == NULL)
		mesg = va_arg(args, char *);
#endif
	count = (bp - thisline) + strlen(mesg) + 2 + 1;
	emalloc(buf, char *, count, "yyerror");

	bp = buf;

	if (lexptr != NULL) {
		scan = thisline;
		while (scan < lexeme)
			if (*scan++ == '\t')
				*bp++ = '\t';
			else
				*bp++ = ' ';
		*bp++ = '^';
		*bp++ = ' ';
	}
	strcpy(bp, mesg);
	err("", buf, args);
	va_end(args);
	free(buf);
}

/* get_src_buf --- read the next buffer of source program */

static char *
get_src_buf()
{
	static int samefile = FALSE;
	static int nextfile = 0;
	static char *buf = NULL;
	static int fd;
	int n;
	register char *scan;
	static int len = 0;
	static int did_newline = FALSE;
	int newfile;
	struct stat sbuf;

#	define	SLOP	128	/* enough space to hold most source lines */

again:
	newfile = FALSE;
	if (nextfile > numfiles)
		return NULL;

	if (srcfiles[nextfile].stype == CMDLINE) {
		if (len == 0) {
			len = strlen(srcfiles[nextfile].val);
			if (len == 0) {
				/*
				 * Yet Another Special case:
				 *	gawk '' /path/name
				 * Sigh.
				 */
				static int warned = FALSE;

				if (do_lint && ! warned) {
					warned = TRUE;
					lintwarn(_("empty program text on command line"));
				}
				++nextfile;
				goto again;
			}
			sourceline = 1;
			lexptr = lexptr_begin = srcfiles[nextfile].val;
			lexend = lexptr + len;
		} else if (! did_newline && *(lexptr-1) != '\n') {
			/*
			 * The following goop is to ensure that the source
			 * ends with a newline and that the entire current
			 * line is available for error messages.
			 */
			int offset;

			did_newline = TRUE;
			offset = lexptr - lexeme;
			for (scan = lexeme; scan > lexptr_begin; scan--)
				if (*scan == '\n') {
					scan++;
					break;
				}
			len = lexptr - scan;
			emalloc(buf, char *, len+1, "get_src_buf");
			memcpy(buf, scan, len);
			thisline = buf;
			lexptr = buf + len;
			*lexptr = '\n';
			lexeme = lexptr - offset;
			lexptr_begin = buf;
			lexend = lexptr + 1;
		} else {
			len = 0;
			lexeme = lexptr = lexptr_begin = NULL;
		}
		if (lexptr == NULL && ++nextfile <= numfiles)
			goto again;
		return lexptr;
	}
	if (! samefile) {
		source = srcfiles[nextfile].val;
		if (source == NULL) {
			if (buf != NULL) {
				free(buf);
				buf = NULL;
			}
			len = 0;
			return lexeme = lexptr = lexptr_begin = NULL;
		}
		fd = pathopen(source);
		if (fd <= INVALID_HANDLE) {
			char *in;

			/* suppress file name and line no. in error mesg */
			in = source;
			source = NULL;
			fatal(_("can't open source file `%s' for reading (%s)"),
				in, strerror(errno));
		}
		len = optimal_bufsize(fd, & sbuf);
		newfile = TRUE;
		if (buf != NULL)
			free(buf);
		emalloc(buf, char *, len + SLOP, "get_src_buf");
		lexptr_begin = buf + SLOP;
		samefile = TRUE;
		sourceline = 1;
	} else {
		/*
		 * Here, we retain the current source line (up to length SLOP)
		 * in the beginning of the buffer that was overallocated above
		 */
		int offset;
		int linelen;

		offset = lexptr - lexeme;
		for (scan = lexeme; scan > lexptr_begin; scan--)
			if (*scan == '\n') {
				scan++;
				break;
			}
		linelen = lexptr - scan;
		if (linelen > SLOP)
			linelen = SLOP;
		thisline = buf + SLOP - linelen;
		memcpy(thisline, scan, linelen);
		lexeme = buf + SLOP - offset;
		lexptr_begin = thisline;
	}
	n = read(fd, buf + SLOP, len);
	if (n == -1)
		fatal(_("can't read sourcefile `%s' (%s)"),
			source, strerror(errno));
	if (n == 0) {
		if (newfile) {
			static int warned = FALSE;

			if (do_lint && ! warned) {
				warned = TRUE;
				lintwarn(_("source file `%s' is empty"), source);
			}
		}
		if (fd != fileno(stdin)) /* safety */
			close(fd);
		samefile = FALSE;
		nextfile++;
		if (lexeme)
			*lexeme = '\0';
		len = 0;
		goto again;
	}
	lexptr = buf + SLOP;
	lexend = lexptr + n;
	return buf;
}

/* tokadd --- add a character to the token buffer */

#define	tokadd(x) (*tok++ = (x), tok == tokend ? tokexpand() : tok)

/* tokexpand --- grow the token buffer */

char *
tokexpand()
{
	static int toksize = 60;
	int tokoffset;

	tokoffset = tok - tokstart;
	toksize *= 2;
	if (tokstart != NULL)
		erealloc(tokstart, char *, toksize, "tokexpand");
	else
		emalloc(tokstart, char *, toksize, "tokexpand");
	tokend = tokstart + toksize;
	tok = tokstart + tokoffset;
	return tok;
}

/* nextc --- get the next input character */

#ifdef MBS_SUPPORT

static int
nextc()
{
	if (MB_CUR_MAX > 1)	{
		/* Update the buffer index.  */
		cur_ring_idx = (cur_ring_idx == RING_BUFFER_SIZE - 1)? 0 :
			cur_ring_idx + 1;

		/* Did we already check the current character?  */
		if (cur_char_ring[cur_ring_idx] == 0) {
			/* No, we need to check the next character on the buffer.  */
			int idx, work_ring_idx = cur_ring_idx;
			mbstate_t tmp_state;
			size_t mbclen;

			if (!lexptr || lexptr >= lexend)
				if (!get_src_buf()) {
					return EOF;
				}

			for (idx = 0 ; lexptr + idx < lexend ; idx++) {
				tmp_state = cur_mbstate;
				mbclen = mbrlen(lexptr, idx + 1, &tmp_state);

				if (mbclen == 1 || mbclen == (size_t)-1 || mbclen == 0) {
					/* It is a singlebyte character, non-complete multibyte
					   character or EOF.  We treat it as a singlebyte
					   character.  */
					cur_char_ring[work_ring_idx] = 1;
					break;
				} else if (mbclen == (size_t)-2) {
					/* It is not a complete multibyte character.  */
					cur_char_ring[work_ring_idx] = idx + 1;
				} else {
					/* mbclen > 1 */
					cur_char_ring[work_ring_idx] = mbclen;
					break;
				}
				work_ring_idx = (work_ring_idx == RING_BUFFER_SIZE - 1)?
					0 : work_ring_idx + 1;
			}
			cur_mbstate = tmp_state;

			/* Put a mark on the position on which we write next character.  */
			work_ring_idx = (work_ring_idx == RING_BUFFER_SIZE - 1)?
				0 : work_ring_idx + 1;
			cur_char_ring[work_ring_idx] = 0;
		}

		return (int) (unsigned char) *lexptr++;
	}
	else {
		int c;

		if (lexptr && lexptr < lexend)
			c = (int) (unsigned char) *lexptr++;
		else if (get_src_buf())
			c = (int) (unsigned char) *lexptr++;
		else
			c = EOF;

		return c;
	}
}

#else /* MBS_SUPPORT */

#if GAWKDEBUG
int
nextc()
{
	int c;

	if (lexptr && lexptr < lexend)
		c = (int) (unsigned char) *lexptr++;
	else if (get_src_buf())
		c = (int) (unsigned char) *lexptr++;
	else
		c = EOF;

	return c;
}
#else
#define	nextc()	((lexptr && lexptr < lexend) ? \
		    ((int) (unsigned char) *lexptr++) : \
		    (get_src_buf() ? ((int) (unsigned char) *lexptr++) : EOF) \
		)
#endif

#endif /* MBS_SUPPORT */

/* pushback --- push a character back on the input */

#ifdef MBS_SUPPORT

static void
pushback()
{
	if (MB_CUR_MAX > 1) {
		cur_ring_idx = (cur_ring_idx == 0)? RING_BUFFER_SIZE - 1 :
			cur_ring_idx - 1;
		(lexptr && lexptr > lexptr_begin ? lexptr-- : lexptr);
	} else
		(lexptr && lexptr > lexptr_begin ? lexptr-- : lexptr);
}

#else

#define pushback() (lexptr && lexptr > lexptr_begin ? lexptr-- : lexptr)

#endif /* MBS_SUPPORT */

/* allow_newline --- allow newline after &&, ||, ? and : */

static void
allow_newline()
{
	int c;

	for (;;) {
		c = nextc();
		if (c == EOF)
			break;
		if (c == '#') {
			while ((c = nextc()) != '\n' && c != EOF)
				continue;
			if (c == EOF)
				break;
		}
		if (c == '\n')
			sourceline++;
		if (! ISSPACE(c)) {
			pushback();
			break;
		}
	}
}

/* yylex --- Read the input and turn it into tokens. */

static int
yylex()
{
	register int c;
	int seen_e = FALSE;		/* These are for numbers */
	int seen_point = FALSE;
	int esc_seen;		/* for literal strings */
	int low, mid, high;
	static int did_newline = FALSE;
	char *tokkey;
	static int lasttok = 0, eof_warned = FALSE;
	int inhex = FALSE;
	int intlstr = FALSE;

	if (nextc() == EOF) {
		if (lasttok != NEWLINE) {
			lasttok = NEWLINE;
			if (do_lint && ! eof_warned) {
				lintwarn(_("source file does not end in newline"));
				eof_warned = TRUE;
			}
			return NEWLINE;	/* fake it */
		}
		return 0;
	}
	pushback();
#if defined OS2 || defined __EMX__
	/*
	 * added for OS/2's extproc feature of cmd.exe
	 * (like #! in BSD sh)
	 */
	if (strncasecmp(lexptr, "extproc ", 8) == 0) {
		while (*lexptr && *lexptr != '\n')
			lexptr++;
	}
#endif
	lexeme = lexptr;
	thisline = NULL;
	if (want_regexp) {
		int in_brack = 0;	/* count brackets, [[:alnum:]] allowed */
		/*
		 * Counting brackets is non-trivial. [[] is ok,
		 * and so is [\]], with a point being that /[/]/ as a regexp
		 * constant has to work.
		 *
		 * Do not count [ or ] if either one is preceded by a \.
		 * A `[' should be counted if
		 *  a) it is the first one so far (in_brack == 0)
		 *  b) it is the `[' in `[:'
		 * A ']' should be counted if not preceded by a \, since
		 * it is either closing `:]' or just a plain list.
		 * According to POSIX, []] is how you put a ] into a set.
		 * Try to handle that too.
		 *
		 * The code for \ handles \[ and \].
		 */

		want_regexp = FALSE;
		tok = tokstart;
		for (;;) {
			c = nextc();
#ifdef MBS_SUPPORT
			if (MB_CUR_MAX == 1 || nextc_is_1stbyte)
#endif
			switch (c) {
			case '[':
				/* one day check for `.' and `=' too */
				if (nextc() == ':' || in_brack == 0)
					in_brack++;
				pushback();
				break;
			case ']':
				if (tokstart[0] == '['
				    && (tok == tokstart + 1
					|| (tok == tokstart + 2
					    && tokstart[1] == '^')))
					/* do nothing */;
				else
					in_brack--;
				break;
			case '\\':
				if ((c = nextc()) == EOF) {
					yyerror(_("unterminated regexp ends with `\\' at end of file"));
					return lasttok = REGEXP; /* kludge */
				} else if (c == '\n') {
					sourceline++;
					continue;
				} else {
					tokadd('\\');
					tokadd(c);
					continue;
				}
				break;
			case '/':	/* end of the regexp */
				if (in_brack > 0)
					break;

				pushback();
				tokadd('\0');
				yylval.sval = tokstart;
				return lasttok = REGEXP;
			case '\n':
				pushback();
				yyerror(_("unterminated regexp"));
				return lasttok = REGEXP;	/* kludge */
			case EOF:
				yyerror(_("unterminated regexp at end of file"));
				return lasttok = REGEXP;	/* kludge */
			}
			tokadd(c);
		}
	}
retry:
	while ((c = nextc()) == ' ' || c == '\t')
		continue;

	lexeme = lexptr ? lexptr - 1 : lexptr;
	thisline = NULL;
	tok = tokstart;
	yylval.nodetypeval = Node_illegal;

#ifdef MBS_SUPPORT
	if (MB_CUR_MAX == 1 || nextc_is_1stbyte)
#endif
	switch (c) {
	case EOF:
		if (lasttok != NEWLINE) {
			lasttok = NEWLINE;
			if (do_lint && ! eof_warned) {
				lintwarn(_("source file does not end in newline"));
				eof_warned = TRUE;
			}
			return NEWLINE;	/* fake it */
		}
		return 0;

	case '\n':
		sourceline++;
		return lasttok = NEWLINE;

	case '#':		/* it's a comment */
		while ((c = nextc()) != '\n') {
			if (c == EOF) {
				if (lasttok != NEWLINE) {
					lasttok = NEWLINE;
					if (do_lint && ! eof_warned) {
						lintwarn(
				_("source file does not end in newline"));
						eof_warned = TRUE;
					}
					return NEWLINE;	/* fake it */
				}
				return 0;
			}
		}
		sourceline++;
		return lasttok = NEWLINE;

	case '\\':
#ifdef RELAXED_CONTINUATION
		/*
		 * This code puports to allow comments and/or whitespace
		 * after the `\' at the end of a line used for continuation.
		 * Use it at your own risk. We think it's a bad idea, which
		 * is why it's not on by default.
		 */
		if (! do_traditional) {
			/* strip trailing white-space and/or comment */
			while ((c = nextc()) == ' ' || c == '\t')
				continue;
			if (c == '#') {
				if (do_lint)
					lintwarn(
		_("use of `\\ #...' line continuation is not portable"));
				while ((c = nextc()) != '\n')
					if (c == EOF)
						break;
			}
			pushback();
		}
#endif /* RELAXED_CONTINUATION */
		if (nextc() == '\n') {
			sourceline++;
			goto retry;
		} else {
			yyerror(_("backslash not last character on line"));
			exit(1);
		}
		break;

	case '$':
		want_assign = TRUE;
		return lasttok = '$';

	case ':':
	case '?':
		if (! do_posix)
			allow_newline();
		return lasttok = c;

	case ')':
	case '(':	
	case ';':
	case '{':
	case ',':
		want_assign = FALSE;
		/* fall through */
	case '[':
	case ']':
		return lasttok = c;

	case '*':
		if ((c = nextc()) == '=') {
			yylval.nodetypeval = Node_assign_times;
			return lasttok = ASSIGNOP;
		} else if (do_posix) {
			pushback();
			return lasttok = '*';
		} else if (c == '*') {
			/* make ** and **= aliases for ^ and ^= */
			static int did_warn_op = FALSE, did_warn_assgn = FALSE;

			if (nextc() == '=') {
				if (! did_warn_assgn) {
					did_warn_assgn = TRUE;
					if (do_lint)
						lintwarn(_("POSIX does not allow operator `**='"));
					if (do_lint_old)
						warning(_("old awk does not support operator `**='"));
				}
				yylval.nodetypeval = Node_assign_exp;
				return ASSIGNOP;
			} else {
				pushback();
				if (! did_warn_op) {
					did_warn_op = TRUE;
					if (do_lint)
						lintwarn(_("POSIX does not allow operator `**'"));
					if (do_lint_old)
						warning(_("old awk does not support operator `**'"));
				}
				return lasttok = '^';
			}
		}
		pushback();
		return lasttok = '*';

	case '/':
		if (want_assign) {
			if (nextc() == '=') {
				yylval.nodetypeval = Node_assign_quotient;
				return lasttok = ASSIGNOP;
			}
			pushback();
		}
		return lasttok = '/';

	case '%':
		if (nextc() == '=') {
			yylval.nodetypeval = Node_assign_mod;
			return lasttok = ASSIGNOP;
		}
		pushback();
		return lasttok = '%';

	case '^':
	{
		static int did_warn_op = FALSE, did_warn_assgn = FALSE;

		if (nextc() == '=') {
			if (do_lint_old && ! did_warn_assgn) {
				did_warn_assgn = TRUE;
				warning(_("operator `^=' is not supported in old awk"));
			}
			yylval.nodetypeval = Node_assign_exp;
			return lasttok = ASSIGNOP;
		}
		pushback();
		if (do_lint_old && ! did_warn_op) {
			did_warn_op = TRUE;
			warning(_("operator `^' is not supported in old awk"));
		}
		return lasttok = '^';
	}

	case '+':
		if ((c = nextc()) == '=') {
			yylval.nodetypeval = Node_assign_plus;
			return lasttok = ASSIGNOP;
		}
		if (c == '+')
			return lasttok = INCREMENT;
		pushback();
		return lasttok = '+';

	case '!':
		if ((c = nextc()) == '=') {
			yylval.nodetypeval = Node_notequal;
			return lasttok = RELOP;
		}
		if (c == '~') {
			yylval.nodetypeval = Node_nomatch;
			want_assign = FALSE;
			return lasttok = MATCHOP;
		}
		pushback();
		return lasttok = '!';

	case '<':
		if (nextc() == '=') {
			yylval.nodetypeval = Node_leq;
			return lasttok = RELOP;
		}
		yylval.nodetypeval = Node_less;
		pushback();
		return lasttok = '<';

	case '=':
		if (nextc() == '=') {
			yylval.nodetypeval = Node_equal;
			return lasttok = RELOP;
		}
		yylval.nodetypeval = Node_assign;
		pushback();
		return lasttok = ASSIGNOP;

	case '>':
		if ((c = nextc()) == '=') {
			yylval.nodetypeval = Node_geq;
			return lasttok = RELOP;
		} else if (c == '>') {
			yylval.nodetypeval = Node_redirect_append;
			return lasttok = APPEND_OP;
		}
		yylval.nodetypeval = Node_greater;
		pushback();
		return lasttok = '>';

	case '~':
		yylval.nodetypeval = Node_match;
		want_assign = FALSE;
		return lasttok = MATCHOP;

	case '}':
		/*
		 * Added did newline stuff.  Easier than
		 * hacking the grammar.
		 */
		if (did_newline) {
			did_newline = FALSE;
			return lasttok = c;
		}
		did_newline++;
		--lexptr;	/* pick up } next time */
		return lasttok = NEWLINE;

	case '"':
	string:
		esc_seen = FALSE;
		while ((c = nextc()) != '"') {
			if (c == '\n') {
				pushback();
				yyerror(_("unterminated string"));
				exit(1);
			}
#ifdef MBS_SUPPORT
			if (MB_CUR_MAX == 1 || nextc_is_1stbyte)
#endif
			if (c == '\\') {
				c = nextc();
				if (c == '\n') {
					sourceline++;
					continue;
				}
				esc_seen = TRUE;
				tokadd('\\');
			}
			if (c == EOF) {
				pushback();
				yyerror(_("unterminated string"));
				exit(1);
			}
			tokadd(c);
		}
		yylval.nodeval = make_str_node(tokstart,
					tok - tokstart, esc_seen ? SCAN : 0);
		yylval.nodeval->flags |= PERM;
		if (intlstr) {
			yylval.nodeval->flags |= INTLSTR;
			intlstr = FALSE;
			if (do_intl)
				dumpintlstr(yylval.nodeval->stptr,
						yylval.nodeval->stlen);
 		}
		return lasttok = YSTRING;

	case '-':
		if ((c = nextc()) == '=') {
			yylval.nodetypeval = Node_assign_minus;
			return lasttok = ASSIGNOP;
		}
		if (c == '-')
			return lasttok = DECREMENT;
		pushback();
		return lasttok = '-';

	case '.':
		c = nextc();
		pushback();
		if (! ISDIGIT(c))
			return lasttok = '.';
		else
			c = '.';
		/* FALL THROUGH */
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		/* It's a number */
		for (;;) {
			int gotnumber = FALSE;

			tokadd(c);
			switch (c) {
			case 'x':
			case 'X':
				if (do_traditional)
					goto done;
				if (tok == tokstart + 2)
					inhex = TRUE;
				break;
			case '.':
				if (seen_point) {
					gotnumber = TRUE;
					break;
				}
				seen_point = TRUE;
				break;
			case 'e':
			case 'E':
				if (inhex)
					break;
				if (seen_e) {
					gotnumber = TRUE;
					break;
				}
				seen_e = TRUE;
				if ((c = nextc()) == '-' || c == '+')
					tokadd(c);
				else
					pushback();
				break;
			case 'a':
			case 'A':
			case 'b':
			case 'B':
			case 'c':
			case 'C':
			case 'D':
			case 'd':
			case 'f':
			case 'F':
				if (do_traditional || ! inhex)
					goto done;
				/* fall through */
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			default:
			done:
				gotnumber = TRUE;
			}
			if (gotnumber)
				break;
			c = nextc();
		}
		if (c != EOF)
			pushback();
		else if (do_lint && ! eof_warned) {
			lintwarn(_("source file does not end in newline"));
			eof_warned = TRUE;
		}
		tokadd('\0');
		if (! do_traditional && isnondecimal(tokstart)) {
			static short warned = FALSE;
			if (do_lint && ! warned) {
				warned = TRUE;
				lintwarn("numeric constant `%.*s' treated as octal or hexadecimal",
					strlen(tokstart)-1, tokstart);
			}
			yylval.nodeval = make_number(nondec2awknum(tokstart, strlen(tokstart)));
		} else
			yylval.nodeval = make_number(atof(tokstart));
		yylval.nodeval->flags |= PERM;
		return lasttok = YNUMBER;

	case '&':
		if ((c = nextc()) == '&') {
			yylval.nodetypeval = Node_and;
			allow_newline();
			want_assign = FALSE;
			return lasttok = LEX_AND;
		}
		pushback();
		return lasttok = '&';

	case '|':
		if ((c = nextc()) == '|') {
			yylval.nodetypeval = Node_or;
			allow_newline();
			want_assign = FALSE;
			return lasttok = LEX_OR;
		} else if (! do_traditional && c == '&') {
			yylval.nodetypeval = Node_redirect_twoway;
			want_assign = FALSE;
			return lasttok = TWOWAYIO;
		}
		pushback();
		return lasttok = '|';
	}

	if (c != '_' && ! ISALPHA(c)) {
		yyerror(_("invalid char '%c' in expression"), c);
		exit(1);
	}

	/*
	 * Lots of fog here.  Consider:
	 *
	 * print "xyzzy"$_"foo"
	 *
	 * Without the check for ` lasttok != '$'' ', this is parsed as
	 *
	 * print "xxyzz" $(_"foo")
	 *
	 * With the check, it is "correctly" parsed as three
	 * string concatenations.  Sigh.  This seems to be
	 * "more correct", but this is definitely one of those
	 * occasions where the interactions are funny.
	 */
	if (! do_traditional && c == '_' && lasttok != '$') {
		if ((c = nextc()) == '"') {
			intlstr = TRUE;
			goto string;
		}
		pushback();
		c = '_';
	}

	/* it's some type of name-type-thing.  Find its length. */
	tok = tokstart;
	while (is_identchar(c)) {
		tokadd(c);
		c = nextc();
	}
	tokadd('\0');
	emalloc(tokkey, char *, tok - tokstart, "yylex");
	memcpy(tokkey, tokstart, tok - tokstart);
	if (c != EOF)
		pushback();
	else if (do_lint && ! eof_warned) {
		lintwarn(_("source file does not end in newline"));
		eof_warned = TRUE;
	}

	/* See if it is a special token. */
	low = 0;
	high = (sizeof(tokentab) / sizeof(tokentab[0])) - 1;
	while (low <= high) {
		int i;

		mid = (low + high) / 2;
		c = *tokstart - tokentab[mid].operator[0];
		i = c ? c : strcmp(tokstart, tokentab[mid].operator);

		if (i < 0)		/* token < mid */
			high = mid - 1;
		else if (i > 0)		/* token > mid */
			low = mid + 1;
		else {
			if (do_lint) {
				if (tokentab[mid].flags & GAWKX)
					lintwarn(_("`%s' is a gawk extension"),
						tokentab[mid].operator);
				if (tokentab[mid].flags & RESX)
					lintwarn(_("`%s' is a Bell Labs extension"),
						tokentab[mid].operator);
				if (tokentab[mid].flags & NOT_POSIX)
					lintwarn(_("POSIX does not allow `%s'"),
						tokentab[mid].operator);
			}
			if (do_lint_old && (tokentab[mid].flags & NOT_OLD))
				warning(_("`%s' is not supported in old awk"),
						tokentab[mid].operator);
			if ((do_traditional && (tokentab[mid].flags & GAWKX))
			    || (do_posix && (tokentab[mid].flags & NOT_POSIX)))
				break;
			if (tokentab[mid].class == LEX_BUILTIN
			    || tokentab[mid].class == LEX_LENGTH
			   )
				yylval.lval = mid;
			else
				yylval.nodetypeval = tokentab[mid].value;

			free(tokkey);
			return lasttok = tokentab[mid].class;
		}
	}

	yylval.sval = tokkey;
	if (*lexptr == '(')
		return lasttok = FUNC_CALL;
	else {
		static short goto_warned = FALSE;

		want_assign = TRUE;
#define SMART_ALECK	1
		if (SMART_ALECK && do_lint
		    && ! goto_warned && strcasecmp(tokkey, "goto") == 0) {
			goto_warned = TRUE;
			lintwarn(_("`goto' considered harmful!\n"));
		}
		return lasttok = NAME;
	}
}

/* node_common --- common code for allocating a new node */

static NODE *
node_common(NODETYPE op)
{
	register NODE *r;

	getnode(r);
	r->type = op;
	r->flags = MALLOC;
	if (r->type == Node_var)
		r->flags |= UNINITIALIZED;
	/* if lookahead is NL, lineno is 1 too high */
	if (lexeme && *lexeme == '\n')
		r->source_line = sourceline - 1;
	else
		r->source_line = sourceline;
	r->source_file = source;
	return r;
}

/* node --- allocates a node with defined lnode and rnode. */

NODE *
node(NODE *left, NODETYPE op, NODE *right)
{
	register NODE *r;

	r = node_common(op);
	r->lnode = left;
	r->rnode = right;
	return r;
}

/* snode ---	allocate a node with defined subnode and proc for builtin
		functions. Checks for arg. count and supplies defaults where
		possible. */

static NODE *
snode(NODE *subn, NODETYPE op, int idx)
{
	register NODE *r;
	register NODE *n;
	int nexp = 0;
	int args_allowed;

	r = node_common(op);

	/* traverse expression list to see how many args. given */
	for (n = subn; n != NULL; n = n->rnode) {
		nexp++;
		if (nexp > 5)
			break;
	}

	/* check against how many args. are allowed for this builtin */
	args_allowed = tokentab[idx].flags & ARGS;
	if (args_allowed && (args_allowed & A(nexp)) == 0)
		fatal(_("%d is invalid as number of arguments for %s"),
				nexp, tokentab[idx].operator);

	r->proc = tokentab[idx].ptr;

	/* special case processing for a few builtins */
	if (nexp == 0 && r->proc == do_length) {
		subn = node(node(make_number(0.0), Node_field_spec, (NODE *) NULL),
		            Node_expression_list,
			    (NODE *) NULL);
	} else if (r->proc == do_match) {
		static short warned = FALSE;

		if (subn->rnode->lnode->type != Node_regex)
			subn->rnode->lnode = mk_rexp(subn->rnode->lnode);

		if (subn->rnode->rnode != NULL) {	/* 3rd argument there */
			if (do_lint && ! warned) {
				warned = TRUE;
				lintwarn(_("match: third argument is a gawk extension"));
			}
			if (do_traditional)
				fatal(_("match: third argument is a gawk extension"));
		}
	} else if (r->proc == do_sub || r->proc == do_gsub) {
		if (subn->lnode->type != Node_regex)
			subn->lnode = mk_rexp(subn->lnode);
		if (nexp == 2)
			append_right(subn, node(node(make_number(0.0),
						     Node_field_spec,
						     (NODE *) NULL),
					        Node_expression_list,
						(NODE *) NULL));
		else if (subn->rnode->rnode->lnode->type == Node_val) {
			if (do_lint) {
				char *f;

				f = (r->proc == do_sub) ? "sub" : "gsub";
				lintwarn(_("%s: string literal as last arg of substitute has no effect"), f);
			}
		} else if (! isassignable(subn->rnode->rnode->lnode)) {
			if (r->proc == do_sub)
				yyerror(_("sub third parameter is not a changeable object"));
			else
				yyerror(_("gsub third parameter is not a changeable object"));
		}
	} else if (r->proc == do_gensub) {
		if (subn->lnode->type != Node_regex)
			subn->lnode = mk_rexp(subn->lnode);
		if (nexp == 3)
			append_right(subn, node(node(make_number(0.0),
						     Node_field_spec,
						     (NODE *) NULL),
					        Node_expression_list,
						(NODE *) NULL));
	} else if (r->proc == do_split) {
		if (nexp == 2)
			append_right(subn,
			    node(FS_node, Node_expression_list, (NODE *) NULL));
		n = subn->rnode->rnode->lnode;
		if (n->type != Node_regex)
			subn->rnode->rnode->lnode = mk_rexp(n);
		if (nexp == 2)
			subn->rnode->rnode->lnode->re_flags |= FS_DFLT;
	} else if (r->proc == do_close) {
		static short warned = FALSE;

		if ( nexp == 2) {
			if (do_lint && nexp == 2 && ! warned) {
				warned = TRUE;
				lintwarn(_("close: second argument is a gawk extension"));
			}
			if (do_traditional)
				fatal(_("close: second argument is a gawk extension"));
		}
	} else if (do_intl					/* --gen-po */
			&& r->proc == do_dcgettext		/* dcgettext(...) */
			&& subn->lnode->type == Node_val	/* 1st arg is constant */
			&& (subn->lnode->flags & STR) != 0) {	/* it's a string constant */
		/* ala xgettext, dcgettext("some string" ...) dumps the string */
		NODE *str = subn->lnode;

		if ((str->flags & INTLSTR) != 0)
			warning(_("use of dcgettext(_\"...\") is incorrect: remove leading underscore"));
			/* don't dump it, the lexer already did */
		else
			dumpintlstr(str->stptr, str->stlen);
	} else if (do_intl					/* --gen-po */
			&& r->proc == do_dcngettext		/* dcngettext(...) */
			&& subn->lnode->type == Node_val	/* 1st arg is constant */
			&& (subn->lnode->flags & STR) != 0	/* it's a string constant */
			&& subn->rnode->lnode->type == Node_val	/* 2nd arg is constant too */
			&& (subn->rnode->lnode->flags & STR) != 0) {	/* it's a string constant */
		/* ala xgettext, dcngettext("some string", "some plural" ...) dumps the string */
		NODE *str1 = subn->lnode;
		NODE *str2 = subn->rnode->lnode;

		if (((str1->flags | str2->flags) & INTLSTR) != 0)
			warning(_("use of dcngettext(_\"...\") is incorrect: remove leading underscore"));
		else
			dumpintlstr2(str1->stptr, str1->stlen, str2->stptr, str2->stlen);
	}

	r->subnode = subn;
	if (r->proc == do_sprintf) {
		count_args(r);
		r->lnode->printf_count = r->printf_count; /* hack */
	}
	return r;
}

/*
 * mkrangenode:
 * This allocates a Node_line_range node with defined condpair and
 * zeroes the trigger word to avoid the temptation of assuming that calling
 * 'node( foo, Node_line_range, 0)' will properly initialize 'triggered'. 
 * Otherwise like node().
 */

static NODE *
mkrangenode(NODE *cpair)
{
	register NODE *r;

	getnode(r);
	r->type = Node_line_range;
	r->condpair = cpair;
	r->triggered = FALSE;
	return r;
}

/* make_for_loop --- build a for loop */

static NODE *
make_for_loop(NODE *init, NODE *cond, NODE *incr)
{
	register FOR_LOOP_HEADER *r;
	NODE *n;

	emalloc(r, FOR_LOOP_HEADER *, sizeof(FOR_LOOP_HEADER), "make_for_loop");
	getnode(n);
	n->type = Node_illegal;
	r->init = init;
	r->cond = cond;
	r->incr = incr;
	n->sub.nodep.r.hd = r;
	return n;
}

/* dup_parms --- return TRUE if there are duplicate parameters */

static int
dup_parms(NODE *func)
{
	register NODE *np;
	char *fname, **names;
	int count, i, j, dups;
	NODE *params;

	if (func == NULL)	/* error earlier */
		return TRUE;

	fname = func->param;
	count = func->param_cnt;
	params = func->rnode;

	if (count == 0)		/* no args, no problem */
		return FALSE;

	if (params == NULL)	/* error earlier */
		return TRUE;

	emalloc(names, char **, count * sizeof(char *), "dup_parms");

	i = 0;
	for (np = params; np != NULL; np = np->rnode) {
		if (np->param == NULL) { /* error earlier, give up, go home */
			free(names);
			return TRUE;
		}
		names[i++] = np->param;
	}

	dups = 0;
	for (i = 1; i < count; i++) {
		for (j = 0; j < i; j++) {
			if (strcmp(names[i], names[j]) == 0) {
				dups++;
				error(
	_("function `%s': parameter #%d, `%s', duplicates parameter #%d"),
					fname, i+1, names[j], j+1);
			}
		}
	}

	free(names);
	return (dups > 0 ? TRUE : FALSE);
}

/* parms_shadow --- check if parameters shadow globals */

static void
parms_shadow(const char *fname, NODE *func)
{
	int count, i;

	if (fname == NULL || func == NULL)	/* error earlier */
		return;

	count = func->lnode->param_cnt;

	if (count == 0)		/* no args, no problem */
		return;

	/*
	 * Use warning() and not lintwarn() so that can warn
	 * about all shadowed parameters.
	 */
	for (i = 0; i < count; i++) {
		if (lookup(func->parmlist[i]) != NULL) {
			warning(
	_("function `%s': parameter `%s' shadows global variable"),
					fname, func->parmlist[i]);
		}
	}
}

/*
 * install:
 * Install a name in the symbol table, even if it is already there.
 * Caller must check against redefinition if that is desired. 
 */

NODE *
install(char *name, NODE *value)
{
	register NODE *hp;
	register size_t len;
	register int bucket;

	var_count++;
	len = strlen(name);
	bucket = hash(name, len, (unsigned long) HASHSIZE);
	getnode(hp);
	hp->type = Node_hashnode;
	hp->hnext = variables[bucket];
	variables[bucket] = hp;
	hp->hlength = len;
	hp->hvalue = value;
	hp->hname = name;
	hp->hvalue->vname = name;
	return hp->hvalue;
}

/* lookup --- find the most recent hash node for name installed by install */

NODE *
lookup(const char *name)
{
	register NODE *bucket;
	register size_t len;

	len = strlen(name);
	for (bucket = variables[hash(name, len, (unsigned long) HASHSIZE)];
			bucket != NULL; bucket = bucket->hnext)
		if (bucket->hlength == len && STREQN(bucket->hname, name, len))
			return bucket->hvalue;

	return NULL;
}

/* var_comp --- compare two variable names */

static int
var_comp(const void *v1, const void *v2)
{
	NODE **npp1, **npp2;
	NODE *n1, *n2;
	int minlen;

	npp1 = (NODE **) v1;
	npp2 = (NODE **) v2;
	n1 = *npp1;
	n2 = *npp2;

	if (n1->hlength > n2->hlength)
		minlen = n1->hlength;
	else
		minlen = n2->hlength;

	return strncmp(n1->hname, n2->hname, minlen);
}

/* valinfo --- dump var info */

static void
valinfo(NODE *n, FILE *fp)
{
	if (n->flags & STRING) {
		fprintf(fp, "string (");
		pp_string_fp(fp, n->stptr, n->stlen, '"', FALSE);
		fprintf(fp, ")\n");
	} else if (n->flags & NUMBER)
		fprintf(fp, "number (%.17g)\n", n->numbr);
	else if (n->flags & STR) {
		fprintf(fp, "string value (");
		pp_string_fp(fp, n->stptr, n->stlen, '"', FALSE);
		fprintf(fp, ")\n");
	} else if (n->flags & NUM)
		fprintf(fp, "number value (%.17g)\n", n->numbr);
	else
		fprintf(fp, "?? flags %s\n", flags2str(n->flags));
}


/* dump_vars --- dump the symbol table */

void
dump_vars(const char *fname)
{
	int i, j;
	NODE **table;
	NODE *p;
	FILE *fp;

	emalloc(table, NODE **, var_count * sizeof(NODE *), "dump_vars");

	if (fname == NULL)
		fp = stderr;
	else if ((fp = fopen(fname, "w")) == NULL) {
		warning(_("could not open `%s' for writing (%s)"), fname, strerror(errno));
		warning(_("sending profile to standard error"));
		fp = stderr;
	}

	for (i = j = 0; i < HASHSIZE; i++)
		for (p = variables[i]; p != NULL; p = p->hnext)
			table[j++] = p;

	assert(j == var_count);

	/* Shazzam! */
	qsort(table, j, sizeof(NODE *), var_comp);

	for (i = 0; i < j; i++) {
		p = table[i];
		if (p->hvalue->type == Node_func)
			continue;
		fprintf(fp, "%.*s: ", (int) p->hlength, p->hname);
		if (p->hvalue->type == Node_var_array)
			fprintf(fp, "array, %ld elements\n", p->hvalue->table_size);
		else if (p->hvalue->type == Node_var)
			valinfo(p->hvalue->var_value, fp);
		else {
			NODE **lhs = get_lhs(p->hvalue, NULL, FALSE);

			valinfo(*lhs, fp);
		}
	}

	if (fp != stderr && fclose(fp) != 0)
		warning(_("%s: close failed (%s)"), fname, strerror(errno));

	free(table);
}

/* release_all_vars --- free all variable memory */

void
release_all_vars()
{
	int i;
	NODE *p, *next;

	for (i = 0; i < HASHSIZE; i++)
		for (p = variables[i]; p != NULL; p = next) {
			next = p->hnext;

			if (p->hvalue->type == Node_func)
				continue;
			else if (p->hvalue->type == Node_var_array)
				assoc_clear(p->hvalue);
			else if (p->hvalue->type == Node_var)
				unref(p->hvalue->var_value);
			else {
				NODE **lhs = get_lhs(p->hvalue, NULL, FALSE);

				unref((*lhs)->var_value);
			}
			unref(p);
	}
}

/* finfo --- for use in comparison and sorting of function names */

struct finfo {
	char *name;
	size_t nlen;
	NODE *func;
};

/* fcompare --- comparison function for qsort */

static int
fcompare(const void *p1, const void *p2)
{
	struct finfo *f1, *f2;
	int minlen;

	f1 = (struct finfo *) p1;
	f2 = (struct finfo *) p2;

	if (f1->nlen > f2->nlen)
		minlen = f2->nlen;
	else
		minlen = f1->nlen;

	return strncmp(f1->name, f2->name, minlen);
}

/* dump_funcs --- print all functions */

void
dump_funcs()
{
	int i, j;
	NODE *p;
	static struct finfo *tab = NULL;

	if (func_count == 0)
		return;

	if (tab == NULL)
		emalloc(tab, struct finfo *, func_count * sizeof(struct finfo), "dump_funcs");

	for (i = j = 0; i < HASHSIZE; i++) {
		for (p = variables[i]; p != NULL; p = p->hnext) {
			if (p->hvalue->type == Node_func) {
				tab[j].name = p->hname;
				tab[j].nlen = p->hlength;
				tab[j].func = p->hvalue;
				j++;
			}
		}
	}

	assert(j == func_count);

	/* Shazzam! */
	qsort(tab, func_count, sizeof(struct finfo), fcompare);

	for (i = 0; i < j; i++)
		pp_func(tab[i].name, tab[i].nlen, tab[i].func);

	free(tab);
}

/* shadow_funcs --- check all functions for parameters that shadow globals */

void
shadow_funcs()
{
	int i, j;
	NODE *p;
	struct finfo *tab;
	static int calls = 0;

	if (func_count == 0)
		return;

	if (calls++ != 0)
		fatal(_("shadow_funcs() called twice!"));

	emalloc(tab, struct finfo *, func_count * sizeof(struct finfo), "shadow_funcs");

	for (i = j = 0; i < HASHSIZE; i++) {
		for (p = variables[i]; p != NULL; p = p->hnext) {
			if (p->hvalue->type == Node_func) {
				tab[j].name = p->hname;
				tab[j].nlen = p->hlength;
				tab[j].func = p->hvalue;
				j++;
			}
		}
	}

	assert(j == func_count);

	/* Shazzam! */
	qsort(tab, func_count, sizeof(struct finfo), fcompare);

	for (i = 0; i < j; i++)
		parms_shadow(tab[i].name, tab[i].func);

	free(tab);
}

/*
 * append_right:
 * Add new to the rightmost branch of LIST.  This uses n^2 time, so we make
 * a simple attempt at optimizing it.
 */

static NODE *
append_right(NODE *list, NODE *new)
{
	register NODE *oldlist;
	static NODE *savefront = NULL, *savetail = NULL;

	if (list == NULL || new == NULL)
		return list;

	oldlist = list;
	if (savefront == oldlist) {
		savetail = savetail->rnode = new;
		return oldlist;
	} else
		savefront = oldlist;
	while (list->rnode != NULL)
		list = list->rnode;
	savetail = list->rnode = new;
	return oldlist;
}

/*
 * func_install:
 * check if name is already installed;  if so, it had better have Null value,
 * in which case def is added as the value. Otherwise, install name with def
 * as value. 
 *
 * Extra work, build up and save a list of the parameter names in a table
 * and hang it off params->parmlist. This is used to set the `vname' field
 * of each function parameter during a function call. See eval.c.
 */

static void
func_install(NODE *params, NODE *def)
{
	NODE *r, *n, *thisfunc;
	char **pnames, *names, *sp;
	size_t pcount = 0, space = 0;
	int i;

	/* check for function foo(foo) { ... }.  bleah. */
	for (n = params->rnode; n != NULL; n = n->rnode) {
		if (strcmp(n->param, params->param) == 0)
			fatal(_("function `%s': can't use function name as parameter name"),
					params->param); 
	}

	thisfunc = NULL;	/* turn off warnings */

	/* symbol table managment */
	pop_var(params, FALSE);
	r = lookup(params->param);
	if (r != NULL) {
		fatal(_("function name `%s' previously defined"), params->param);
	} else {
		thisfunc = node(params, Node_func, def);
		(void) install(params->param, thisfunc);
	}

	/* figure out amount of space to allocate */
	for (n = params->rnode; n != NULL; n = n->rnode) {
		pcount++;
		space += strlen(n->param) + 1;
	}

	/* allocate it and fill it in */
	if (pcount != 0) {
		emalloc(names, char *, space, "func_install");
		emalloc(pnames, char **, pcount * sizeof(char *), "func_install");
		sp = names;
		for (i = 0, n = params->rnode; i < pcount; i++, n = n->rnode) {
			pnames[i] = sp;
			strcpy(sp, n->param);
			sp += strlen(n->param) + 1;
		}
		thisfunc->parmlist = pnames;
	} else {
		thisfunc->parmlist = NULL;
	}

	/* remove params from symbol table */
	pop_params(params->rnode);

	/* update lint table info */
	func_use(params->param, FUNC_DEFINE);

	func_count++;	/* used by profiling / pretty printer */
}

/* pop_var --- remove a variable from the symbol table */

static void
pop_var(NODE *np, int freeit)
{
	register NODE *bucket, **save;
	register size_t len;
	char *name;

	name = np->param;
	len = strlen(name);
	save = &(variables[hash(name, len, (unsigned long) HASHSIZE)]);
	for (bucket = *save; bucket != NULL; bucket = bucket->hnext) {
		if (len == bucket->hlength && STREQN(bucket->hname, name, len)) {
			var_count--;
			*save = bucket->hnext;
			freenode(bucket);
			if (freeit)
				free(np->param);
			return;
		}
		save = &(bucket->hnext);
	}
}

/* pop_params --- remove list of function parameters from symbol table */

/*
 * pop parameters out of the symbol table. do this in reverse order to
 * avoid reading freed memory if there were duplicated parameters.
 */
static void
pop_params(NODE *params)
{
	if (params == NULL)
		return;
	pop_params(params->rnode);
	pop_var(params, TRUE);
}

/* make_param --- make NAME into a function parameter */

static NODE *
make_param(char *name)
{
	NODE *r;

	getnode(r);
	r->type = Node_param_list;
	r->rnode = NULL;
	r->param = name;
	r->param_cnt = param_counter++;
	return (install(name, r));
}

static struct fdesc {
	char *name;
	short used;
	short defined;
	struct fdesc *next;
} *ftable[HASHSIZE];

/* func_use --- track uses and definitions of functions */

static void
func_use(char *name, enum defref how)
{
	struct fdesc *fp;
	int len;
	int ind;

	len = strlen(name);
	ind = hash(name, len, HASHSIZE);

	for (fp = ftable[ind]; fp != NULL; fp = fp->next) {
		if (strcmp(fp->name, name) == 0) {
			if (how == FUNC_DEFINE)
				fp->defined++;
			else
				fp->used++;
			return;
		}
	}

	/* not in the table, fall through to allocate a new one */

	emalloc(fp, struct fdesc *, sizeof(struct fdesc), "func_use");
	memset(fp, '\0', sizeof(struct fdesc));
	emalloc(fp->name, char *, len + 1, "func_use");
	strcpy(fp->name, name);
	if (how == FUNC_DEFINE)
		fp->defined++;
	else
		fp->used++;
	fp->next = ftable[ind];
	ftable[ind] = fp;
}

/* check_funcs --- verify functions that are called but not defined */

static void
check_funcs()
{
	struct fdesc *fp, *next;
	int i;

	for (i = 0; i < HASHSIZE; i++) {
		for (fp = ftable[i]; fp != NULL; fp = fp->next) {
#ifdef REALLYMEAN
			/* making this the default breaks old code. sigh. */
			if (fp->defined == 0) {
				error(
		_("function `%s' called but never defined"), fp->name);
				errcount++;
			}
#else
			if (do_lint && fp->defined == 0)
				lintwarn(
		_("function `%s' called but never defined"), fp->name);
#endif
			if (do_lint && fp->used == 0) {
				lintwarn(_("function `%s' defined but never called"),
					fp->name);
			}
		}
	}

	/* now let's free all the memory */
	for (i = 0; i < HASHSIZE; i++) {
		for (fp = ftable[i]; fp != NULL; fp = next) {
			next = fp->next;
			free(fp->name);
			free(fp);
		}
	}
}

/* param_sanity --- look for parameters that are regexp constants */

static void
param_sanity(NODE *arglist)
{
	NODE *argp, *arg;
	int i;

	for (i = 1, argp = arglist; argp != NULL; argp = argp->rnode, i++) {
		arg = argp->lnode;
		if (arg->type == Node_regex)
			warning(_("regexp constant for parameter #%d yields boolean value"), i);
	}
}

/* variable --- make sure NAME is in the symbol table */

NODE *
variable(char *name, int can_free, NODETYPE type)
{
	register NODE *r;
	static int env_loaded = FALSE;
	static int procinfo_loaded = FALSE;

	if (! env_loaded && STREQ(name, "ENVIRON")) {
		load_environ();
		env_loaded = TRUE;
	}
	if (! do_traditional && ! procinfo_loaded && STREQ(name, "PROCINFO")) {
		load_procinfo();
		procinfo_loaded = TRUE;
	}
	if ((r = lookup(name)) == NULL)
		r = install(name, node(Nnull_string, type, (NODE *) NULL));
	else if (can_free)
		free(name);
	return r;
}

/* mk_rexp --- make a regular expression constant */

static NODE *
mk_rexp(NODE *exp)
{
	NODE *n;

	if (exp->type == Node_regex)
		return exp;

	getnode(n);
	n->type = Node_dynregex;
	n->re_exp = exp;
	n->re_text = NULL;
	n->re_reg = NULL;
	n->re_flags = 0;
	n->re_cnt = 1;
	return n;
}

/* isnoeffect --- when used as a statement, has no side effects */

/*
 * To be completely general, we should recursively walk the parse
 * tree, to make sure that all the subexpressions also have no effect.
 * Instead, we just weaken the actual warning that's printed, up above
 * in the grammar.
 */

static int
isnoeffect(NODETYPE type)
{
	switch (type) {
	case Node_times:
	case Node_quotient:
	case Node_mod:
	case Node_plus:
	case Node_minus:
	case Node_subscript:
	case Node_concat:
	case Node_exp:
	case Node_unary_minus:
	case Node_field_spec:
	case Node_and:
	case Node_or:
	case Node_equal:
	case Node_notequal:
	case Node_less:
	case Node_greater:
	case Node_leq:
	case Node_geq:
	case Node_match:
	case Node_nomatch:
	case Node_not:
	case Node_val:
	case Node_in_array:
	case Node_NF:
	case Node_NR:
	case Node_FNR:
	case Node_FS:
	case Node_RS:
	case Node_FIELDWIDTHS:
	case Node_IGNORECASE:
	case Node_OFS:
	case Node_ORS:
	case Node_OFMT:
	case Node_CONVFMT:
	case Node_BINMODE:
	case Node_LINT:
		return TRUE;
	default:
		break;	/* keeps gcc -Wall happy */
	}

	return FALSE;
}

/* isassignable --- can this node be assigned to? */

static int
isassignable(register NODE *n)
{
	switch (n->type) {
	case Node_var:
	case Node_FIELDWIDTHS:
	case Node_RS:
	case Node_FS:
	case Node_FNR:
	case Node_NR:
	case Node_NF:
	case Node_IGNORECASE:
	case Node_OFMT:
	case Node_CONVFMT:
	case Node_ORS:
	case Node_OFS:
	case Node_LINT:
	case Node_BINMODE:
	case Node_field_spec:
	case Node_subscript:
		return TRUE;
	case Node_param_list:
		return ((n->flags & FUNC) == 0);  /* ok if not func name */
	default:
		break;	/* keeps gcc -Wall happy */
	}
	return FALSE;
}

/* stopme --- for debugging */

NODE *
stopme(NODE *tree)
{
	return 0;
}

/* dumpintlstr --- write out an initial .po file entry for the string */

static void
dumpintlstr(char *str, size_t len)
{
	char *cp;

	/* See the GNU gettext distribution for details on the file format */

	if (source != NULL) {
		/* ala the gettext sources, remove leading `./'s */
		for (cp = source; cp[0] == '.' && cp[1] == '/'; cp += 2)
			continue;
		printf("#: %s:%d\n", cp, sourceline);
	}

	printf("msgid ");
	pp_string_fp(stdout, str, len, '"', TRUE);
	putchar('\n');
	printf("msgstr \"\"\n\n");
	fflush(stdout);
}

/* dumpintlstr2 --- write out an initial .po file entry for the string and its plural */

static void
dumpintlstr2(char *str1, size_t len1, char *str2, size_t len2)
{
	char *cp;

	/* See the GNU gettext distribution for details on the file format */

	if (source != NULL) {
		/* ala the gettext sources, remove leading `./'s */
		for (cp = source; cp[0] == '.' && cp[1] == '/'; cp += 2)
			continue;
		printf("#: %s:%d\n", cp, sourceline);
	}

	printf("msgid ");
	pp_string_fp(stdout, str1, len1, '"', TRUE);
	putchar('\n');
	printf("msgid_plural ");
	pp_string_fp(stdout, str2, len2, '"', TRUE);
	putchar('\n');
	printf("msgstr[0] \"\"\nmsgstr[1] \"\"\n\n");
	fflush(stdout);
}

/* count_args --- count the number of printf arguments */

static void
count_args(NODE *tree)
{
	size_t count = 0;
	NODE *save_tree;

	assert(tree->type == Node_K_printf
		|| (tree->type == Node_builtin && tree->proc == do_sprintf));
	save_tree = tree;

	tree = tree->lnode;	/* printf format string */

	for (count = 0; tree != NULL; tree = tree->rnode)
		count++;

	save_tree->printf_count = count;
}
static YYCONST yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 2,
	0, 6,
	267, 6,
	-2, 0,
-1, 5,
	0, 68,
	-2, 0,
-1, 85,
	264, 86,
	267, 86,
	62, 86,
	124, 86,
	294, 86,
	41, 86,
	59, 86,
	-2, 0,
-1, 112,
	41, 94,
	-2, 0,
-1, 113,
	41, 94,
	-2, 0,
-1, 114,
	41, 94,
	-2, 0,
-1, 129,
	266, 0,
	-2, 110,
-1, 131,
	263, 0,
	60, 0,
	62, 0,
	124, 0,
	294, 0,
	-2, 114,
-1, 132,
	263, 0,
	60, 0,
	62, 0,
	124, 0,
	294, 0,
	-2, 115,
-1, 133,
	263, 0,
	60, 0,
	62, 0,
	124, 0,
	294, 0,
	-2, 116,
-1, 159,
	264, 87,
	267, 87,
	62, 87,
	124, 87,
	294, 87,
	41, 87,
	59, 87,
	-2, 0,
-1, 193,
	41, 95,
	-2, 0,
-1, 231,
	41, 77,
	-2, 0,
-1, 264,
	266, 0,
	-2, 127,
-1, 266,
	263, 0,
	-2, 129,
-1, 272,
	41, 78,
	-2, 0,
	};
# define YYNPROD 167
# define YYLAST 1864
static YYCONST yytabelem yyact[]={

    16,   108,    83,   170,   169,   214,    92,    93,   236,   126,
   125,   249,   303,   191,    63,   227,   175,    64,     4,    98,
    24,    63,   207,    91,    65,   110,    68,    35,    36,    34,
    44,   115,   116,    71,   279,    63,    63,   275,   172,   173,
    92,    93,    43,    43,    43,    44,   265,   167,    92,    93,
   298,   274,   297,   273,    24,   179,   130,   213,   178,   178,
   178,   178,   152,    97,   139,    63,   181,   216,    22,   101,
   178,   178,   230,    67,   104,   240,   108,   179,   192,   102,
   105,   182,   106,    44,   103,   180,   162,    66,    63,   278,
   104,   231,   156,   137,    22,   102,    63,   193,   147,   145,
   103,   140,    63,    63,    63,    63,    63,    63,   285,   239,
    11,    43,    43,    43,    43,    94,   114,    43,   113,   112,
    26,    47,   268,   174,   123,    63,    98,    98,    63,    63,
    63,   101,    63,    63,    63,    63,    63,     6,   122,   154,
   136,   111,    90,    38,    45,   109,   143,   101,    40,   137,
   208,   119,   171,   168,    63,    85,   121,   140,   271,    73,
   144,    63,   158,   159,    98,   272,    63,    10,     5,     1,
   140,   137,    49,   205,    12,     0,   218,     0,     0,     0,
   177,     0,     0,    63,     0,     0,    43,     0,   197,   198,
   199,    63,   194,   195,     0,     0,   196,   250,     0,     0,
     0,   201,     0,     0,     0,    63,   189,   215,     0,     0,
     0,     0,    63,     0,    21,     0,     0,   210,   203,   162,
   162,   162,   162,   162,     0,   162,   162,   228,    62,     0,
   140,     0,     0,    63,    63,   100,    63,   217,     4,     0,
     0,   251,    23,     0,    43,    32,    33,    63,     0,   117,
   118,     0,     0,     4,     0,   212,    63,    63,    63,    63,
    63,   244,    63,    63,    63,    63,     0,    63,    63,   162,
   176,   176,   176,   288,    30,    31,    23,     0,   241,   100,
   242,   243,   176,   219,   162,    63,     0,     0,     2,   287,
   248,    63,   160,    63,    37,   269,    63,   253,   254,     0,
   166,     0,    62,    19,   289,   208,     0,     0,   294,   208,
    62,   107,   282,     0,   293,     0,   183,   184,   185,   186,
   187,   188,     0,     0,    99,   307,   301,   305,   270,   309,
     0,     0,   120,     0,     0,   311,     0,     0,     0,    62,
   315,   316,    62,    62,    62,     0,    62,    62,    62,    62,
    62,     0,     0,     0,     0,     0,     0,   286,     0,   141,
   142,     0,     0,     0,   146,   291,    72,     0,    62,    14,
     0,     0,    14,     0,     0,   226,   299,    14,    64,    50,
   100,    24,     0,     0,     0,    65,     0,     0,    35,   164,
    34,     0,     0,   310,     0,   312,     0,   238,     0,    20,
     0,     0,    24,     0,     0,    62,    17,    14,     0,    35,
     0,    34,    14,    25,     0,     0,     0,     0,     0,    62,
     0,     0,     0,     0,     0,    44,    62,     0,   202,     0,
     0,     0,     0,   166,   166,   166,   166,   166,     0,   166,
   166,     0,    20,     0,     0,    24,     0,    62,    62,    17,
    62,     0,    35,     0,    34,     0,    25,     0,     0,     0,
     0,    62,     0,     0,     0,     0,     0,   237,    44,   229,
    62,    62,    62,    62,   226,     0,   226,   226,   226,   226,
     0,   226,   226,   166,     0,     0,   245,     0,     0,    22,
   247,    70,     0,     0,    20,     0,     0,    24,   166,   226,
     0,    17,     0,     0,    35,    62,    34,    62,    25,     0,
   226,   259,   261,   262,   263,   264,     0,   266,   267,     0,
    44,     0,   164,   164,   164,   164,   164,     0,   164,   164,
     0,     0,    22,   276,    70,     0,     0,   277,    64,     0,
   280,    24,   281,     0,     0,    65,     0,     0,    35,     0,
    34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   284,     0,     0,    44,    59,     0,    60,    61,     0,
     0,     0,   164,     0,     0,   296,   295,     0,     0,     0,
   300,     0,   302,     0,    22,     0,    64,   164,   304,    24,
   306,     0,   308,    65,   179,     0,    35,     0,    34,   313,
     0,   314,     0,    23,     0,     0,    32,    33,   224,     0,
     0,     0,     0,    59,     0,    60,    61,     0,     0,     0,
     0,     0,   138,    29,    23,     0,     0,    32,    33,    52,
     0,     0,     0,     0,     0,    30,    31,    84,     0,    82,
    86,    74,    75,    76,    77,    78,    88,    89,    79,    81,
     0,    18,    80,     0,     0,     0,    30,    31,    27,    28,
     0,     0,     0,     0,     0,    69,    29,    23,     0,    64,
    32,    33,    24,     0,     0,     0,    65,    52,     0,    35,
    84,    34,    82,    86,    74,    75,    76,    77,    78,    88,
    89,    79,    81,     0,    18,    80,    59,     0,    60,    30,
    31,    27,    28,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    29,    23,
     0,     0,    32,    33,     0,     0,     0,     0,     0,     0,
     0,     0,    84,     0,    82,    86,    74,    75,    76,    77,
    78,    88,    89,    79,    81,     0,    18,    80,     0,     0,
     0,    30,    31,    27,    28,     0,     0,     0,     0,     0,
    52,     0,    29,    23,     0,     0,    32,    33,    58,     0,
     0,    56,     0,     0,     0,    64,     0,     0,    24,     0,
     0,     0,    65,     0,     0,    35,     0,    34,     0,     0,
     0,     0,    57,    54,    55,    30,    31,    27,    28,    53,
   200,     0,    59,     0,    60,    61,     0,     0,     0,     0,
    29,    23,     0,     0,    32,    33,    58,     0,     0,    56,
     0,    64,     0,     0,    24,     0,     3,     0,    65,     0,
     0,    35,    51,    34,     0,    42,    42,    42,     0,     0,
    57,    54,    55,    30,    31,    27,    28,    53,    59,     0,
    60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
    64,     0,     0,    24,     0,     0,    52,    65,     0,     0,
    35,     0,    34,     0,    64,     0,     0,    24,     0,     0,
     0,    65,     0,     0,    35,     0,    34,    59,     0,    60,
    61,     0,     0,    29,    23,     0,     0,    32,    33,    58,
     0,    59,    56,    60,    42,    42,    42,    42,     0,     0,
    42,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    57,    54,     0,    30,    31,    27,    28,
    53,    20,     0,     0,    24,     0,     0,     0,    17,     0,
     0,    35,     0,    34,     0,    25,    64,     0,     0,    24,
     0,    52,     0,    65,     0,     0,    35,     0,    34,     0,
     0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
     0,   283,     0,     0,     0,     0,   225,     0,     0,    42,
     0,     0,     0,     0,    20,     0,     0,    24,     0,     0,
     0,    17,     0,     0,    35,     0,    34,     0,    25,    29,
    23,     0,     0,    32,    33,    58,     0,     0,    56,     0,
     0,     0,     0,     0,     0,     0,    41,     0,     0,     0,
     0,    22,     0,     0,     0,     0,    46,    48,     0,    57,
    54,    55,    30,    31,    27,    28,    53,    42,     0,     0,
     0,     0,     0,     0,     0,    29,    23,     0,     0,    32,
    33,    58,     0,     0,    56,     0,    64,     0,     0,    24,
     0,     0,     0,    65,     0,     0,    35,     0,    34,     0,
     0,     0,     0,     0,    22,    57,    54,    55,    30,    31,
    27,    28,    53,     0,    29,    23,   225,     0,    32,    33,
    58,     0,     0,    56,   148,   149,   150,   151,    29,    23,
   155,     0,    32,    33,    58,     0,     0,    56,     0,     0,
     0,     0,     0,     0,    57,    54,    55,    30,    31,    27,
    28,    53,     0,     0,     0,     0,     0,     0,    57,     0,
     0,    30,    31,    27,    28,    53,     0,     0,    20,     0,
     0,    24,     0,     0,     0,    17,     0,     0,    35,     0,
    34,     0,    25,     0,    39,    29,    23,     0,     0,    32,
    33,     0,     0,     0,     0,     4,     0,     8,     9,   209,
    29,    23,     0,     0,    32,    33,   224,     0,     0,   222,
     0,     0,    15,    18,     0,     0,     0,     0,    30,    31,
    27,    28,     0,    87,     0,     0,    13,     0,     0,    13,
   223,   220,   221,    30,    31,    27,    28,     7,    29,    23,
     0,    95,    32,    33,     0,     0,     0,     0,     0,    20,
     8,     9,    24,     0,     0,     0,    17,   252,     0,    35,
     0,    34,     0,    25,     0,    15,    18,     0,     0,     0,
     0,    30,    31,    27,    28,   124,     0,     0,   127,   128,
   129,     0,   131,   132,   133,   134,     0,    64,     0,   135,
    24,     0,     0,     0,    65,     0,     0,    35,     0,    34,
     0,     0,     0,     0,     0,   153,     0,     0,     0,     0,
    29,    23,     0,     0,    32,    33,   224,     0,     0,   222,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   190,     0,     0,     0,   190,   190,   190,     0,
   223,   220,   221,    30,    31,    27,    28,    20,     0,     0,
    24,     0,     0,     0,    17,    64,     0,    35,    24,    34,
     0,    25,    65,     0,     0,    35,     0,    34,     0,   204,
     0,     0,    20,    44,     0,    24,     0,     0,   153,    17,
   211,    95,    35,     0,    34,     0,    25,     0,     0,     0,
     0,     0,    29,    23,     0,     0,    32,    33,   232,   233,
   235,     0,     0,     0,     0,     0,     0,     0,     0,    86,
     0,     0,     0,     0,     0,    88,    89,     0,     0,     0,
    18,     0,     0,     0,   246,    30,    31,    27,    28,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   255,   256,
   257,   258,     0,   165,     0,     0,    24,     0,     0,     0,
    65,     0,     0,    35,   190,    34,    20,    25,     0,    24,
     0,     0,     0,    17,     0,     0,    35,     0,    34,   165,
    25,     0,    24,    29,   206,     0,   157,    32,    33,    35,
     0,    34,    64,    25,     0,    24,     0,     0,     0,    65,
    86,     0,    35,     0,    34,     0,    88,    89,     0,     0,
     0,    18,   290,     0,   292,     0,    30,    31,    27,    28,
     0,    29,    23,     0,     0,    32,    33,   224,     0,   165,
   222,     0,    24,     0,     0,    64,    65,     0,    24,    35,
     0,    34,    65,    25,     0,    35,     0,    34,     0,     0,
     0,   223,   220,     0,    30,    31,    27,    28,     0,     0,
     0,     0,    59,     0,    60,     0,    20,     0,     0,    24,
     0,     0,     0,    17,     0,     0,    35,     0,    34,     0,
    25,    29,    23,     0,     0,    32,    33,     0,     0,    29,
    23,     0,     0,    32,    33,   224,     0,     0,   222,     0,
     0,     0,     0,     0,     0,    96,    29,    23,     0,    18,
    32,    33,     0,     0,    30,    31,    27,    28,     0,   223,
     0,     0,    30,    31,    27,    28,    52,     0,     0,     0,
     0,     0,     0,     0,    18,     0,     0,     0,     0,    30,
    31,    27,    28,    64,     0,     0,    24,     0,     0,     0,
    65,     0,     0,    35,     0,    34,     0,    25,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   260,    29,    23,     0,
     0,    32,    33,     0,     0,     0,     0,     0,     0,   234,
    29,    23,     0,     0,    32,    33,     0,     0,     0,     0,
     0,     0,   161,    29,    23,   163,     0,    32,    33,     0,
    30,    31,    27,    28,     0,     0,    29,    23,    18,     0,
    32,    33,     0,    30,    31,    27,    28,     0,     0,     0,
     0,   163,     0,     0,     0,     0,    30,    31,    27,    28,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
    31,    27,    28,    29,    23,     0,     0,    32,    33,     0,
    23,     0,     0,    32,    33,    58,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   163,     0,     0,     0,     0,    30,    31,    27,    28,
    29,    23,    30,    31,    32,    33,    53,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
     0,     0,     0,    30,    31,    27,    28,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    29,    23,     0,
     0,    32,    33,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    30,    31,    27,    28 };
static YYCONST yytabelem yypact[]={

  -249,-10000000,   951,  -239,-10000000,   898,-10000000,-10000000,   -14,   -14,
   -29,-10000000,   -55,   788,   409,-10000000,  -242,  1309,    18,-10000000,
  1570,    37,  -249,   -15,  1419,-10000000,-10000000,    79,    78,    76,
    18,    18,-10000000,-10000000,  1419,  1419,-10000000,-10000000,-10000000,-10000000,
   -55,-10000000,  -239,  -249,-10000000,   -55,-10000000,-10000000,-10000000,-10000000,
   409,  1493,  -275,  -276,  1493,  1493,  1493,  -202,  1493,  1493,
  1493,  1493,    37,  -284,  1419,  1493,   366,    24,-10000000,-10000000,
  -249,  -249,   409,-10000000,    59,  -249,    58,   -14,   -14,   -14,
   -14,  1493,-10000000,   -14,    52,  1406,  -211,   827,-10000000,-10000000,
  -254,-10000000,-10000000,-10000000,    14,   553,-10000000,    21,-10000000,-10000000,
   -25,  1419,  1419,  1419,  1419,  1419,  1419,-10000000,  1309,-10000000,
-10000000,  -246,  1309,  1309,  1309,-10000000,-10000000,   -25,   -25,-10000000,
-10000000,-10000000,   366,    24,   827,    18,    18,   841,   636,  1462,
-10000000,   -16,   -16,   -16,   742,   553,    24,-10000000,-10000000,  -249,
-10000000,-10000000,-10000000,-10000000,   366,  1493,   461,  1186,-10000000,-10000000,
-10000000,-10000000,   -14,   827,  1493,-10000000,  1493,  1309,   -57,    27,
  1023,-10000000,  -250,    18,-10000000,  1570,    37,   -19,    51,-10000000,
-10000000,-10000000,-10000000,-10000000,  1493,  1393,  1493,  -279,  -249,-10000000,
-10000000,-10000000,  1419,   -25,   -25,   -25,   -25,    53,    53,    16,
   827,    28,    36,    26,    36,    36,    24,  -249,-10000000,-10000000,
  1493,  -249,-10000000,-10000000,   553,  -264,   -90,    24,-10000000,-10000000,
   -14,   553,    14,-10000000,  1493,  1493,  1493,  1493,  1380,  1456,
  1456,  1456,  1456,  -212,  1456,  1456,    37,-10000000,    21,-10000000,
  1309,  -205,   827,   827,-10000000,   827,  -221,-10000000,    37,-10000000,
-10000000,-10000000,-10000000,-10000000,  -249,-10000000,   827,-10000000,  -249,    49,
  -224,  -249,-10000000,  -249,   -57,   827,   827,   827,   827,  1023,
-10000000,  1023,  1292,  1224,   345,-10000000,   -16,   913,  1456,-10000000,
    15,    36,    17,-10000000,-10000000,-10000000,-10000000,   461,  1493,    36,
  1284,   461,-10000000,  1456,  1023,-10000000,  -249,  -206,-10000000,-10000000,
   553,  -249,   505,  -249,  -260,  1023,-10000000,-10000000,-10000000,  -249,
   461,  -249,  1105,  -249,-10000000,-10000000,  1105,    36,   461,    36,
  -249,-10000000,  -249,   461,   461,-10000000,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,   174,   172,   292,  1193,   169,   168,   137,   214,   120,
   167,   110,     0,   165,   163,   162,    97,    78,    87,    26,
   159,   158,     2,    22,    62,    63,   303,    66,    57,   155,
   153,   152,   288,   148,   144,  1016,   142,    85,   366,    73,
    64,   141,   826,    33,   139,    16,   123,   122 };
static YYCONST yytabelem yyr1[]={

     0,     5,     6,     6,     6,     6,     6,    33,     7,    34,
     7,     7,     7,     7,     7,     7,     7,    30,    30,    30,
    31,    31,    36,     1,     2,     2,    10,    10,    41,    26,
    11,    11,    18,    18,    18,    18,    35,    35,    19,    19,
    19,    19,    19,    19,    19,    19,    19,    19,    19,    19,
    19,    19,    44,    19,    19,    22,    22,    22,    22,    22,
    23,    23,    29,    29,    20,    20,    42,    42,    32,    32,
    27,    27,    28,    28,    28,    28,    28,    21,    21,    13,
    13,    13,    13,    13,    24,    24,    15,    15,    14,    14,
    14,    14,    14,    14,    17,    17,    16,    16,    16,    16,
    16,    16,    46,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
    47,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     8,     8,     8,     8,     8,     8,     8,
     8,     8,     9,     9,     9,     9,     9,     9,     9,     9,
     9,     9,     9,     9,     9,    25,    25,    12,    12,    12,
    38,    39,    37,    40,    40,    43,    45 };
static YYCONST yytabelem yyr2[]={

     0,     7,     3,     5,     3,     5,     1,     1,     7,     1,
     7,     5,     5,     5,     3,     5,     5,     3,     3,     3,
     2,     2,     1,    15,    11,     9,     3,     7,     1,     9,
    11,     9,     3,     5,     3,     5,     2,     4,     5,     5,
     7,     3,    13,    17,    17,    25,    23,     5,     5,     5,
     5,     7,     1,     9,     4,    11,     7,    11,     5,     3,
     1,     3,     3,     3,    13,    19,     3,     4,     0,     2,
     1,     5,     1,     5,     5,     5,     5,     1,     3,     3,
     7,     3,     5,     7,     1,     3,     1,     3,     3,     7,
     3,     5,     7,     7,     1,     3,     3,     7,     3,     5,
     7,     7,     1,     9,    11,     9,     9,     7,     7,     7,
     7,     3,     5,     7,     7,     7,     7,    11,     3,     5,
     1,     9,     7,     7,     7,     3,     5,     7,     7,     7,
    11,     3,     5,     2,     7,     7,     7,     7,     7,     7,
     5,     5,     5,     7,     9,     9,     3,     9,     2,     5,
     5,     3,     3,     5,     5,     1,     3,     3,     9,     5,
     4,     5,     3,     0,     2,     3,     5 };
static YYCONST yytabelem yychk[]={

-10000000,    -5,   -32,   -42,   267,    -6,    -7,   256,   269,   270,
   -10,   -11,    -1,    -4,   -38,   284,   -12,    40,   285,   -26,
    33,    -8,   123,   258,    36,    47,    -9,   292,   293,   257,
   290,   291,   261,   262,    45,    43,   267,   -32,    -7,   256,
   -33,   -35,   -42,   -43,    59,   -34,   -35,   -11,   -35,    -2,
   -38,    44,   124,   294,   288,   289,   266,   287,   263,    60,
    62,    63,    -8,   -12,    33,    40,   -18,   -39,   -19,   256,
   125,   -43,   -38,   -20,   275,   276,   277,   278,   279,   282,
   286,   283,   273,   -22,   271,   -29,   274,    -4,   280,   281,
   -36,   265,   290,   291,   -16,    -4,   256,   -25,   -12,   -26,
    -8,    94,    42,    47,    37,    43,    45,   -32,    91,    -9,
   -12,   -41,    40,    40,    40,   -12,   -12,    -8,    -8,   -11,
   -32,   -11,   -18,   -39,    -4,   285,   285,    -4,    -4,    -4,
   258,    -4,    -4,    -4,    -4,    -4,   -39,   -19,   256,   -40,
   -43,   -32,   -32,   -39,   -18,    40,   -32,    40,   -35,   -35,
   -35,   -35,   -24,    -4,   -44,   -35,    40,    40,   -15,   -14,
    -3,   256,   -12,   285,   -26,    33,    -8,   258,   -30,   258,
   257,   -31,   292,   293,   -46,   -45,   256,   -37,    44,    41,
   -37,   -27,    60,    -8,    -8,    -8,    -8,    -8,    -8,   -16,
    -4,   259,   -17,   -16,   -17,   -17,   -39,   -40,   -25,   -25,
    58,   -40,   -32,   -39,    -4,   -19,   258,   -23,   -22,   -35,
   -24,    -4,   -16,   -28,    62,   264,   124,   294,   -45,   256,
   288,   289,   266,   287,   263,    63,    -8,   265,   -25,   -26,
    91,    40,    -4,    -4,   256,    -4,   287,   -32,    -8,    93,
    47,   -37,   -37,   -37,   -40,   -32,    -4,   -32,   -37,   275,
   287,   -43,   -35,   -37,   -37,    -4,    -4,    -4,    -4,    -3,
   256,    -3,    -3,    -3,    -3,   258,    -3,    -3,   -47,   -27,
   -16,   -21,   -13,   258,   256,   258,   -32,   -32,    40,   258,
   -32,   -32,   -28,    58,    -3,    93,   -37,   -45,   256,   -19,
    -4,   -37,    -4,   -43,   -19,    -3,   -32,   258,   256,   -37,
   -32,   -43,   -32,   272,   -32,   -19,   -32,   -23,   -32,   -23,
   -37,   -19,   -37,   -32,   -32,   -19,   -19 };
static YYCONST yytabelem yydef[]={

    68,    -2,    -2,    69,    66,    -2,     2,     4,     7,     9,
     0,    14,     0,    26,     0,    22,   148,     0,   155,   111,
     0,   118,    68,   157,     0,    28,   133,     0,   146,     0,
     0,     0,   151,   152,     0,     0,    67,     1,     3,     5,
     0,    11,    36,    68,   165,     0,    12,    13,    15,    16,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   119,   148,     0,     0,     0,   163,    32,    34,
    68,    68,     0,    41,     0,    68,     0,     0,     0,     0,
     0,    84,    52,     0,     0,    -2,     0,    59,    62,    63,
     0,   102,   140,   141,     0,    96,    98,    70,   156,   112,
   142,     0,     0,     0,     0,     0,     0,   160,     0,   159,
   148,     0,    -2,    -2,    -2,   149,   150,   153,   154,     8,
    37,    10,     0,   163,    27,   155,   155,   108,   109,    -2,
   113,    -2,    -2,    -2,     0,     0,   163,    33,    35,    68,
   164,   161,    38,    39,     0,     0,     0,    60,    47,    48,
    49,    50,     0,    85,    84,    54,     0,     0,    72,    -2,
    88,    90,   148,   155,   125,     0,   131,    58,     0,    17,
    18,    19,    20,    21,     0,     0,    99,     0,    68,   162,
   143,   107,     0,   134,   135,   136,   137,   138,   139,     0,
    96,     0,     0,    -2,     0,     0,   163,    68,   105,   106,
     0,    68,    31,    40,     0,     0,   157,     0,    61,    51,
     0,     0,     0,    56,     0,     0,     0,     0,     0,    91,
     0,     0,     0,     0,     0,     0,   132,   120,    70,   126,
     0,    -2,   103,    97,   101,   100,     0,   166,    71,   158,
    29,   144,   145,   147,    68,    25,   117,    30,    68,     0,
     0,    68,    53,    68,    72,    73,    74,    75,    76,    89,
    93,    92,   122,   123,    -2,   128,    -2,     0,     0,   124,
     0,     0,    -2,    79,    81,   104,    24,     0,     0,     0,
     0,     0,    55,     0,   121,    57,    68,     0,    82,    42,
     0,    68,     0,    68,    64,   130,    23,    80,    83,    68,
     0,    68,    60,    68,    43,    44,    60,     0,     0,     0,
    68,    65,    68,     0,     0,    46,    45 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"FUNC_CALL",	257,
	"NAME",	258,
	"REGEXP",	259,
	"ERROR",	260,
	"YNUMBER",	261,
	"YSTRING",	262,
	"RELOP",	263,
	"APPEND_OP",	264,
	"ASSIGNOP",	265,
	"MATCHOP",	266,
	"NEWLINE",	267,
	"CONCAT_OP",	268,
	"LEX_BEGIN",	269,
	"LEX_END",	270,
	"LEX_IF",	271,
	"LEX_ELSE",	272,
	"LEX_RETURN",	273,
	"LEX_DELETE",	274,
	"LEX_WHILE",	275,
	"LEX_DO",	276,
	"LEX_FOR",	277,
	"LEX_BREAK",	278,
	"LEX_CONTINUE",	279,
	"LEX_PRINT",	280,
	"LEX_PRINTF",	281,
	"LEX_NEXT",	282,
	"LEX_EXIT",	283,
	"LEX_FUNCTION",	284,
	"LEX_GETLINE",	285,
	"LEX_NEXTFILE",	286,
	"LEX_IN",	287,
	"LEX_AND",	288,
	"LEX_OR",	289,
	"INCREMENT",	290,
	"DECREMENT",	291,
	"LEX_BUILTIN",	292,
	"LEX_LENGTH",	293,
	"?",	63,
	":",	58,
	",",	44,
	"<",	60,
	">",	62,
	"|",	124,
	"TWOWAYIO",	294,
	"+",	43,
	"-",	45,
	"*",	42,
	"/",	47,
	"%",	37,
	"!",	33,
	"UNARY",	295,
	"^",	94,
	"$",	36,
	"(",	40,
	")",	41,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"start : opt_nls program opt_nls",
	"program : rule",
	"program : program rule",
	"program : error",
	"program : program error",
	"program : /* empty */",
	"rule : LEX_BEGIN",
	"rule : LEX_BEGIN action",
	"rule : LEX_END",
	"rule : LEX_END action",
	"rule : LEX_BEGIN statement_term",
	"rule : LEX_END statement_term",
	"rule : pattern action",
	"rule : action",
	"rule : pattern statement_term",
	"rule : function_prologue function_body",
	"func_name : NAME",
	"func_name : FUNC_CALL",
	"func_name : lex_builtin",
	"lex_builtin : LEX_BUILTIN",
	"lex_builtin : LEX_LENGTH",
	"function_prologue : LEX_FUNCTION",
	"function_prologue : LEX_FUNCTION func_name '(' opt_param_list r_paren opt_nls",
	"function_body : l_brace statements r_brace opt_semi opt_nls",
	"function_body : l_brace r_brace opt_semi opt_nls",
	"pattern : exp",
	"pattern : exp ',' exp",
	"regexp : '/'",
	"regexp : '/' REGEXP '/'",
	"action : l_brace statements r_brace opt_semi opt_nls",
	"action : l_brace r_brace opt_semi opt_nls",
	"statements : statement",
	"statements : statements statement",
	"statements : error",
	"statements : statements error",
	"statement_term : nls",
	"statement_term : semi opt_nls",
	"statement : semi opt_nls",
	"statement : l_brace r_brace",
	"statement : l_brace statements r_brace",
	"statement : if_statement",
	"statement : LEX_WHILE '(' exp r_paren opt_nls statement",
	"statement : LEX_DO opt_nls statement LEX_WHILE '(' exp r_paren opt_nls",
	"statement : LEX_FOR '(' NAME LEX_IN NAME r_paren opt_nls statement",
	"statement : LEX_FOR '(' opt_simple_stmt semi opt_nls exp semi opt_nls opt_simple_stmt r_paren opt_nls statement",
	"statement : LEX_FOR '(' opt_simple_stmt semi opt_nls semi opt_nls opt_simple_stmt r_paren opt_nls statement",
	"statement : LEX_BREAK statement_term",
	"statement : LEX_CONTINUE statement_term",
	"statement : LEX_NEXT statement_term",
	"statement : LEX_NEXTFILE statement_term",
	"statement : LEX_EXIT opt_exp statement_term",
	"statement : LEX_RETURN",
	"statement : LEX_RETURN opt_exp statement_term",
	"statement : simple_stmt statement_term",
	"simple_stmt : print '(' expression_list r_paren output_redir",
	"simple_stmt : print opt_rexpression_list output_redir",
	"simple_stmt : LEX_DELETE NAME '[' expression_list ']'",
	"simple_stmt : LEX_DELETE NAME",
	"simple_stmt : exp",
	"opt_simple_stmt : /* empty */",
	"opt_simple_stmt : simple_stmt",
	"print : LEX_PRINT",
	"print : LEX_PRINTF",
	"if_statement : LEX_IF '(' exp r_paren opt_nls statement",
	"if_statement : LEX_IF '(' exp r_paren opt_nls statement LEX_ELSE opt_nls statement",
	"nls : NEWLINE",
	"nls : nls NEWLINE",
	"opt_nls : /* empty */",
	"opt_nls : nls",
	"input_redir : /* empty */",
	"input_redir : '<' simp_exp",
	"output_redir : /* empty */",
	"output_redir : '>' exp",
	"output_redir : APPEND_OP exp",
	"output_redir : '|' exp",
	"output_redir : TWOWAYIO exp",
	"opt_param_list : /* empty */",
	"opt_param_list : param_list",
	"param_list : NAME",
	"param_list : param_list comma NAME",
	"param_list : error",
	"param_list : param_list error",
	"param_list : param_list comma error",
	"opt_exp : /* empty */",
	"opt_exp : exp",
	"opt_rexpression_list : /* empty */",
	"opt_rexpression_list : rexpression_list",
	"rexpression_list : rexp",
	"rexpression_list : rexpression_list comma rexp",
	"rexpression_list : error",
	"rexpression_list : rexpression_list error",
	"rexpression_list : rexpression_list error rexp",
	"rexpression_list : rexpression_list comma error",
	"opt_expression_list : /* empty */",
	"opt_expression_list : expression_list",
	"expression_list : exp",
	"expression_list : expression_list comma exp",
	"expression_list : error",
	"expression_list : expression_list error",
	"expression_list : expression_list error exp",
	"expression_list : expression_list comma error",
	"exp : variable ASSIGNOP",
	"exp : variable ASSIGNOP exp",
	"exp : '(' expression_list r_paren LEX_IN NAME",
	"exp : exp '|' LEX_GETLINE opt_variable",
	"exp : exp TWOWAYIO LEX_GETLINE opt_variable",
	"exp : LEX_GETLINE opt_variable input_redir",
	"exp : exp LEX_AND exp",
	"exp : exp LEX_OR exp",
	"exp : exp MATCHOP exp",
	"exp : regexp",
	"exp : '!' regexp",
	"exp : exp LEX_IN NAME",
	"exp : exp RELOP exp",
	"exp : exp '<' exp",
	"exp : exp '>' exp",
	"exp : exp '?' exp ':' exp",
	"exp : simp_exp",
	"exp : exp simp_exp",
	"rexp : variable ASSIGNOP",
	"rexp : variable ASSIGNOP rexp",
	"rexp : rexp LEX_AND rexp",
	"rexp : rexp LEX_OR rexp",
	"rexp : LEX_GETLINE opt_variable input_redir",
	"rexp : regexp",
	"rexp : '!' regexp",
	"rexp : rexp MATCHOP rexp",
	"rexp : rexp LEX_IN NAME",
	"rexp : rexp RELOP rexp",
	"rexp : rexp '?' rexp ':' rexp",
	"rexp : simp_exp",
	"rexp : rexp simp_exp",
	"simp_exp : non_post_simp_exp",
	"simp_exp : simp_exp '^' simp_exp",
	"simp_exp : simp_exp '*' simp_exp",
	"simp_exp : simp_exp '/' simp_exp",
	"simp_exp : simp_exp '%' simp_exp",
	"simp_exp : simp_exp '+' simp_exp",
	"simp_exp : simp_exp '-' simp_exp",
	"simp_exp : variable INCREMENT",
	"simp_exp : variable DECREMENT",
	"non_post_simp_exp : '!' simp_exp",
	"non_post_simp_exp : '(' exp r_paren",
	"non_post_simp_exp : LEX_BUILTIN '(' opt_expression_list r_paren",
	"non_post_simp_exp : LEX_LENGTH '(' opt_expression_list r_paren",
	"non_post_simp_exp : LEX_LENGTH",
	"non_post_simp_exp : FUNC_CALL '(' opt_expression_list r_paren",
	"non_post_simp_exp : variable",
	"non_post_simp_exp : INCREMENT variable",
	"non_post_simp_exp : DECREMENT variable",
	"non_post_simp_exp : YNUMBER",
	"non_post_simp_exp : YSTRING",
	"non_post_simp_exp : '-' simp_exp",
	"non_post_simp_exp : '+' simp_exp",
	"opt_variable : /* empty */",
	"opt_variable : variable",
	"variable : NAME",
	"variable : NAME '[' expression_list ']'",
	"variable : '$' non_post_simp_exp",
	"l_brace : '{' opt_nls",
	"r_brace : '}' opt_nls",
	"r_paren : ')'",
	"opt_semi : /* empty */",
	"opt_semi : semi",
	"semi : ';'",
	"comma : ',' opt_nls",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.16	99/01/20 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			long yyps_index = (yy_ps - yys);
			long yypv_index = (yy_pv - yyv);
			long yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register YYCONST int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 1:
# line 160 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			expression_value = yypvt[-1].nodeval;
			check_funcs();
		} break;
case 2:
# line 168 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ 
			if (yypvt[-0].nodeval != NULL)
				yyval.nodeval = yypvt[-0].nodeval;
			else
				yyval.nodeval = NULL;
			yyerrok;
		} break;
case 3:
# line 177 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			if (yypvt[-0].nodeval == NULL)
				yyval.nodeval = yypvt[-1].nodeval;
			else if (yypvt[-1].nodeval == NULL)
				yyval.nodeval = yypvt[-0].nodeval;
			else {
				if (yypvt[-1].nodeval->type != Node_rule_list)
					yypvt[-1].nodeval = node(yypvt[-1].nodeval, Node_rule_list,
						(NODE*) NULL);
				yyval.nodeval = append_right(yypvt[-1].nodeval,
				   node(yypvt[-0].nodeval, Node_rule_list, (NODE *) NULL));
			}
			yyerrok;
		} break;
case 4:
# line 191 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 5:
# line 192 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 6:
# line 193 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 7:
# line 197 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ io_allowed = FALSE; } break;
case 8:
# line 199 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		if (begin_block != NULL) {
			if (begin_block->type != Node_rule_list)
				begin_block = node(begin_block, Node_rule_list,
					(NODE *) NULL);
			(void) append_right(begin_block, node(
			    node((NODE *) NULL, Node_rule_node, yypvt[-0].nodeval),
			    Node_rule_list, (NODE *) NULL) );
		} else
			begin_block = node((NODE *) NULL, Node_rule_node, yypvt[-0].nodeval);
		yyval.nodeval = NULL;
		io_allowed = TRUE;
		yyerrok;
	  } break;
case 9:
# line 213 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ io_allowed = FALSE; parsing_end_rule = TRUE; } break;
case 10:
# line 215 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		if (end_block != NULL) {
			if (end_block->type != Node_rule_list)
				end_block = node(end_block, Node_rule_list,
					(NODE *) NULL);
			(void) append_right (end_block, node(
			    node((NODE *) NULL, Node_rule_node, yypvt[-0].nodeval),
			    Node_rule_list, (NODE *) NULL));
		} else
			end_block = node((NODE *) NULL, Node_rule_node, yypvt[-0].nodeval);
		yyval.nodeval = NULL;
		io_allowed = TRUE;
		parsing_end_rule = FALSE;
		yyerrok;
	  } break;
case 11:
# line 231 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		warning(_("BEGIN blocks must have an action part"));
		errcount++;
		yyerrok;
	  } break;
case 12:
# line 237 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		warning(_("END blocks must have an action part"));
		errcount++;
		yyerrok;
	  } break;
case 13:
# line 243 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-1].nodeval, Node_rule_node, yypvt[-0].nodeval); yyerrok; } break;
case 14:
# line 245 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node((NODE *) NULL, Node_rule_node, yypvt[-0].nodeval); yyerrok; } break;
case 15:
# line 247 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  yyval.nodeval = node(yypvt[-1].nodeval,
			     Node_rule_node,
			     node(node(node(make_number(0.0),
					    Node_field_spec,
					    (NODE *) NULL),
					Node_expression_list,
					(NODE *) NULL),
				  Node_K_print,
				  (NODE *) NULL));
		  yyerrok;
		} break;
case 16:
# line 260 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			func_install(yypvt[-1].nodeval, yypvt[-0].nodeval);
			yyval.nodeval = NULL;
			yyerrok;
		} break;
case 17:
# line 269 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.sval = yypvt[-0].sval; } break;
case 18:
# line 271 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.sval = yypvt[-0].sval; } break;
case 19:
# line 273 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyerror(_("`%s' is a built-in function, it cannot be redefined"),
			tokstart);
		errcount++;
		/* yyerrok; */
	  } break;
case 22:
# line 288 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			param_counter = 0;
		} break;
case 23:
# line 292 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			NODE *t;

			t = make_param(yypvt[-4].sval);
			t->flags |= FUNC;
			yyval.nodeval = append_right(t, yypvt[-2].nodeval);
			can_return = TRUE;
			/* check for duplicate parameter names */
			if (dup_parms(yyval.nodeval))
				errcount++;
		} break;
case 24:
# line 307 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyval.nodeval = yypvt[-3].nodeval;
		can_return = FALSE;
	  } break;
case 25:
# line 312 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyval.nodeval = node((NODE *) NULL, Node_K_return, (NODE *) NULL);
		can_return = FALSE;
	  } break;
case 26:
# line 321 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 27:
# line 323 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = mkrangenode(node(yypvt[-2].nodeval, Node_cond_pair, yypvt[-0].nodeval)); } break;
case 28:
# line 332 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ ++want_regexp; } break;
case 29:
# line 334 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  NODE *n;
		  size_t len;

		  getnode(n);
		  n->type = Node_regex;
		  len = strlen(yypvt[-1].sval);
		  n->re_exp = make_string(yypvt[-1].sval, len);
		  n->re_reg = make_regexp(yypvt[-1].sval, len, FALSE, TRUE);
		  n->re_text = NULL;
		  n->re_flags = CONST;
		  n->re_cnt = 1;
		  yyval.nodeval = n;
		} break;
case 30:
# line 352 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-3].nodeval; } break;
case 31:
# line 354 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 32:
# line 359 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			yyval.nodeval = yypvt[-0].nodeval;
			if (do_lint && isnoeffect(yyval.nodeval->type))
				lintwarn(_("statement may have no effect"));
		} break;
case 33:
# line 365 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			if (yypvt[-1].nodeval == NULL || yypvt[-1].nodeval->type != Node_statement_list)
				yypvt[-1].nodeval = node(yypvt[-1].nodeval, Node_statement_list, (NODE *) NULL);
	    		yyval.nodeval = append_right(yypvt[-1].nodeval,
				node(yypvt[-0].nodeval, Node_statement_list, (NODE *)   NULL));
	    		yyerrok;
		} break;
case 34:
# line 373 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 35:
# line 375 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 38:
# line 385 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 39:
# line 387 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 40:
# line 389 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-1].nodeval; } break;
case 41:
# line 391 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 42:
# line 393 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-3].nodeval, Node_K_while, yypvt[-0].nodeval); } break;
case 43:
# line 395 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_K_do, yypvt[-5].nodeval); } break;
case 44:
# line 397 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		/*
		 * Efficiency hack.  Recognize the special case of
		 *
		 * 	for (iggy in foo)
		 * 		delete foo[iggy]
		 *
		 * and treat it as if it were
		 *
		 * 	delete foo
		 *
		 * Check that the body is a `delete a[i]' statement,
		 * and that both the loop var and array names match.
		 */
		if (yypvt[-0].nodeval != NULL && yypvt[-0].nodeval->type == Node_K_delete) {
			NODE *arr, *sub;

			assert(yypvt[-0].nodeval->rnode->type == Node_expression_list);
			arr = yypvt[-0].nodeval->lnode;	/* array var */
			sub = yypvt[-0].nodeval->rnode->lnode;	/* index var */

			if (   (arr->type == Node_var
				|| arr->type == Node_var_array
				|| arr->type == Node_param_list)
			    && (sub->type == Node_var || sub->type == Node_param_list)
			    && strcmp(yypvt[-5].sval, sub->vname) == 0
			    && strcmp(yypvt[-3].sval, arr->vname) == 0) {
				yypvt[-0].nodeval->type = Node_K_delete_loop;
				yyval.nodeval = yypvt[-0].nodeval;
			}
			else
				goto regular_loop;
		} else {
	regular_loop:
			yyval.nodeval = node(yypvt[-0].nodeval, Node_K_arrayfor,
				make_for_loop(variable(yypvt[-5].sval, CAN_FREE, Node_var),
				(NODE *) NULL, variable(yypvt[-3].sval, CAN_FREE, Node_var_array)));
		}
	  } break;
case 45:
# line 437 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyval.nodeval = node(yypvt[-0].nodeval, Node_K_for, (NODE *) make_for_loop(yypvt[-9].nodeval, yypvt[-6].nodeval, yypvt[-3].nodeval));
	  } break;
case 46:
# line 441 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyval.nodeval = node(yypvt[-0].nodeval, Node_K_for,
			(NODE *) make_for_loop(yypvt[-8].nodeval, (NODE *) NULL, yypvt[-3].nodeval));
	  } break;
case 47:
# line 447 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node((NODE *) NULL, Node_K_break, (NODE *) NULL); } break;
case 48:
# line 450 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node((NODE *) NULL, Node_K_continue, (NODE *) NULL); } break;
case 49:
# line 452 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ NODETYPE type;

		  if (! io_allowed)
			yyerror(_("`next' used in BEGIN or END action"));
		  type = Node_K_next;
		  yyval.nodeval = node((NODE *) NULL, type, (NODE *) NULL);
		} break;
case 50:
# line 460 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (do_lint)
			lintwarn(_("`nextfile' is a gawk extension"));
		  if (do_traditional) {
			/*
			 * can't use yyerror, since may have overshot
			 * the source line
			 */
			errcount++;
			error(_("`nextfile' is a gawk extension"));
		  }
		  if (! io_allowed) {
			/* same thing */
			errcount++;
			error(_("`nextfile' used in BEGIN or END action"));
		  }
		  yyval.nodeval = node((NODE *) NULL, Node_K_nextfile, (NODE *) NULL);
		} break;
case 51:
# line 479 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-1].nodeval, Node_K_exit, (NODE *) NULL); } break;
case 52:
# line 481 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (! can_return)
			yyerror(_("`return' used outside function context"));
		} break;
case 53:
# line 486 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-1].nodeval, Node_K_return, (NODE *) NULL); } break;
case 55:
# line 500 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			yyval.nodeval = node(yypvt[-2].nodeval, yypvt[-4].nodetypeval, yypvt[-0].nodeval);
			if (yyval.nodeval->type == Node_K_printf)
				count_args(yyval.nodeval);
		} break;
case 56:
# line 506 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			if (yypvt[-2].nodetypeval == Node_K_print && yypvt[-1].nodeval == NULL) {
				static int warned = FALSE;

				yypvt[-1].nodeval = node(node(make_number(0.0),
					       Node_field_spec,
					       (NODE *) NULL),
					  Node_expression_list,
					  (NODE *) NULL);

				if (do_lint && ! io_allowed && ! warned) {
					warned = TRUE;
					lintwarn(
	_("plain `print' in BEGIN or END rule should probably be `print \"\"'"));
				}
			}

			yyval.nodeval = node(yypvt[-1].nodeval, yypvt[-2].nodetypeval, yypvt[-0].nodeval);
			if (yyval.nodeval->type == Node_K_printf)
				count_args(yyval.nodeval);
		} break;
case 57:
# line 528 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(variable(yypvt[-3].sval, CAN_FREE, Node_var_array), Node_K_delete, yypvt[-1].nodeval); } break;
case 58:
# line 530 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (do_lint)
			lintwarn(_("`delete array' is a gawk extension"));
		  if (do_traditional) {
			/*
			 * can't use yyerror, since may have overshot
			 * the source line
			 */
			errcount++;
			error(_("`delete array' is a gawk extension"));
		  }
		  yyval.nodeval = node(variable(yypvt[-0].sval, CAN_FREE, Node_var_array), Node_K_delete, (NODE *) NULL);
		} break;
case 59:
# line 544 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 60:
# line 549 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 61:
# line 551 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 62:
# line 556 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodetypeval = yypvt[-0].nodetypeval; } break;
case 63:
# line 558 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodetypeval = yypvt[-0].nodetypeval; } break;
case 64:
# line 563 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyval.nodeval = node(yypvt[-3].nodeval, Node_K_if, 
			node(yypvt[-0].nodeval, Node_if_branches, (NODE *) NULL));
	  } break;
case 65:
# line 569 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-6].nodeval, Node_K_if,
				node(yypvt[-3].nodeval, Node_if_branches, yypvt[-0].nodeval)); } break;
case 66:
# line 575 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ want_assign = FALSE; } break;
case 70:
# line 586 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 71:
# line 588 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_redirect_input, (NODE *) NULL); } break;
case 72:
# line 593 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 73:
# line 595 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_redirect_output, (NODE *) NULL); } break;
case 74:
# line 597 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_redirect_append, (NODE *) NULL); } break;
case 75:
# line 599 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_redirect_pipe, (NODE *) NULL); } break;
case 76:
# line 601 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (yypvt[-0].nodeval->type == Node_K_getline
		      && yypvt[-0].nodeval->rnode->type == Node_redirect_twoway)
			yyerror(_("multistage two-way pipelines don't work"));
		  yyval.nodeval = node(yypvt[-0].nodeval, Node_redirect_twoway, (NODE *) NULL);
		} break;
case 77:
# line 611 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 78:
# line 613 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 79:
# line 618 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = make_param(yypvt[-0].sval); } break;
case 80:
# line 620 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = append_right(yypvt[-2].nodeval, make_param(yypvt[-0].sval)); yyerrok; } break;
case 81:
# line 622 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 82:
# line 624 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 83:
# line 626 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 84:
# line 632 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 85:
# line 634 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 86:
# line 639 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 87:
# line 641 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 88:
# line 646 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_expression_list, (NODE *) NULL); } break;
case 89:
# line 648 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyval.nodeval = append_right(yypvt[-2].nodeval,
			node(yypvt[-0].nodeval, Node_expression_list, (NODE *) NULL));
		yyerrok;
	  } break;
case 90:
# line 654 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 91:
# line 656 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 92:
# line 658 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 93:
# line 660 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 94:
# line 665 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 95:
# line 667 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 96:
# line 672 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_expression_list, (NODE *) NULL); } break;
case 97:
# line 674 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
			yyval.nodeval = append_right(yypvt[-2].nodeval,
				node(yypvt[-0].nodeval, Node_expression_list, (NODE *) NULL));
			yyerrok;
		} break;
case 98:
# line 680 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 99:
# line 682 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 100:
# line 684 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 101:
# line 686 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 102:
# line 691 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ want_assign = FALSE; } break;
case 103:
# line 693 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (do_lint && yypvt[-0].nodeval->type == Node_regex)
			lintwarn(_("regular expression on right of assignment"));
		  yyval.nodeval = node(yypvt[-3].nodeval, yypvt[-2].nodetypeval, yypvt[-0].nodeval);
		} break;
case 104:
# line 699 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(variable(yypvt[-0].sval, CAN_FREE, Node_var_array), Node_in_array, yypvt[-3].nodeval); } break;
case 105:
# line 701 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  yyval.nodeval = node(yypvt[-0].nodeval, Node_K_getline,
			 node(yypvt[-3].nodeval, Node_redirect_pipein, (NODE *) NULL));
		} break;
case 106:
# line 706 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  yyval.nodeval = node(yypvt[-0].nodeval, Node_K_getline,
			 node(yypvt[-3].nodeval, Node_redirect_twoway, (NODE *) NULL));
		} break;
case 107:
# line 711 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (do_lint && ! io_allowed && parsing_end_rule && yypvt[-0].nodeval == NULL)
			lintwarn(_("non-redirected `getline' undefined inside END action"));
		  yyval.nodeval = node(yypvt[-1].nodeval, Node_K_getline, yypvt[-0].nodeval);
		} break;
case 108:
# line 717 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_and, yypvt[-0].nodeval); } break;
case 109:
# line 719 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_or, yypvt[-0].nodeval); } break;
case 110:
# line 721 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (yypvt[-2].nodeval->type == Node_regex)
			warning(_("regular expression on left of `~' or `!~' operator"));
		  yyval.nodeval = node(yypvt[-2].nodeval, yypvt[-1].nodetypeval, mk_rexp(yypvt[-0].nodeval));
		} break;
case 111:
# line 727 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  yyval.nodeval = yypvt[-0].nodeval;
		  if (do_lint && tokstart[0] == '*') {
			/* possible C comment */
			int n = strlen(tokstart) - 1;
			if (tokstart[n] == '*')
				lintwarn(_("regexp constant `/%s/' looks like a C comment, but is not"), tokstart);
		  }
		} break;
case 112:
# line 737 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  yyval.nodeval = node(node(make_number(0.0),
				 Node_field_spec,
				 (NODE *) NULL),
		            Node_nomatch,
			    yypvt[-0].nodeval);
		} break;
case 113:
# line 745 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(variable(yypvt[-0].sval, CAN_FREE, Node_var_array), Node_in_array, yypvt[-2].nodeval); } break;
case 114:
# line 747 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (do_lint && yypvt[-0].nodeval->type == Node_regex)
			lintwarn(_("regular expression on right of comparison"));
		  yyval.nodeval = node(yypvt[-2].nodeval, yypvt[-1].nodetypeval, yypvt[-0].nodeval);
		} break;
case 115:
# line 753 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_less, yypvt[-0].nodeval); } break;
case 116:
# line 755 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_greater, yypvt[-0].nodeval); } break;
case 117:
# line 757 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-4].nodeval, Node_cond_exp, node(yypvt[-2].nodeval, Node_if_branches, yypvt[-0].nodeval));} break;
case 118:
# line 759 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 119:
# line 761 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-1].nodeval, Node_concat, yypvt[-0].nodeval); } break;
case 120:
# line 766 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ want_assign = FALSE; } break;
case 121:
# line 768 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-3].nodeval, yypvt[-2].nodetypeval, yypvt[-0].nodeval); } break;
case 122:
# line 770 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_and, yypvt[-0].nodeval); } break;
case 123:
# line 772 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_or, yypvt[-0].nodeval); } break;
case 124:
# line 774 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (do_lint && ! io_allowed && yypvt[-0].nodeval == NULL)
			lintwarn(_("non-redirected `getline' undefined inside BEGIN or END action"));
		  yyval.nodeval = node(yypvt[-1].nodeval, Node_K_getline, yypvt[-0].nodeval);
		} break;
case 125:
# line 780 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 126:
# line 782 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node((NODE *) NULL, Node_nomatch, yypvt[-0].nodeval); } break;
case 127:
# line 784 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, yypvt[-1].nodetypeval, mk_rexp(yypvt[-0].nodeval)); } break;
case 128:
# line 786 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(variable(yypvt[-0].sval, CAN_FREE, Node_var_array), Node_in_array, yypvt[-2].nodeval); } break;
case 129:
# line 788 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, yypvt[-1].nodetypeval, yypvt[-0].nodeval); } break;
case 130:
# line 790 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-4].nodeval, Node_cond_exp, node(yypvt[-2].nodeval, Node_if_branches, yypvt[-0].nodeval));} break;
case 131:
# line 792 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 132:
# line 794 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-1].nodeval, Node_concat, yypvt[-0].nodeval); } break;
case 134:
# line 801 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_exp, yypvt[-0].nodeval); } break;
case 135:
# line 803 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_times, yypvt[-0].nodeval); } break;
case 136:
# line 805 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_quotient, yypvt[-0].nodeval); } break;
case 137:
# line 807 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_mod, yypvt[-0].nodeval); } break;
case 138:
# line 809 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_plus, yypvt[-0].nodeval); } break;
case 139:
# line 811 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-2].nodeval, Node_minus, yypvt[-0].nodeval); } break;
case 140:
# line 813 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-1].nodeval, Node_postincrement, (NODE *) NULL); } break;
case 141:
# line 815 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-1].nodeval, Node_postdecrement, (NODE *) NULL); } break;
case 142:
# line 820 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_not, (NODE *) NULL); } break;
case 143:
# line 822 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-1].nodeval; } break;
case 144:
# line 825 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = snode(yypvt[-1].nodeval, Node_builtin, (int) yypvt[-3].lval); } break;
case 145:
# line 827 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = snode(yypvt[-1].nodeval, Node_builtin, (int) yypvt[-3].lval); } break;
case 146:
# line 829 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		if (do_lint)
			lintwarn(_("call of `length' without parentheses is not portable"));
		yyval.nodeval = snode((NODE *) NULL, Node_builtin, (int) yypvt[-0].lval);
		if (do_posix)
			warning(_("call of `length' without parentheses is deprecated by POSIX"));
	  } break;
case 147:
# line 837 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		yyval.nodeval = node(yypvt[-1].nodeval, Node_func_call, make_string(yypvt[-3].sval, strlen(yypvt[-3].sval)));
		func_use(yypvt[-3].sval, FUNC_USE);
		param_sanity(yypvt[-1].nodeval);
		free(yypvt[-3].sval);
	  } break;
case 149:
# line 845 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_preincrement, (NODE *) NULL); } break;
case 150:
# line 847 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_predecrement, (NODE *) NULL); } break;
case 151:
# line 849 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 152:
# line 851 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 153:
# line 854 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  if (yypvt[-0].nodeval->type == Node_val && (yypvt[-0].nodeval->flags & (STR|STRING)) == 0) {
			yypvt[-0].nodeval->numbr = -(force_number(yypvt[-0].nodeval));
			yyval.nodeval = yypvt[-0].nodeval;
		  } else
			yyval.nodeval = node(yypvt[-0].nodeval, Node_unary_minus, (NODE *) NULL);
		} break;
case 154:
# line 862 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		  /*
		   * was: $$ = $2
		   * POSIX semantics: force a conversion to numeric type
		   */
		  yyval.nodeval = node (make_number(0.0), Node_plus, yypvt[-0].nodeval);
		} break;
case 155:
# line 873 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = NULL; } break;
case 156:
# line 875 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = yypvt[-0].nodeval; } break;
case 157:
# line 880 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = variable(yypvt[-0].sval, CAN_FREE, Node_var); } break;
case 158:
# line 882 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{
		if (yypvt[-1].nodeval == NULL) {
			fatal(_("invalid subscript expression"));
		} else if (yypvt[-1].nodeval->rnode == NULL) {
			yyval.nodeval = node(variable(yypvt[-3].sval, CAN_FREE, Node_var_array), Node_subscript, yypvt[-1].nodeval->lnode);
			freenode(yypvt[-1].nodeval);
		} else
			yyval.nodeval = node(variable(yypvt[-3].sval, CAN_FREE, Node_var_array), Node_subscript, yypvt[-1].nodeval);
		} break;
case 159:
# line 892 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyval.nodeval = node(yypvt[-0].nodeval, Node_field_spec, (NODE *) NULL); } break;
case 161:
# line 900 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyerrok; } break;
case 162:
# line 904 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyerrok; } break;
case 165:
# line 913 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyerrok; want_assign = FALSE; } break;
case 166:
# line 916 "/osm/homedev/chunt/dev/porting/Toolset_4.1a4/Toolset/sde/OSMgawk/src/awkgram.y"
{ yyerrok; } break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

