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
 *  Object CLass: Frame
 */

typedef enum {
    FL_NO_FRAME,
    FL_UP_FRAME,
    FL_DOWN_FRAME,
    FL_BORDER_FRAME,
    FL_SHADOW_FRAME,
    FL_ENGRAVED_FRAME,
    FL_ROUNDED_FRAME,
    FL_EMBOSSED_FRAME,
    FL_OVAL_FRAME
} FL_FRAME_TYPE;

FL_EXPORT FL_OBJECT * fl_create_frame( int          type,
                                       FL_COORD     x,
                                       FL_COORD     y,
                                       FL_COORD     w,
                                       FL_COORD     h,
                                       const char * label );

FL_EXPORT FL_OBJECT * fl_add_frame( int          type,
                                    FL_COORD     x,
                                    FL_COORD     y,
                                    FL_COORD     w,
                                    FL_COORD     h,
                                    const char * label );

/* labeld frame */

FL_EXPORT FL_OBJECT * fl_create_labelframe( int          type,
                                            FL_COORD     x,
                                            FL_COORD     y,
                                            FL_COORD     w,
                                            FL_COORD     h,
                                            const char * label );

FL_EXPORT FL_OBJECT * fl_add_labelframe( int          type,
                                         FL_COORD     x,
                                         FL_COORD     y,
                                         FL_COORD     w,
                                         FL_COORD     h,
                                         const char * label );
