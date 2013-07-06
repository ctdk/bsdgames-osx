#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/fish/Makefile,v 1.4.6.3 2002/02/18 16:02:41 ru Exp $
# $DragonFly: src/games/fish/Makefile,v 1.4 2006/10/08 16:22:35 pavalos Exp $

PROG=	fish
FILES=	fish.instr
MAN=	fish.6
HIDEGAME=hidegame

beforeinstall:
.if !exists(${BINDIR})
	mkdir -p ${BINDIR}
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
