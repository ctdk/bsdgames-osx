#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/arithmetic/Makefile,v 1.1.1.1.14.1 2001/04/25 09:28:43 ru Exp $
# $DragonFly: src/games/arithmetic/Makefile,v 1.4 2006/10/08 16:22:35 pavalos Exp $

PROG=	arithmetic
MAN=	arithmetic.6
HIDEGAME=hidegame

beforeinstall:
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
