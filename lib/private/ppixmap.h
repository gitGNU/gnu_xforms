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
 *  You should have received a copy of the GNU General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef PPIXMAP_H
#define PPIXMAP_H

#include "private/pbutton.h"
#include XPM_H_LOCATION


typedef struct {
    XpmAttributes * xpma;
    GC              gc;
    int             align;
    int             dx,
                    dy;
    int             show_focus;
    unsigned int    focus_w,    /* these should be in button_spec?b */
                    focus_h;
} FLI_PIXMAP_SPEC;


/* Pixmap defaults */

#define FLI_PIXMAP_BOXTYPE   FL_NO_BOX
#define FLI_PIXMAP_COL1      FL_COL1     /* background of bitmap */
#define FLI_PIXMAP_COL2      FL_COL1     /* not used currently   */
#define FLI_PIXMAP_LCOL      FL_LCOL     /* foreground of bitmap */
#define FLI_PIXMAP_ALIGN     FL_ALIGN_BOTTOM

/* Pixmap button defaults */

#define FLI_PIXMAPBUTTON_BOXTYPE     FL_UP_BOX
#define FLI_PIXMAPBUTTON_COL1        FLI_BUTTON_COL1
#define FLI_PIXMAPBUTTON_COL2        FLI_BUTTON_COL2
#define FLI_PIXMAPBUTTON_LCOL        FLI_BUTTON_LCOL
#define FLI_PIXMAPBUTTON_MCOL1       FLI_BUTTON_MCOL1
#define FLI_PIXMAPBUTTON_MCOL2       FLI_BUTTON_MCOL2
#define FLI_PIXMAPBUTTON_ALIGN       FL_ALIGN_BOTTOM


#endif /* PPIXMAP_H */


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
