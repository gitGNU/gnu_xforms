/*
 *
 *  This file is part of the XForms library package.
 *
 * XForms is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1, or
 * (at your option) any later version.
 *
 * XForms is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XForms; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */


/**
 * \file version.c
 *.
 *  This file is part of the XForms library package.
 *  Copyright (c) 1996-2002  T.C. Zhao
 *  All rights reserved.
 *.
 *
 *  Version info
 *
 */

#if defined(F_ID) || defined(DEBUG)
char *fl_id_ver = "$Id: version.c,v 1.4 2003/04/10 21:30:55 leeming Exp $";
#endif

#include <stdio.h>
#include "forms.h"

static const char *version[] =
{
    "(Compiled "__DATE__")",
#ifdef FL_WIN32
    "Copyright (c) 1996-2002 by T.C. Zhao, Gang Li and Mark Overmars",
#else
    "Copyright (c) 1996-2002 by T.C. Zhao and Mark Overmars",
#endif
    "Parts Copyright(c) 1999-2002 by T.C. Zhao and Steve Lamont",
    0
};


int
fl_library_version(int *ver, int *rev)
{
    int v, r = -1;
    int c = 0;

    if (rev)
	*rev = FL_REVISION;
    if (ver)
	*ver = FL_VERSION;

    return v * 1000 + r;
}


void
fl_print_version(int g)
{
    char tmp[100];
#ifndef M_XENIX
    const char *p[5], **q = version;
#else
    char *p[5], **q = (char **) version;
#endif
    int n;

    snprintf(tmp, 100, "FORMS Library Version %d.%d", FL_VERSION, FL_REVISION);
    p[0] = tmp;

    for (n = 1; *q; n++, q++) {
	/* SCO compiler needs the cast */
	p[n] = (char *) fl_rm_rcs_kw(*q);
    }

    if (g) {
	if (n >= 3)
	    fl_show_message(p[0], p[1], p[2]);
	else if (n == 2)
	    fl_show_message(p[0], "", p[1]);
	else
	    fl_show_message("", p[0], "");
    } else {
	int i = 0;
	for (; i<n; ++i)
	    fprintf(stderr, "%s\n", p[i]);
    }
}