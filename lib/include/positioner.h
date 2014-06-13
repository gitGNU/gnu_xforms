/*
 *  This file is part of the XForms library package.
 *
 *  XForms is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1, or
 *  (at your option) any later version.
 *
 *  XForms is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************** crop here for forms.h **********************/

/**
 *  Object Class: Positioner
 */

typedef enum {
    FL_NORMAL_POSITIONER,
    FL_OVERLAY_POSITIONER,
    FL_INVISIBLE_POSITIONER
} FL_POSITIONER_TYPE;

typedef int ( * FL_POSITIONER_VALIDATOR )( FL_OBJECT * obj,
                                           double      x,
                                           double      y,
                                           double    * x_repl,
                                           double    * y_repl );

#define FL_POSITIONER_INVALID  0
#define FL_POSITIONER_VALID    1
#define FL_POSITIONER_REPLACED 2

/***** Routines *****/

FL_EXPORT FL_OBJECT * fl_create_positioner( int          type,
                                            FL_Coord     x,
                                            FL_Coord     y,
                                            FL_Coord     w,
                                            FL_Coord     h,
                                            const char * label );

FL_EXPORT FL_OBJECT * fl_add_positioner( int          type,
                                         FL_Coord     x,
                                         FL_Coord     y,
                                         FL_Coord     w,
                                         FL_Coord     h,
                                         const char * label );

FL_EXPORT int fl_set_positioner_values( FL_OBJECT * obj,
                                        double      new_x,
                                        double      new_y );

FL_EXPORT int fl_set_positioner_xvalue( FL_OBJECT * ob,
                                        double      val );

FL_EXPORT double fl_get_positioner_xvalue( FL_OBJECT * ob );

FL_EXPORT void fl_set_positioner_xbounds( FL_OBJECT * ob,
                                          double      min,
                                          double      max );

FL_EXPORT void fl_get_positioner_xbounds( FL_OBJECT * ob,
                                          double    * min,
                                          double    * max );

FL_EXPORT int fl_set_positioner_yvalue( FL_OBJECT * ob,
                                        double      val );

FL_EXPORT double fl_get_positioner_yvalue( FL_OBJECT * ob );

FL_EXPORT void fl_set_positioner_ybounds( FL_OBJECT * ob,
                                          double      min,
                                          double      max );

FL_EXPORT void fl_get_positioner_ybounds( FL_OBJECT * ob,
                                          double    * min,
                                          double    * max );

FL_EXPORT void fl_set_positioner_xstep( FL_OBJECT * ob,
                                        double      value );

FL_EXPORT double fl_get_positioner_xstep( FL_OBJECT * ob );

FL_EXPORT void fl_set_positioner_ystep( FL_OBJECT * ob,
                                        double      value );

FL_EXPORT double fl_get_positioner_ystep( FL_OBJECT * ob );

FL_EXPORT void fl_set_positioner_return( FL_OBJECT    * ob,
                                         unsigned int   when );

FL_EXPORT unsigned int
    fl_set_positioner_mouse_buttons( FL_OBJECT    * obj,
                                     unsigned int   mouse_buttons );

FL_EXPORT void fl_get_positioner_mouse_buttons( FL_OBJECT    * obj,
                                                unsigned int * mouse_buttons );

FL_EXPORT int fl_get_positioner_numb( FL_OBJECT * obj );

FL_EXPORT FL_POSITIONER_VALIDATOR
fl_set_positioner_validator( FL_OBJECT               * obj,
                             FL_POSITIONER_VALIDATOR   validator );

FL_EXPORT void fl_reset_positioner( FL_OBJECT * obj );
