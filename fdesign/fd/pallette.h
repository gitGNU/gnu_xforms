/** Header file generated by fdesign on Thu Apr 10 01:19:06 2008.**/

#ifndef FD_pmain_h_
#define FD_pmain_h_

/** Callbacks, globals and object handlers **/

extern void dismiss_pallete( FL_OBJECT *, long );

extern void pallette_entry_callback( FL_OBJECT *, long );

extern void pallette_entry_callback( FL_OBJECT *, long );

extern void pallette_entry_callback( FL_OBJECT *, long );

extern void pallette_entry_callback( FL_OBJECT *, long );

extern void pallette_entry_callback( FL_OBJECT *, long );


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *pmain;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *folder;
} FD_pmain;

extern FD_pmain * create_form_pmain(void);
typedef struct {
	FL_FORM *buttform;
	void *vdata;
	char *cdata;
	long  ldata;
} FD_buttform;

extern FD_buttform * create_form_buttform(void);
typedef struct {
	FL_FORM *staticform;
	void *vdata;
	char *cdata;
	long  ldata;
} FD_staticform;

extern FD_staticform * create_form_staticform(void);
typedef struct {
	FL_FORM *valuatorform;
	void *vdata;
	char *cdata;
	long  ldata;
} FD_valuatorform;

extern FD_valuatorform * create_form_valuatorform(void);
typedef struct {
	FL_FORM *choiceform;
	void *vdata;
	char *cdata;
	long  ldata;
} FD_choiceform;

extern FD_choiceform * create_form_choiceform(void);
typedef struct {
	FL_FORM *miscform;
	void *vdata;
	char *cdata;
	long  ldata;
} FD_miscform;

extern FD_miscform * create_form_miscform(void);

#endif /* FD_pmain_h_ */
