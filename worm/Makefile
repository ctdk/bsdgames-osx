#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/worm/Makefile,v 1.2.14.1 2001/04/25 09:29:04 ru Exp $
# $DragonFly: src/games/worm/Makefile,v 1.4 2006/10/08 16:22:36 pavalos Exp $

PROG=	worm
MAN=	worm.6
DPADD=	${LIBCURSES}
LDADD=	-lcurses
HIDEGAME=hidegame

beforeinstall:
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
