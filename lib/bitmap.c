/*
 *
 *	This file is part of the XForms library package.
 *
 * XForms is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1, or
 * (at your option) any later version.
 *
 * XForms is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XForms; see the file COPYING.	 If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */


/**
 * \file bitmap.c
 *.
 *	This file is part of the XForms library package.
 *	Copyright (c) 1996-2002	 T.C. Zhao and Mark Overmars
 *	All rights reserved.
 *.
 *
 *	FL_BITMAP & FL_BITMAPBUTTON class.
 *
 */

#if defined F_ID  || defined DEBUG
char *fl_id_bmp = "$Id: bitmap.c,v 1.10 2008/06/22 19:05:32 jtt Exp $";
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "include/forms.h"
#include "flinternal.h"


typedef FL_BUTTON_STRUCT SPEC;


/***************************************
 ***************************************/

static void
free_bitmap( SPEC * p )
{
	if ( p->pixmap )
		XFreePixmap( flx->display, p->pixmap );
	p->pixmap = None;
}


/********************************************************************
 * static bitmap
 ******************************************************************{*/

static void
drawit( Window	 win,
		FL_Coord x,
		FL_Coord y,
		FL_Coord w,
		FL_Coord h,
		FL_Coord src_x,
		FL_Coord src_y,
		FL_COLOR fcol,
		FL_COLOR bcol,
		Pixmap	 bitmap )
{
	FL_Coord clip_x,
		     clip_y,
		     clip_w,
		     clip_h;

	/* Get the currently set clipping */

	fli_get_clipping( &clip_x, &clip_y, &clip_w, &clip_h );
	if ( clip_w > 0 && clip_h > 0 )
	{
		/* If the bitmap is not within the clipping region nothing is
		   to be drawn */

		if (    x + w < clip_x
			 || x > clip_x + clip_w
             || y + h < clip_y
			 || y > clip_y + clip_h )
			return;

		/* If the bitmap isn't completely within the clipping region
		   recalculate what to draw */

		if (    x <= clip_x
			 || x + w >= clip_x + clip_w
			 || y <= clip_y
			 || y + h >= clip_y + clip_h )
		{
			if ( x < clip_x )
			{
				src_x  += clip_x - x;
				w -= clip_x - x;
				x = clip_x;
			}

			if ( x + w > clip_x + clip_w )
				w = clip_x + clip_w - x;

			if ( y < clip_y )
			{
				src_y  += clip_y - y;
				h -= clip_y - y;
				y = clip_y;
			}

			if ( y + h > clip_y + clip_h )
				h = clip_y + clip_h - y;
		}
	}

	fl_color( fcol );
	fl_bk_color( bcol );

	XCopyPlane( flx->display, bitmap, win, flx->gc, src_x, src_y,
				w, h, x, y, 1 );
}


/***************************************
 ***************************************/

static void
draw_bitmap( FL_OBJECT * ob )
{
	SPEC *sp = ob->spec;
	FL_Coord xx,			/* position of bitmap */
			 yy;

	/* Draw the box */

	fl_drw_box( ob->boxtype, ob->x, ob->y, ob->w, ob->h, ob->col1, ob->bw );

	/* do nothing is not empty data */

	if ( sp->bits_w == 0 || ! sp->pixmap )
		return;

	/* Calculate position so the bitmap is centered */

	xx = ob->x + ( ob->w - sp->bits_w ) / 2;
	yy = ob->y + ( ob->h - sp->bits_h ) / 2;

	drawit( FL_ObjWin( ob ), xx, yy, sp->bits_w, sp->bits_h, 0, 0,
			ob->lcol, ob->col1, sp->pixmap );
}



/***************************************
 * Handles an event, returns whether value has changed.
 ***************************************/

static int
handle_it( FL_OBJECT * ob,
		   int		   event,
		   FL_Coord	   mx	FL_UNUSED_ARG,
		   FL_Coord	   my	FL_UNUSED_ARG,
		   int		   key	FL_UNUSED_ARG,
		   void		 * ev	FL_UNUSED_ARG )
{
	switch ( event )
	{
		case FL_DRAW:
			draw_bitmap( ob );
			/* fall through */

		case FL_DRAWLABEL:
			fl_draw_object_label( ob );
			break;

		case FL_FREEMEM:
			free_bitmap( ob->spec );
			fl_free( ob->spec );
			break;
	}

	return 0;
}


/***************************************
 * Creates an object
 ***************************************/

FL_OBJECT *
fl_create_bitmap( int		   type,
				  FL_Coord	   x,
				  FL_Coord	   y,
				  FL_Coord	   w,
				  FL_Coord	   h,
				  const char * label )
{
	FL_OBJECT *ob;
	SPEC *sp;

	ob = fl_make_object( FL_BITMAP, type, x, y, w, h, label, handle_it );
	ob->boxtype = FL_BITMAP_BOXTYPE;
	ob->col1 = FL_BITMAP_COL1;
	ob->col2 = FL_BITMAP_COL2;
	ob->lcol = FL_BITMAP_LCOL;
	ob->align = FL_BITMAP_ALIGN;
	ob->active = type != FL_NORMAL_BITMAP;

	sp = ob->spec = fl_calloc( 1, sizeof *sp );
	sp->pixmap = sp->mask = sp->focus_pixmap = sp->focus_mask = None;
	sp->cspecv = NULL;
	sp->filename = sp->focus_filename = NULL;

	return ob;
}


/***************************************
 * Adds an object
 ***************************************/

FL_OBJECT *
fl_add_bitmap( int			type,
			   FL_Coord		x,
			   FL_Coord		y,
			   FL_Coord		w,
			   FL_Coord		h,
			   const char * label )
{
	FL_OBJECT *ob;

	ob = fl_create_bitmap( type, x, y, w, h, label );
	fl_add_object( fl_current_form, ob );
	return ob;
}


/***************************************
 * Fills the bitmap with a bitmap.
 ***************************************/

void
fl_set_bitmap_data( FL_OBJECT *		ob,
					int				w,
					int				h,
					unsigned char * data )
{
	SPEC *sp;
	Pixmap p;

	if ( ob == NULL || ob->objclass != FL_BITMAP )
		return;

	/* only occurs with fdesign -convert */

	if ( ! flx->display )
		return;

	sp = ob->spec;

	p = XCreateBitmapFromData( flx->display,
							   FL_ObjWin( ob ) ? FL_ObjWin( ob ) : fl_root,
							   ( char * ) data, w, h );

	if ( p == None )
	{
		M_err( "BitmapData", "Can't create bitmap" );
		return;
	}

	sp->bits_w = w;
	sp->bits_h = h;
	sp->pixmap = p;

	fl_redraw_object( ob );
}


/***************************************
 ***************************************/

Pixmap
fl_read_bitmapfile( Window		   win,
					const char *   file,
					unsigned int * w,
					unsigned int * h,
					int *		   hotx,
					int *		   hoty )
{
	Pixmap p = None;
	int status;

	status = XReadBitmapFile( flx->display, win, ( char * ) file,
							  w, h, &p, hotx, hoty );

	if ( status != BitmapSuccess )
		M_err( "ReadBitmap", "%s: %s", file,
			   status == BitmapFileInvalid ? "Invalid file" : "Can't read" );
	return p;
}


/***************************************
 ***************************************/

void
fl_set_bitmap_file( FL_OBJECT *	 ob,
					const char * fname )
{
	int xhot,
		yhot;
	Pixmap p;
	SPEC *sp = ob->spec;

	if ( ! flx->display )
		return;

	if ( ob == NULL || ob->objclass != FL_BITMAP )
	{
		M_err( "set_bitmap_file", "object %s not bitmap class",
			   ob ? ob->label : "null" );
		return;
	}

	p = fl_read_bitmapfile( FL_ObjWin( ob ) ? FL_ObjWin( ob ) : fl_root,
							fname, &sp->bits_w, &sp->bits_h, &xhot, &yhot );
	if ( p != None )
	{
		free_bitmap( sp );
		sp->pixmap = p;
	}

	fl_redraw_object( ob );
}

/***** End of static BITMAP ************************/


/*******************************************************************
 * BITMAP buttons
 ****************************************************************{*/

/***************************************
 ***************************************/

static void
draw_bitmapbutton( FL_OBJECT * ob )
{
	SPEC *sp = ob->spec;

	fli_draw_button( ob );

	if ( sp->pixmap != None && sp->bits_w > 0 && sp->bits_h > 0 )
	{
		int dest_x,
			dest_y,
			dest_w,
			dest_h,
			src_x,
			src_y;
		FL_COLOR col;

		/* Make sure the bitmap gets clipped to the maximum size fitting
		   into the button */

		if ( ob->w - 2 * FL_abs( ob->bw ) > ( int ) sp->bits_w )
		{
			dest_x = ob->x + ( ob->w - sp->bits_w ) / 2;
			dest_w = sp->bits_w;
			src_x  = 0;
		}
		else
		{
			dest_x = ob->x + FL_abs( ob->bw );
			dest_w = ob->w - 2 * FL_abs( ob->bw );
			src_x  = ( sp->bits_w - dest_w ) / 2;
		}

		if ( ob->h - 2 * FL_abs( ob->bw ) > ( int ) sp->bits_h )
		{
			dest_y = ob->y + ( ob->h - sp->bits_h ) / 2;
			dest_h = sp->bits_h;
			src_y  = 0;
		}
		else
		{
			dest_y = ob->y + FL_abs( ob->bw );
			dest_h = ob->h - 2 * FL_abs( ob->bw );
			src_y  = ( sp->bits_h - dest_h ) / 2;
		}

		col = sp->val ? ob->col2 : ob->col1;

		if ( ob->belowmouse && col == FL_BUTTON_COL1 )
			col = FL_BUTTON_MCOL1;
		if ( ob->belowmouse && col == FL_BUTTON_COL2 )
			col = FL_BUTTON_MCOL2;

		drawit( FL_ObjWin( ob ), dest_x, dest_y, dest_w,  dest_h,
				src_x, src_y, ob->lcol, col, sp->pixmap );
	}

	fl_draw_object_label( ob );

}


/***************************************
 ***************************************/

FL_OBJECT *
fl_create_bitmapbutton( int			 type,
						FL_Coord	 x,
						FL_Coord	 y,
						FL_Coord	 w,
						FL_Coord	 h,
						const char * label )
{
	FL_OBJECT *ob;

	fl_add_button_class( FL_BITMAPBUTTON, draw_bitmapbutton, 0 );
	ob = fl_create_generic_button( FL_BITMAPBUTTON, type, x, y, w, h, label );
	ob->boxtype = FL_BITMAPBUTTON_BOXTYPE;
	ob->col1    = FL_BITMAPBUTTON_COL1;
	ob->col2    = FL_BITMAPBUTTON_COL2;
	ob->align   = FL_BITMAPBUTTON_ALIGN;
	ob->lcol    = FL_BITMAP_LCOL;
	return ob;
}


/***************************************
 ***************************************/

FL_OBJECT *
fl_add_bitmapbutton( int		  type,
					 FL_Coord	  x,
					 FL_Coord	  y,
					 FL_Coord	  w,
					 FL_Coord	  h,
					 const char * label )
{
	FL_OBJECT *ob;

	ob = fl_create_bitmapbutton( type, x, y, w, h, label );
	fl_add_object( fl_current_form, ob );
	return ob;
}


/***************************************
 ***************************************/

void
fl_set_bitmapbutton_data( FL_OBJECT *	  ob,
						  int			  w,
						  int			  h,
						  unsigned char * bits )
{
	SPEC *sp;
	Window win;

	if ( ! ob || ob->objclass != FL_BITMAPBUTTON )
		return;

	win = FL_ObjWin( ob ) ? FL_ObjWin( ob ) : fl_root;

	sp = ob->spec;
	free_bitmap( sp );
	sp->bits_w = w;
	sp->bits_h = h;

	sp->pixmap = XCreateBitmapFromData( flx->display, win, ( char * ) bits,
										sp->bits_w, sp->bits_h );

	fl_redraw_object( ob );
}


/***************************************
 ***************************************/

void
fl_set_bitmapbutton_file( FL_OBJECT *  ob,
						  const char * file )
{
	SPEC *sp;
	int hx,
		hy;

	if ( ! flx->display )
		return;

	if ( ! ob || ob->objclass != FL_BITMAPBUTTON )
		return;

	sp = ob->spec;
	sp->pixmap =
		fl_read_bitmapfile( FL_ObjWin( ob ) ? FL_ObjWin( ob ) : fl_root,
							file, &sp->bits_w, &sp->bits_h, &hx, &hy );
	fl_redraw_object( ob );
}
