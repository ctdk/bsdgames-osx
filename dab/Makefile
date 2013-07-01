#	$NetBSD: Makefile,v 1.3 2004/01/05 15:35:59 christos Exp $

WARNS=0
DPADD+=${LIBCURSES} ${LIBTERMCAP} ${LIBM}
LDADD+=-lcurses -ltermcap -lm

PROG_CXX=dab
MAN=dab.6
SRCS=algor.cc board.cc main.cc human.cc box.cc player.cc gamescreen.cc \
    ttyscrn.cc random.cc

beforeinstall:
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
