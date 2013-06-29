#	@(#)Makefile	8.1 (Berkeley) 5/31/93

SUBDIR= canfield cfscores

beforeinstall:
.if !exists(${PREFIX}/var/games)
	mkdir -p ${PREFIX}/var/games
.endif

.include <bsd.subdir.mk>
