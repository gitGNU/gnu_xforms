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


#ifndef PBUTTON_H
#define PBUTTON_H


typedef struct {
    int            val;             /* state of button (on/off) */
    FL_Char        mousebut;        /* mouse button that caused the push     */
    int            timdel;          /* time since last touch (TOUCH buttons) */
    int            event;           /* what event triggers redraw            */
    int            is_pushed;       /* set while drawn as pushed down        */

    /* All of the following members are for pixmap and butmap buttons (or
       objects!) and actually should better be in a structure just for
       these... */

    Pixmap         pixmap,
                   mask;
    unsigned int   bits_w,
                   bits_h;
    char         * filename;
    Pixmap         focus_pixmap,
                   focus_mask;
    char         * focus_filename;
} FLI_BUTTON_SPEC;


extern void draw_normal_button( FL_OBJECT *,
								int );

/* normal button default */

#define FLI_BUTTON_BOXTYPE           FL_UP_BOX
#define FLI_BUTTON_COL1              FL_COL1
#define FLI_BUTTON_COL2              FL_COL1
#define FLI_BUTTON_LCOL              FL_LCOL
#define FLI_BUTTON_ALIGN             FL_ALIGN_CENTER
#define FLI_BUTTON_MCOL1             FL_MCOL
#define FLI_BUTTON_MCOL2             FL_MCOL


#endif /* PBUTTON_H */


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
