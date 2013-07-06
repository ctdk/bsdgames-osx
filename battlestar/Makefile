#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/battlestar/Makefile,v 1.5.2.3 2002/08/07 16:31:41 ru Exp $

PROG=	battlestar
SRCS=	battlestar.c command1.c command2.c command3.c command4.c \
	command5.c command6.c command7.c \
	init.c cypher.c getcom.c parse.c room.c save.c fly.c misc.c \
	globals.c dayfile.c nightfile.c dayobjs.c nightobjs.c words.c
MAN=	battlestar.6
DPADD=	${LIBCURSES} ${LIBTERMCAP}
LDADD=	-lcurses -ltermcap
HIDEGAME=hidegame

beforeinstall:
.if !exists(${VARDIR})
	mkdir -p ${VARDIR}
.endif
.if !exists(${VARDIR}/battlestar.log)
	${INSTALL} -o ${BINOWN} -g ${BINGRP} -m 664 /dev/null \
	    ${VARDIR}/battlestar.log
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
