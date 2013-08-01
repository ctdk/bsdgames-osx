#	$NetBSD: Makefile,v 1.2 2008/10/30 21:37:55 mrg Exp $

SCRIPTS=ching.sh
MAN=	ching.6

.include <bsd.own.mk>

FILES=	hexagrams macros
FILESDIR?= ${SHAREDIR}/games

beforeinstall:
.if !exists(${BINDIR})
	mkdir -p ${BINDIR}
.endif
.if !exists(${FILESDIR})
	mkdir -p ${FILESDIR}
.endif

.include <bsd.prog.mk>
