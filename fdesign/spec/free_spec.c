/* Form definition file generated by fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "free_spec.h"



/***************************************
 ***************************************/

FD_buttonattrib *
create_form_buttonattrib( void )
{
    FL_OBJECT *obj;
    FD_buttonattrib *fdui = fl_malloc( sizeof *fdui );

    int old_bw = fl_get_border_width( );

    fl_set_border_width( -2 );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->buttonattrib = fl_bgn_form( FL_NO_BOX, 540, 280 );

    obj = fl_add_box( FL_NO_BOX, 0, 0, 540, 280, "" );

    fdui->background = obj = fl_add_box( FL_FLAT_BOX, 0, 0, 540, 280, "" );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 20, 20, 490, 240, "Button Attributes" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );
    fl_set_object_lstyle( obj, FL_TIMESBOLD_STYLE );

    fdui->initialval = obj = fl_add_checkbutton( FL_PUSH_BUTTON, 170, 55, 30, 30, "Initially Set" );
    fl_set_object_lalign( obj, FL_ALIGN_LEFT );
    fl_set_object_callback( obj, initialval_change, 0 );

    fdui->filename = obj = fl_add_input( FL_NORMAL_INPUT, 160, 113, 210, 25, "Bitmap/Pixmap File" );
    fl_set_object_callback( obj, iconbutton_filename_change, 0 );

    fdui->pixalign = obj = fl_add_choice( FL_NORMAL_CHOICE2, 282, 60, 155, 25, "" );
    fl_set_object_callback( obj, pixalign_change, 0 );

    fdui->showfocus = obj = fl_add_checkbutton( FL_PUSH_BUTTON, 170, 80, 30, 30, "Show Focus" );
    fl_set_object_lalign( obj, FL_ALIGN_LEFT );
    fl_set_object_callback( obj, showfocus_change, 0 );
    fl_set_button( obj, 1 );

    fdui->browse = obj = fl_add_button( FL_NORMAL_BUTTON, 372, 113, 66, 25, "Browse ...." );
    fl_set_button_shortcut( obj, "#B", 1 );
    fl_set_object_callback( obj, lookfor_pixmapfile_cb, 0 );

    fdui->use_data = obj = fl_add_checkbutton( FL_PUSH_BUTTON, 175, 155, 30, 30, "Use Data" );
    fl_set_object_lalign( obj, FL_ALIGN_LEFT );
    fl_set_object_callback( obj, usedata_change, 0 );

    fdui->fullpath = obj = fl_add_checkbutton( FL_PUSH_BUTTON, 275, 155, 30, 30, "Full Path" );
    fl_set_object_lalign( obj, FL_ALIGN_LEFT );
    fl_set_object_callback( obj, fullpath_cb, 0 );
    fl_end_form( );

    fdui->buttonattrib->fdui = fdui;
    fl_set_border_width( old_bw );

    return fdui;
}
