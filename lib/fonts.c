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


/* Include different sources if true type font support is enabled
   or just X11 fonts are to be used */

#if defined ENABLE_XFT
#include "ttfonts.cx"
#include "font_utils.cx"
#else
#include "xfonts.cx"
#endif



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

#if defined ENABLE_XFT
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
