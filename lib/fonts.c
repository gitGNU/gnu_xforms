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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "include/forms.h"
#include "flinternal.h"

/* These default X11 bitmap fonts may not be the most beautiful X11 fonts
   available on the system but they are part of X11 distributions since at
   least 20 years, so we can be rather sure that they're available everywhere.
   (And, remember, these fonts must be available on the machine where the X
   server is running on, which is not necessarily the machine where the
   program using XForms is executed.) */

#if ! defined X_HAVE_UTF8_STRING
static const char * bitmap_fonts[ ] =
{
    "-*-helvetica-medium-r-*-*-*-?-*-*-p-*-*-*",
    "-*-helvetica-bold-r-*-*-*-?-*-*-p-*-*-*",
    "-*-helvetica-medium-o-*-*-*-?-*-*-p-*-*-*",
    "-*-helvetica-bold-o-*-*-*-?-*-*-p-*-*-*",

    "-*-courier-medium-r-*-*-*-?-*-*-*-*-*-*",
    "-*-courier-bold-r-*-*-*-?-*-*-*-*-*-*",
    "-*-courier-medium-o-*-*-*-?-*-*-*-*-*-*",
    "-*-courier-bold-o-*-*-*-?-*-*-*-*-*-*",

    "-*-times-medium-r-*-*-*-?-*-*-p-*-*-*",
    "-*-times-bold-r-*-*-*-?-*-*-p-*-*-*",
    "-*-times-medium-i-*-*-*-?-*-*-p-*-*-*",
    "-*-times-bold-i-*-*-*-?-*-*-p-*-*-*",

    "-*-charter-medium-r-*-*-*-?-*-*-*-*-*-*",
    "-*-charter-bold-r-*-*-*-?-*-*-*-*-*-*",
    "-*-charter-medium-i-*-*-*-?-*-*-*-*-*-*",
    "-*-charter-bold-i-*-*-*-?-*-*-*-*-*-*",

    NULL
};
#else
static const char * bitmap_fonts[ ] =
{
    "-*-helvetica-medium-r-*-*-*-?-*-*-p-*-iso10646-1",
    "-*-helvetica-bold-r-*-*-*-?-*-*-p-*-iso10646-1",
    "-*-helvetica-medium-o-*-*-*-?-*-*-p-*-iso10646-1",
    "-*-helvetica-bold-o-*-*-*-?-*-*-p-*-iso10646-1",

    "-*-courier-medium-r-*-*-*-?-*-*-*-*-iso10646-1",
    "-*-courier-bold-r-*-*-*-?-*-*-*-*-iso10646-1",
    "-*-courier-medium-o-*-*-*-?-*-*-*-*-iso10646-1",
    "-*-courier-bold-o-*-*-*-?-*-*-*-*-iso10646-1",

    "-*-times-medium-r-*-*-*-?-*-*-p-*-iso10646-1",
    "-*-times-bold-r-*-*-*-?-*-*-p-*-iso10646-1",
    "-*-times-medium-i-*-*-*-?-*-*-p-*-iso10646-1",
    "-*-times-bold-i-*-*-*-?-*-*-p-*-iso10646-1",

    "-*-charter-medium-r-*-*-*-?-*-*-*-*-iso10646-1",
    "-*-charter-bold-r-*-*-*-?-*-*-*-*-iso10646-1",
    "-*-charter-medium-i-*-*-*-?-*-*-*-*-iso10646-1",
    "-*-charter-bold-i-*-*-*-?-*-*-*-*-iso10646-1",

    NULL
};
#endif


static const char * cv_fname( const char *f );


/* Include different sources if true type font support is enabled
   or just X11 fonts are to be used */

#if FL_ENABLE_XFT
#include "ttfonts.cx"
#include "font_utils.cx"
#else
#include "xfonts.cx"
#endif


/***************************************
 * Convert X font names to more conventional names by stripping the
 * auxiliary info.
 ***************************************/

static const char *
cv_fname( const char *f )
{
    static char * fname = NULL;
    char *q,
         *p;

    if ( fname )
        fl_free( fname );
    fname = fl_strdup( f );

    /* Remove all the garbages from head */

    for ( q = strcpy( fname, f ); *q && ! isalnum( ( unsigned char ) *q ); q++ )
        /* empty */ ;

    /* Remove all the garbage from the end, starting from '?' */

    if ( ( p = strchr( fname, '?' ) ) )
        *--p = '\0';

    /* Remove all remaining garbages */

    for ( p = fname + strlen( fname ) - 1;
          p > q && ! isalnum( ( unsigned char ) *p ); p-- )
        /* empty */ ;

    *++p = '\0';

    return q;
}


/***************************************
 * Some compatibility stuff, i.e. functions that were never documented
 * and were removed from V0.89, but apparently this broke some applications
 * that were using them. Put back in 10/22/00.
 ***************************************/

int
fl_fdesc_( void )
{
    return flx->fdesc;
}


/***************************************
 ***************************************/

int
fl_fheight_( void )
{
    return flx->fheight;
}


/***************************************
 ***************************************/

GC
fl_gc_( void )
{
    return flx->gc;
}


/***************************************
 ***************************************/

Window
fl_cur_win_( void )
{
    return flx->win;
}


/***************************************
 ***************************************/

#if FL_ENABLE_XFT
XftFont *
#else
XFontStruct *
#endif
fl_cur_fs_( void )
{
    return flx->fs;
}


/***************************************
 ***************************************/

Display *
fl_display_( void )
{
    return flx->display;
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
