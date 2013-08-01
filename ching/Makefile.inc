#	$NetBSD: Makefile.inc,v 1.2 2009/06/04 06:51:11 dholland Exp $

CPPFLAGS+=-I${.CURDIR}/../include
CFLAGS+=-I${.CURDIR}/../include
BINDIR?=${PREFIX}/bin
BINGRP?=staff
WARNS?=4
