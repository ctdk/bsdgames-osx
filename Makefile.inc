#	@(#)Makefile.inc	8.1 (Berkeley) 5/31/93
# $FreeBSD: src/games/Makefile.inc,v 1.10.2.2 2002/02/18 16:02:39 ru Exp $
# $DragonFly: src/games/Makefile.inc,v 1.3 2006/10/08 16:22:34 pavalos Exp $

PREFIX?=	/usr/local
BINDIR?=	${PREFIX}/bin
SHAREDIR=	${PREFIX}/share
MANDIR=		${SHAREDIR}/man/man
FILESDIR?=	${SHAREDIR}/games
# VARDIR?= 	${PREFIX}/var/games
VARDIR?= 	/var/games/bsdgames
WARNS?=		6

BINGRP?=	staff
SHAREGRP?= 	staff

# cribbed from DragonflyBSD build system
CFLAGS+=	-std=c11 -Wno-nullability-completeness

NXCC?=		${CC}
NXCFLAGS?=	${CFLAGS:N-mtune*:N-mcpu*:N-march*}

.SUFFIXES: .nx .no

# .no == native object file, for helper code when cross building.
#
.c.no:
	${NXCC} ${_${.IMPSRC:T}_FLAGS} ${NXCFLAGS} -c ${.IMPSRC} -o ${.TARGET}

.y.no:
	${YACC} ${YFLAGS} ${.IMPSRC}
	${NXCC} ${NXCFLAGS} -c y.tab.c -o ${.TARGET}
	rm -f y.tab.c

.l.no:
	${LEX} ${LFLAGS} -o${.TARGET}.c ${.IMPSRC}
	${NXCC} ${NXCFLAGS} -c ${.TARGET}.c -o ${.TARGET}
	rm -f ${.TARGET}.c

.no.nx .c.nx:
	${NXCC} ${_${.IMPSRC:T}_FLAGS} ${NXCFLAGS} ${NXLDFLAGS} ${.IMPSRC} \
	    ${NXLDLIBS} -o ${.TARGET}


# ##########
# from dragonfly Makefile.inc

# These games write to /var/games.bsdgames
.if defined(VARGAMES)
BINGRP=		games
BINMODE=	2555
.endif

# Handle libcrypto dependency
# .if defined(GAMESCRYPTO)
# LDADD+=		-lprivate_crypto
# DPADD+=		${LIBCRYPTO}
# CFLAGS+=	${PRIVATELIB_CFLAGS}
# LDFLAGS+=	${PRIVATELIB_LDFLAGS}
# .endif

# Handle libncurses dependency
.if defined(GAMESCURSES)
DPADD+=		${LIBNCURSES}
LDADD+=		-lncurses
.endif
