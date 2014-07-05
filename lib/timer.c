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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * \file timer.c
 *
 *  This file is part of the XForms library package.
 *  Copyright (c) 1996-2002  T.C. Zhao and Mark Overmars
 *  All rights reserved.
 *
 * Forms Object class: TIMER
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdlib.h>
#include "include/forms.h"
#include "flinternal.h"
#include "private/ptimer.h"


/***************************************
 ***************************************/

static char *
default_filter( FL_OBJECT * obj  FL_UNUSED_ARG,
                double      totalsec )
{
    static char buf[ 32 ];
    int minutes;
    double sec;

    if ( totalsec >= 3600.0 )
    {
        int hr = totalsec / 3600.0 + 0.001;

        minutes = totalsec / 60.0 + 0.001;
        minutes -= hr * 60;
        sec = totalsec - 60 * ( minutes + 60 * hr );
        sprintf( buf, "%d:%02d:%04.1f", hr, minutes, sec );
    }
    else if ( totalsec >= 60.0 )
    {
        minutes = totalsec / 60.0 + 0.001;
        sec = totalsec - minutes * 60;
        sprintf( buf, "%d:%04.1f", minutes, sec );
    }
    else
        sprintf( buf, "%.1f", totalsec );

    return buf;
}


/***************************************
 * draws the timer
 ***************************************/

static void
draw_timer( FL_OBJECT * obj )
{
    FL_COLOR col;
    char *str;
    FLI_TIMER_SPEC *sp = obj->spec;

    if ( obj->type == FL_HIDDEN_TIMER )
        return;

    if ( ! sp->on || sp->time_left > 0.0 )
        col = obj->col1;
    else if ( ( int ) ( sp->time_left / FLI_TIMER_BLINKRATE ) % 2 )
        col = obj->col1;
    else
        col = obj->col2;

    fl_draw_box( obj->boxtype, obj->x, obj->y, obj->w, obj->h, col, obj->bw );

    if ( obj->type == FL_VALUE_TIMER && sp->time_left > 0.0 )
    {
        double time_shown = sp->up ? sp->timer - sp->time_left : sp->time_left;

        str = ( sp->filter ? sp->filter : default_filter )( obj, time_shown );
        fl_draw_text( FL_ALIGN_CENTER, obj->x, obj->y, obj->w, obj->h,
                      obj->lcol, obj->lstyle, obj->lsize, str );
    }
}


/***************************************
 * Handles an event
 ***************************************/

static int
handle_timer( FL_OBJECT * obj,
              int         event,
              FL_Coord    mx   FL_UNUSED_ARG,
              FL_Coord    my   FL_UNUSED_ARG,
              FL_Char     key  FL_UNUSED_ARG,
              void      * ev   FL_UNUSED_ARG )
{
    FLI_TIMER_SPEC *sp = obj->spec;
    long sec,
         usec;
    double lasttime_left;
    int ret = FL_RETURN_NONE;
    static int update_only;

    switch ( event )
    {
        case FL_ATTRIB :
            if ( obj->type == FL_VALUE_TIMER )
                obj->align = fl_to_outside_lalign( obj->align );
            break;

        case FL_DRAW:
            draw_timer( obj );
            /* fall through */

        case FL_DRAWLABEL:
            if (    obj->type == FL_HIDDEN_TIMER
                 || ( obj->type == FL_VALUE_TIMER && update_only ) )
                break;
            if ( fl_is_outside_lalign( obj->align ) )
                fl_draw_text_beside( obj->align, obj->x, obj->y, obj->w, obj->h,
                                     obj->lcol, obj->lstyle, obj->lsize,
                                     obj->label );
            else
                fl_draw_text( obj->align, obj->x, obj->y, obj->w, obj->h,
                              obj->lcol, obj->lstyle, obj->lsize,
                              obj->label );
            break;

        case FL_RELEASE:
            if ( obj->type != FL_HIDDEN_TIMER && sp->time_left < 0.0 )
                fl_set_timer( obj, 0.0 );
            break;

        case FL_STEP:
            if ( ! sp->on )
                break;
            lasttime_left = sp->time_left;
            fl_gettime( &sec, &usec );
            sp->time_left = sp->timer - ( sec - sp->sec )
                            - ( usec - sp->usec ) * 1.0e-6;
            update_only = 1;

            /* Don't check for zero, we can overshoot quite a bit. Instead try
               to split the error by already returning 10 ms too early. */

            if ( sp->time_left > 0.01 )
            {
                if (    obj->type == FL_VALUE_TIMER
                     && ( int ) ( 10.0 * sp->time_left ) !=
                                              ( int ) ( 10.0 * lasttime_left ) )
                    fl_redraw_object( obj );
            }
            else if ( lasttime_left > 0.0 )
            {
                if ( obj->type == FL_HIDDEN_TIMER )
                    fl_set_timer( obj, 0.0 );
                else
                    fl_redraw_object( obj );
                update_only = 0;
                ret = FL_RETURN_CHANGED | FL_RETURN_END;
                break;
            }
            else if ( ( int ) ( lasttime_left / FLI_TIMER_BLINKRATE ) !=
                              ( int ) ( sp->time_left / FLI_TIMER_BLINKRATE ) )
                fl_redraw_object( obj );

            update_only = 0;
            break;

        case FL_FREEMEM:
            fl_free( obj->spec );
            break;
    }

    return ret;
}


/***************************************
 * creates an object
 ***************************************/

FL_OBJECT *
fl_create_timer( int          type,
                 FL_Coord     x,
                 FL_Coord     y,
                 FL_Coord     w,
                 FL_Coord     h,
                 const char * l )
{
    FLI_TIMER_SPEC *sp;
    FL_OBJECT *obj = fl_make_object( FL_TIMER, type, x, y, w, h, l,
                                     handle_timer );

    obj->boxtype   = FLI_TIMER_BOXTYPE;
    obj->col1      = FLI_TIMER_COL1;
    obj->col2      = FLI_TIMER_COL2;
    if ( type != FL_VALUE_TIMER )
        obj->align = FLI_TIMER_ALIGN;
    else
        obj->align     = FL_ALIGN_LEFT;
    obj->lcol      = FLI_TIMER_LCOL;
    obj->spec = sp = fl_calloc( 1, sizeof *sp );

    fl_set_timer( obj, 0.0 );       /* disabled timer */
    sp->filter = default_filter;

    return obj;
}


/***************************************
 * Adds an object
 ***************************************/

FL_OBJECT *
fl_add_timer( int          type,
              FL_Coord     x,
              FL_Coord     y,
              FL_Coord     w,
              FL_Coord     h,
              const char * l )
{
    FL_OBJECT * obj = fl_create_timer( type, x, y, w, h, l );

    fl_add_object( fl_current_form, obj );
    if ( obj->type == FL_VALUE_TIMER )
        fl_set_object_dblbuffer( obj, 1 );
    return obj;
}


/***************************************
 * Sets the timer clock to the particular delay. (0.0 to reset)
 ***************************************/

void
fl_set_timer( FL_OBJECT * obj,
              double      total )
{
    FLI_TIMER_SPEC *sp = obj->spec;

    sp->time_left = sp->timer = total;
    sp->on = total > 0.0;
    fl_set_object_automatic( obj, sp->on );
    fl_gettime( &sp->sec, &sp->usec );
    if ( obj->type != FL_HIDDEN_TIMER )
        fl_redraw_object( obj );
}


/***************************************
 * returns the amount of time left
 ***************************************/

double
fl_get_timer( FL_OBJECT * obj )
{
    FLI_TIMER_SPEC *sp = obj->spec;

    return sp->time_left > 0.0 ? sp->time_left : 0.0;
}


/***************************************
 ***************************************/

void
fl_set_timer_countup( FL_OBJECT * obj,
                      int         yes )
{
    ( ( FLI_TIMER_SPEC * ) obj->spec )->up = yes;
}


/***************************************
 ***************************************/

FL_TIMER_FILTER
fl_set_timer_filter( FL_OBJECT       * obj,
                     FL_TIMER_FILTER   filter )
{
    FLI_TIMER_SPEC *sp = obj->spec;
    FL_TIMER_FILTER old = sp->filter;

    if ( filter != sp->filter )
    {
        sp->filter = filter;
        fl_redraw_object( obj );
    }
    return old;
}


/***************************************
 ***************************************/

void
fl_suspend_timer( FL_OBJECT * obj )
{
    ( ( FLI_TIMER_SPEC * ) obj->spec )->on = 0;
    fl_set_object_automatic( obj, 0 );
}


/***************************************
 ***************************************/

void
fl_resume_timer( FL_OBJECT * obj )
{
    FLI_TIMER_SPEC *sp = obj->spec;
    long sec, usec;
    double elapsed;

    if ( sp->on )
        return;

    elapsed = sp->timer - sp->time_left;
    fl_gettime( &sec, &usec );
    sp->sec = sec - ( long ) elapsed;
    sp->usec = usec - ( long ) ( ( elapsed - ( long ) elapsed ) * 1.0e6 );
    fl_set_object_automatic( obj, 1 );
    sp->on = 1;
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
