#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/cribbage/Makefile,v 1.7.2.5 2002/08/07 16:31:41 ru Exp $
# $DragonFly: src/games/cribbage/Makefile,v 1.4 2006/10/08 16:22:35 pavalos Exp $

PROG=	cribbage
DPADD=	${LIBCURSES}
LDADD=	-lcurses
SRCS=	extern.c crib.c cards.c instr.c io.c score.c support.c
FILES=	cribbage.n
FILESNAME_cribbage.n=	cribbage.instr
MAN=	cribbage.6
HIDEGAME=hidegame

beforeinstall:
.if !exists(${BINDIR})
	mkdir -p ${BINDIR}
.endif
.if !exists(${FILESDIR})
	mkdir -p ${FILESDIR}
.endif
.if !exists(${VARDIR})
	mkdir -p ${VARDIR}
.endif
.if !exists(${VARDIR}/criblog)
	${INSTALL} -o ${BINOWN} -g ${BINGRP} -m 664 /dev/null \
	    ${VARDIR}/criblog
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
