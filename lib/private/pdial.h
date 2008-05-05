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
 * \file pdial.h
 *
 *  This file is part of the XForms library package.
 *  Copyright (c) 1995-1997  T.C. Zhao and Mark Overmars
 *  All rights reserved.
 *.
 * Private header for dial object
 *
 */

#ifndef PDIAL_H
#define PDIAL_H

typedef struct
{
    double a,				/* for speed                      */
	       b;
    double min,				/* min/max value of dial          */
	       max;
    double val;				/* current value of dial          */
    double step;			/* step size                      */
    double thetai;			/* start angle (degrees)          */
    double thetaf;			/* end angle   (degrees)          */
    double origin;			/* where the origin is            */
    short  how_return;		/* whether always returning value */
    short  cross_over;
    short  changed;
    short  direction;		/* not currently used            */
} FLI_DIAL_SPEC;


#endif /* pdial_h */
