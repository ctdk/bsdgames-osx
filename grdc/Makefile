# $FreeBSD: src/games/grdc/Makefile,v 1.4.2.2 2001/10/02 11:51:49 ru Exp $
# $DragonFly: src/games/grdc/Makefile,v 1.4 2006/10/08 16:22:35 pavalos Exp $

PROG=   grdc
MAN=   grdc.6
DPADD=  ${LIBNCURSES}
LDADD=  -lncurses

beforeinstall:
.if !exists(${BINDIR})
	mkdir -p ${BINDIR}
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.prog.mk>
