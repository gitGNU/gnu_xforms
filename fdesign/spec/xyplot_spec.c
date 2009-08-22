/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "xyplot_spec.h"

FD_xyplotattrib *create_form_xyplotattrib(void)
{
  FL_OBJECT *obj;
  FD_xyplotattrib *fdui = (FD_xyplotattrib *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(-1);
  fdui->xyplotattrib = fl_bgn_form(FL_NO_BOX, 520, 270);
  obj = fl_add_box(FL_FLAT_BOX,0,0,520,270,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,10,20,500,240,"XYPlot Attributes");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->xgrid = obj = fl_add_choice(FL_NORMAL_CHOICE2,70,60,85,23,"XGrid");
    fl_set_object_callback(obj,grid_change_cb,0);
  fdui->ygrid = obj = fl_add_choice(FL_NORMAL_CHOICE2,70,90,85,23,"YGrid");
    fl_set_object_callback(obj,grid_change_cb,0);
  fdui->xscale = obj = fl_add_choice(FL_NORMAL_CHOICE2,70,170,85,23,"XScale");
    fl_set_object_callback(obj,xscale_change_cb,0);
  fdui->yscale = obj = fl_add_choice(FL_NORMAL_CHOICE2,70,210,85,23,"YScale");
    fl_set_object_callback(obj,yscale_change_cb,0);
  fdui->ymajor = obj = fl_add_counter(FL_NORMAL_COUNTER,370,160,115,20,"YMajor");
    fl_set_object_callback(obj,ymajorminor_change_cb,0);
    fl_set_counter_precision(obj, 0);
    fl_set_counter_bounds(obj, -1, 20);
    fl_set_counter_value(obj, 1);
    fl_set_counter_step(obj, 1, 2);
  fdui->yminor = obj = fl_add_counter(FL_NORMAL_COUNTER,370,200,115,20,"YMinor");
    fl_set_object_callback(obj,ymajorminor_change_cb,0);
    fl_set_counter_precision(obj, 0);
    fl_set_counter_bounds(obj, 0, 10);
    fl_set_counter_value(obj, 1);
    fl_set_counter_step(obj, 1, 2);
  fdui->xmajor = obj = fl_add_counter(FL_NORMAL_COUNTER,370,70,115,20,"XMajor");
    fl_set_object_callback(obj,xmajorminor_change_cb,0);
    fl_set_counter_precision(obj, 0);
    fl_set_counter_bounds(obj, -1, 20);
    fl_set_counter_value(obj, 1);
    fl_set_counter_step(obj, 1, 2);
  fdui->xminor = obj = fl_add_counter(FL_NORMAL_COUNTER,370,110,115,20,"XMinor");
    fl_set_object_callback(obj,xmajorminor_change_cb,0);
    fl_set_counter_precision(obj, 0);
    fl_set_counter_bounds(obj, 0, 10);
    fl_set_counter_value(obj, 1);
    fl_set_counter_step(obj, 1, 2);
  fdui->xbase = obj = fl_add_input(FL_FLOAT_INPUT,160,170,39,23,"xbase");
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,xscale_change_cb,0);
  fdui->ybase = obj = fl_add_input(FL_FLOAT_INPUT,158,210,39,23,"ybase");
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,yscale_change_cb,0);
  fdui->how_return = obj = fl_add_menu(FL_PULLDOWN_MENU,193,29,150,22,"Return Setting");
    fl_set_object_boxtype(obj,FL_UP_BOX);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_callback(obj,xyplot_returnsetting_change,0);
  fdui->gridstyle = obj = fl_add_choice(FL_NORMAL_CHOICE2,70,120,85,23,"Style");
    fl_set_object_callback(obj,gridstyle_change_cb,0);
  fdui->mark_active = obj = fl_add_checkbutton(FL_PUSH_BUTTON,230,80,100,25,"MarkActive");
    fl_set_object_callback(obj,markactive_change_cb,0);
  fl_end_form();

  fdui->xyplotattrib->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

