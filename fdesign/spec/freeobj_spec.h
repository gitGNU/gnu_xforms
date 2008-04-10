/** Header file generated by fdesign on Thu Apr 10 01:00:56 2008.**/

#ifndef FD_freeobjattrib_h_
#define FD_freeobjattrib_h_

/** Callbacks, globals and object handlers **/

extern void handler_name_change_cb( FL_OBJECT *, long );


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *freeobjattrib;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *background;
	FL_OBJECT *hname;
} FD_freeobjattrib;

extern FD_freeobjattrib * create_form_freeobjattrib(void);

#endif /* FD_freeobjattrib_h_ */
