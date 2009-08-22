/** Header file generated with fdesign on Sat Aug 22 23:45:21 2009.**/

#ifndef FD_counterattrib_h_
#define FD_counterattrib_h_

/** Callbacks, globals and object handlers **/
extern void cnt_precision_cb(FL_OBJECT *, long);
extern void cnt_minmax_change(FL_OBJECT *, long);
extern void cnt_returnsetting_change(FL_OBJECT *, long);
extern void cnt_initialvalue_change(FL_OBJECT *, long);
extern void cnt_stepchange_cb(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *counterattrib;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *background;
	FL_OBJECT *prec;
	FL_OBJECT *minval;
	FL_OBJECT *maxval;
	FL_OBJECT *returnsetting;
	FL_OBJECT *initialval;
	FL_OBJECT *step1;
	FL_OBJECT *step2;
} FD_counterattrib;

extern FD_counterattrib * create_form_counterattrib(void);

#endif /* FD_counterattrib_h_ */
