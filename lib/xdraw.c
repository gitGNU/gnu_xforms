/*
 *  This file is part of the XForms library package.
 *
 *  XForms is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1, or
 *  (at your option) any later version.
 *
 *  XForms is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * \file xdraw.c
 *
 *  This file is part of the XForms library package.
 *  Copyright (c) 1996-2002  T.C. Zhao and Mark Overmars
 *  All rights reserved.
 *
 *  Basic low level drawing routines in Xlib.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "include/forms.h"
#include "flinternal.h"


static int mono_dither( unsigned long );
static void set_current_gc( GC );

static GC dithered_gc;


/***************************************
 * Function for drawing a (possibly filled) rectangle
 ***************************************/

void
fl_rectangle( int      fill,
              FL_COORD x,
              FL_COORD y,
              FL_COORD w,
              FL_COORD h,
              FL_COLOR col )
{
    int bw = fli_dithered( fl_vmode ) && mono_dither( col );
    GC gc = flx->gc;
    int ( * draw_as )( Display *,
                       Drawable,
                       GC,
                       int,
                       int,
                       unsigned int,
                       unsigned int );

    if ( flx->win == None || w <= 0 || h <= 0 )
        return;

    fli_canonicalize_rect( &x, &y, &w, &h );

    draw_as = fill ? XFillRectangle : XDrawRectangle;

    if ( bw && fill )
    {
        set_current_gc( fli_whitegc );
        draw_as( flx->display, flx->win, flx->gc, x, y, w, h );
        set_current_gc( dithered_gc );
    }


    fl_color( bw ? FL_BLACK : col );
    draw_as( flx->display, flx->win, flx->gc, x, y, w, h );

    if ( bw )
        set_current_gc( gc );
}


/***************************************
 * Function for drawing a closed (possibly filled) polygon.
 * The 'xp' argument must point to an array with n + 1 elements!
 ***************************************/

void
fl_polygon( int        fill,
            FL_POINT * xp,
            int        n,
            FL_COLOR   col )
{
    int bw = fli_dithered( fl_vmode ) && mono_dither( col );
    GC gc = flx->gc;

    if ( flx->win == None || n <= 0 )
        return;

    if ( bw )
    {
        flx->gc = dithered_gc;
        fl_color( FL_WHITE );
        if ( fill )
            XFillPolygon( flx->display, flx->win, flx->gc, xp, n,
                          Nonconvex, CoordModeOrigin );
        else
        {
            xp[ n ].x = xp[ 0 ].x;
            xp[ n ].y = xp[ 0 ].y;
            XDrawLines( flx->display, flx->win, flx->gc, xp, n + 1,
                        CoordModeOrigin );
        }
    }

    fl_color( bw ? FL_BLACK : col );

    if ( fill )
        XFillPolygon( flx->display, flx->win, flx->gc, xp, n,
                      Nonconvex, CoordModeOrigin );
    else
    {
        xp[ n ].x = xp[ 0 ].x;
        xp[ n ].y = xp[ 0 ].y;
        XDrawLines( flx->display, flx->win, flx->gc, xp, n + 1,
                    CoordModeOrigin );
    }

    if ( bw )
        flx->gc = gc;
}


/****************************************************************
 * Draws a circle at (x, y) woth radius r and col as to color
 * of the outline
 **********************************************************{******/

void
fl_circ( FL_COORD x,
         FL_COORD y,
         FL_COORD r,
         FL_COLOR col )
{
    fl_oval( 0, x - r, y - r, 2 * r, 2 * r, col );
}


/****************************************************************
 * Draws a filled circle at (x, y) woth radius r in color col
 **********************************************************{******/

void
fl_circf( FL_COORD x,
          FL_COORD y,
          FL_COORD r,
          FL_COLOR col )
{
    fl_oval( 1, x - r, y - r, 2 * r, 2 * r, col );
}


/****************************************************************
 * Draws a filled circle at (x, y) woth radius r in color col and
 * the circumfence in black.
 **********************************************************{******/

void
fl_circbound( FL_COORD x,
              FL_COORD y,
              FL_COORD r,
              FL_COLOR col )
{
    fl_ovalbound( x - r, y - r, 2 * r, 2 * r, col );
}


/****************************************************************
 * Function for drawing a (possibly filled) ellipse.
 **********************************************************{******/

void
fl_oval( int      fill,
         FL_COORD x,
         FL_COORD y,
         FL_COORD w,
         FL_COORD h,
         FL_COLOR col )
{
    int bw = fli_dithered( fl_vmode ) && mono_dither( col );
    GC gc = flx->gc;
    int ( * draw_as )( Display *,
                       Drawable,
                       GC,
                       int,
                       int,
                       unsigned int,
                       unsigned int,
                       int,
                       int );

    if ( flx->win == None || w <= 0 || h <= 0 )
        return;

    draw_as = fill ? XFillArc : XDrawArc;

    if ( bw )
    {
        set_current_gc( fli_whitegc );
        draw_as( flx->display, flx->win, flx->gc, x, y, w, h, 0, 360 * 64 );
        set_current_gc( dithered_gc );
    }

    fl_color( bw ? FL_BLACK : col );

    if ( w >= 0 && h >= 0 )
        draw_as( flx->display, flx->win, flx->gc, x, y, w, h, 0, 360 * 64 );

    if ( bw )
        set_current_gc( gc );
}


/***************************************
 * Draws a filled ellipse with a black border
 ***************************************/

void
fl_ovalbound( FL_COORD x,
              FL_COORD y,
              FL_COORD w,
              FL_COORD h,
              FL_COLOR col )
{
    if ( flx->win == None || w <= 0 || h <= 0 )
        return;

    fl_color( col );
    XFillArc( flx->display, flx->win, flx->gc, x, y, w, h, 0, 360 * 64 );
    fl_color( FL_BLACK );
    XDrawArc( flx->display, flx->win, flx->gc, x, y, w - 1, h - 1, 0,
              360 * 64 );
}


/***************************************
 ***************************************/

void
fl_ovalarc( int      fill,
            FL_COORD x,
            FL_COORD y,
            FL_COORD w,
            FL_COORD h,
            int      t0,
            int      dt,
            FL_COLOR col )
{
    int mono = fli_dithered( fl_vmode ) && mono_dither( col );
    GC gc = flx->gc;
    int ( * draw_as )( Display *,
                       Drawable,
                       GC,
                       int,
                       int,
                       unsigned int,
                       unsigned int,
                       int,
                       int );

    if ( flx->win == None || w <= 0 || h <= 0 )
        return;

    draw_as = fill ? XFillArc : XDrawArc;

    if ( mono )
    {
        set_current_gc( fli_whitegc );
        draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
                 t0 * 6.4, dt * 6.4 );
        set_current_gc( dithered_gc );
    }

    fl_color( mono ? FL_BLACK : col );

    if ( w >= 0 && h >= 0 )
        draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
                 t0 * 6.4, dt * 6.4 );

    if ( mono )
        set_current_gc( gc );
}


/***************************************
 ***************************************/

void
fl_arcf( FL_COORD x,
         FL_COORD y,
         FL_COORD r,
         int      a1,
         int      a2,
         FL_COLOR col )
{
    fl_pieslice( 1, x - r, y - r, 2 * r, 2 * r, a1, a2, col );
}


/***************************************
 ***************************************/

void
fl_arc( FL_COORD x,
        FL_COORD y,
        FL_COORD r,
        int      a1,
        int      a2,
        FL_COLOR col )
{
    fl_pieslice( 0, x - r, y - r, 2 * r, 2 * r, a1, a2, col );
}


/***************************************
 ***************************************/

void
fl_pieslice( int      fill,
             FL_COORD x,
             FL_COORD y,
             FL_COORD w,
             FL_COORD h,
             int      a1,
             int      a2,
             FL_COLOR col )
{
    int delta = a2 - a1,
        bw = fli_dithered( fl_vmode ) && mono_dither( col );
    GC gc = flx->gc;
    int ( * draw_as )( Display *,
                       Drawable,
                       GC,
                       int,
                       int,
                       unsigned int,
                       unsigned int,
                       int,
                       int );

    if ( flx->win == None || w <= 0 || h <= 0)
        return;

    draw_as = fill ? XFillArc : XDrawArc;

    if ( bw )
    {
        set_current_gc( fli_whitegc );
        draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
                 a1 * 6.4, delta * 6.4 );
        set_current_gc( dithered_gc );
    }

    fl_color( bw ? FL_BLACK : col );

    if ( w >= 0 && h >= 0 )
        draw_as( flx->display, flx->win, flx->gc, x, y, w, h,
                 a1 * 6.4, delta * 6.4 );
    if ( bw )
        set_current_gc( gc );
}


/***************************************
 ***************************************/

void
fl_lines( FL_POINT * xp,
          int        n,
          FL_COLOR   col )
{
    if ( flx->win == None  || n <= 0 )
        return;

    fl_color( col );

    /* We may need to break up the request into smaller pieces */

    if ( fli_context->ext_request_size >= n )
        XDrawLines( flx->display, flx->win, flx->gc, xp, n, CoordModeOrigin );
    else
    {
        int req = fli_context->ext_request_size;
        int i,
            nchunks = ( n + ( n / req ) ) / req,
            left;
        FL_POINT *p = xp;

        for ( i = 0; i < nchunks; i++, p += req - 1 )
            XDrawLines( flx->display, flx->win, flx->gc, p, req,
                        CoordModeOrigin );

        left = xp + n - p;

        if ( left )
        {
            if ( left == 1 )
            {
                p--;
                left++;
            }

            XDrawLines( flx->display, flx->win, flx->gc, p, left,
                        CoordModeOrigin );
        }
    }
}


/***************************************
 * Drwas a simple line from (x1,y1) to (x2,y2)
 ***************************************/

void
fl_line( FL_COORD xi,
         FL_COORD yi,
         FL_COORD xf,
         FL_COORD yf,
         FL_COLOR c )
{
    if ( flx->win == None )
        return;

    fl_color( c );
    XDrawLine( flx->display, flx->win, flx->gc, xi, yi, xf, yf );
}


/***************************************
 * Draws a simple point at the given position
 ***************************************/

void
fl_point( FL_COORD x,
          FL_COORD y,
          FL_COLOR c )
{
    if ( flx->win == None )
        return;

    fl_color( c );
    XDrawPoint( flx->display, flx->win, flx->gc, x, y );
}


/***************************************
 * Draws a set of simple point at the given positions
 ***************************************/

void
fl_points( FL_POINT * p,
           int        np,
           FL_COLOR   c )
{
    if ( flx->win == None || np <= 0 )
        return;

    fl_color( c );
    XDrawPoints( flx->display, flx->win, flx->gc, p, np, CoordModeOrigin );
}


/********************************************************************
 * Basic drawing attributes
 ****************************************************************{*/

static int lw     = 0,
           ls     = LineSolid,
           drmode = GXcopy;


/***************************************
 ***************************************/

void
fl_linewidth( int n )
{
    XGCValues gcvalue;
    unsigned long gcmask;

    if ( lw == n )
        return;

    gcmask = GCLineWidth;
    gcvalue.line_width = lw = n;
    XChangeGC( flx->display, flx->gc, gcmask, &gcvalue );
}


/***************************************
 ***************************************/

int
fl_get_linewidth( void )
{
    return lw;
}


static void fli_xdashedlinestyle( Display *,
                                  GC,
                                  const char *,
                                  int );

/***************************************
 ***************************************/

void
fli_xlinestyle( Display * d,
                GC       gc,
                int      n )
{
    static char dots[ ]    = { 2, 4 };
    static char dotdash[ ] = { 7, 3, 2, 3 };
    static char ldash[ ]   = { 10, 4 };
    XGCValues gcvalue;
    unsigned long gcmask;

    if ( ls == n )
        return;

    ls = n;

    gcmask = GCLineStyle;

    if ( n == FL_DOT )
        fli_xdashedlinestyle( d, gc, dots, 2 );
    else if ( n == FL_DOTDASH )
        fli_xdashedlinestyle( d, gc, dotdash, 4 );
    else if ( n == FL_LONGDASH )
        fli_xdashedlinestyle( d, gc, ldash, 2 );
    if ( n > LineDoubleDash )
        n = LineOnOffDash;

    gcvalue.line_style = n;
    XChangeGC( d, gc, gcmask, &gcvalue );
}


/***************************************
 ***************************************/

void
fl_linestyle( int n )
{
    fli_xlinestyle( flx->display, flx->gc, n );
}


/***************************************
 ***************************************/

int
fl_get_linestyle( void )
{
    return ls;
}


/***************************************
 ***************************************/

int
fl_get_drawmode( void )
{
    return drmode;
}


/***************************************
 ***************************************/

void
fl_drawmode( int request )
{
    if ( drmode != request )
        XSetFunction( flx->display, flx->gc, drmode = request );
}


/***************************************
 ***************************************/

static void
fli_xdashedlinestyle( Display    * d,
                      GC           gc,
                      const char * dash,
                      int          ndash )
{
    static char default_dash[ ] = { 4, 4 };

    if ( dash == NULL )
    {
        dash = default_dash;
        ndash = 2;
    }

    XSetDashes( d, gc, 0, ( char * ) dash, ndash );
}


/***************************************
 ***************************************/

void
fl_dashedlinestyle( const char * dash,
                    int          ndash )
{
    static char default_dash[ ] = { 4, 4 };

    /* Check that the input is reasonable - XSetDashes() requires that
       there's at least one element in the 'dash' array and that none
       of its elements are 0. If one of these conditions isn't satis-
       fied (or 'dash' itself is NULL) set the defaut dash pattern. */

    if ( dash )
    {
        int i;

        for ( i = 0; i < ndash; i++ )
            if ( dash[ i ] == '\0' )
            {
                M_warn( "fl_dashedlinestyle", "Invalid '\0' in dash pattern "
                        "array, using default pattern" );
                ndash = 0;
                break;
            }
    }

    if ( ! dash || ! ndash )
    {
        dash = default_dash;
        ndash = 2;
    }

    XSetDashes( flx->display, flx->gc, 0, ( char * ) dash, ndash );
}


/***************************************
 ***************************************/

static void
set_current_gc( GC gc )
{
    if ( flx->gc == gc )
        return;

    flx->gc    = gc;
    flx->color = FL_NoColor;

    fli_apply_clipping_to_gc( gc );
}


/***************************************
 * Manually dither non-gray scale colors by changing default GC. Grayscales
 * are typically used in buttons, boxes etc, better not to dither them
 ***************************************/

static int
mono_dither( unsigned long col )
{
    int bwtrick = 0;

    switch ( col )
    {
        case FL_RED:
        case FL_MAGENTA:
        case FL_SLATEBLUE:
        case FL_PALEGREEN:
        case FL_DARKGOLD:
        case FL_INACTIVE_COL:
            dithered_gc = fli_bwgc[ 1 ];
            bwtrick     = 1;
            break;

        case FL_YELLOW:
        case FL_CYAN:
        case FL_INDIANRED:
        case FL_GREEN:
            dithered_gc = fli_bwgc[ 2 ];
            bwtrick     = 1;
            break;

        case FL_BLUE:
            dithered_gc = fli_bwgc[ 0 ];
            bwtrick     = 1;
            break;

        default:
            if ( col >= FL_FREE_COL1 )
            {
                int r,
                    g,
                    b;

                fl_get_icm_color( col, &r, &g, &b );
                if ( ( bwtrick = ( r > 70 && r <= 210 ) ) )
                    dithered_gc = fli_bwgc[ r / 70 - 1 ];
            }
            break;
    }

    return bwtrick;
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
