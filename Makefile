#	@(#)Makefile	8.2 (Berkeley) 3/31/94
# $FreeBSD: src/games/Makefile,v 1.16 1999/08/27 23:28:45 peter Exp $
# $DragonFly: src/games/Makefile,v 1.3 2008/09/02 21:50:17 dillon Exp $



# XXX missing: chess ching monop [copyright]
SUBDIR= adventure \
	arithmetic \
	atc \
	backgammon \
	battlestar \
	bcd \
	bs \
	caesar \
	ching \
	canfield \
	cribbage \
	dab \
	factor \
	fish \
	grdc \
	gomoku \
	hack \
	hangman \
	hunt \
	larn \
	mille \
	number \
	phantasia \
	pig \
	pom \
	ppt \
	primes \
	quiz \
	rain \
	random \
	robots \
	rogue \
	sail \
	snake \
	trek \
	wargames \
	worm \
	worms \
	wtf \
	wump

# maximum parallelism
#
SUBDIR_ORDERED=

.include <bsd.subdir.mk>
