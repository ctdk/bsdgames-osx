/*	$NetBSD: main.c,v 1.73 2022/05/29 21:02:37 rillig Exp $	*/

/*
 * Copyright (c) 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__COPYRIGHT("@(#) Copyright (c) 1994\
 The Regents of the University of California.  All rights reserved.");
/*	@(#)main.c	8.4 (Berkeley) 5/4/95	*/
__RCSID("$NetBSD: main.c,v 1.73 2022/05/29 21:02:37 rillig Exp $");

#include <sys/stat.h>
#include <curses.h>
#include <err.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <utmpx.h>
#undef EMPTY

#include "gomoku.h"

enum input_source {
	USER,			/* get input from standard input */
	PROGRAM,		/* get input from program */
	INPUTF			/* get input from a file */
};

enum testing_mode {
	NORMAL_PLAY,
	USER_VS_USER,
	PROGRAM_VS_PROGRAM
};

bool	interactive = true;	/* true if interactive */
int	debug;			/* > 0 if debugging */
static enum testing_mode test = NORMAL_PLAY;
static char *prog;		/* name of program */
static char user[_UTX_USERSIZE]; /* name of player */
static FILE *debugfp;		/* file for debug output */
static FILE *inputfp;		/* file for debug input */

const char	pdir[4]		= "-\\|/";

struct	spotstr	board[BAREA];		/* info for board */
struct	combostr frames[FAREA];		/* storage for all frames */
struct	combostr *sortframes[2];	/* sorted list of non-empty frames */
u_char	overlap[FAREA * FAREA];		/* non-zero if frame [a][b] overlap;
					 * see init_overlap */
spot_index intersect[FAREA * FAREA];	/* frame [a][b] intersection */
struct game game;
const char *plyr[2] = { "???", "???" };	/* who's who */

static spot_index readinput(FILE *);
static void misclog(const char *, ...) __printflike(1, 2);
static void quit(void) __dead;
#if !defined(DEBUG)
static void quitsig(int) __dead;
#endif

static void
warn_if_exists(const char *fname)
{
	struct stat st;

	if (lstat(fname, &st) == 0) {
		int x, y;
		getyx(stdscr, y, x);
		addstr("  (already exists)");
		move(y, x);
	} else
		clrtoeol();
}

static void
save_game(void)
{
	char fname[PATH_MAX];
	FILE *fp;

	ask("Save file name? ");
	(void)get_line(fname, sizeof(fname), warn_if_exists);
	if ((fp = fopen(fname, "w")) == NULL) {
		misclog("cannot create save file");
		return;
	}
	for (unsigned int m = 0; m < game.nmoves; m++)
		fprintf(fp, "%s\n", stoc(game.moves[m]));
	fclose(fp);
}

static void
parse_args(int argc, char **argv)
{
	int ch;

	prog = strrchr(argv[0], '/');
	prog = prog != NULL ? prog + 1 : argv[0];

	while ((ch = getopt(argc, argv, "bcdD:u")) != -1) {
		switch (ch) {
		case 'b':	/* background */
			interactive = false;
			break;
		case 'c':
			test = PROGRAM_VS_PROGRAM;
			break;
		case 'd':
			debug++;
			break;
		case 'D':	/* log debug output to file */
			if ((debugfp = fopen(optarg, "w")) == NULL)
				err(1, "%s", optarg);
			break;
		case 'u':
			test = USER_VS_USER;
			break;
		default:
		usage:
			fprintf(stderr, "usage: %s [-bcdu] [-Dfile] [file]\n",
			    getprogname());
			exit(EXIT_FAILURE);
		}
	}
	argc -= optind;
	argv += optind;
	if (argc > 1)
		goto usage;
	if (argc == 1 && (inputfp = fopen(*argv, "r")) == NULL)
		err(1, "%s", *argv);
}

static void
set_input_sources(enum input_source *input, player_color color)
{
	switch (test) {
	case NORMAL_PLAY:
		input[color] = USER;
		input[color != BLACK ? BLACK : WHITE] = PROGRAM;
		break;
	case USER_VS_USER:
		input[BLACK] = USER;
		input[WHITE] = USER;
		break;
	case PROGRAM_VS_PROGRAM:
		input[BLACK] = PROGRAM;
		input[WHITE] = PROGRAM;
		break;
	}
}

static int
ask_user_color(void)
{
	int color;

	mvprintw(BSZ + 3, 0, "Black moves first. ");
	ask("(B)lack or (W)hite? ");
	for (;;) {
		int ch = get_key(NULL);
		if (ch == 'b' || ch == 'B') {
			color = BLACK;
			break;
		}
		if (ch == 'w' || ch == 'W') {
			color = WHITE;
			break;
		}
		if (ch == 'q' || ch == 'Q')
			quit();

		beep();
		ask("Please choose (B)lack or (W)hite: ");
	}
	move(BSZ + 3, 0);
	clrtoeol();
	return color;
}

static int
read_color(void)
{
	char buf[128];

	get_line(buf, sizeof(buf), NULL);
	if (strcmp(buf, "black") == 0)
		return BLACK;
	if (strcmp(buf, "white") == 0)
		return WHITE;
	panic("Huh?  Expected `black' or `white', got `%s'\n", buf);
	
	return 0;  /* NOTREACHED */
}

static spot_index
read_move(void)
{
again:
	if (interactive) {
		ask("Select move, (S)ave or (Q)uit.");
		spot_index s = get_coord();
		if (s == SAVE) {
			save_game();
			goto again;
		}
		if (s != RESIGN && board[s].s_occ != EMPTY) {
			beep();
			goto again;
		}
		return s;
	} else {
		char buf[128];
		if (!get_line(buf, sizeof(buf), NULL))
			return RESIGN;
		if (buf[0] == '\0')
			goto again;
		return ctos(buf);
	}
}

static void
declare_winner(int outcome, const enum input_source *input, player_color color)
{

	move(BSZ + 3, 0);
	switch (outcome) {
	case WIN:
		if (input[color] == PROGRAM)
			addstr("Ha ha, I won");
		else if (input[0] == USER && input[1] == USER)
			addstr("Well, you won (and lost)");
		else
			addstr("Rats! you won");
		break;
	case TIE:
		addstr("Wow! It's a tie");
		break;
	case ILLEGAL:
		addstr("Illegal move");
		break;
	}
	clrtoeol();
	bdisp();
}

struct outcome {
	int result;
	player_color winner;
};

static struct outcome
main_game_loop(enum input_source *input)
{
	spot_index curmove = 0;
	player_color color = BLACK;

again:
	switch (input[color]) {
	case INPUTF:
		curmove = readinput(inputfp);
		if (curmove != END_OF_INPUT)
			break;
		set_input_sources(input, color);
		plyr[BLACK] = input[BLACK] == USER ? user : prog;
		plyr[WHITE] = input[WHITE] == USER ? user : prog;
		bdwho();
		refresh();
		goto again;

	case USER:
		curmove = read_move();
		break;

	case PROGRAM:
		if (interactive)
			ask("Thinking...");
		curmove = pickmove(color);
		break;
	}

	if (interactive && curmove != ILLEGAL) {
		misclog("%3u%*s%-6s",
		    game.nmoves + 1, color == BLACK ? 2 : 9, "",
		    stoc(curmove));
	}

	int outcome;
	if ((outcome = makemove(color, curmove)) != MOVEOK)
		return (struct outcome){ outcome, color };

	if (interactive)
		bdisp();
	color = color != BLACK ? BLACK : WHITE;
	goto again;
}

int
main(int argc, char **argv)
{
	char *user_name;
	int color;
	enum input_source input[2];

	/* Revoke setgid privileges */
	setgid(getgid());

	setprogname(argv[0]);

	user_name = getlogin();
	strlcpy(user, user_name != NULL ? user_name : "you", sizeof(user));

	color = BLACK;

	parse_args(argc, argv);

	if (debug == 0)
		srandom((unsigned int)time(0));
	if (interactive)
		cursinit();		/* initialize curses */
again:
	init_board();			/* initialize board contents */

	if (interactive) {
		bdisp_init();		/* initialize display of board */
#ifdef DEBUG
		signal(SIGINT, whatsup);
#else
		signal(SIGINT, quitsig);
#endif

		if (inputfp == NULL && test == NORMAL_PLAY)
			color = ask_user_color();
	} else {
		setbuf(stdout, NULL);
		color = read_color();
	}

	if (inputfp != NULL) {
		input[BLACK] = INPUTF;
		input[WHITE] = INPUTF;
	} else {
		set_input_sources(input, color);
	}
	if (interactive) {
		plyr[BLACK] = input[BLACK] == USER ? user : prog;
		plyr[WHITE] = input[WHITE] == USER ? user : prog;
		bdwho();
		refresh();
	}

	struct outcome outcome = main_game_loop(input);

	if (interactive) {
		declare_winner(outcome.result, input, outcome.winner);
		if (outcome.result != RESIGN) {
		replay:
			ask("Play again? ");
			int ch = get_key("YyNnQqSs");
			if (ch == 'Y' || ch == 'y')
				goto again;
			if (ch == 'S' || ch == 's') {
				save_game();
				goto replay;
			}
		}
	}
	quit();
}

static spot_index
readinput(FILE *fp)
{
	int c;
	char buf[128];
	size_t pos;

	pos = 0;
	while ((c = getc(fp)) != EOF && c != '\n' && pos < sizeof(buf) - 1)
		buf[pos++] = c;
	buf[pos] = '\0';
	return c == EOF ? END_OF_INPUT : ctos(buf);
}

#ifdef DEBUG

static bool
skip_any(const char **pp, const char *s)
{
	while (strchr(s, **pp) != NULL)
		(*pp)++;
	return true;
}

static bool
parse_char_index(const char **pp, const char *s, unsigned int *out)
{
	const char *found = strchr(s, **pp);
	if (found != NULL)
		*out = (unsigned int)(found - s), (*pp)++;
	return found != NULL;
}

static bool
parse_direction(const char **pp, direction *out)
{
	unsigned int u;
	if (!parse_char_index(pp, "-\\|/", &u))
		return false;
	*out = (direction)u;
	return true;
}

static bool
parse_row(const char **pp, unsigned int *out)
{
	if (!('0' <= **pp && **pp <= '9'))
		return false;
	unsigned int u = *(*pp)++ - '0';
	if ('0' <= **pp && **pp <= '9')
		u = 10 * u + *(*pp)++ - '0';
	*out = u;
	return 1 <= u && u <= BSZ;
}

static bool
parse_spot(const char **pp, spot_index *out)
{
	unsigned row, col;
	if (!parse_char_index(pp, "abcdefghjklmnopqrst", &col) &&
	    !parse_char_index(pp, "ABCDEFGHJKLMNOPQRST", &col))
		return false;
	if (!parse_row(pp, &row))
		return false;
	*out = PT(col + 1, row);
	return true;
}

/*
 * Handle strange situations and ^C.
 */
/* ARGSUSED */
void
whatsup(int signum __unused)
{
	unsigned int n;
	player_color color;
	spot_index s, s1, s2;
	direction r1, r2;
	struct spotstr *sp;
	FILE *fp;
	const char *str;
	struct elist *ep;
	struct combostr *cbp;
	char input[128];
	char tmp[128];

	if (!interactive)
		quit();
top:
	ask("debug command: ");
	if (!get_line(input, sizeof(input), NULL))
		quit();
	switch (*input) {
	case '\0':
		goto top;
	case 'q':		/* conservative quit */
		quit();
		/* NOTREACHED */
	case 'd':		/* set debug level */
		debug = input[1] - '0';
		debuglog("Debug set to %d", debug);
		goto top;
	case 'c':
		ask("");
		return;
	case 'b':		/* back up a move */
		if (game.nmoves > 0) {
			game.nmoves--;
			board[game.moves[game.nmoves]].s_occ = EMPTY;
			bdisp();
		}
		goto top;
	case 's':		/* suggest a move */
		color = input[1] == 'b' ? BLACK : WHITE;
		debuglog("suggest %c %s", color == BLACK ? 'B' : 'W',
			stoc(pickmove(color)));
		goto top;
	case 'f':		/* go forward a move */
		board[game.moves[game.nmoves]].s_occ =
		    game.nmoves % 2 == 0 ? BLACK : WHITE;
		game.nmoves++;
		bdisp();
		goto top;
	case 'l':		/* print move history */
		if (input[1] == '\0') {
			for (unsigned int m = 0; m < game.nmoves; m++)
				debuglog("%s", stoc(game.moves[m]));
			goto top;
		}
		if ((fp = fopen(input + 1, "w")) == NULL)
			goto top;
		for (unsigned int m = 0; m < game.nmoves; m++) {
			fprintf(fp, "%s", stoc(game.moves[m]));
			if (++m < game.nmoves)
				fprintf(fp, " %s\n", stoc(game.moves[m]));
			else
				fputc('\n', fp);
		}
		bdump(fp);
		fclose(fp);
		goto top;
	case 'o':
		str = input + 1;
		if (skip_any(&str, " ") &&
		    parse_spot(&str, &s1) &&
		    parse_direction(&str, &r1) &&
		    skip_any(&str, ", ") &&
		    parse_spot(&str, &s2) &&
		    parse_direction(&str, &r2) &&
		    *str == '\0') {
			n = board[s1].s_frame[r1] * FAREA
			    + board[s2].s_frame[r2];
			debuglog("overlap %s%c,%s%c = %02x",
			    stoc(s1), pdir[r1], stoc(s2), pdir[r2],
			    overlap[n]);
		} else
			debuglog("usage: o <spot><dir> <spot><dir>");
		goto top;
	case 'p':
		sp = &board[s = ctos(input + 1)];
		debuglog("V %s %x/%d %d %x/%d %d %d %x", stoc(s),
			sp->s_combo[BLACK].s, sp->s_level[BLACK],
			sp->s_nforce[BLACK],
			sp->s_combo[WHITE].s, sp->s_level[WHITE],
			sp->s_nforce[WHITE], sp->s_wval, sp->s_flags);
		debuglog("FB %s %x %x %x %x", stoc(s),
			sp->s_fval[BLACK][0].s, sp->s_fval[BLACK][1].s,
			sp->s_fval[BLACK][2].s, sp->s_fval[BLACK][3].s);
		debuglog("FW %s %x %x %x %x", stoc(s),
			sp->s_fval[WHITE][0].s, sp->s_fval[WHITE][1].s,
			sp->s_fval[WHITE][2].s, sp->s_fval[WHITE][3].s);
		goto top;
	case 'e':	/* e [0-9] spot */
		str = input + 1;
		if (*str >= '0' && *str <= '9')
			n = *str++ - '0';
		else
			n = 0;
		sp = &board[ctos(str)];
		for (ep = sp->s_empty; ep != NULL; ep = ep->e_next) {
			cbp = ep->e_combo;
			if (n != 0) {
				if (cbp->c_nframes > n)
					continue;
				if (cbp->c_nframes != n)
					break;
			}
			printcombo(cbp, tmp, sizeof(tmp));
			debuglog("%s", tmp);
		}
		goto top;
	default:
		debuglog("Options are:");
		debuglog("q    - quit");
		debuglog("c    - continue");
		debuglog("d#   - set debug level to #");
		debuglog("p#   - print values at #");
		goto top;
	}
}
#endif /* DEBUG */

/*
 * Display debug info.
 */
void
debuglog(const char *fmt, ...)
{
	va_list ap;
	char buf[128];

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	if (debugfp != NULL)
		fprintf(debugfp, "%s\n", buf);
	if (interactive)
		dislog(buf);
	else
		fprintf(stderr, "%s\n", buf);
}

static void
misclog(const char *fmt, ...)
{
	va_list ap;
	char buf[128];

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	if (debugfp != NULL)
		fprintf(debugfp, "%s\n", buf);
	if (interactive)
		dislog(buf);
	else
		printf("%s\n", buf);
}

static void
quit(void)
{
	if (interactive) {
		bdisp();		/* show final board */
		cursfini();
	}
	exit(0);
}

#if !defined(DEBUG)
static void
quitsig(int dummy __unused)
{
	quit();
}
#endif

/*
 * Die gracefully.
 */
void
panic(const char *fmt, ...)
{
	va_list ap;

	if (interactive) {
		bdisp();
		cursfini();
	}

	fprintf(stderr, "%s: ", prog);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");

	fputs("I resign\n", stdout);
	exit(1);
}
