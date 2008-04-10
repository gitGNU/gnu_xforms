/* Form definition file generated by fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "freeobj_spec.h"



/***************************************
 ***************************************/

FD_freeobjattrib *
create_form_freeobjattrib( void )
{
    FL_OBJECT *obj;
    FD_freeobjattrib *fdui = fl_malloc( sizeof *fdui );



    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->freeobjattrib = fl_bgn_form( FL_NO_BOX, 540, 280 );

    obj = fl_add_box( FL_NO_BOX, 0, 0, 540, 280, "" );

    fdui->background = obj = fl_add_box( FL_FLAT_BOX, 0, 0, 540, 280, "" );

    obj = fl_add_labelframe( FL_ENGRAVED_FRAME, 20, 20, 490, 240, "Free Object Attributes" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE );

    fdui->hname = obj = fl_add_input( FL_NORMAL_INPUT, 145, 110, 280, 30, "Freeobject Handler" );
    fl_set_object_callback( obj, handler_name_change_cb, 0 );
    fl_end_form( );

    fdui->freeobjattrib->fdui = fdui;

    return fdui;
}
