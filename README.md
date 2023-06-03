bsdgames-osx: the classic bsdgames ported to Mac OS X
-----------------------------------------------------

These are the classic bsdgames of old. They've been imported from DragonFly 
BSD's sources (the games in Dragonfly BSD can be found at 
http://gitweb.dragonflybsd.org/dragonfly.git/tree/v2.4.0:/games), along with 
some patches from FreeBSD ports and a couple of games from NetBSD, and only 
modified enough to get them to build on Mac OS X. 

WARNING: These games could conceivably be a security risk (that's the warning I
remember seeing with bsdgames in FreeBSD). If this is a concern, don't install
them. I am also not responsible if they blow up your computer, lure aliens to 
your house, run up huge credit card bills, eat your dog, or give you a menacing
side-eye. There's also NO WARRANTY, implied or otherwise.

*** hunt used to use hosts.allow and hosts.deny for access to the huntd server.
Mac OS 10.8 "Mountain Lion", however, has dropped tcp_wrappers from the
operating system. If you do run hunt on a server connected directly to the
Internet, use firewall rules to control who can connect to the huntd server
rather than hosts.allow and hosts.deny. ***

Games included are:

	adventure 
	arithmetic 
	atc 
	backgammon 
	battlestar 
	bcd 
	bs 
	caesar 
	canfield 
	ching
	cribbage 
	dab
	factor 
	fish 
	gomoku
	grdc 
	hack 
	hangman 
	hunt 
	larn 
	mille 
	number 
	phantasia 
	pig 
	pom 
	ppt 
	primes 
	quiz 
	rain 
	random 
	robots 
	rogue 
	sail 
	snake 
	trek 
	wargames 
	worm 
	worms 
	wtf
	wump

INSTALLATION
------------

bsdmake is needed to build the games. If needed, you can install it out of 
homebrew (it doesn't come with recent versions of Xcode).

These games have been built on Mac OS X 10.7 Lion through 10.12 Sierra. They 
seem to build best with clang - if the compiler complains about redundant 
declarations, "CC=clang bsdmake" seems to work wonders. Under 10.9 "Mavericks" 
(or other OS X versions if a newer clang is installed), if the compiler is 
complaining about "redefinition of typedef 'va_list' is a C11", add 
CFLAGS="-std=c11" to the build command. With 10.12 Sierra, the 
"-Wno-nullability-completeness" may be needed, so use CFLAGS="-std=c11 
-Wno-nullability-completeness". For me it was defaulting to clang on Mountain 
Lion, but to gcc on Lion. Other versions of Mac OS X have not been tested - 
older versions may work, and newer versions may or may not depending on if there
have been breaking clang/XCode changes (which happens every couple of OS X 
releases in my experience). Newer versions get tested when I get around to 
upgrading one of my Macs.

Installation is pretty basic. The steps are to run "bsdmake", then "sudo 
bsdmake install", and it will install in /usr/local (or elsewhere, if you edit 
the Makefile). Homebrew formulae are available in the Homebrew/ directory to
make installing bsdgames-osx easier, as well as a more general homebrew tap for
some of my programs at https://github.com/ctdk/homebrew-ctdk that includes
bsdgames-osx. See BUGS for information for 10.7 users (at least), however.

BUGS
----
Currently, to the best of my knowledge, all games are building correctly.

Many of these programs have hard coded paths. Currently all of them are now
changed to sensible defaults under /usr/local, but if you want to install 
somewhere that *isn't* /usr/local, you'll need to find and update the paths.

morse and piano have been removed because they depend on sound stuff that's not 
readily available on OS X. dm was removed because, well, no one would want to
deal with it anyway.

fortune has been removed because it is readily available elsewhere, and in fact
has its own homebrew formula. It's best not to duplicate it here.

If you use the bsdmake in /usr/bin on 10.7 (and probably before), it will try
to install as root by default. Either install the bsdmake from homebrew and use
the homebrew version (you'll need to directly use the version in 
/usr/local/Cellar) or set BINOWN, BINGRP, LIBOWN, LIBGRP, etc. to the user and
groups you want.

COPYRIGHT
---------
See the COPYRIGHT file & the source for the programs themselves. I claim no
authorship of them. Most of the games were imported from Dragonfly BSD, but
gomoku, dab, ching, and wtf were from NetBSD.

"AUTHOR"
--------
For lack of a better term. The work porting the bsdgames to OS X was done by me,
Jeremy Bingham <jbingham@gmail.com>.


What's new in this fork
-----------------------

* Gomoku can now be compiled on a mac M1.
* Use a recent version of backgammon from dragonfly in order to
  fix the display.
* Some other conflicts with mac M1 fixed.
* ming needs groff:
      brew install groff

This version can be compiled on Apple Silicon or on Intel CPU

To compile:

    bsdmake PREFIX=/usr/local VARDIR=/var/games

To install:

First, create a group called games

Then,

    BINOWN=$USER LIBOWN=$USER MANOWN=$USER SHAREOWN=$USER bsdmake install PREFIX=/usr/local VARDIR=/var/games
