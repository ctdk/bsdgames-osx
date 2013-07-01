#	@(#)Makefile	8.1 (Berkeley) 5/31/93

SUBDIR= canfield cfscores

beforeinstall:
.if !exists(${PREFIX}/var/games)
	mkdir -p ${PREFIX}/var/games
.endif
.if !exists(${MANDIR}6)
	mkdir -p ${MANDIR}6
.endif

.include <bsd.subdir.mk>
