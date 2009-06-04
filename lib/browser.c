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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * \file browser.c
 *
 *.
 *  This file is part of the XForms library package.
 *  Copyright (c) 1996-2002  T.C. Zhao and Mark Overmars
 *  All rights reserved.
 *.
 *
 *   Browser composite.
 *   scrollbar redrawing can be further optimized.
 *
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "include/forms.h"
#include "flinternal.h"
#include "private/pbrowser.h"


#define MaxPixels( sp )   ( sp->maxpixels + 5 )


/***************************************
 ***************************************/

static void
attrib_change( FL_OBJECT * ob )
{
    FLI_BROWSER_SPEC *sp = ob->spec;

    /* Text box stuff */

    sp->tb->x = ob->x;
    sp->tb->y = ob->y;

    sp->tb->visible = 1;
    sp->tb->input = ob->input;

    sp->tb->type    = ob->type;
    sp->tb->boxtype = ob->boxtype;
    sp->tb->lcol    = ob->lcol;
    sp->tb->col1    = ob->col1;
    sp->tb->col2    = ob->col2;
    sp->tb->bw      = ob->bw;

    /* Scrollbars */

    if (    ob->boxtype == FL_DOWN_BOX
		 && sp->hsl->type == FL_HOR_NICE_SCROLLBAR )
    {
		sp->hsl->boxtype = FL_FRAME_BOX;
		sp->vsl->boxtype = FL_FRAME_BOX;
    }
    else if (    ob->boxtype == FL_DOWN_BOX
			  && sp->hsl->type == FL_HOR_SCROLLBAR )
    {
		sp->hsl->boxtype = FL_UP_BOX;
		sp->vsl->boxtype = FL_UP_BOX;
    }
    else
    {
		sp->hsl->boxtype = ob->boxtype;
		sp->vsl->boxtype = ob->boxtype;
    }

    sp->hsl->bw = sp->vsl->bw = ob->bw;

    if ( ! sp->user_set && ob->boxtype != FL_DOWN_BOX )
		sp->vw = sp->vw_def = sp->hh = sp->hh_def =
			                                fli_get_default_scrollbarsize( ob );
}


/***************************************
 ***************************************/

static void
get_geometry( FL_OBJECT * ob )
{
    FLI_BROWSER_SPEC *comp = ob->spec;
    FL_OBJECT *tb = comp->tb;
    FLI_TBOX_SPEC *sp = comp->tb->spec;
    int h_on = comp->h_on,
		v_on = comp->v_on;
	double old_xrel = fli_tbox_get_rel_xoffset( tb );
	double old_yrel = fli_tbox_get_rel_yoffset( tb );

    comp->hh = comp->vw = 0;
    comp->h_on = comp->v_on = 0;

	tb->w = ob->w;
	tb->h = ob->h;

	/* Check if we need a vertical slider */

    if (    ( sp->max_height > sp->h && comp->v_pref != FL_OFF )
		 || comp->v_pref == FL_ON )
    {
		comp->v_on  = 1;
		comp->vw    = comp->vw_def;
		tb->w      -= comp->vw;
    }

	/* Check if we need a horizontal slider */

    if (    ( sp->max_width > sp->w && comp->h_pref != FL_OFF )
		 || comp->h_pref == FL_ON )
    {
		comp->h_on  = 1;
		comp->hh    = comp->hh_def;
		tb->h      -= comp->hh;
    }

	/* Due to the addition of a horizontal slider also a vertical slider
	   may now be needed, so recheck for this possibility */

    if ( ! comp->v_on && sp->max_height > sp->h && comp->v_pref != FL_OFF )
    {
		comp->v_on  = 1;
		comp->vw    = comp->vw_def;
		tb->w      -= comp->vw;
    }

    comp->hsl->visible = comp->h_on;
    comp->vsl->visible = comp->v_on;

    if ( comp->v_on )
    {
		comp->vsl->x = ob->x + ob->w - comp->vw;
		comp->vsl->y = ob->y;
		comp->vsl->w = comp->vw;
		comp->vsl->h = ob->h - comp->hh;

		comp->vval  = old_yrel;
		comp->vsize = comp->vinc1 = ( double ) sp->h / sp->max_height;
		comp->vinc2 = ( double ) sp->def_height / sp->max_height;
	}
	else
	{
		comp->vval  = 0.0;
		comp->vsize = 1.0;
	}

    if ( comp->h_on )
    {
		comp->hsl->x = ob->x;
		comp->hsl->y = ob->y + ob->h - comp->hh;
		comp->hsl->w = ob->w - comp->vw;
		comp->hsl->h = comp->hh;

		comp->hval  = old_xrel;
		comp->hsize = ( double ) sp->w / sp->max_width;
		comp->hinc1 = ( 8.0 * sp->def_height ) / sp->max_width;
		comp->hinc2 = ( sp->def_height - 2.0 ) / sp->max_width;
	}
	else
	{
		comp->hval  = 0.0;
		comp->hsize = 1.0;
	}

    if ( h_on != comp->h_on || v_on != comp->v_on )
    {
		comp->attrib = 1;
		comp->dead_area = ! ( comp->h_on ^ comp->v_on );
		if ( h_on )
			fli_tbox_set_xoffset( comp->tb, 0 );
    }
    else
		comp->attrib = 0;

	/* Recompute size of textbox drawing area */

	fli_tbox_prepare_drawing( comp->tb );

	tbox_do_not_redraw = 1;
	comp->vval = fli_tbox_set_rel_yoffset( tb, comp->vval );
	fl_set_scrollbar_value( comp->vsl, comp->vval );

	tbox_do_not_redraw = 1;
	comp->hval = fli_tbox_set_rel_xoffset( tb, comp->hval );
	fl_set_scrollbar_value( comp->hsl, comp->hval );
}


/***************************************
 * The "dead area" is the small square in the lower right hand corner
 * of the browser (to the right of the vertical slider and below the
 * horizontal one) that shows up when both the sliders are displayed.
 ***************************************/

static void
draw_dead_area( FL_OBJECT        * ob,
				FLI_BROWSER_SPEC * comp )
{
    if ( comp->dead_area && FL_ObjWin( comp->tb ) )
    {
		fl_winset( FL_ObjWin( comp->tb ) );
		fl_drw_box( FL_FLAT_BOX, ob->x + ob->w - comp->vw,
					ob->y + ob->h - comp->hh, comp->vw, comp->hh,
					comp->vsl->col1, 1 );
		comp->dead_area = 0;
    }
}


/***************************************
 ***************************************/

static int
handle_browser( FL_OBJECT * ob,
				int         event,
				FL_Coord    mx   FL_UNUSED_ARG,
				FL_Coord    my   FL_UNUSED_ARG,
				int         key  FL_UNUSED_ARG,
				void      * ev   FL_UNUSED_ARG )
{
    FLI_BROWSER_SPEC *comp = ob->spec;

    switch ( event )
    {
		case FL_DRAW:
			attrib_change( ob );
			get_geometry( ob );
			draw_dead_area( ob, comp );
			/* fall through */

		case FL_DRAWLABEL:
			fl_draw_object_label( ob );
			break;

		case FL_FREEMEM:
			fl_free( comp );
			break;
    }

    return FL_RETURN_NONE;
}


/***************************************
 ***************************************/

static void
redraw_scrollbar( FL_OBJECT * ob )
{
    FLI_BROWSER_SPEC *comp = ob->spec;

    get_geometry( ob );

    fl_freeze_form( ob->form );

    if ( comp->v_on )
    {
		fl_set_scrollbar_size( comp->vsl, comp->vsize );
		fl_set_scrollbar_value( comp->vsl, comp->vval );

		if ( comp->vsize != 1.0 )
			fl_set_scrollbar_increment( comp->vsl, comp->vinc1, comp->vinc2 );
    }

    if ( comp->h_on )
    {
		fl_set_scrollbar_size( comp->hsl, comp->hsize );
		fl_set_scrollbar_value( comp->hsl, comp->hval );

		if ( comp->hsize != 1.0 )
			fl_set_scrollbar_increment( comp->hsl, comp->hinc1, comp->hinc2 );
    }

    if ( comp->attrib )
    {
		fl_redraw_object( comp->vsl );
		fl_redraw_object( comp->hsl );
		fl_redraw_object( comp->tb );
    }

    draw_dead_area( ob, comp );
    fl_unfreeze_form( ob->form );
}


/***************************************
 ***************************************/

static void
hcb( FL_OBJECT * obj,
	 long        data  FL_UNUSED_ARG )
{
    FLI_BROWSER_SPEC *comp = obj->parent->spec;
	double hp = fli_tbox_set_rel_xoffset( comp->tb,
										  fl_get_scrollbar_value( comp->hsl ) );

	if ( obj->returned & FL_RETURN_END )
		obj->parent->returned |= FL_RETURN_END;

	if ( hp != comp->old_hp )
		obj->parent->returned |= FL_RETURN_CHANGED;

	if (    obj->parent->how_return & FL_RETURN_END_CHANGED
		 && ! (    obj->parent->returned & FL_RETURN_CHANGED
				&& obj->parent->returned & FL_RETURN_END ) )
			obj->parent->returned = FL_RETURN_NONE;

	if ( obj->parent->returned & FL_RETURN_END )
 		comp->old_hp = hp;

	if ( obj->returned & FL_RETURN_CHANGED && comp->hcb )
		comp->hcb( obj->parent, fli_tbox_get_topline( comp->tb ) + 1,
				   comp->hcb_data );
}


/***************************************
 ***************************************/

static void
vcb( FL_OBJECT * obj,
	 long        data  FL_UNUSED_ARG )
{
    FLI_BROWSER_SPEC *comp = obj->parent->spec;
	double vp = fli_tbox_set_rel_yoffset( comp->tb,
										  fl_get_scrollbar_value( comp->vsl ) );

	if ( obj->returned & FL_RETURN_END )
		obj->parent->returned |= FL_RETURN_END;

	if ( vp != comp->old_vp )
		obj->parent->returned |= FL_RETURN_CHANGED;

	if (    obj->parent->how_return & FL_RETURN_END_CHANGED
		 && ! (    obj->parent->returned & FL_RETURN_CHANGED
				&& obj->parent->returned & FL_RETURN_END ) )
			obj->parent->returned = FL_RETURN_NONE;

	if ( obj->parent->returned & FL_RETURN_END )
 		comp->old_vp = vp;

	if ( obj->returned & FL_RETURN_CHANGED && comp->vcb )
		comp->vcb( obj->parent, fli_tbox_get_topline( comp->tb ) + 1,
				   comp->vcb_data );
}


/***************************************
 * Textbox callback routine, we simply pass the return value of the
 * textbox on as the parents new return value after a readjustment of
 * the scollbars.
 ***************************************/

static void
tbcb( FL_OBJECT * obj,
	  long        data  FL_UNUSED_ARG )
{
	FLI_BROWSER_SPEC *psp = obj->parent->spec;
	double vp = fli_tbox_get_rel_yoffset( obj );
	double hp = fli_tbox_get_rel_xoffset( obj );

	if ( obj->returned & FL_RETURN_CHANGED )
	{
		if ( hp != psp->old_hp )
		{
			fl_set_scrollbar_value( psp->hsl, psp->old_hp = hp );
			if ( psp->hcb )
				psp->hcb( obj->parent, fli_tbox_get_topline( psp->tb ) + 1,
						  psp->hcb_data );
		}

		if ( vp != psp->old_vp )
		{
			fl_set_scrollbar_value( psp->vsl, psp->old_vp = vp );
			if ( psp->vcb )
				psp->vcb( obj->parent, fli_tbox_get_topline( psp->tb ) + 1,
						  psp->vcb_data );
		}
	}	

	obj->parent->returned = obj->returned;
}


/***************************************
 * Textbox dblclick callback
 ***************************************/

static void
tb_dblcallback( FL_OBJECT * ob,
				long        data  FL_UNUSED_ARG )
{
    FLI_BROWSER_SPEC *sp = ob->parent->spec;

    if ( sp->callback )
		sp->callback( ob->parent, sp->callback_data );
}


/***************************************
 ***************************************/

static int
tbpost( FL_OBJECT * ob,
		int         ev,
		FL_Coord    mx,
		FL_Coord    my,
       int          key,
		void      * xev )
{
    FL_OBJECT *br = ob->parent;

    return br->posthandle ? br->posthandle( br, ev, mx, my, key, xev ) : 0;
}


/***************************************
 ***************************************/

static int
tbpre( FL_OBJECT * ob,
	   int         ev,
	   FL_Coord    mx,
	   FL_Coord    my,
      int          key,
	   void      * xev )
{

    FL_OBJECT *br = ob->parent;

    return br->prehandle ? br->prehandle( br, ev, mx, my, key, xev ) : 0;
}


#define IS_FLATBOX(b) (    b == FL_FRAME_BOX    \
                        || b == FL_EMBOSSED_BOX \
                        || b == FL_BORDER_BOX   \
                        || b == FL_ROUNDED_BOX )


/***************************************
 ***************************************/

int
fli_get_default_scrollbarsize( FL_OBJECT * ob )
{
    int delta = ( FL_abs( ob->bw ) + 3 * ( ob->bw > 0 ) );
    int flat = IS_FLATBOX( ob->boxtype ) ? 2 : 0;

    if ( ob->w > 250 && ob->h > 250 )
		return 15 + delta - flat;
    else if ( ob->w < 150 || ob->h < 150 )
		return 13 + delta - flat;
    else
		return 14 + delta - flat;
}


/***************************************
 ***************************************/

FL_OBJECT *
fl_create_browser( int          type,
				   FL_Coord     x,
				   FL_Coord     y,
				   FL_Coord     w,
				   FL_Coord     h,
				   const char * label )
{
    FL_OBJECT *ob;
    FLI_BROWSER_SPEC *sp;
	int D;

    ob = fl_make_object( FL_BROWSER, type, x, y, w, h, label,
						 handle_browser );

    sp = ob->spec = fl_calloc( 1, sizeof *sp );
    sp->tb = fli_create_tbox( type, x, y, w, h, NULL );

	sp->callback = NULL;
	sp->hsize    = sp->vsize = sp->hval = sp->vval =
	sp->hinc1    = sp->hinc2 = sp->vinc1 = sp->vinc2 = 0.0;
	sp->hcb      = sp->vcb = NULL;
	sp->hcb_data = sp->vcb_data = NULL;
	sp->old_hp   = sp->old_vp = 0.0;

    /* Copy browser attributes from textbox */

    ob->boxtype  = sp->tb->boxtype;
    ob->lcol     = sp->tb->lcol;
    ob->align    = sp->tb->align;
    ob->col1     = sp->tb->col1;
    ob->col2     = sp->tb->col2;

    /* Textbox handlers */
 
	fl_set_object_callback( sp->tb, tbcb, 0 );
    fli_tbox_set_dblclick_callback( sp->tb, tb_dblcallback, 0 );
    fl_set_object_posthandler( sp->tb, tbpost );
    fl_set_object_prehandler( sp->tb, tbpre );

    /* Scrollbars */

    D = sp->vw_def = sp->hh_def = fli_get_default_scrollbarsize( ob );
    sp->v_pref = sp->h_pref = FL_AUTO;

    sp->hsl = fl_create_scrollbar( fli_context->hscb, x, y + h - D,
								   w - D, D, NULL );
    sp->hsl->visible = sp->h_pref == FL_ON;
    fl_set_object_callback( sp->hsl, hcb, 0 );
    fl_set_scrollbar_value( sp->hsl, 0.0 );
    sp->hsl->resize = FL_RESIZE_NONE;

    sp->vsl = fl_create_scrollbar( fli_context->vscb, x + w - D, y,
								   D, h - D, NULL );
    sp->vsl->visible = sp->v_pref == FL_ON;
    fl_set_object_callback( sp->vsl, vcb, 0 );
    fl_set_scrollbar_value( sp->hsl, 0.0 );
    sp->vsl->resize = FL_RESIZE_NONE;

    fl_add_child( ob, sp->tb  );
    fl_add_child( ob, sp->hsl );
    fl_add_child( ob, sp->vsl );

	fl_set_object_return( ob, FL_RETURN_SELECTION | FL_RETURN_DESELECTION );
	fl_set_object_return( sp->hsl, FL_RETURN_ALWAYS );
	fl_set_object_return( sp->vsl, FL_RETURN_ALWAYS );
	fl_set_object_return( sp->tb, FL_RETURN_ALWAYS );

    return ob;
}


/***************************************
 ***************************************/

FL_OBJECT *
fl_add_browser( int          type,
				FL_Coord     x,
				FL_Coord     y,
				FL_Coord     w,
				FL_Coord     h,
				const char * label )
{
    FL_OBJECT *ob = fl_create_browser( type, x, y, w, h, label );

    fl_add_object( fl_current_form, ob );

    return ob;
}


/***************************************
 ***************************************/

void
fl_set_browser_vscrollbar( FL_OBJECT * obj,
						   int         on )
{
    FLI_BROWSER_SPEC *comp = obj->spec;

    if ( comp->v_pref != on )
    {
		comp->v_pref = on;
		redraw_scrollbar( obj );
		fli_tbox_react_to_vert( comp->tb, on != FL_OFF );
		get_geometry( obj );
		fl_redraw_object( obj );
    }
}


/***************************************
 ***************************************/

void
fl_set_browser_hscrollbar( FL_OBJECT * obj,
						   int         on )
{
    FLI_BROWSER_SPEC *comp = obj->spec;

    if ( comp->h_pref != on )
    {
		comp->h_pref = on;
		redraw_scrollbar( obj );
		fli_tbox_react_to_hori( comp->tb, on != FL_OFF );
		get_geometry( obj );
		fl_redraw_object( obj );
    }
}


/***************************************
 ***************************************/

void
fl_set_browser_hscroll_callback( FL_OBJECT                  * ob,
                                 FL_BROWSER_SCROLL_CALLBACK   cb,
								 void                       * data )
{
      FLI_BROWSER_SPEC *comp = ob->spec;

	  comp->hcb = cb;
      comp->hcb_data = data;
}


/***************************************
 ***************************************/

FL_BROWSER_SCROLL_CALLBACK
fl_get_browser_hscroll_callback( FL_OBJECT * ob )
{
      return ( ( FLI_BROWSER_SPEC * ) ob->spec )->hcb;
}


/***************************************
 ***************************************/

void
fl_set_browser_vscroll_callback( FL_OBJECT                  * ob,
								 FL_BROWSER_SCROLL_CALLBACK   cb,
								 void                       * data )
{
      FLI_BROWSER_SPEC *comp = ob->spec;

	  comp->vcb = cb;
      comp->vcb_data = data;
}


/***************************************
 ***************************************/

FL_BROWSER_SCROLL_CALLBACK
fl_get_browser_vscroll_callback( FL_OBJECT * ob )
{
      return ( ( FLI_BROWSER_SPEC * ) ob->spec )->vcb;
}


/***************************************
 * meant for the textbox to handle scroll callback properly
 ***************************************/

void
fli_adjust_browser_scrollbar( FL_OBJECT * ob )
{
      FLI_BROWSER_SPEC *comp = ob->spec;

      fl_call_object_callback( comp->hsl );
      fl_call_object_callback( comp->vsl );
}


/***************************************
 ***************************************/

void
fl_clear_browser( FL_OBJECT * ob )
{
    FLI_BROWSER_SPEC *comp = ob->spec;

    fli_tbox_clear( comp->tb );
    fl_freeze_form( ob->form );
    fl_set_scrollbar_value( comp->hsl, 0.0 );
    fl_set_scrollbar_size( comp->hsl, 1.0 );
    fl_set_scrollbar_value( comp->vsl, 0.0 );
    fl_set_scrollbar_size( comp->vsl, 1.0 );
	redraw_scrollbar( ob );
    fl_unfreeze_form( ob->form );
}


/***************************************
 ***************************************/

FL_Coord
fl_get_browser_xoffset( FL_OBJECT * obj )
{
	FLI_TBOX_SPEC *sp = ( ( FLI_BROWSER_SPEC * ) obj->spec )->tb->spec;

	return sp->xoffset;
}


/***************************************
 * set_xoffset, set_topline and select_line are kind of nasty in that
 * they might get called from browser's child object!
 ***************************************/

void
fl_set_browser_xoffset( FL_OBJECT * ob,
						FL_Coord    npixels )
{
    FLI_BROWSER_SPEC *sp = ob->spec;

	fli_tbox_set_xoffset( sp->tb, npixels );
	redraw_scrollbar( ob );
}


/***************************************
 ***************************************/

void
fl_set_browser_topline( FL_OBJECT * ob,
						int         topline )
{
    FLI_BROWSER_SPEC *sp = ob->spec;

    fli_tbox_set_topline( sp->tb, topline - 1 );
	redraw_scrollbar( ob );
}


/***************************************
 ***************************************/

void
fl_select_browser_line( FL_OBJECT * ob,
						int         line )
{
    fli_tbox_select_line( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb, line - 1 );
}


/***************************************
 ***************************************/

void
fl_addto_browser( FL_OBJECT  * ob,
				  const char * text )
{
    fli_tbox_add_line( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb, text );
    redraw_scrollbar( ob );
}


/***************************************
 ***************************************/

void
fl_insert_browser_line( FL_OBJECT  * ob,
						int          linenumb,
						const char * newtext )
{
    fli_tbox_insert_line( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb,
						  linenumb - 1, newtext );
    redraw_scrollbar( ob );
}


/***************************************
 ***************************************/

void
fl_delete_browser_line( FL_OBJECT * ob,
						int         linenumb )
{
    fli_tbox_delete_line( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb,
						  linenumb - 1 );
    redraw_scrollbar( ob );
}


/***************************************
 ***************************************/

void
fl_replace_browser_line( FL_OBJECT  * ob,
						 int          linenumb,
						 const char * newtext )
{
    fli_tbox_replace_line( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb,
						   linenumb - 1, newtext );
    redraw_scrollbar( ob );
}


/***************************************
 ***************************************/

const char *
fl_get_browser_line( FL_OBJECT * ob,
					 int         linenumb )
{
    return fli_tbox_get_line( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb,
							  linenumb - 1 );
}


/***************************************
 ***************************************/

int
fl_get_browser_maxline( FL_OBJECT * obj )
{
	FLI_BROWSER_SPEC *sp = obj->spec;

    return ( ( FLI_TBOX_SPEC * ) sp->tb->spec )->num_lines;
}


/***************************************
 ***************************************/

void
fl_deselect_browser_line( FL_OBJECT * ob,
						  int         line )
{
    fli_tbox_deselect_line( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb, line - 1 );
}


/***************************************
 ***************************************/

void
fl_deselect_browser( FL_OBJECT * ob )
{
    fli_tbox_deselect( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb );
}


/***************************************
 ***************************************/

int
fl_isselected_browser_line( FL_OBJECT * ob,
							int         line )
{
    return fli_tbox_is_line_selected( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb,
									  line - 1 );
}


/***************************************
 ***************************************/

int
fl_get_browser( FL_OBJECT * ob )
{
    return fli_tbox_get_selection( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb );
}


/***************************************
 ***************************************/

void
fl_set_browser_fontsize( FL_OBJECT * ob,
						 int         size )
{
    FLI_BROWSER_SPEC *sp = ob->spec;

	fli_tbox_set_fontsize( sp->tb, size );
	redraw_scrollbar( ob );
	fl_redraw_object( ob );
}


/***************************************
 ***************************************/

void
fl_set_browser_fontstyle( FL_OBJECT * ob,
						  int         style )
{
    FLI_BROWSER_SPEC *sp = ob->spec;

	fli_tbox_set_fontstyle( sp->tb, style );
	redraw_scrollbar( ob );
	fl_redraw_object( ob );
}


/***************************************
 ***************************************/

int
fl_get_browser_topline( FL_OBJECT * ob )
{
    FLI_BROWSER_SPEC *sp = ob->spec;
	int line = fli_tbox_get_topline( sp->tb );

	if ( line < fl_get_browser_maxline( sp->tb ) )
		 line++;

	return line;
}


/***************************************
 ***************************************/

int
fl_load_browser( FL_OBJECT  * ob,
				 const char * f )
{
    FLI_BROWSER_SPEC *sp = ob->spec;
    int status = fli_tbox_load( sp->tb, f );

    redraw_scrollbar( ob );
    return status;
}


/***************************************
 ***************************************/

void
fl_add_browser_line( FL_OBJECT  * ob,
					 const char * newtext )
{
    FLI_BROWSER_SPEC *sp = ob->spec;

    fli_tbox_add_line( sp->tb, newtext );
    redraw_scrollbar( ob );
}


/***************************************
 ***************************************/

void
fl_set_browser_dblclick_callback( FL_OBJECT      * ob,
								  FL_CALLBACKPTR   cb,
								  long             a )
{
    FLI_BROWSER_SPEC *comp = ob->spec;

    comp->callback = cb;
    comp->callback_data = a;
}


/***************************************
 ***************************************/

void
fl_set_browser_scrollbarsize( FL_OBJECT * ob,
							  int         hh,
							  int         vw )
{
    FLI_BROWSER_SPEC *comp = ob->spec;
    int redraw = 0;

    if ( hh > 0 && hh != comp->hsl->h )
    {
		comp->hsl->h = comp->hh_def = hh;
		redraw = 1;
    }

    if ( vw > 0 && vw != comp->vsl->w )
    {
		comp->vsl->w = comp->vw_def = vw;
		redraw = 1;
    }

    if ( redraw )
    {
		comp->user_set = 1;
		fl_redraw_object( ob );
		fl_redraw_object( comp->tb );
		fl_redraw_object( comp->hsl );
		fl_redraw_object( comp->vsl );
    }
}


/***************************************
 ***************************************/

void
fl_get_browser_dimension( FL_OBJECT * obj,
						  FL_Coord  * x,
						  FL_Coord  * y,
						  FL_Coord  * w,
						  FL_Coord  * h )
{
	FLI_TBOX_SPEC *sp = ( ( FLI_BROWSER_SPEC * ) obj->spec )->tb->spec;

	*x = sp->x;
	*y = sp->y;
	*w = sp->w;
	*h = sp->h;
}


/***************************************
 ***************************************/

void
fl_set_browser_line_selectable( FL_OBJECT * ob,
								int         line,
								int         flag )
{
    fli_tbox_make_line_selectable( ob, line - 1, flag );
}


/***************************************
 ***************************************/

void
fl_addto_browser_chars( FL_OBJECT  * ob,
						const char * str )
{
    FLI_BROWSER_SPEC *sp = ob->spec;

    fli_tbox_add_chars( sp->tb, str );
    redraw_scrollbar( ob );
}


/***************************************
 * Returns an approimation of the number of lines shown in the browser
 ***************************************/

int
fl_get_browser_screenlines( FL_OBJECT * ob )
{
	FLI_TBOX_SPEC *sp = ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb->spec;

    return sp->h / sp->def_height;
}


/***************************************
 * Sets the escape key used in the text
 ***************************************/

void
fl_set_browser_specialkey( FL_OBJECT * ob,
						   int         specialkey )
{
    FLI_TBOX_SPEC *sp = ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb->spec;

	sp->specialkey = specialkey;
}


/***************************************
 * Bring a line into view
 ***************************************/

void
fl_show_browser_line( FL_OBJECT * ob,
					  int         line )
{
	fli_tbox_set_centerline( ( ( FLI_BROWSER_SPEC * ) ob->spec )->tb, line );
}


/***************************************
 ***************************************/

int
fl_set_default_browser_maxlinelength( int n  FL_UNUSED_ARG )
{
    return 0;
}
