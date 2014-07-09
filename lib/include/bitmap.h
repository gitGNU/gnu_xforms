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
 *   Object Class: Bitmap
 */

typedef enum {
	FL_NORMAL_BITMAP
} FL_BITMAP_TYPE;

/***** Routines *****/

FL_EXPORT FL_OBJECT * fl_create_bitmap( int          type,
                                        FL_COORD     x,
                                        FL_COORD     y,
                                        FL_COORD     w,
                                        FL_COORD     h,
                                        const char * label );

FL_EXPORT FL_OBJECT * fl_add_bitmap( int          type,
                                     FL_COORD     x,
                                     FL_COORD     y,
                                     FL_COORD     w,
                                     FL_COORD     h,
                                     const char * label );

FL_EXPORT void fl_set_bitmap_data( FL_OBJECT     * ob,
                                   int             w,
                                   int             h,
                                   unsigned char * data );

FL_EXPORT void fl_set_bitmap_file( FL_OBJECT  * ob,
                                   const char * fname );

FL_EXPORT Pixmap fl_read_bitmapfile( Window         win,
                                     const char   * file,
                                     unsigned int * w,
                                     unsigned int * h,
                                     int          * hotx,
                                     int          * hoty );

FL_EXPORT Pixmap fl_create_from_bitmapdata( Window       win,
											const char * data,
											int          width,
											int          height );

/* for compatibility */

#define fl_set_bitmap_datafile    fl_set_bitmap_file
