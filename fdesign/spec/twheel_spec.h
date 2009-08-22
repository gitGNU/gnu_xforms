/** Header file generated with fdesign on Sat Aug 22 23:46:53 2009.**/

#ifndef FD_twheelattrib_h_
#define FD_twheelattrib_h_

/** Callbacks, globals and object handlers **/
extern void twheel_adjust_precision(FL_OBJECT *, long);
extern void twheel_minmax_change(FL_OBJECT *, long);
extern void twheel_returnsetting_change(FL_OBJECT *, long);
extern void twheel_initialvalue_change(FL_OBJECT *, long);
extern void twheel_step_change(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *twheelattrib;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *background;
	FL_OBJECT *prec;
	FL_OBJECT *minval;
	FL_OBJECT *maxval;
	FL_OBJECT *returnsetting;
	FL_OBJECT *initial_val;
	FL_OBJECT *step;
} FD_twheelattrib;

extern FD_twheelattrib * create_form_twheelattrib(void);

#endif /* FD_twheelattrib_h_ */
