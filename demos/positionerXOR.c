/*
 *  This file is part of XForms.
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
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with XForms; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
 *  MA 02111-1307, USA.
 */


/*
 * This demo shows the use of a positioner with XOR drawmode, most
 * useful for overlaying positioner on top of other object
 *
 *  This file is part of xforms package
 *  T.C. Zhao and M. Overmars  1997
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "include/forms.h"

FL_OBJECT * xval,
          * yval;


/***************************************
 * Callback routine
 ***************************************/

void
positioner_cb( FL_OBJECT * obj,
               long        data   FL_UNUSED_ARG )
{
    char str[ 30 ];

    sprintf( str, "%f", fl_get_positioner_xvalue( obj ) );
    fl_set_object_label( xval, str );

    sprintf( str, "%f", fl_get_positioner_yvalue( obj ) );
    fl_set_object_label( yval, str );
}


/***************************************
 ***************************************/

int
main( int     argc,
      char  * argv[ ] )
{
    FL_FORM   * form;
    FL_OBJECT * pos,
              * obj;

    fl_set_border_width( -2 );
    fl_initialize( &argc, argv, "FormDemo", 0, 0 );

    form = fl_bgn_form( FL_UP_BOX, 350, 250 );

    obj = fl_add_pixmap( FL_NORMAL_PIXMAP, 60, 70, 100, 100, "" );
    fl_set_object_boxtype( obj, FL_DOWN_BOX );
    fl_set_pixmap_file( obj, "porsche.xpm" );

    pos = fl_add_positioner( FL_OVERLAY_POSITIONER, 60, 70, 100, 100, "" );
    fl_set_positioner_xbounds( pos, 0, 1 );
    fl_set_positioner_ybounds( pos, 0, 1 );
    fl_set_object_callback( pos, positioner_cb, 0 );

    xval = fl_add_box( FL_DOWN_BOX, 230, 40, 100, 30, "" );
    fl_set_object_color( xval, FL_COL1, FL_COL1 );

    yval = fl_add_box( FL_DOWN_BOX, 230, 90, 100, 30, "" );
    fl_set_object_color( yval, FL_COL1, FL_COL1 );

    fl_add_button( FL_NORMAL_BUTTON, 230, 200, 100, 30, "Exit" );

    fl_end_form( );

    fl_show_form( form, FL_PLACE_CENTER, FL_TRANSIENT, "XOR Positioner" );

    positioner_cb( pos, 0 );
    fl_do_forms( );
    fl_hide_form( form );
    fl_finish( );

    return 0;
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
