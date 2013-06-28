/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* hack.h - version 1.0.3 */
/* $DragonFly: src/games/hack/hack.h,v 1.4 2006/08/21 19:45:32 pavalos Exp $ */

#include "config.h"
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "def.objclass.h"

typedef struct {
	xchar x, y;
} coord;

#include "def.monst.h"	/* uses coord */
#include "def.gold.h"
#include "def.trap.h"
#include "def.obj.h"
#include "def.flag.h"
#include "def.mkroom.h"
#include "def.wseg.h"

#define	plur(x)	(((x) == 1) ? "" : "s")

#define	BUFSZ	256	/* for getlin buffers */
#define	PL_NSIZ	32	/* name of player, ghost, shopkeeper */

#include "def.rm.h"
#include "def.permonst.h"

extern xchar xdnstair, ydnstair, xupstair, yupstair; /* stairs up and down. */

#define	newstring(x)	alloc((unsigned)(x))
#include "hack.onames.h"

#define	ON	1
#define	OFF	0

extern struct obj *invent, *uwep, *uarm, *uarm2, *uarmh, *uarms, *uarmg,
	*uleft, *uright, *fcobj;
extern struct obj *uchain;	/* defined iff PUNISHED */
extern struct obj *uball;	/* defined if PUNISHED */

struct prop {
#define	TIMEOUT		007777	/* mask */
#define	LEFT_RING	W_RINGL	/* 010000L */
#define	RIGHT_RING	W_RINGR	/* 020000L */
#define	INTRINSIC	040000L
#define	LEFT_SIDE	LEFT_RING
#define	RIGHT_SIDE	RIGHT_RING
#define	BOTH_SIDES	(LEFT_SIDE | RIGHT_SIDE)
	long p_flgs;
	void (*p_tofn)(void);	/* called after timeout */
};

struct you {
	xchar ux, uy;
	schar dx, dy, dz;	/* direction of move (or zap or ... ) */
#ifdef QUEST
	schar di;		/* direction of FF */
	xchar ux0, uy0;		/* initial position FF */
#endif /* QUEST */
	xchar udisx, udisy;	/* last display pos */
	char usym;		/* usually '@' */
	schar uluck;
#define	LUCKMAX		10	/* on moonlit nights 11 */
#define	LUCKMIN		(-10)
	int last_str_turn:3;	/* 0: none, 1: half turn, 2: full turn */
				/* +: turn right, -: turn left */
	unsigned udispl:1;	/* @ on display */
	unsigned ulevel:4;	/* 1 - 14 */
#ifdef QUEST
	unsigned uhorizon:7;
#endif /* QUEST */
	unsigned utrap:3;	/* trap timeout */
	unsigned utraptype:1;	/* defined if utrap nonzero */
#define	TT_BEARTRAP	0
#define	TT_PIT		1
	unsigned uinshop:6;	/* used only in shk.c - (roomno+1) of shop */


/* perhaps these #define's should also be generated by makedefs */
#define	TELEPAT		LAST_RING		/* not a ring */
#define	Telepat		u.uprops[TELEPAT].p_flgs
#define	FAST		(LAST_RING+1)		/* not a ring */
#define	Fast		u.uprops[FAST].p_flgs
#define	CONFUSION	(LAST_RING+2)		/* not a ring */
#define	Confusion	u.uprops[CONFUSION].p_flgs
#define	INVIS		(LAST_RING+3)		/* not a ring */
#define	Invis		u.uprops[INVIS].p_flgs
#define	Invisible	(Invis && !See_invisible)
#define	GLIB		(LAST_RING+4)		/* not a ring */
#define	Glib		u.uprops[GLIB].p_flgs
#define	PUNISHED	(LAST_RING+5)		/* not a ring */
#define	Punished	u.uprops[PUNISHED].p_flgs
#define	SICK		(LAST_RING+6)		/* not a ring */
#define	Sick		u.uprops[SICK].p_flgs
#define	BLIND		(LAST_RING+7)		/* not a ring */
#define	Blind		u.uprops[BLIND].p_flgs
#define	WOUNDED_LEGS	(LAST_RING+8)		/* not a ring */
#define	Wounded_legs	u.uprops[WOUNDED_LEGS].p_flgs
#define	STONED		(LAST_RING+9)		/* not a ring */
#define	Stoned		u.uprops[STONED].p_flgs
#define	PROP(x)		(x-RIN_ADORNMENT)       /* convert ring to index in uprops */
	unsigned umconf:1;
	const char *usick_cause;
	struct prop uprops[LAST_RING+10];

	unsigned uswallow:1;		/* set if swallowed by a monster */
	unsigned uswldtim:4;		/* time you have been swallowed */
	unsigned uhs:3;			/* hunger state - see hack.eat.c */
	schar ustr, ustrmax;
	schar udaminc;
	schar uac;
	int uhp, uhpmax;
	long int ugold, ugold0, uexp, urexp;
	int uhunger;			/* refd only in eat.c and shk.c */
	int uinvault;
	struct monst *ustuck;
	int nr_killed[CMNUM+2];		/* used for experience bookkeeping */
};

extern struct you u;

extern const char *traps[];
extern char vowels[];

extern xchar curx, cury;	/* cursor location on screen */

extern coord bhitpos;	/* place where thrown weapon falls to the ground */

extern xchar seehx, seelx, seehy, seely; /* where to see*/
extern const char *save_cm, *killer, *nomovemsg;

extern xchar dlevel, maxdlevel; /* dungeon level */

extern long moves;

extern int multi;

extern char lock[];

extern const char *occtxt;
extern const char *hu_stat[];

#define	DIST(x1,y1,x2,y2)	(((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)))

#define	PL_CSIZ		20	/* sizeof pl_character */
#define	MAX_CARR_CAP	120	/* so that boulders can be heavier */
#define	MAXLEVEL	40
#define	FAR	(COLNO+2)	/* position outside screen */

extern schar xdir[], ydir[];
extern int hackpid, locknum, doorindex, done_stopprint;
extern char mlarge[], pl_character[PL_CSIZ], genocided[60], fut_geno[60];
extern char *hname, morc, plname[PL_NSIZ], sdir[];
extern boolean level_exists[], restoring, in_mklev;
extern struct permonst pm_eel, pm_ghost;
extern void (*afternmv)(void);
extern struct monst *mydogs;
extern bool (*occupation)(void);

/* Non-static function prototypes */

/* alloc.c */
void	*alloc(size_t);

/* hack.apply.c */
int	doapply(void);
int	holetime(void);
void	dighole(void);

/* hack.bones.c */
void	savebones(void);
int	getbones(void);

/* hack.c */
void	unsee(void);
void	seeoff(bool);
void	domove(void);
int	dopickup(void);
void	pickup(int);
void	lookaround(void);
bool	monster_nearby(void);
bool	cansee(xchar, xchar);
int	sgn(int);
void	setsee(void);
void	nomul(int);
int	abon(void);
int	dbon(void);
void	losestr(int);
void	losehp(int, const char *);
void	losehp_m(int, struct monst *);
void	losexp(void);
int	inv_weight(void);
long	newuexp(void);

/* hack.cmd.c */
void	rhack(const char *);
bool	movecmd(char);
bool	getdir(bool);
void	confdir(void);
#ifdef QUEST
void	finddir(void);
#endif
bool	isok(int, int);

/* hack.do.c */
int		 dodrop(void);
void		 dropx(struct obj *);
int		 doddrop(void);
int		 dodown(void);
int		 doup(void);
void		 goto_level(int, bool);
int		 donull(void);
int		 dopray(void);
int		 dothrow(void);
struct obj	*splitobj(struct obj *, int);
void		 more_experienced(int, int);
void		 set_wounded_legs(long, int);
void		 heal_legs(void);

/* hack.do_name.c */
coord	 getpos(int, const char *);
int	 do_mname(void);
int	 ddocall(void);
void	 docall(struct obj *);
char	*monnam(struct monst *);
char	*Monnam(struct monst *);
char	*amonnam(struct monst *, const char *);
char	*Amonnam(struct monst *, const char *);
char	*Xmonnam(struct monst *);

/* hack.do_wear.c */
int		 doremarm(void);
int		 doremring(void);
bool		 armoroff(struct obj *);
int		 doweararm(void);
int		 dowearring(void);
void		 ringoff(struct obj *);
void		 find_ac(void);
void		 glibr(void);
struct obj	*some_armor(void);
void		 corrode_armor(void);

/* hack.dog.c */
void	makedog(void);
void	losedogs(void);
void	keepdogs(void);
void	fall_down(struct monst *);
int	dog_move(struct monst *, int);
int	inroom(xchar, xchar);
bool	tamedog(struct monst *, struct obj *);

/* hack.eat.c */
void	init_uhunger(void);
int	doeat(void);
void	gethungry(void);
void	morehungry(int);
void	lesshungry(int);
bool	poisonous(struct obj *);

/* hack.end.c */
void	 done1(int);
void	 done_in_by(struct monst *);
void	 done(const char *);
void	 clearlocks(void);
#ifdef NOSAVEONHANGUP
void	 hangup(int);
#endif
char	*eos(char *);
void	 charcat(char *, char);
void	 prscore(int, char **);

/* hack.engrave.c */
bool	sengr_at(const char *, xchar, xchar);
void	u_wipe_engr(int);
void	wipe_engr_at(xchar, xchar, xchar);
void	read_engr_at(int, int);
void	make_engr_at(int, int, const char *);
int	doengrave(void);
void	save_engravings(int);
void	rest_engravings(int);

/* hack.fight.c */
int	hitmm(struct monst *, struct monst *);
void	mondied(struct monst *);
int	fightm(struct monst *);
bool	thitu(int, int, const char *);
bool	hmon(struct monst *, struct obj *, int);
bool	attack(struct monst *);

/* hack.invent.c */
struct obj	*addinv(struct obj *);
void		 useup(struct obj *);
void		 freeinv(struct obj *);
void		 delobj(struct obj *);
void		 freeobj(struct obj *);
void		 freegold(struct gold *);
void		 deltrap(struct trap *);
struct monst	*m_at(int, int);
struct obj	*o_at(int, int);
struct obj	*sobj_at(int, int, int);
bool		 carried(struct obj *);
bool		 carrying(int);
struct obj	*o_on(unsigned int, struct obj *);
struct trap	*t_at(int, int);
struct gold	*g_at(int, int);
struct obj	*getobj(const char *, const char *);
int		 ggetobj(const char *, int (*)(struct obj *), int);
int		 askchain(struct obj *, char *, int, int (*)(struct obj *),
			  bool (*)(struct obj *), int);
void		 prinv(struct obj *);
int		 ddoinv(void);
int		 dotypeinv(void);
int		 dolook(void);
void		 stackobj(struct obj *);
int		 doprgold(void);
int		 doprwep(void);
int		 doprarm(void);
int		 doprring(void);
bool		 digit(char);

/* hack.ioctl.c */
void	getioctls(void);
void	setioctls(void);
#ifdef SUSPEND
int	dosuspend(void);
#endif

/* hack.lev.c */
void	savelev(int, xchar);
void	bwrite(int, char *, unsigned int);
void	saveobjchn(int, struct obj *);
void	savemonchn(int, struct monst *);
void	getlev(int, int, xchar);
void	mread(int, char *, unsigned int);
void	mklev(void);

/* hack.main.c */
void	glo(int);
void	askname(void);
void	impossible(const char *, ...) __printflike(1, 2);
void	stop_occupation(void);

/* hack.makemon.c */
struct monst	*makemon(struct permonst *, int, int);
coord		 enexto(xchar, xchar);
bool		 goodpos(int, int);
void		 rloc(struct monst *);
struct monst	*mkmon_at(char, int, int);

/* hack.mhitu.c */
bool	mhitu(struct monst *);
bool	hitu(struct monst *, int);

/* hack.mklev.c */
void	makelevel(void);
void	mktrap(int, int, struct mkroom *);

/* hack.mkmaze.c */
void	makemaz(void);
coord	mazexy(void);

/* hack.mkobj.c */
struct obj	*mkobj_at(int, int, int);
void		 mksobj_at(int, int, int);
struct obj	*mkobj(int);
struct obj	*mksobj(int);
bool		 letter(char);
int		 weight(struct obj *);
void		 mkgold(long, int, int);

/* hack.mkshop.c */
#ifndef QUEST
void	mkshop(void);
void	mkzoo(int);
void	mkswamp(void);
#endif

/* hack.mon.c */
void	movemon(void);
void	justswld(struct monst *, const char *);
void	youswld(struct monst *, int, int, const char *);
bool	dochug(struct monst *);
int	m_move(struct monst *, int);
int	mfndpos(struct monst *, coord *, int *, int);
int	dist(int, int);
void	poisoned(const char *, const char *);
void	mondead(struct monst *);
void	replmon(struct monst *, struct monst *);
void	relmon(struct monst *);
void	monfree(struct monst *);
void	unstuck(struct monst *);
void	killed(struct monst *);
void	kludge(const char *, const char *);
void	rescham(void);
bool	newcham(struct monst *, struct permonst *);
void	mnexto(struct monst *);
void	setmangry(struct monst *);
bool	canseemon(struct monst *);

/* hack.o_init.c */
int	letindex(char);
void	init_objects(void);
int	probtype(char);
void	oinit(void);
void	savenames(int);
void	restnames(int);
int	dodiscovered(void);

/* hack.objnam.c */
char		*typename(int);
char		*xname(struct obj *);
char		*doname(struct obj *);
void		 setan(const char *, char *);
char		*aobjnam(struct obj *, const char *);
char		*Doname(struct obj *);
struct obj	*readobjnam(char *);

/* hack.options.c */
void	initoptions(void);
int	doset(void);

/* hack.pager.c */
int	dowhatis(void);
void	set_whole_screen(void);
#ifdef NEWS
bool	readnews(void);
#endif
void	set_pager(int);
bool	page_line(const char *);
void	cornline(int, const char *);
int	dohelp(void);
bool	page_file(const char *, bool);
#ifdef UNIX
#ifdef SHELL
int	dosh(void);
#endif /* SHELL */
bool	child(bool);
#endif /* UNIX */

/* hack.potion.c */
int	dodrink(void);
void	pluslvl(void);
void	strange_feeling(struct obj *, const char *);
void	potionhit(struct monst *, struct obj *);
void	potionbreathe(struct obj *);
int	dodip(void);

/* hack.pri.c */
void	swallowed(void);
void	panic(const char *, ...) __printflike(1, 2);
void	atl(int, int, char);
void	on_scr(int, int);
void	tmp_at(schar, schar);
void	Tmp_at(schar, schar);
void	setclipped(void);
void	at(xchar, xchar, char);
void	prme(void);
int	doredraw(void);
void	docrt(void);
void	docorner(int, int);
void	curs_on_u(void);
void	pru(void);
void	prl(int, int);
char	news0(xchar, xchar);
void	newsym(int, int);
void	mnewsym(int, int);
void	nosee(int, int);
#ifndef QUEST
void	prl1(int, int);
void	nose1(int, int);
#endif
bool	vism_at(int, int);
void	unpobj(struct obj *);
void	seeobjs(void);
void	seemons(void);
void	pmon(struct monst *);
void	unpmon(struct monst *);
void	nscr(void);
void	bot(void);
#ifdef WAN_PROBING
void	mstatusline(struct monst *);
#endif
void	cls(void);

/* hack.read.c */
int	doread(void);
int	identify(struct obj *);
void	litroom(bool);

/* hack.rip.c */
void	outrip(void);

/* hack.rumors.c */
void	outrumor(void);

/* hack.save.c */
int		 dosave(void);
#ifndef NOSAVEONHANGUP
void		 hangup(int);
#endif
bool		 dorecover(int);
struct obj	*restobjchn(int);
struct monst	*restmonchn(int);

/* hack.search.c */
int	findit(void);
int	dosearch(void);
int	doidtrap(void);
void	wakeup(struct monst *);
void	seemimic(struct monst *);

/* hack.shk.c */
#ifdef QUEST
void		 obfree(struct obj *, struct obj *);
int		 inshop(void);
void		 shopdig(void);
void		 addtobill(void);
void		 subfrombill(void);
void		 splitbill(void);
int		 dopay(void);
void		 paybill(void);
int		 doinvbill(void);
void		 shkdead(void);
int		 shkcatch(void);
int		 shk_move(void);
void		 replshk(struct monst *, struct monst *);
const char	*shkname(void);
#else
char		*shkname(struct monst *);
void		 shkdead(struct monst *);
void		 replshk(struct monst *, struct monst *);
int		 inshop(void);
void		 obfree(struct obj *, struct obj *);
int		 dopay(void);
void		 paybill(void);
void		 addtobill(struct obj *);
void		 splitbill(struct obj *, struct obj *);
void		 subfrombill(struct obj *);
int		 doinvbill(int);
bool		 shkcatch(struct obj *);
int		 shk_move(struct monst *);
void		 shopdig(int);
#endif
bool		 online(int, int);
bool		 follower(struct monst *);

/* hack.shknam.c */
void	findname(char *, char);

/* hack.steal.c */
long	somegold(void);
void	stealgold(struct monst *);
bool	steal(struct monst *);
void	mpickobj(struct monst *, struct obj *);
bool	stealamulet(struct monst *);
void	relobj(struct monst *, int);

/* hack.termcap.c */
void	startup(void);
void	start_screen(void);
void	end_screen(void);
void	curs(int, int);
void	cl_end(void);
void	clear_screen(void);
void	home(void);
void	standoutbeg(void);
void	standoutend(void);
void	backsp(void);
void	bell(void);
void	cl_eos(void);

/* hack.timeout.c */
void	p_timeout(void);

/* hack.topl.c */
int	doredotopl(void);
void	remember_topl(void);
void	addtopl(const char *);
void	more(void);
void	cmore(const char *);
void	clrlin(void);
void	pline(const char *, ...) __printflike(1, 2);
void	vpline(const char *, va_list) __printflike(1, 0);
void	putsym(char);
void	putstr(const char *);

/* hack.track.c */
void	 initrack(void);
void	 settrack(void);
coord	*gettrack(int, int);

/* hack.trap.c */
struct trap	*maketrap(int, int, int);
void		 dotrap(struct trap *);
int		 mintrap(struct monst *);
void		 selftouch(const char *);
void		 float_up(void);
void		 float_down(void);
void		 tele(void);
int		 dotele(void);
void		 placebc(int);
void		 unplacebc(void);
void		 level_tele(void);
void		 drown(void);

/* hack.tty.c */
void	 gettty(void);
void	 settty(const char *);
void	 setftty(void);
void	 error(const char *, ...) __printflike(1, 2);
void	 getlin(char *);
void	 getret(void);
void	 cgetret(const char *);
void	 xwaitforspace(const char *);
char	*parse(void);
char	 readchar(void);
void	 end_of_input(void);

/* hack.u_init.c */
void	u_init(void);
void	plnamesuffix(void);

/* hack.unix.c */
void	 setrandom(void);
int	 getyear(void);
char	*hack_getdate(void);
int	 phase_of_the_moon(void);
bool	 night(void);
bool	 midnight(void);
void	 gethdate(const char *);
bool	 uptodate(int);
void	 getlock(void);
#ifdef MAIL
void	 getmailstatus(void);
void	 ckmailstatus(void);
void	 readmail(void);
#endif
void	 regularize(char *);

/* hack.vault.c */
void	setgd(void);
int	gd_move(void);
void	replgd(struct monst *, struct monst *);
void	invault(void);
#ifdef QUEST
void	gddead(struct monst *);
#else
void	gddead(void);
#endif

/* hack.version.c */
int	doversion(void);

/* hack.wield.c */
void	setuwep(struct obj *);
int	dowield(void);
void	corrode_weapon(void);
bool	chwepon(struct obj *, int);

/* hack.wizard.c */
void	amulet(void);
bool	wiz_hit(struct monst *);
void	inrange(struct monst *);

/* hack.worm.c */
#ifndef NOWORM
bool	getwn(struct monst *);
void	initworm(struct monst *);
void	worm_move(struct monst *);
void	worm_nomove(struct monst *);
void	wormdead(struct monst *);
void	wormhit(struct monst *);
void	wormsee(unsigned int);
void	pwseg(struct wseg *);
void	cutworm(struct monst *, xchar, xchar, uchar);
#endif

/* hack.worn.c */
void	setworn(struct obj *, long);
void	setnotworn(struct obj *);

/* hack.zap.c */
int		 dozap(void);
const char	*exclam(int);
void		 hit(const char *, struct monst *, const char *);
void		 miss(const char *, struct monst *);
struct monst	*bhit(int, int, int, char,
		      void (*)(struct monst *, struct obj *),
		      bool (*)(struct obj *, struct obj *), struct obj *);
struct monst	*boomhit(int, int);
void		 buzz(int, xchar, xchar, int, int);
void		 fracture_rock(struct obj *);

/* rnd.c */
int	rn1(int, int);
int	rn2(int);
int	rnd(int);
int	d(int, int);
