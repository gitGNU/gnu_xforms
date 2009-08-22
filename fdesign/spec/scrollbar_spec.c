/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "scrollbar_spec.h"

FD_scrollbarattrib *create_form_scrollbarattrib(void)
{
  FL_OBJECT *obj;
  FD_scrollbarattrib *fdui = (FD_scrollbarattrib *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(-1);
  fdui->scrollbarattrib = fl_bgn_form(FL_NO_BOX, 525, 280);
  fdui->background = obj = fl_add_box(FL_FLAT_BOX,0,0,525,280,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,20,15,470,245,"Scrollbar Attributes");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->minval = obj = fl_add_input(FL_FLOAT_INPUT,190,35,82,25,"Value at bottom/left");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,scb_minmax_change,0);
  fdui->maxval = obj = fl_add_input(FL_FLOAT_INPUT,190,64,82,25,"Value at top/right");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,scb_minmax_change,0);
  fdui->returnsetting = obj = fl_add_menu(FL_PULLDOWN_MENU,310,110,143,24,"Return Setting");
    fl_set_object_boxtype(obj,FL_UP_BOX);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_callback(obj,scb_returnsetting_change,0);
  fdui->initial_val = obj = fl_add_input(FL_FLOAT_INPUT,190,94,82,25,"Initial Value");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,scb_initialvalue_change,0);
  fdui->slsize = obj = fl_add_input(FL_FLOAT_INPUT,190,123,82,26,"Scrollbar Size");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,scb_slsize_change,0);
  fdui->step = obj = fl_add_input(FL_FLOAT_INPUT,190,154,82,27,"Step Size");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,scb_step_change,0);
  fdui->ldelta = obj = fl_add_input(FL_FLOAT_INPUT,191,185,82,25,"Leftmouse  Increment");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,scb_delta_change,0);
  fdui->rdelta = obj = fl_add_input(FL_FLOAT_INPUT,191,215,82,25,"Rightmouse Increment");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,scb_delta_change,0);
  fl_end_form();

  fdui->scrollbarattrib->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

