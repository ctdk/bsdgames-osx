#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/caesar/Makefile,v 1.4.6.2 2001/04/25 14:03:58 ru Exp $
# $DragonFly: src/games/caesar/Makefile,v 1.4 2006/10/08 16:22:35 pavalos Exp $

PROG=	caesar
DPADD=	${LIBM}
LDADD=	-lm
SCRIPTS=rot13.sh
MAN=	caesar.6
MLINKS=	caesar.6 rot13.6

beforeinstall:
.if !exists(${BINDIR})
	mkdir -p ${BINDIR}
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
