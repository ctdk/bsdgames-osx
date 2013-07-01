#	@(#)Makefile	8.1 (Berkeley) 5/31/93

SUBDIR=	backgammon teachgammon

beforeinstall:
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.subdir.mk>
