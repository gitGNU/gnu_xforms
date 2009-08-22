/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "counter_spec.h"

FD_counterattrib *create_form_counterattrib(void)
{
  FL_OBJECT *obj;
  FD_counterattrib *fdui = (FD_counterattrib *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(-1);
  fdui->counterattrib = fl_bgn_form(FL_NO_BOX, 540, 280);
  fdui->background = obj = fl_add_box(FL_FLAT_BOX,0,0,540,280,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,25,20,485,235,"Counter Attributes");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->prec = obj = fl_add_counter(FL_NORMAL_COUNTER,60,150,145,23,"Precision");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,cnt_precision_cb,0);
  fdui->minval = obj = fl_add_input(FL_FLOAT_INPUT,340,52,92,24,"Counter Min");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,cnt_minmax_change,0);
  fdui->maxval = obj = fl_add_input(FL_FLOAT_INPUT,340,87,92,24,"Counter Max");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,cnt_minmax_change,0);
  fdui->returnsetting = obj = fl_add_menu(FL_PULLDOWN_MENU,61,70,138,24,"Return Setting");
    fl_set_object_boxtype(obj,FL_UP_BOX);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_callback(obj,cnt_returnsetting_change,0);
  fdui->initialval = obj = fl_add_input(FL_FLOAT_INPUT,340,122,92,24,"Initial Value");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,cnt_initialvalue_change,0);
  fdui->step1 = obj = fl_add_input(FL_FLOAT_INPUT,340,157,92,24,"Sml.Step Size");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,cnt_stepchange_cb,0);
  fdui->step2 = obj = fl_add_input(FL_FLOAT_INPUT,340,192,92,24,"Lg. Step Size");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,cnt_stepchange_cb,1);
  fl_end_form();

  fdui->counterattrib->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

