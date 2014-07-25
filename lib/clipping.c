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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "include/forms.h"
#include "flinternal.h"


/*
 *  Remember global clipping so unset_clipping will restore it. Most
 *  useful as part of event dispatching
 */
 
enum {
    GLOBAL_CLIP = 0,
    NORMAL_CLIP = 1,
    TEXT_CLIP   = 2,
    GC_CLIP     = 3
};


static int clipped_flags[ ] = { 0, 0, 0, 0 };
static FL_RECT clip_rect[ ] = { { 0, 0, 0, 0 },
                                { 0, 0, 0, 0 },
                                { 0, 0, 0, 0 },
                                { 0, 0, 0, 0 } };


#define SET_RECT( t, a, b, c, d )  \
   do {                            \
       ( t ).x      = a;           \
       ( t ).y      = b;           \
       ( t ).width  = c;           \
       ( t ).height = d;           \
   } while ( 0 )


#define GET_RECT( t, a, b, c, d )  \
   do {                            \
       *a = ( t ).x;               \
       *b = ( t ).y;               \
       *c = ( t ).width;           \
       *d = ( t ).height;          \
   } while ( 0 )


/***************************************
 * Helper function for determining if normal or text clipping is set
 * (also reports if global clipping is set when 'include_global' is
 * set).
 ***************************************/

static int
is_clipped( int type,
            int include_global )
{
    return    clipped_flags[ type ]
           || ( include_global && clipped_flags[ GLOBAL_CLIP ] );
}


/***************************************
 * Returns if "normal" (i.e. non-text) clipping is switched on.
 * If 'include_global' is set also also global clipping (used
 * e.g. when redrawing on an expose event) is on, otherwise only
 * clipping set via a call of fl_set_clipping() is reported.
 ***************************************/

int
fl_is_clipped( int include_global )
{
    return is_clipped( NORMAL_CLIP, include_global );
}


/***************************************
 * Returns if global clipping is switched on - in which case
 * the region for global clipping can be obtained via a call
 * of fli_get_global_clipping( ).
 ***************************************/

int
fl_is_global_clipped( void )
{
    return clipped_flags[ GLOBAL_CLIP ];
}


/***************************************
 * Returns if "text" clipping is switched on. If 'include_global'
 * is set also also global clipping (used e.g. when redrawing on
 * an expose event) is on, otherwise only clipping set via a call
 * of fl_set_text_clipping() is reported.
 ***************************************/

int
fl_is_text_clipped( int include_global )
{
    return is_clipped( TEXT_CLIP, include_global );
}


/***************************************
 * Sets global clipping to the specified rectangle. If normal clipping
 * is already on the rectange use is the one resulting from intersecting
 * the requested rectangle with the one for normal clipping. Text is
 * clipped in the same way. If called with a negative width or height
 * global clipping is switched off.
 ***************************************/

void
fli_set_global_clipping( FL_COORD x,
                         FL_COORD y,
                         FL_COORD w,
                         FL_COORD h )
{
    /* Calling the function with a rectangle with a negative width or height
       results in switching off of global clipping */

    if ( w < 0 || h < 0 )
    {
        fli_unset_global_clipping( );
        return;
    }

    SET_RECT( clip_rect[ GLOBAL_CLIP ], x, y, w, h );

    /* If normal clipping is already on intersect the new global and the
       normal clip area and set clipping to the result. If normal clipping
       is off just use the rectangle specified by the arguments. */

    if ( clipped_flags[ NORMAL_CLIP ] )
    {
        FL_RECT * r = fli_intersect_rects( clip_rect + GLOBAL_CLIP,
                                           clip_rect + NORMAL_CLIP );
        
        if ( r )
        {
            XSetClipRectangles( flx->display, flx->gc, 0, 0, r, 1, Unsorted );
            fli_safe_free( r );
        }
        else
        {
            FL_RECT n = { 0, 0, 0, 0 };

            XSetClipRectangles( flx->display, flx->gc, 0, 0, &n, 1, Unsorted );
        }
    }
    else
        XSetClipRectangles( flx->display, flx->gc, 0, 0,
                            &clip_rect[ GLOBAL_CLIP ], 1, Unsorted );

    /* The same again for text clipping */

    if ( clipped_flags[ TEXT_CLIP ] )
    {
        FL_RECT * r = fli_intersect_rects( clip_rect + GLOBAL_CLIP,
                                           clip_rect + TEXT_CLIP );

        if ( r )
        {
#if FL_ENABLE_XFT
            XftDrawSetClipRectangles( flx->textdraw, 0, 0, r, 1 );
            XftDrawSetClipRectangles( flx->bgdraw, 0, 0, r, 1 );
#else
            XSetClipRectangles( flx->display, flx->textgc, 0, 0, r, 1,
                                Unsorted );
#endif
            fli_safe_free( r );
        }
        else
        {
            XRectangle n = { 0, 0, 0, 0 };

#if FL_ENABLE_XFT
            XftDrawSetClipRectangles( flx->textdraw, 0, 0, &n, 1 );
            XftDrawSetClipRectangles( flx->bgdraw, 0, 0, &n, 1 );
#else

            XSetClipRectangles( flx->display, flx->textgc, 0, 0, &n, 1,
                                Unsorted );
#endif
        }
    }
    else
    {
#if FL_ENABLE_XFT
        XftDrawSetClipRectangles( flx->textdraw, 0, 0,
                                  clip_rect + GLOBAL_CLIP, 1 );
        XftDrawSetClipRectangles( flx->bgdraw, 0, 0,
                                  clip_rect + GLOBAL_CLIP, 1 );
#else
        XSetClipRectangles( flx->display, flx->textgc, 0, 0,
                            clip_rect + GLOBAL_CLIP, 1, Unsorted );
#endif
    }

    clipped_flags[ GLOBAL_CLIP ] = 1;
}


/***************************************
 * Unsets global clipping - "normal" and text clipping, if set, are retained
 ***************************************/

void
fli_unset_global_clipping( void )
{
    if ( ! clipped_flags[ GLOBAL_CLIP ] )
        return;

    SET_RECT( clip_rect[ GLOBAL_CLIP ], 0, 0, 0, 0 );

    /* If normal clipping is also on set the clipping rectangle to that set
       for normal clipping, otherwise switch clipping off completely. */

    if ( clipped_flags[ NORMAL_CLIP ] )
        XSetClipRectangles( flx->display, flx->gc, 0, 0,
                            clip_rect + NORMAL_CLIP, 1, Unsorted );
    else
        XSetClipMask( flx->display, flx->gc, None );

    /* Same for text clipping */

    if ( clipped_flags[ TEXT_CLIP ] )
    {
#if FL_ENABLE_XFT
        XftDrawSetClipRectangles( flx->textdraw, 0, 0,
                                  clip_rect + TEXT_CLIP, 1 );
        XftDrawSetClipRectangles( flx->bgdraw, 0, 0,
                                  clip_rect + TEXT_CLIP, 1 );
#else
        XSetClipRectangles( flx->display, flx->textgc, 0, 0,
                            clip_rect + TEXT_CLIP, 1, Unsorted );
#endif
    }
    else
    {
#if FL_ENABLE_XFT
        XRectangle r = { 0, 0, SHRT_MAX, SHRT_MAX };

        XftDrawSetClipRectangles( flx->textdraw, 0, 0, &r, 1 );
        XftDrawSetClipRectangles( flx->bgdraw, 0, 0, &r, 1 );
#else
        XSetClipMask( flx->display, flx->textgc, None );
#endif
    }

    clipped_flags[ GLOBAL_CLIP ] = 0;
}


/***************************************
 * Returns a pointer to the rectangle used for global clipping (only
 * to be used when global clipping is on!)
 ***************************************/

FL_RECT *
fli_get_global_clip_rect( void )
{
    return clip_rect + GLOBAL_CLIP;
}


/***************************************
 * Function returns as its return value if global clipping is on or off
 * and, via the pointer arguments, the clipping rectangle.
 ***************************************/

int
fl_get_global_clipping( FL_COORD * x,
                        FL_COORD * y,
                        FL_COORD * w,
                        FL_COORD * h )
{
    GET_RECT( clip_rect[ GLOBAL_CLIP ], x, y, w, h );
    return clipped_flags[ GLOBAL_CLIP ];
}


/***************************************
 * Helper function to switch off normal, text or GC clipping - if global
 * clipping is on it will be retained.
 ***************************************/

static void
unset_clipping( int type,
                GC  gc )
{
    if ( ! clipped_flags[ type ] )
        return;

    SET_RECT( clip_rect[ type ], 0, 0, 0, 0 );

    if ( clipped_flags[ GLOBAL_CLIP ] )
        XSetClipRectangles( flx->display, gc, 0, 0,
                            clip_rect + GLOBAL_CLIP, 1, Unsorted );
    else
        XSetClipMask( flx->display, gc, None );

    clipped_flags[ type ] = 0;
}


/***************************************
 * Helper function to switch normal, text or GC clipping on for a certain
 * a certain region. 'type' is the type of clipping and 'gc' the GC for
 * which the clipping is to be set. If global clipping is already on the
 * clipping region is the intersection of the requested clipping region
 * with the clipping region for global clipping.
 ***************************************/

static void
set_clipping( int      type,
              GC       gc,
              FL_COORD x,
              FL_COORD y,
              FL_COORD w,
              FL_COORD h )
{
    if ( w < 0 || h < 0 )
    {
        unset_clipping( type, gc );
        return;
    }

    SET_RECT( clip_rect[ type ], x, y, w, h );

    if ( clipped_flags[ GLOBAL_CLIP ] )
    {
        FL_RECT * r = fli_intersect_rects( clip_rect + GLOBAL_CLIP,
                                           clip_rect + type );
        
        if ( r )
        {
            XSetClipRectangles( flx->display, gc, 0, 0, r, 1, Unsorted );
            fli_safe_free( r );
        }
        else
        {
            FL_RECT n = { 0, 0, 0, 0 };

            XSetClipRectangles( flx->display, gc, 0, 0, &n, 1, Unsorted );
        }
    }
    else
        XSetClipRectangles( flx->display, gc, 0, 0, &clip_rect[ type ],
                            1, Unsorted );

    clipped_flags[ type ] = 1;
}


/***************************************
 * Helper function for determining if normal or text clipping is on and the
 * region clipped to. 'type' is the type of clipping. If 'include_global' is
 * set also clipping due to global clipping is taken into account (and the
 * reported clipping region is the one resulting from intersection of the
 * clipping region for the requested type of clipping and the one for global
 * clipping). The function returns as its return value if there's clipping
 * going on (which may, when 'include_global', also be due to global clipping).
 ***************************************/

static int
get_clipping( int        type,
              int        include_global,
              FL_COORD * x,
              FL_COORD * y,
              FL_COORD * w,
              FL_COORD * h )
{
    if (    ! ( include_global && clipped_flags[ GLOBAL_CLIP ] )
         && clipped_flags[ type ] )
        GET_RECT( clip_rect[ type ], x, y, w, h );
    else if ( include_global && clipped_flags[ GLOBAL_CLIP ] )
    {
        if ( clipped_flags[ type ] )
        {
            FL_RECT * r =
                fli_intersect_rects( clip_rect + GLOBAL_CLIP,
                                     clip_rect + type );
            
            if ( r )
            {
                GET_RECT( *r, x, y, w, h );
                fl_free( r );
            }
        }
        else
            GET_RECT( clip_rect[ GLOBAL_CLIP ], x, y, w, h );
    }
    else
        *x = *y = *w = *h = 0;

    return is_clipped( type, include_global );
}


/***************************************
 * Function for setting normal clipping. If global clipping is on the
 * rectangle set for clipping is the intersection of the rectangle specified
 * by the arguments and the one for global clipping.  Specifying a negative
 * clipping width or height results in clipping becoming switched off.
 ***************************************/

void
fl_set_clipping( FL_COORD x,
                 FL_COORD y,
                 FL_COORD w,
                 FL_COORD h )
{
    set_clipping( NORMAL_CLIP, flx->gc, x, y, w, h );
}


/****************************************
 * Function for switching of normal clipping. Note that global clipping
 * will be retained.
 ***************************************/

void
fl_unset_clipping( void )
{
    unset_clipping( NORMAL_CLIP, flx->gc );
}


/***************************************
 * Function for determining if normal clipping is on and the region clipped
 * to. If 'include_global' is set also clipping due to global clipping is
 * taken into account (and the reported clipping region is the one resulting
 * from intersection of the normal clipping region and the one for global
 * clipping). The function returns as its return value if there's clipping
 * going on (which may, when 'include_global', also be due to global clipping).
 ***************************************/

int
fl_get_clipping( int        include_global,
                 FL_COORD * x,
                 FL_COORD * y,
                 FL_COORD * w,
                 FL_COORD * h )
{
    return get_clipping( NORMAL_CLIP, include_global, x, y, w, h );
}


/***************************************
 * Function for setting text clipping. If global clipping is on the rectangle
 * set for clipping is the intersection of the rectangle specified by the
 * arguments and the one for global clipping. Specifying a negative clipping
 * width or height results in clipping becoming switched off.
 ***************************************/

void
fl_set_text_clipping( FL_COORD x,
                      FL_COORD y,
                      FL_COORD w,
                      FL_COORD h )
{
#if FL_ENABLE_XFT
    if ( w < 0 || h < 0 )
    {
        fl_unset_text_clipping( );
        return;
    }

    SET_RECT( clip_rect[ TEXT_CLIP ], x, y, w, h );

    if ( clipped_flags[ GLOBAL_CLIP ] )
    {
        FL_RECT * r = fli_intersect_rects( clip_rect + GLOBAL_CLIP,
                                           clip_rect + TEXT_CLIP );
        
        if ( r )
        {
            XftDrawSetClipRectangles( flx->textdraw, 0, 0, r, 1 );
            XftDrawSetClipRectangles( flx->bgdraw, 0, 0, r, 1 );

            fli_safe_free( r );
        }
        else
        {
            XRectangle n = { 0, 0, 0, 0 };

            XftDrawSetClipRectangles( flx->textdraw, 0, 0, &n, 1 );
            XftDrawSetClipRectangles( flx->bgdraw, 0, 0, &n, 1 );
        }
    }
    else
    {
        XftDrawSetClipRectangles( flx->textdraw, 0, 0,
                                  clip_rect + TEXT_CLIP, 1 );
        XftDrawSetClipRectangles( flx->bgdraw, 0, 0,
                                  clip_rect + TEXT_CLIP , 1 );
    }

    clipped_flags[ TEXT_CLIP ] = 1;
#else
    set_clipping( TEXT_CLIP, flx->textgc, x, y, w, h );
#endif
}


/***************************************
 * Function for switching off text clipping. Note that global clipping
 * will be retained.
 ***************************************/

void
fl_unset_text_clipping( void )
{
#if FL_ENABLE_XFT
    SET_RECT( clip_rect[ TEXT_CLIP ], 0, 0, 0, 0 );

    if ( clipped_flags[ GLOBAL_CLIP ] )
    {
        XftDrawSetClipRectangles( flx->textdraw, 0, 0,
                                  clip_rect + GLOBAL_CLIP, 1 );
        XftDrawSetClipRectangles( flx->bgdraw, 0, 0,
                                  clip_rect + GLOBAL_CLIP, 1 );
    }
    else
    {
        XRectangle r = { 0, 0, SHRT_MAX, SHRT_MAX };

        XftDrawSetClipRectangles( flx->textdraw, 0, 0, &r, 1 );
        XftDrawSetClipRectangles( flx->bgdraw, 0, 0, &r, 1 );
    }

#else
    unset_clipping( TEXT_CLIP, flx->textgc );
#endif

    clipped_flags[ TEXT_CLIP ] = 0;
}


/***************************************
 * Function for determining if text clipping is on and the region clipped
 * to. If 'include_global' is set also clipping due to global clipping is
 * taken into account (and the reported clipping region is the one resulting
 * from intersection of the text clipping region and the one for global
 * clipping). The function returns as its return value if there's clipping
 * going on (which may, when 'include_global', also be due to global clipping).
 ***************************************/

int
fl_get_text_clipping( int        include_global,
                      FL_COORD * x,
                      FL_COORD * y,
                      FL_COORD * w,
                      FL_COORD * h )
{
    return get_clipping( TEXT_CLIP, include_global, x, y, w, h );
}


/***************************************
 * Function for setting clipping for the specified GC. If global clipping is
 * on the rectangle set for clipping is the intersection of the rectangle
 * specified by the arguments and the one for global clipping. Specifying a
 * negative clipping width or height results in clipping becoming switched off.
 ***************************************/

void
fl_set_gc_clipping( GC       gc,
                    FL_COORD x,
                    FL_COORD y,
                    FL_COORD w,
                    FL_COORD h )
{
    clipped_flags[ GC_CLIP ] = 1;
    set_clipping( GC_CLIP, gc, x, y, w, h );
}


/***************************************
 * Function for switching of clipping for the given gc. Note that global
 * clipping will be retained.
 ***************************************/

void
fl_unset_gc_clipping( GC gc )
{
    clipped_flags[ GC_CLIP ] = 1;
    unset_clipping( GC_CLIP, gc );
}


/***************************************
 * Function that allows to set additional (always reducing the clipping
 * rectangle further) to already set clipping - the intersection of the
 * region and the already set clipping region is calculated and clipping
 * then set to the resulting rectangle.
 ***************************************/

void
fli_set_additional_clipping( FL_COORD x,
                             FL_COORD y,
                             FL_COORD w,
                             FL_COORD h )
{
    FL_RECT rect;

    SET_RECT( rect, x, y, w, h );

    if ( clipped_flags[ NORMAL_CLIP ] )
    {
        FL_RECT * r = fli_intersect_rects( clip_rect + NORMAL_CLIP, &rect );

        if ( r )
        {
            rect = *r;
            fli_safe_free( r );
        }
        else
            SET_RECT( rect, 0, 0, 0, 0 );
    }

    fl_set_clipping( rect.x, rect.y, rect.width, rect.height );
}


/***************************************
 * Sets the current global clipping to the GC passed to the function
 ***************************************/

void
fli_apply_clipping_to_gc( GC gc )
{
    if ( clipped_flags[ GLOBAL_CLIP ] && clipped_flags[ NORMAL_CLIP ] )
    {
        FL_RECT * r = fli_intersect_rects( clip_rect + GLOBAL_CLIP,
                                           clip_rect + NORMAL_CLIP );
        
        if ( r )
        {
            XSetClipRectangles( flx->display, gc, 0, 0, r, 1, Unsorted );
            fli_safe_free( r );
        }
        else
        {
            FL_RECT n = { 0, 0, 0, 0 };

            XSetClipRectangles( flx->display, gc, 0, 0, &n, 1, Unsorted );
        }
    }
    else if ( clipped_flags[ GLOBAL_CLIP ] )
        XSetClipRectangles( flx->display, gc, 0, 0,
                            clip_rect + GLOBAL_CLIP, 1, Unsorted );
    else if ( clipped_flags[ NORMAL_CLIP ] )
        XSetClipRectangles( flx->display, gc, 0, 0,
                            clip_rect + NORMAL_CLIP, 1, Unsorted );
    else
        XSetClipMask( flx->display, gc, None );
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
