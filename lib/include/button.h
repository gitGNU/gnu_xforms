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
 *  Object Class: Button
 */

typedef enum {
    FL_NORMAL_BUTTON,
    FL_PUSH_BUTTON,
    FL_RADIO_BUTTON,
    FL_HIDDEN_BUTTON,
    FL_TOUCH_BUTTON,
    FL_INOUT_BUTTON,
    FL_RETURN_BUTTON,
    FL_HIDDEN_RET_BUTTON,
    FL_MENU_BUTTON
} FL_BUTTON_TYPE;

#define FL_TOGGLE_BUTTON    FL_PUSH_BUTTON

typedef void ( * FL_DrawButton )( FL_OBJECT *, int );
typedef void ( * FL_CleanupButton )( FL_OBJECT * );

/***** Routines *****/

FL_EXPORT FL_OBJECT * fl_create_button( int          type,
                                        FL_Coord     x,
                                        FL_Coord     y,
                                        FL_Coord     w,
                                        FL_Coord     h,
                                        const char * label );

FL_EXPORT FL_OBJECT * fl_create_roundbutton( int          type,
                                             FL_Coord     x,
                                             FL_Coord     y,
                                             FL_Coord     w,
                                             FL_Coord     h,
                                             const char * label );

FL_EXPORT FL_OBJECT * fl_create_round3dbutton( int        type,
                                               FL_Coord   x,
                                               FL_Coord   y,
                                               FL_Coord   w,
                                               FL_Coord   h,
                                               const char * label );

FL_EXPORT FL_OBJECT * fl_create_lightbutton( int          type,
                                             FL_Coord     x,
                                             FL_Coord     y,
                                             FL_Coord     w,
                                             FL_Coord     h,
                                             const char * label );

FL_EXPORT FL_OBJECT * fl_create_checkbutton( int          type,
                                             FL_Coord     x,
                                             FL_Coord     y,
                                             FL_Coord     w,
                                             FL_Coord     h,
                                             const char * label );

FL_EXPORT FL_OBJECT * fl_create_bitmapbutton( int          type,
                                              FL_Coord     x,
                                              FL_Coord     y,
                                              FL_Coord     w,
                                              FL_Coord     h,
                                              const char * label );

FL_EXPORT FL_OBJECT * fl_create_pixmapbutton( int          type,
                                              FL_Coord     x,
                                              FL_Coord     y,
                                              FL_Coord     w,
                                              FL_Coord     h,
                                              const char * label );

FL_EXPORT FL_OBJECT * fl_create_scrollbutton( int          type,
                                              FL_Coord     x,
                                              FL_Coord     y,
                                              FL_Coord     w,
                                              FL_Coord     h,
                                              const char * label );

FL_EXPORT FL_OBJECT * fl_create_labelbutton( int           type,
                                             FL_Coord      x,
                                             FL_Coord      y,
                                             FL_Coord      w,
                                             FL_Coord      h,
                                             const char * label );

FL_EXPORT FL_OBJECT *fl_add_roundbutton( int          type,
                                         FL_Coord     x,
                                         FL_Coord     y,
                                         FL_Coord     w,
                                         FL_Coord     h,
                                         const char * label );

FL_EXPORT FL_OBJECT * fl_add_round3dbutton( int          type,
                                            FL_Coord     x,
                                            FL_Coord     y,
                                            FL_Coord     w,
                                            FL_Coord     h,
                                            const char * label );

FL_EXPORT FL_OBJECT * fl_add_lightbutton( int         type,
                                          FL_Coord    x,
                                          FL_Coord    y,
                                          FL_Coord    w,
                                          FL_Coord    h,
                                          const char * label );

FL_EXPORT FL_OBJECT * fl_add_checkbutton( int          type,
                                          FL_Coord     x,
                                          FL_Coord     y,
                                          FL_Coord     w,
                                          FL_Coord     h,
                                          const char * label );

FL_EXPORT FL_OBJECT * fl_add_button( int          type,
                                     FL_Coord     x,
                                     FL_Coord     y,
                                     FL_Coord     w,
                                     FL_Coord     h,
                                     const char * label );

FL_EXPORT FL_OBJECT * fl_add_bitmapbutton( int          type,
                                           FL_Coord     x,
                                           FL_Coord     y,
                                           FL_Coord     w,
                                           FL_Coord     h,
                                           const char * label );

FL_EXPORT FL_OBJECT * fl_add_scrollbutton( int          type,
                                           FL_Coord     x,
                                           FL_Coord     y,
                                           FL_Coord     w,
                                           FL_Coord     h,
                                           const char * label );

FL_EXPORT FL_OBJECT * fl_add_labelbutton( int          type,
                                          FL_Coord     x,
                                          FL_Coord     y,
                                          FL_Coord     w,
                                          FL_Coord     h,
                                          const char * label );

#define fl_set_bitmapbutton_file  fl_set_bitmap_file

FL_EXPORT void fl_set_bitmapbutton_data( FL_OBJECT     * ob,
                                         int             w,
                                         int             h,
                                         unsigned char * bits );

#define fl_set_bitmapbutton_datafile  fl_set_bitmapbutton_file

FL_EXPORT FL_OBJECT * fl_add_pixmapbutton( int          type,
                                           FL_Coord     x,
                                           FL_Coord     y,
                                           FL_Coord     w,
                                           FL_Coord     h,
                                           const char * label );

#define fl_set_pixmapbutton_data       fl_set_pixmap_data
#define fl_set_pixmapbutton_file       fl_set_pixmap_file
#define fl_set_pixmapbutton_pixmap     fl_set_pixmap_pixmap
#define fl_get_pixmapbutton_pixmap     fl_get_pixmap_pixmap
#define fl_set_pixmapbutton_align      fl_set_pixmap_align
#define fl_free_pixmapbutton_pixmap    fl_free_pixmap_pixmap
#define fl_set_pixmapbutton_datafile   fl_set_pixmapbutton_file
#define fl_set_pixmapbutton_show_focus fl_set_pixmapbutton_focus_outline


FL_EXPORT void fl_set_pixmapbutton_focus_outline( FL_OBJECT * ob,
                                                  int         yes );

FL_EXPORT void fl_set_pixmapbutton_focus_data( FL_OBJECT  * ob,
                                               char      ** bits );

FL_EXPORT void fl_set_pixmapbutton_focus_file( FL_OBJECT  * ob,
                                               const char * fname );

FL_EXPORT void fl_set_pixmapbutton_focus_pixmap( FL_OBJECT * ob,
                                                 Pixmap      id,
                                                 Pixmap      mask );

FL_EXPORT int fl_get_button( FL_OBJECT * ob );

FL_EXPORT void fl_set_button( FL_OBJECT * ob,
                              int         pushed );

FL_EXPORT int fl_get_button_numb( FL_OBJECT * ob );

#define fl_set_button_shortcut  fl_set_object_shortcut

FL_EXPORT FL_OBJECT * fl_create_generic_button( int          objclass,
                                                int          type,
                                                FL_Coord     x,
                                                FL_Coord     y,
                                                FL_Coord     w,
                                                FL_Coord     h,
                                                const char * label );

FL_EXPORT void fl_add_button_class( int              bclass,
                                    FL_DrawButton    drawit,
                                    FL_CleanupButton cleanup );

FL_EXPORT void fl_set_button_mouse_buttons( FL_OBJECT    * ob,
                                            unsigned int   buttons );

FL_EXPORT void fl_get_button_mouse_buttons( FL_OBJECT    * ob,
                                            unsigned int * buttons );
