/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "positioner_spec.h"

FD_posattrib *create_form_posattrib(void)
{
  FL_OBJECT *obj;
  FD_posattrib *fdui = (FD_posattrib *) fl_calloc(1, sizeof(*fdui));
  int old_bw = fl_get_border_width();

  fl_set_border_width(-1);
  fdui->posattrib = fl_bgn_form(FL_NO_BOX, 540, 280);
  fdui->background = obj = fl_add_box(FL_FLAT_BOX,0,0,540,280,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,30,20,475,235,"Positioner Attributes");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_TIMESBOLD_STYLE);
  fdui->returnsetting = obj = fl_add_menu(FL_PULLDOWN_MENU,195,55,138,24,"Return Setting");
    fl_set_object_boxtype(obj,FL_UP_BOX);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_callback(obj,pos_returnsetting_change,0);
  fdui->xminval = obj = fl_add_input(FL_FLOAT_INPUT,170,95,92,24,"Xmin(left)");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,pos_xminmax_change,0);
  fdui->xmaxval = obj = fl_add_input(FL_FLOAT_INPUT,170,131,92,24,"Xmax(right)");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,pos_xminmax_change,0);
  fdui->initialxval = obj = fl_add_input(FL_FLOAT_INPUT,170,168,92,24,"Initial XValue");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,pos_initialxvalue_change,0);
  fdui->xstep = obj = fl_add_input(FL_FLOAT_INPUT,170,205,92,24,"X StepSize");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,pos_xstepchange_cb,0);
  fdui->yminval = obj = fl_add_input(FL_FLOAT_INPUT,275,95,92,24,"Ymin(bottom)");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,pos_yminmax_change,0);
  fdui->ymaxval = obj = fl_add_input(FL_FLOAT_INPUT,275,131,92,24,"Ymax(top)");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,pos_yminmax_change,0);
  fdui->initialyval = obj = fl_add_input(FL_FLOAT_INPUT,275,168,92,24,"Initial YValue");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,pos_initialyvalue_change,0);
  fdui->ystep = obj = fl_add_input(FL_FLOAT_INPUT,275,205,92,24,"YStepSize");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_callback(obj,pos_ystepchange_cb,0);
  fl_end_form();

  fdui->posattrib->fdui = fdui;
  fl_set_border_width(old_bw);

  return fdui;
}
/*---------------------------------------*/

