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
 *  Object Class: Dial
 */

typedef enum {
    FL_NORMAL_DIAL,
    FL_LINE_DIAL,
    FL_FILL_DIAL
} FL_DIAL_TYPE;

enum {
    FL_DIAL_CW,
    FL_DIAL_CCW
};


/***** Routines *****/

FL_EXPORT FL_OBJECT * fl_create_dial( int          type,
                                      FL_Coord     x,
                                      FL_Coord     y,
                                      FL_Coord     w,
                                      FL_Coord     h,
                                      const char * label );

FL_EXPORT FL_OBJECT * fl_add_dial( int          type,
                                   FL_Coord     x,
                                   FL_Coord     y,
                                   FL_Coord     w,
                                   FL_Coord     h,
                                   const char * label );

FL_EXPORT void fl_set_dial_value( FL_OBJECT * obj,
                                  double      val );

FL_EXPORT double fl_get_dial_value( FL_OBJECT * obj );

FL_EXPORT void fl_set_dial_bounds( FL_OBJECT * obj,
                                   double      min,
                                   double      max );

FL_EXPORT void fl_get_dial_bounds( FL_OBJECT * obj,
                                   double    * min,
                                   double    * max );

FL_EXPORT void fl_set_dial_step( FL_OBJECT * obj,
                                 double      value );

FL_EXPORT double fl_get_dial_step( FL_OBJECT * obj );

FL_EXPORT void fl_set_dial_return( FL_OBJECT    * obj,
                                   unsigned int   value );

FL_EXPORT void fl_set_dial_angles( FL_OBJECT * obj,
                                   double      amin,
                                   double      amax );

FL_EXPORT void fl_get_dial_angles( FL_OBJECT * obj,
                                   double    * amin,
                                   double    * amax );

FL_EXPORT void fl_set_dial_cross( FL_OBJECT * obj,
                                  int         flag );

#define fl_set_dial_crossover  fl_set_dial_cross

FL_EXPORT void fl_set_dial_direction( FL_OBJECT * obj,
                                      int         dir );

FL_EXPORT int fl_get_dial_direction( FL_OBJECT * obj );
