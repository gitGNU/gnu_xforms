/* Header file generated by fdesign on Sat Sep 17 15:12:01 2016 */

#ifndef FD_posattrib_h_
#define FD_posattrib_h_

#include  "../lib/include/forms.h" 

#if defined __cplusplus
extern "C"
{
#endif

/* Callbacks, globals and object handlers */

void pos_returnsetting_change( FL_OBJECT *, long );
void pos_xminmax_change( FL_OBJECT *, long );
void pos_initialxvalue_change( FL_OBJECT *, long );
void pos_xstepchange_cb( FL_OBJECT *, long );
void pos_yminmax_change( FL_OBJECT *, long );
void pos_initialyvalue_change( FL_OBJECT *, long );
void pos_ystepchange_cb( FL_OBJECT *, long );


/* Forms and Objects */

typedef struct {
    FL_FORM   * posattrib;
    void      * vdata;
    char      * cdata;
    long        ldata;
    FL_OBJECT * background;
    FL_OBJECT * returnsetting;
    FL_OBJECT * xminval;
    FL_OBJECT * xmaxval;
    FL_OBJECT * initialxval;
    FL_OBJECT * xstep;
    FL_OBJECT * yminval;
    FL_OBJECT * ymaxval;
    FL_OBJECT * initialyval;
    FL_OBJECT * ystep;
} FD_posattrib;

FD_posattrib * create_form_posattrib( void );

#if defined __cplusplus
}
#endif

#endif /* FD_posattrib_h_ */
