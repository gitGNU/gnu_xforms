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
 *  Object Class: Text
 */

typedef enum {
    FL_NORMAL_TEXT
} FL_TEXT_TYPE;

FL_EXPORT FL_OBJECT * fl_create_text( int          type,
                                      FL_COORD     x,
                                      FL_COORD     y,
                                      FL_COORD     w,
                                      FL_COORD     h,
                                      const char * label );

FL_EXPORT FL_OBJECT * fl_add_text( int          type,
                                   FL_COORD     x,
                                   FL_COORD     y,
                                   FL_COORD     w,
                                   FL_COORD     h,
                                   const char * label );
