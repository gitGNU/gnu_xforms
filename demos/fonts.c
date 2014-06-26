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
 * Demo, showing the different fonts in different sizes.
 * M. Overmars and T.C. Zhao
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include "include/forms.h"

typedef struct {
    FL_FORM   * fontsform;
    void      * vdata;
    char      * cdata;
    long        ldata;
    FL_OBJECT * fontobj;
    FL_OBJECT * sizeobj;
    FL_OBJECT * textobj;
} FD_fontsform;

extern FD_fontsform *create_form_fontsform( void );

FD_fontsform *ui;


/***************************************
 ***************************************/

void
done_cb( FL_OBJECT * obj  FL_UNUSED_ARG,
         long        arg  FL_UNUSED_ARG )
{
    fl_finish( );
    exit( 0 );
}


/***************************************
 ***************************************/

void
style_cb( FL_OBJECT * obj,
          long        arg  FL_UNUSED_ARG )
{
    fl_set_object_lstyle( ui->textobj, fl_get_browser( obj ) - 1 );
}


/***************************************
 ***************************************/

void
size_cb( FL_OBJECT * obj,
         long        arg  FL_UNUSED_ARG )
{
#if FL_ENABLE_XFT
    fl_set_object_lsize( ui->textobj, fl_get_spinner_value( obj ) );
#else
    int sizes[ ] = { 8, 10, 11, 12, 13, 14, 18, 24, 30 };

    fl_set_object_lsize( ui->textobj, sizes[ fl_get_browser( obj ) - 1 ] );
#endif
}


/***************************************
 ***************************************/

void
addit( const char * str )
{
    fl_add_browser_line( ui->fontobj, str );
}


/***************************************
 ***************************************/

int
main( int    argc,
      char * argv[ ] )
{
    fl_initialize( &argc, argv, "FormDemo", 0, 0 );
    ui = create_form_fontsform( );
    fl_scale_form( ui->fontsform, 1.1, 1.2 );
    fl_set_object_dblbuffer( ui->textobj, 1 );
    fl_set_object_bw( ui->textobj, 5 );

    fl_enumerate_fonts( addit, 1 );
    fl_select_browser_line( ui->fontobj, 1 );
#if FL_ENABLE_XFT
    fl_set_spinner_value( ui->sizeobj, 10 );
#else
    fl_addto_browser( ui->sizeobj, "8  (FL_TINY_SIZE)" );
    fl_addto_browser( ui->sizeobj, "10 (FL_SMALL_SIZE)" );
    fl_addto_browser( ui->sizeobj, "11" );
    fl_addto_browser( ui->sizeobj, "12 (FL_NORMAL_SIZE)" );
    fl_addto_browser( ui->sizeobj, "13" );
    fl_addto_browser( ui->sizeobj, "14 (FL_MEDIUM_SIZE)" );
    fl_addto_browser( ui->sizeobj, "18 (FL_LARGE_SIZE)" );
    fl_addto_browser( ui->sizeobj, "24 (FL_HUGE_SIZE)" );
    fl_addto_browser( ui->sizeobj, "30" );
    fl_select_browser_line( ui->sizeobj, 2 );
#endif
    fl_set_object_lstyle( ui->textobj, FL_NORMAL_STYLE );
    fl_call_object_callback( ui->fontobj );
    fl_call_object_callback( ui->sizeobj );
    fl_show_form( ui->fontsform, FL_PLACE_CENTER, FL_TRANSIENT, "Fonts" );

    fl_do_forms( );

    return 0;
}


/***************************************
 ***************************************/

FD_fontsform *
create_form_fontsform( void )
{
    FL_OBJECT *obj;
    FD_fontsform *fdui = fl_calloc( 1, sizeof *fdui );

    fdui->fontsform = fl_bgn_form( FL_NO_BOX, 371, 296 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 371, 296, "" );
    fl_set_object_color( obj, FL_SLATEBLUE, FL_COL1 );

#if FL_ENABLE_XFT
    fdui->fontobj = obj = fl_add_browser( FL_HOLD_BROWSER, 10, 145, 240, 135,
                                          "" );
#else
    fdui->fontobj = obj = fl_add_browser( FL_HOLD_BROWSER, 10, 145, 195, 135,
                                          "" );
#endif
    fl_set_object_lalign( obj, fl_to_inside_lalign( FL_ALIGN_BOTTOM ) );
    fl_set_object_callback( obj, style_cb, 0 );

#if FL_ENABLE_XFT
    fdui->sizeobj = obj = fl_add_spinner( FL_INT_SPINNER, 270, 200, 85, 30,
                                          "" );
    fl_set_spinner_bounds( obj, 1, 100 );
#else
    fdui->sizeobj = obj = fl_add_browser( FL_HOLD_BROWSER, 215, 145, 145, 135,
                                          "" );
    fl_set_object_lalign( obj, fl_to_inside_lalign( FL_ALIGN_BOTTOM ) );
#endif
    fl_set_object_callback( obj, size_cb, 0 );

    fdui->textobj = obj = fl_add_text( FL_NORMAL_TEXT, 10, 5, 351, 125,
                                       "The quick brown\nfox jumps over\n"
                                       "the lazy dog." );
    fl_set_object_boxtype( obj, FL_FRAME_BOX );
    fl_set_object_lalign( obj, FL_ALIGN_CENTER );

    obj = fl_add_button( FL_HIDDEN_BUTTON, 0, 0, 370, 140, "Button" );
    fl_set_button_shortcut( obj, "^[qQ", 1 );
    fl_set_object_callback( obj, done_cb, 0 );

    fl_end_form( );

    return fdui;
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
