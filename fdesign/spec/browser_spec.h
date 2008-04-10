/** Header file generated by fdesign on Thu Apr 10 01:00:56 2008.**/

#ifndef FD_browserattrib_h_
#define FD_browserattrib_h_

/** Callbacks, globals and object handlers **/

extern void add_item_cb( FL_OBJECT *, long );
extern void delete_item_cb( FL_OBJECT *, long );
extern void insert_cb( FL_OBJECT *, long );
extern void hscb_pref_cb( FL_OBJECT *, long );
extern void vscb_pref_cb( FL_OBJECT *, long );
extern void clear_field_cb( FL_OBJECT *, long );
extern void replace_item_cb( FL_OBJECT *, long );


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *browserattrib;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *content_br;
	FL_OBJECT *input;
	FL_OBJECT *hscb_pref;
	FL_OBJECT *vscb_pref;
	FL_OBJECT *auto_clear;
} FD_browserattrib;

extern FD_browserattrib * create_form_browserattrib(void);

#endif /* FD_browserattrib_h_ */
