/*-
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)rnd_pos.c	8.1 (Berkeley) 5/31/93
 * $FreeBSD: src/games/robots/rnd_pos.c,v 1.5 1999/11/30 03:49:20 billf Exp $
 * $DragonFly: src/games/robots/rnd_pos.c,v 1.3 2006/08/27 21:45:07 pavalos Exp $
 */

#include "robots.h"

#define IS_SAME(p,y,x)	((p).y != -1 && (p).y == y && (p).x == x)

static int rnd(int);

/*
 * rnd_pos:
 *	Pick a random, unoccupied position
 */
COORD *
rnd_pos(void)
{
	static COORD pos;
//	static int call = 0;

	do {
		pos.y = rnd(Y_FIELDSIZE - 1) + 1;
		pos.x = rnd(X_FIELDSIZE - 1) + 1;
		refresh();
	} while (Field[pos.y][pos.x] != 0);
//	call++;
	return &pos;
}

static int
rnd(int range)
{

	return random() % range;
}
