/** Header file generated with fdesign on Sat Aug 22 23:46:12 2009.**/

#ifndef FD_scrollbarattrib_h_
#define FD_scrollbarattrib_h_

/** Callbacks, globals and object handlers **/
extern void scb_minmax_change(FL_OBJECT *, long);
extern void scb_returnsetting_change(FL_OBJECT *, long);
extern void scb_initialvalue_change(FL_OBJECT *, long);
extern void scb_slsize_change(FL_OBJECT *, long);
extern void scb_step_change(FL_OBJECT *, long);
extern void scb_delta_change(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *scrollbarattrib;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *background;
	FL_OBJECT *minval;
	FL_OBJECT *maxval;
	FL_OBJECT *returnsetting;
	FL_OBJECT *initial_val;
	FL_OBJECT *slsize;
	FL_OBJECT *step;
	FL_OBJECT *ldelta;
	FL_OBJECT *rdelta;
} FD_scrollbarattrib;

extern FD_scrollbarattrib * create_form_scrollbarattrib(void);

#endif /* FD_scrollbarattrib_h_ */
