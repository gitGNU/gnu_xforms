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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * \file pflps.h
 *
 * private headers of postscript support. Must come
 * after forms.h
 */

#ifndef PFLPS_H
#define PFLPS_H

#include "include/forms.h"

/* configuration infomation. contains all information needed for
 * both regular gui postscript and image_postscript members
 */

typedef struct {
    /* The first entries must be identical to the ones of a FLPS_CONTROL
       structure defined in flps.h */

    int          ps_color;
    int          orientation;
    int          auto_fit;
    int          drawbox;
    int          eps;
    float        xdpi,
                 ydpi;
    float        paper_w,
                 paper_h;
    float        gamma;
    const char * tmpdir;
    int          printer_dpi;
    float        hm,
                 vm;
    float        xscale,
                 yscale;
    int          scale_text;
    int          first_page_only;
    int          clip;

    /* Now follow elements not from the FLPS_CONTROL structure */

    FILE       * fp;
    int          lastc,
                 literal,
                 len;
    int          pack;
    int          verbose;

    /* cache */

    int          cur_lw,            /* line width                */
                 last_lw;
    int          cur_style,         /* font style and size       */
                 cur_size;
    FL_COLOR     cur_color;         /* color cache               */
    int          landscape;
    float        final_xscale,
                 final_yscale;

    /* private fields for regular flps  */

    int          unit;              /* unit of measure           */
    int          pages;             /* how many pages            */
    int          page;              /* current page              */
    int          epsf_import;       /* true if importing EPS     */
    int          inverted;          /* take care of coord switch */
    int          user_bw;           /* bw specified on cmdline   */
    int          bw;                /* current border width      */
    const char * poly_name;         /* PS poly name              */

    /* private field for image postscript */

    char       * prefix;            /* output file prefix        */
    int          misct,             /* misc. margins         */
                 miscl,
                 miscb,
                 miscr;
    int          lastr,
                 lastg,
                 lastb;
    int          comment;
    int          isRGBColor;
    int          rotation;
    float        s2px;             /* screen to paper scaling        */
    float        s2py;             /* screen to paper scaling        */
} FLPSInfo;

extern FLPSInfo *flps;

void flps_color( FL_COLOR );

void flps_rgbcolor( int,
                    int,
                    int );

int flps_get_gray255( FL_COLOR );

void flps_emit_prolog( void );

void flps_emit_header( const char *,
                       int,
                       int,
                       int,
                       int,
                       int );

void flps_switch_flps( FLPSInfo * );

void flps_restore_flps( void );


/* basic drawing of simple geomtric figures */

void flps_draw_init( void );

void flps_rectangle( int,
                     int,
                     int,
                     int,
                     int,
                     FL_COLOR );

void flps_roundrectangle( int,
                          int,
                          int,
                          int,
                          int,
                          FL_COLOR );

void flps_lines( FL_POINT *,
                 int,
                 FL_COLOR );

void flps_line( int,
                int,
                int,
                int,
                FL_COLOR );

void flps_poly( int,
                FL_POINT *,
                int,
                FL_COLOR );

int flps_draw_symbol( const char *,
                      int,
                      int,
                      int,
                      int,
                      FL_COLOR );

void flps_oval( int,
                int,
                int,
                int,
                int,
                FL_COLOR );

void flps_pieslice( int,
                    int,
                    int,
                    int,
                    int,
                    int,
                    int,
                    FL_COLOR );

void flps_circ( int,
                int,
                int,
                int,
                FL_COLOR );

void flps_arc( int,
               int,
               int,
               int,
               int,
               int,
               FL_COLOR );


#define flps_rectf( x, y, w, h, c )   flps_rectangle( 1, x, y, w, h, c )
#define flps_rect( x, y, w, h, c )    flps_rectangle( 0, x, y, w, h, c )


void flps_draw_box( int,
                    int,
                    int,
                    int,
                    int,
                    FL_COLOR,
                    int );

void flps_draw_tbox( int,
                     int,
                     int,
                     int,
                     int,
                     FL_COLOR,
                     int );

void flps_draw_frame( int,
                      int,
                      int,
                      int,
                      int,
                      FL_COLOR,
                      int );

void flps_draw_checkbox( int,
                         int,
                         int,
                         int,
                         int,
                         FL_COLOR,
                         int );


/* basic text drawing routines */

void flps_draw_text( int,
                     int,
                     int,
                     int,
                     int,
                     FL_COLOR,
                     int,
                     int,
                     const char * );

void flps_draw_text_beside( int,
                            int,
                            int,
                            int,
                            int,
                            FL_COLOR,
                            int,
                            int,
                            const char * );

void flps_text_init( void );

int find_type_val( int,
                   const char * );

void flps_reset_cache( void );

void flps_invalidate_color_cache( void );

void flps_invalidate_font_cache( void );

void flps_invalidate_linewidth_cache( void );

void flps_invalidate_symbol_cache( void );

void flps_linewidth( int );

int flps_get_linewidth( void );

void flps_reset_linewidth( void );

void flps_linestyle( int );

int flps_get_linestyle( void );

void flps_log( const char * );

void flps_output( const char *,
                  ... );

void flps_set_font( int,
                    int );

int get_gray255( FL_COLOR );

void get_scale_unit( int,
                     float *,
                     float * );

void ps_invalidate_font_cache(void);

char *ps_literal( const char * );

void flps_set_clipping( int,
                        int,
                        int,
                        int);

void flps_unset_clipping( void );

void flps_apply_gamma( float );

FL_COLOR flps_get_namedcolor( const char * );

#define PS_SPECIAL( c )  (    ( c ) == '('   \
                           || ( c ) == ')'   \
                           || ( c ) == '['   \
                           || ( c ) == ']'   \
                           || ( c ) == '<'   \
                           || ( c ) == '>'   \
                           || ( c ) == '%'   \
                           || ( c ) == '#'   \
                           || ( c ) == '/' )


#endif  /* ifndef PFLPS_H */


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
