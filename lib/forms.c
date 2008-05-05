/*
 *
 *  This file is part of the XForms library package.
 *
 * XForms is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1, or
 * (at your option) any later version.
 *
 * XForms is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XForms; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */


/**
 * \file forms.c
 *.
 *  This file is part of the XForms library package.
 *  Copyright (c) 1996-2002  T.C. Zhao and Mark Overmars
 *  All rights reserved.
 *.
 *
 *  Main event dispatcher.
 *
 */

#if defined F_ID || defined DEBUG
char *fl_id_fm = "$Id: forms.c,v 1.36 2008/05/05 14:21:52 jtt Exp $";
#endif


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ctype.h>
#include "include/forms.h"
#include "flinternal.h"
#include "global.h"


static FL_FORM * fli_find_event_form( XEvent * );
static void fli_force_visible( FL_FORM *,
							   int,
							   int );
static int fli_XLookupString( XKeyEvent *,
							 char *,
							 int,
							 KeySym * );
void fli_redraw_form_using_xevent( FL_FORM *,
								   int,
								   XEvent * );
static void fli_do_interaction_step( int  );
static int fli_get_next_event_or_idle( int,
									   FL_FORM **,
									   XEvent   * );
static void fli_handle_EnterNotify_event( FL_FORM * );
static void fli_handle_LeaveNotify_event( void );
static void fli_handle_MotionNotify_event( FL_FORM * );
static void fli_handle_Expose_event( FL_FORM  *,
									 FL_FORM ** );
static void fli_handle_ConfigureNotify_event( FL_FORM  *,
											  FL_FORM ** );
static void fli_set_form_property( FL_FORM *,
								   unsigned int );


static FL_FORM *fli_mainform;
static int nomainform;
static int reopened_group;

extern void ( * fli_handle_signal )( void );
extern int ( * fli_handle_clipboard )( void * );


/* Waiting time (in ms) for fl_check_forms() and fl_check_only_forms().
   Originally this value was 10 ms. */

#define SHORT_PAUSE   1


/* The following variables store the last recorded mouse position
   and state of buttons and modifier keys as well as how old the
   information is. They're not defined as static since they are\
   also needed in xpopup.c */

FL_Coord fli_mousex,
         fli_mousey;
unsigned int fli_keymask;
unsigned int fli_query_age = UINT_MAX;


static FL_FORM **forms = NULL;	    /* All existing forms */
static int formnumb = 0;	        /* Number of visible forms */
static int hidden_formnumb = 0;  	/* Number of hidden forms */
static size_t auto_count = 0;


/***************************************
 ***************************************/

void
fli_set_no_connection( int yes )
{
    if ( ( fl_no_connection = yes ) )
      fli_internal_init( );
}


/***************************************
 ***************************************/

int
fli_get_visible_forms_index( FL_FORM * form )
{
	int i;

    for ( i = 0; i < formnumb; i++ )
		if ( forms[ i ] == form )
			return i;

    return -1;
}


/***************************************
 ***************************************/

static int
fli_get_hidden_forms_index( FL_FORM * form )
{
	int i;

    for ( i = formnumb; i < formnumb + hidden_formnumb; i++ )
		if ( forms[ i ] == form )
			return i;

    return -1;
}


/***************************************
 ***************************************/

static void
fli_add_form_to_hidden_list( FL_FORM *form )
{
	forms = realloc( forms, ( formnumb + ++hidden_formnumb ) * sizeof *forms );
	forms[ formnumb + hidden_formnumb - 1 ] = form;
}


/***************************************
 ***************************************/

static int
fli_move_form_to_visible_list( FL_FORM *form )
{
	int i;

	if ( ( i = fli_get_hidden_forms_index( form ) ) < 0 )
	{
		M_err( "fli_move_form_to_visble_list", "Form not on hidden list" );
		return -1;
	}
		
	if ( i != formnumb )
	{
		forms[ i ] = forms[ formnumb ];
		forms[ formnumb ] = form;
	}
	hidden_formnumb--;

    if ( form->has_auto )
			auto_count++;

	return formnumb++;
}


/***************************************
 ***************************************/

static int
fli_move_form_to_hidden_list( FL_FORM *form )
{
	int i;

	if ( ( i = fli_get_visible_forms_index( form ) ) < 0 )
	{
		M_err( "fli_move_form_to_hidden_list", "Form not onvisible list" );
		return -1;
	}

	if ( formnumb > 0 )
	{
		forms[ i ] = forms[ --formnumb ];
		forms[ formnumb ] = form;
	}

	hidden_formnumb++;

    if ( form->has_auto )
    {
		if ( auto_count == 0 )
			M_err( "fli_move_form_to_hidden_list", "Bad auto count" );
		else
			auto_count--;
    }

	return formnumb;
}


/***************************************
 ***************************************/

int
fli_remove_form_from_hidden_list( FL_FORM *form )
{
	int i;

	if ( ( i = fli_get_hidden_forms_index( form ) ) < 0 )
	{
		M_err( "fli_remove_form_from_hidden_list", "Form not on hidden list" );
		return -1;
	}

	if ( i != formnumb + --hidden_formnumb )
		forms[ i ] = forms[ formnumb + hidden_formnumb ];

	forms = fl_realloc( forms, ( formnumb + hidden_formnumb ) * sizeof *forms );

	return formnumb;
}


/***************************************
 * Starts a form definition
 ***************************************/

FL_FORM *
fl_bgn_form( int      type,
			 FL_Coord w,
			 FL_Coord h )
{
    if ( ! fl_no_connection && ! flx->display )
    {
		M_err( "fl_bgn_form", "Missing or failed fl_initialize()" );
		exit( 1 );
    }

    if ( fl_current_form )
    {
		/* error actually is serious and can't be fixed easily as it might
		   be due to a bad recursion */

		M_err( "fl_bgn_form", "You forgot to call fl_end_form" );
		exit( 1 );
    }

    fl_current_form = fli_make_form( w, h );

	/* Add the new form to the list of still hidden forms */

	fli_add_form_to_hidden_list( fl_current_form );

    fl_add_box( type, 0, 0, w, h, "" );

    return fl_current_form;
}


/***************************************
 * Ends a form definition
 ***************************************/

void
fl_end_form( void )
{
    if ( ! fl_current_form )
		M_err( "fl_end_form", "NULL form." );

    if ( fl_current_group )
    {
		M_err( "fl_end_form", "You forgot to call fl_end_group." );
		fl_end_group( );
    }

    fl_current_form = NULL;
}


/***************************************
 * Reopens a form for input
 ***************************************/

void
fl_addto_form( FL_FORM * form )
{
    if ( fl_current_form )
		M_err( "fl_addto_form", "You forgot to call fl_end_form" );

    if ( ! form )
    {
		M_err( "fl_addto_form", "NULL form." );
		return;
    }

    fl_current_form = form;
}


/***************************************
 * Starts a group definition
 ***************************************/

FL_OBJECT *
fl_bgn_group( void )
{
    static int id = 1;

    if ( ! fl_current_form )
    {
		M_err( "fl_bgn_group", "NULL form." );
		return NULL;
    }

    if ( fl_current_group )
    {
		M_err( "fl_bgn_group", "You forgot to call fl_end_group." );
		fl_end_group( );
    }

    fl_current_group = fl_make_object( FL_BEGIN_GROUP, 0, 0, 10, 10, 0,
									   "", NULL );
    fl_current_group->group_id = id++;
    fl_add_object( fl_current_form, fl_current_group );

    return fl_current_group;
}


/***************************************
 * Ends a group definition
 ***************************************/

FL_OBJECT *
fl_end_group( void )
{
    FL_OBJECT *ob = fl_current_group;
    int id;

    if ( fl_current_form == NULL )
    {
		M_err( "fl_end_group", "NULL form." );
		return NULL;
    }

    if ( fl_current_group == NULL )
    {
		M_err( "fl_end_group", "NULL group." );
		return NULL;
    }

    id = fl_current_group->group_id;
    fl_current_group = NULL;

    if ( ! reopened_group )
    {
		ob = fl_make_object( FL_END_GROUP, 0, 0, 0, 0, 0, "", NULL );
		ob->group_id = id;
		fl_add_object( fl_current_form, ob );
    }

    if ( reopened_group == 3 )
		fl_end_form( );

    reopened_group = 0;

    return ob;
}


/************************ Doing the Interaction *************************/

static FL_FORM * mouseform = NULL;  /* The current form under mouse */
static FL_FORM * keyform = NULL;    /* keyboard focus form */
FL_OBJECT * fl_pushobj = NULL;	    /* latest pushed object */
FL_OBJECT * fl_mouseobj = NULL;	    /* object under the mouse */


/***************************************
 ***************************************/

void
fl_freeze_all_forms( void )
{
    int i;

    for ( i = 0; i < formnumb; i++ )
		fl_freeze_form( forms[ i ] );
}


/***************************************
 ***************************************/

void
fl_unfreeze_all_forms( void )
{
	int i;

    for ( i = 0; i < formnumb; i++ )
		fl_unfreeze_form( forms[ i ] );
}


/***************************************
 * Corrects the shape of the form based on the shape of its window
 ***************************************/

static void
fli_reshape_form( FL_FORM * form )
{
    FL_Coord w,
		     h;

    fl_get_wingeometry( form->window, &form->x, &form->y, &w, &h );
    fl_set_form_size( form, w, h );
}


/***************************************
 * Scale a form with the given scaling factors and take care of object
 * gravity. This one differs from fl_scale_form() in the fact that we
 * don't reshape the window in any way. Most useful as a follow up to
 * ConfigureNotify event
 ***************************************/

static void
fli_scale_form( FL_FORM * form,
				double    xsc,
				double    ysc )
{
    FL_OBJECT *obj;
    double neww = form->w_hr * xsc,
		   newh = form->h_hr * ysc;

    if ( FL_abs( neww - form->w ) < 1 && FL_abs( newh - form->h ) < 1 )
		return;

	form->w_hr = neww;
	form->h_hr = newh;

	form->w = FL_crnd( neww );
	form->h = FL_crnd( newh );

    if ( form->hotx >= 0 || form->hoty >= 0 )
    {
		form->hotx = form->hotx * xsc;
		form->hoty = form->hoty * ysc;
    }

    /* need to handle different resizing request */

    for ( obj = form->first; obj; obj = obj->next )
    {
		double oldw = obj->fl2 - obj->fl1;
		double oldh = obj->ft2 - obj->ft1;

		/* Special case to keep the center of gravity of obejcts that have
		   no gravity set and aren't to be resized */

		if (    obj->resize == FL_RESIZE_NONE
			 && obj->segravity == FL_NoGravity
			 && obj->nwgravity == FL_NoGravity )
		{
			obj->fl1 += ( xsc - 1 ) * ( obj->fl1 + 0.5 * oldw );
			obj->ft1 += ( ysc - 1 ) * ( obj->ft1 + 0.5 * oldh );
			obj->fr1 = neww - obj->fl1;
			obj->fb1 = newh - obj->ft1;

			obj->fl2 = obj->fl1 + oldw;
			obj->ft2 = obj->ft1 + oldh;
			obj->fr2 = neww - obj->fl2;
			obj->fb2 = newh - obj->ft2;
		}
		else
		{
			/* In all other cases we recalculate the position of the upper left
			   hand and the lower right hand corner of the object relative to
			   all the borders of the form enclosing it, taking gravity and
			   resizing setting into account. The results sometimes can be
			   unexpected but hopefully are logically correct;-) */

			if ( ULC_POS_LEFT_FIXED( obj ) )
				obj->fr1 = neww - obj->fl1;
			else if ( ULC_POS_RIGHT_FIXED( obj ) )
				obj->fl1 = neww - obj->fr1;

			if ( LRC_POS_LEFT_FIXED( obj ) )
				obj->fr2 = neww - obj->fl2;
			else if ( LRC_POS_RIGHT_FIXED( obj ) )
				obj->fl2 = neww - obj->fr2;

			if ( ! HAS_FIXED_HORI_ULC_POS( obj ) )
			{
				if ( HAS_FIXED_HORI_LRC_POS( obj ) )
				{
					if ( obj->resize & FL_RESIZE_X )
						obj->fl1 = obj->fl2 - xsc * oldw;
					else
						obj->fl1 = obj->fl2 - oldw;
				}
				else
					obj->fl1 *= xsc;
					
				obj->fr1 = neww - obj->fl1;
			}

			if ( ! HAS_FIXED_HORI_LRC_POS( obj ) )
			{
				if ( obj->resize & FL_RESIZE_X )
					obj->fl2 = obj->fl1 + xsc * oldw;
				else
					obj->fl2 = obj->fl1 + oldw;
	
				obj->fr2 = neww - obj->fl2;
			}
			
			if ( ULC_POS_TOP_FIXED( obj ) )
				obj->fb1 = newh - obj->ft1;
			else if ( ULC_POS_BOTTOM_FIXED( obj ) )
				obj->ft1 = newh - obj->fb1;

			if ( LRC_POS_TOP_FIXED( obj ) )
				obj->fb2 = newh - obj->ft2;
			else if ( LRC_POS_BOTTOM_FIXED( obj ) )
				obj->ft2 = newh - obj->fb2;

			if ( ! HAS_FIXED_VERT_ULC_POS( obj ) )
			{
				if ( HAS_FIXED_VERT_LRC_POS( obj ) )
				{
					if ( obj->resize & FL_RESIZE_Y )
						obj->ft1 = obj->ft2 - ysc * oldh;
					else
						obj->ft1 = obj->ft2 - oldh;
				}
				else
					obj->ft1 *= ysc;

				obj->fb1 = newh - obj->ft1;
			}

			if ( ! HAS_FIXED_VERT_LRC_POS( obj ) )
			{
				if ( obj->resize & FL_RESIZE_Y )
					obj->ft2 = obj->ft1 + ysc * oldh;
				else
					obj->ft2 = obj->ft1 + oldh;
	
				obj->fb2 = newh - obj->ft2;
			}
		}

		obj->x = FL_crnd( obj->fl1 );
		obj->y = FL_crnd( obj->ft1 );
		obj->w = FL_crnd( obj->fl2 - obj->fl1 );
		obj->h = FL_crnd( obj->ft2 - obj->ft1 );

		if ( fl_inverted_y )
			obj->y = form->h - obj->h - obj->y;

		fli_handle_object_direct( obj, FL_RESIZED, 0, 0, 0, 0 );
    }
}


/***************************************
 * Externally visible routine to scale a form. Need to reshape the window
 ***************************************/

void
fl_scale_form( FL_FORM * form,
			   double    xsc,
			   double    ysc )
{
    if ( form == NULL )
    {
		M_err( "fl_scale_form", "NULL form." );
		return;
    }

    if (    FL_crnd( form->w_hr * xsc ) == form->w
		 && FL_crnd( form->h_hr * ysc ) == form->h )
		return;

	fli_scale_form( form, xsc, ysc );

    /* resize the window */

    if ( form->visible > 0 )
		fl_winresize( form->window, form->w, form->h );
}


/***************************************
 ***************************************/

void
fl_set_form_minsize( FL_FORM * form,
					 FL_Coord  w,
					 FL_Coord  h )
{
    if ( ! form )
    {
		M_err( "fl_set_form_minsize", "Null form" );
		return;
    }

    fl_winminsize( form->window, w, h );
}


/***************************************
 ***************************************/

void
fl_set_form_maxsize( FL_FORM * form,
					 FL_Coord  w,
					 FL_Coord  h )
{
    if ( ! form )
    {
		M_err( "fl_set_form_maxsize", "NULL form." );
		return;
    }

    fl_winmaxsize( form->window, w, h );
}


/***************************************
 ***************************************/

void
fl_set_form_dblbuffer( FL_FORM * form,
					   int       y )
{
    if ( ! form )
    {
		M_err( "fl_set_form_dblbuffer", "NULL form." );
		return;
    }

    form->use_pixmap = y;
}


/***************************************
 * Sets the size of the form on the screen.
 ***************************************/

void
fl_set_form_size( FL_FORM * form,
				  FL_Coord  w,
				  FL_Coord  h )
{
    if ( ! form )
    {
		M_err( "fl_set_form_size", "NULL form." );
		return;
    }

    if ( w != form->w || h != form->h )
		fl_scale_form( form, w / form->w_hr, h / form->h_hr );
}


/***************************************
 * Sets the position of the form on the screen. Note that the location
 * is specified relative to lower-left corner
 ***************************************/

void
fl_set_form_position( FL_FORM * form,
					  FL_Coord  x,
					  FL_Coord  y )
{
    FL_Coord oldx,
		     oldy;

    if ( ! form )
    {
		M_err( "fl_set_form_position", "NULL form." );
		return;
    }

    oldx = form->x;
    oldy = form->y;
    form->x = x >= 0 ? x : ( fl_scrw + x );
    form->y = y >= 0 ? y : ( fl_scrh + y );

    if ( form->visible && ( oldx != form->x || oldy != form->y ) )
		XMoveWindow( flx->display, form->window, form->x, form->y );
}


/***************************************
 * Sets the position of the form on the screen.
 ***************************************/

void
fl_set_form_hotspot( FL_FORM * form,
					 FL_Coord  x,
					 FL_Coord  y )
{
    if ( ! form )
    {
		M_err( "fl_set_form_hotspot", "NULL form." );
		return;
    }

    form->hotx = x;
    form->hoty = y;
}


/***************************************
 ***************************************/

void
fl_set_form_hotobject( FL_FORM   * form,
					   FL_OBJECT * ob )
{
    if ( ! form  )
    {
		M_err( "fl_set_form_hotobject", "NULL form." );
		return;
    }

    if ( ! ob )
    {
		M_err( "fl_set_form_hotobject", "NULL object." );
		return;
    }


	fl_set_form_hotspot( form, ob->x + ob->w / 2, ob->y + ob->h / 2 );
}


/***************************************
 * make sure a form is completely visible
 ***************************************/

static void
fli_force_visible( FL_FORM * form,
				   int       itx,
				   int       ity )
{
    if ( form->x < itx )
		form->x = itx;

    if ( form->x > fl_scrw - form->w - 2 * itx )
		form->x = fl_scrw - form->w - 2 * itx;

    if ( form->y < ity )
		form->y = ity;

    if ( form->y > fl_scrh - form->h - itx )
		form->y = fl_scrh - form->h - 2 * itx;

    /* be a paranoid */
    if ( form->x < 0 || form->x > fl_scrw - form->w )
    {
		if ( form->w < fl_scrw - 20 )
			M_err( "fli_force_visible", "Can't happen x=%d", form->x );
		form->x = 10;
    }

    if ( form->y < 0 || form->y > fl_scrh - form->h )
    {
		M_warn( "fli_force_visible", "Can't happen y=%d", form->y );
		form->y = 20;
    }
}


/***************************************
 ***************************************/

void
fl_set_form_title( FL_FORM *    form,
				   const char * name )
{
    if ( ! form )
    {
		M_err( "fl_set_form_title", "NULL form." );
		return;
    }

    if ( form->label != name )
    {
		if ( form->label )
			fl_free( form->label );
		form->label = fl_strdup( name ? name : "" );
    }

    if ( form->window )
		fl_wintitle( form->window, form->label );
}


/***************************************
 * Displays a particular form. Returns window handle.
 ***************************************/

static int has_initial;
static int unmanaged_count;

long
fl_prepare_form_window( FL_FORM    * form,
						int          place,
						int          border,
						const char * name )
{
    long screenw,
		 screenh;
    int itx = 0,
		ity = 0,
		dont_fix_size = 0;
    FL_Coord mx,
		     my,
		     nmx,
		     nmy;

    if ( border == 0 )
		border = FL_FULLBORDER;

    if ( fl_current_form )
    {
		M_err( "fl_prepare_form_window", "You forgot to call fl_end_form %s",
			   name ? name : "" );
		fl_current_form = NULL;
    }

    if ( form == NULL )
    {
		M_err( "fl_show_form", "NULL form." );
		return -1;
    }

    if ( form->visible )
		return form->window;

	/* Try to move the form from the part of the list for hidden forms to
	   tha at the start for visible forms */

	fli_move_form_to_visible_list( form );

    if ( form->label != name )
    {
		if ( form->label )
			fl_free( form->label );
		form->label = fl_strdup( name ? name : "" );
    }

    /* if we are using private colormap or non-default visual, unmanaged
       window will not get correct colormap installed by the WM
       automatically. Make life easier by forcing a managed window. fl_vroot
       stuff is a workaround for tvtwm */

#if 0
    if (    border != FL_FULLBORDER
		 && (    fl_state[fl_vmode].pcm
			  || fli_visual( fl_vmode ) !=
				                    DefaultVisual( flx->display, fl_screen ) ) )
/*            || fl_root != fl_vroot ) )  */
    {
		border = FL_TRANSIENT;
    }
#endif

    if ( border != FL_NOBORDER )
    {
		FL_WM_STUFF *fb = &fl_wmstuff;

		itx = fb->bw + ( border == FL_TRANSIENT ? fb->trpx : fb->rpx );
		ity = fb->bw + ( border == FL_TRANSIENT ? fb->trpy : fb->rpy );
    }
    else
		unmanaged_count++;

    form->wm_border = border;
    form->deactivated = 0;
    screenw = fl_scrw;
    screenh = fl_scrh;

    fl_get_mouse( &mx, &my, &fli_keymask );

    if ( ( dont_fix_size = place & FL_FREE_SIZE ) )
		place &= ~ FL_FREE_SIZE;

    if ( place == FL_PLACE_SIZE )
		fl_pref_winsize( form->w, form->h );
    else if ( place == FL_PLACE_ASPECT )
		fl_winaspect( 0, form->w, form->h );
    else if ( place == FL_PLACE_POSITION )
    {
		if ( fl_wmstuff.rep_method == FL_WM_SHIFT && border != FL_NOBORDER )
		{
			form->x -= itx;
			form->y -= ity;
		}
		fl_pref_winposition( form->x, form->y );
		fl_initial_winsize( form->w, form->h );
    }
    else if ( place != FL_PLACE_FREE )
    {
		switch ( place )
		{
			case FL_PLACE_CENTER:
			case FL_PLACE_FREE_CENTER:
				form->x = ( screenw - form->w ) / 2;
				form->y = ( screenh - form->h ) / 2;
				break;

			case FL_PLACE_MOUSE:
				form->x = mx - form->w / 2;
				form->y = my - form->h / 2;
				break;

			case FL_PLACE_FULLSCREEN:
				form->x = 0;
				form->y = 0;
				fl_set_form_size( form, screenw, screenh );
				break;

			case FL_PLACE_HOTSPOT:
				if ( form->hotx < 0 )
				{			                    /* never set */
					form->hotx = form->w / 2;
					form->hoty = form->h / 2;
				}

				nmx = mx;
				nmy = my;
				form->x = mx - form->hotx;
				form->y = my - form->hoty;
				fli_force_visible( form, itx, ity );
				nmx = form->x + form->hotx;
				nmy = form->y + form->hoty;
				if ( nmx != mx || nmy != my )
					fl_set_mouse( nmx, nmy );
				break;
		}

		if ( place == FL_PLACE_GEOMETRY )
		{
			/* Correct form position. X < 0 means measure from right */

			if ( form->x < 0 )
				form->x = screenw + form->x - 2 - itx;

			/* y < 0 means from right */

			if ( form->y < 0 )
				form->y = screenh + form->y - 2 - ity;
		}

		/* final check. Make sure form is visible */

		fli_force_visible( form, itx, ity );

		/* take care of reparenting stuff */

		if ( fl_wmstuff.rep_method == FL_WM_SHIFT && border != FL_NOBORDER )
		{
			form->x -= itx;
			form->y -= ity;
		}

		if ( dont_fix_size && place != FL_PLACE_GEOMETRY )
			fl_initial_wingeometry( form->x, form->y, form->w, form->h );
		else
			fl_pref_wingeometry( form->x, form->y, form->w, form->h );
    }
    else if ( place == FL_PLACE_FREE )
    {
		fl_initial_winsize( form->w, form->h );
		if ( has_initial )
		{
			if ( fl_wmstuff.rep_method == FL_WM_SHIFT && border != FL_NOBORDER )
			{
				form->x -= itx;
				form->y -= ity;
			}
			fl_initial_wingeometry( form->x, form->y, form->w, form->h );
		}
    }
    else
    {
		M_err( "fl_prepare_form_window", "Unknown requests: %d", place );
		fl_initial_wingeometry( form->x, form->y, form->w, form->h );
    }

    /* WM typically does not allow dragging transient windows */

    if ( border != FL_FULLBORDER )
    {
		if ( place == FL_PLACE_ASPECT || place == FL_PLACE_FREE )
		{
			form->x = mx - form->w / 2;
			form->y = my - form->h / 2;
			fli_force_visible( form, itx, ity );
			fl_initial_winposition( form->x, form->y );
		}

		if ( border == FL_NOBORDER )
			fl_noborder( );
		else
			fl_transient( );
    }

    form->vmode = fl_vmode;

    if ( place == FL_PLACE_ICONIC )
		fl_initial_winstate( IconicState );
    if ( form->icon_pixmap )
		fl_winicon( 0, form->icon_pixmap, form->icon_mask );

    has_initial = 0;
    fli_init_colormap( fl_vmode );

    form->window = fli_create_window( fl_root, fli_colormap( fl_vmode ), name );
    fl_winicontitle( form->window, name );

    if ( border == FL_FULLBORDER || form->prop & FLI_COMMAND_PROP )
		fli_set_form_property( form, FLI_COMMAND_PROP );

    return form->window;
}


/***************************************
 ***************************************/

long
fl_show_form_window( FL_FORM * form )
{
    if ( ! form  )
    {
		M_err( "fl_show_form_window", "NULL form." );
		return None;
    }

    if ( form->window == None || form->visible )
		return form->window;

    fl_winshow( form->window );
    form->visible = 1;
    fli_reshape_form( form );
	fl_redraw_form( form );

    return form->window;
}


/***************************************
 ***************************************/

long
fl_show_form( FL_FORM *    form,
			  int          place,
			  int          border,
			  const char * name )
{
    if ( ! form  )
    {
		M_err( "fl_show_form", "NULL form." );
		return None;
    }

    fl_prepare_form_window( form, place, border, name );
    return fl_show_form_window( form );
}


/***************************************
 * Hides a particular form
 ***************************************/

static void
fli_close_form_win( Window win )
{
    XEvent xev;

    XUnmapWindow( flx->display, win );
    XDestroyWindow( flx->display, win );

    XSync( flx->display, 0 );
    while ( XCheckWindowEvent( flx->display, win, AllEventsMask, &xev ) )
		fli_xevent_name( "Eaten", &xev );

    /* Gives subwindows a chance to handle destroy event promptly, take care
	   the window of the form doesn't exist anymore! */

    while ( XCheckTypedEvent( flx->display, DestroyNotify, &xev ) )
    {
		FL_FORM *form;

		if ( ( form = fli_find_event_form( &xev ) ) )
		{
			form->window = None;
			fl_hide_form( form );
		}
		else
			fl_XPutBackEvent( &xev );
    }
}


/***************************************
 ***************************************/

static FL_FORM *
fli_property_set( unsigned int prop )
{
    int i;

    for ( i = 0; i < formnumb; i++ )
		if ( forms[ i ]->prop & prop && forms[ i ]->prop & FLI_PROP_SET )
			return forms[ i ];

    return NULL;
}


/***************************************
 ***************************************/

static void
fli_set_form_property( FL_FORM *    form,
					  unsigned int prop )
{
    if ( ! form  )
    {
		M_err( "fli_set_form_property", "NULL form." );
		return;
    }

    if ( fli_property_set( prop ) )
		return;

    if ( ! ( prop & FLI_COMMAND_PROP ) )
	{
		M_err( "fli_set_form_property", "Unknown form property request %u",
			   prop );
		return;
	}

	if ( form->window )
	{
		fli_set_winproperty( form->window, FLI_COMMAND_PROP );
		form->prop |= FLI_PROP_SET;
	}

	form->prop |= FLI_COMMAND_PROP;
	fli_mainform = form;
}


/***************************************
 ***************************************/

void
fl_hide_form( FL_FORM * form )
{
    Window owin;
	FL_OBJECT *o;

    if ( ! form )
    {
		M_err( "fl_hide_form", "NULL form" );
		return;
    }

    if ( fli_get_visible_forms_index( form ) < 0 )
    {
		M_err( "fl_hide_form", "Hiding unknown form" );
		return;
    }

    if ( form->visible == FL_BEING_HIDDEN )
    {
		M_err( "fl_hide_form", "recursive call ?" );
		/* return; */
    }

    form->visible = FL_BEING_HIDDEN;
    fli_set_form_window( form );

    /* checking mouseobj->form is necessary as it might be deleted from a
       form */

    if ( fl_mouseobj != NULL && fl_mouseobj->form == form )
    {
#if FL_DEBUG >= ML_WARN
		if ( ! fl_mouseobj->visible )
			M_err( "fl_hide_form", "Outdated mouseobj %s",
				   fl_mouseobj->label ? fl_mouseobj->label : "" );
#endif
		fli_handle_object( fl_mouseobj, FL_LEAVE, 0, 0, 0, 0 );
		fl_mouseobj = NULL;
    }

    if ( fl_pushobj != NULL && fl_pushobj->form == form )
    {
		fli_handle_object( fl_pushobj, FL_RELEASE, 0, 0, 0, 0 );
		fl_pushobj = NULL;
    }

    if ( form->focusobj != NULL )
    {
		fli_handle_object( form->focusobj, FL_UNFOCUS, 0, 0, 0, NULL );
		form->focusobj = NULL;
    }

	/* Get canvas objects to unmap their windows */

	for ( o = form->first; o; o = o->next )
		if ( o->objclass == FL_CANVAS || o->objclass == FL_GLCANVAS )
			fli_unmap_canvas_window( o );

#ifdef DELAYED_ACTION
    fli_object_qflush( form );
#endif

    /* free backing store pixmap but keep the pointer */

    fli_free_flpixmap( form->flpixmap );

    if ( mouseform && mouseform->window == form->window )
		mouseform = NULL;

    form->deactivated = 1;
    form->visible = FL_INVISIBLE;
    owin = form->window;
    form->window = None;

    fli_hide_tooltip( );

	/* If the forms window is None it already has been closed */

	if ( owin )
		fli_close_form_win( owin );

	if ( flx->win == owin )
		flx->win = None;

	/* Move the form from the part of the list for visible forms to the
	   part of hidden forms at the end of the array */

	fli_move_form_to_hidden_list( form );

    if ( form->wm_border == FL_NOBORDER )
    {
		unmanaged_count--;
		if ( unmanaged_count < 0 )
		{
			M_err( "fl_hide_form", "Bad unmanaged count" );
			unmanaged_count = 0;
		}
    }

    /* need to re-establish command property */

    if ( formnumb && form->prop & FLI_COMMAND_PROP )
		fli_set_form_property( forms[ 0 ], FLI_COMMAND_PROP );

    if ( form == keyform )
		keyform = NULL;
}


/***************************************
 * Frees the memory used by a form, together with all its objects.
 ***************************************/

void
fl_free_form( FL_FORM * form )
{
    /* Check whether ok to free */

    if ( ! form )
    {
		M_err( "fl_free_form", "NULL form." );
		return;
    }

    if ( fli_get_hidden_forms_index( form ) < 0 )
    {
		M_err( "fl_free_form", "Freeing unknown form" );
		return;
    }

    if ( form->visible == FL_VISIBLE )
    {
		M_err( "fl_free_form", "Freeing visible form." );
		fl_hide_form( form );
    }

    /* Free all objects of the form */

	while ( form->first )
	{
		if ( form->first->child )
		{
			fl_free_object( form->first->child );
			form->first->child = NULL;
		}

		fl_free_object( form->first );
    }

    if ( form->flpixmap )
    {
		fli_free_flpixmap( form->flpixmap );
		fl_free( form->flpixmap );
		form->flpixmap = NULL;
    }

    if ( form->label )
    {
		fl_free( form->label );
		form->label = NULL;
    }

    if ( form == fli_mainform )
		fli_mainform = NULL;

    /* Free the form and remove it from the list of existing forms */

	fl_free( form );

	fli_remove_form_from_hidden_list( form );
}


/***************************************
 * activates a form
 ***************************************/

void
fl_activate_form( FL_FORM * form )
{
    if ( ! form )
    {
		M_err( "fl_activate_form", "NULL form." );
		return;
    }

    if ( form->deactivated )
    {
		form->deactivated--;

		if ( ! form->deactivated && form->activate_callback )
			form->activate_callback( form, form->activate_data );
    }

    if ( form->child )
		fl_activate_form( form->child );
}


/***************************************
 * deactivates a form
 ***************************************/

void
fl_deactivate_form( FL_FORM * form )
{
    if ( ! form )
    {
		M_err( "fl_deactivate_form", "NULL form." );
		return;
    }

    if (    ! form->deactivated
		 && fl_mouseobj != NULL
		 && fl_mouseobj->form == form )
		fli_handle_object( fl_mouseobj, FL_LEAVE, 0, 0, 0, 0 );

    if ( ! form->deactivated && form->deactivate_callback )
		form->deactivate_callback( form, form->deactivate_data );

    form->deactivated++;

    if ( form->child )
		fl_deactivate_form( form->child );
}


/***************************************
 ***************************************/

FL_FORM_ATACTIVATE
fl_set_form_atactivate( FL_FORM            * form,
						FL_FORM_ATACTIVATE   cb,
						void *               data )
{
    FL_FORM_ATACTIVATE old = NULL;

    if ( ! form  )
    {
		M_err( "fl_set_form_atactivate", "NULL form." );
		return NULL;
    }

	old = form->activate_callback;
	form->activate_callback = cb;
	form->activate_data = data;

    return old;
}


/***************************************
 ***************************************/

FL_FORM_ATDEACTIVATE
fl_set_form_atdeactivate( FL_FORM              * form,
						  FL_FORM_ATDEACTIVATE   cb,
						  void                 * data )
{
    FL_FORM_ATDEACTIVATE old = NULL;

    if ( ! form  )
    {
		M_err( "fl_set_form_atdeactivate", "NULL form." );
		return NULL;
    }

	old = form->deactivate_callback;
	form->deactivate_callback = cb;
	form->deactivate_data = data;

    return old;
}


/***************************************
 * activates all forms
 ***************************************/

void
fl_activate_all_forms( void )
{
    int i;

    for ( i = 0; i < formnumb; i++ )
		fl_activate_form( forms[ i ] );
}


/***************************************
 * deactivates all forms
 ***************************************/

void
fl_deactivate_all_forms( void )
{
    int i;

    for ( i = 0; i < formnumb; i++ )
		fl_deactivate_form( forms[ i ] );
}


/***************************************
 * should pass the mask instead of button numbers into the
 * event handler. basically throwing away info ..
 ***************************************/

static int
fli_xmask2key( unsigned int mask )
{
    /* once the FL_XXX_MOUSE is changed to mask, just loose the else */

    if ( mask & Button1Mask )
		return FL_LEFT_MOUSE;

    if ( mask & Button2Mask )
		return FL_MIDDLE_MOUSE;

    if ( mask & Button3Mask )
		return FL_RIGHT_MOUSE;

    if ( mask & Button4Mask )
		return FL_SCROLLUP_MOUSE;

    if ( mask & Button5Mask )
		return FL_SCROLLDOWN_MOUSE;

    return 0;
}


/***************************************
 * a radio object is pushed
 ***************************************/

void
fli_do_radio_push( FL_OBJECT * obj,
				   FL_Coord    x,
				   FL_Coord    y,
				   int         key,
				   void      * xev )
{
	FL_OBJECT *o = obj;

	if ( ! obj || ! obj->radio || fl_get_button( obj ) )
		return;

    /* If this radio button does not belong to any group we have to search
	   the entire form, otherwise just the members of the group */

    if ( obj->group_id == 0 )
	{
		for ( o = obj->form->first; o; o = o->next )
			if (    o != obj
				 && o->radio
				 && o->group_id == 0
				 && fl_get_button( o ) )
			{
				fli_handle_object_direct( o, FL_RELEASE, x, y, key, xev );
				break;
			}
	}
    else
    {
		while ( o->prev && o->prev->objclass != FL_BEGIN_GROUP )
			o = o->prev;

		for ( ; o && o->objclass != FL_END_GROUP; o = o->next )
			if ( o != obj && o->radio && fl_get_button( o ) )
			{
				fli_handle_object_direct( o, FL_RELEASE, x, y, key, xev );
				break;
			}
    }

	fli_handle_object( obj, FL_PUSH, x, y, key, xev );
}


/***************************************
 ***************************************/

static int
fli_do_shortcut2( FL_FORM  * form,
				 int        key,
				 FL_Coord   x,
				 FL_Coord   y,
				 XEvent   * xev )
{
    int key1,
		key2;
    FL_OBJECT *obj;
	long *s;

    /* Check whether the <Alt> key is pressed */

    key1 = key2 = key;

    if ( fl_keypressed( XK_Alt_L ) || fl_keypressed( XK_Alt_R ) )
    {
		if ( key < 256 )
		{
			/* always a good idea to make Alt_k case insensitive */

			key1 = FL_ALT_VAL
				   + ( islower( key ) ? toupper( key ) : tolower( key ) );
			key2 = key + FL_ALT_VAL;
		}
		else
			key1 = key2 = key + FL_ALT_VAL;
    }

    M_info( "fli_do_shortcut2", "win=%lu key=%d %d %d",
			form->window, key, key1, key2 );

    /* Check whether an object has this as a shortcut */

    for ( obj = form->first; obj; obj = obj->next )
    {
		if ( ! obj->visible || obj->active <= 0 || ! obj->shortcut )
			continue;

		for ( s = obj->shortcut; *s; s++ )
		{
			if ( ! ( *s == key1 || *s == key2 ) )
				continue;

			if ( obj->objclass == FL_INPUT )
			{
				if ( obj != form->focusobj )
				{
					fli_handle_object( form->focusobj, FL_UNFOCUS,
									   x, y, 0, xev );
					fli_handle_object( obj, FL_FOCUS, x, y, 0, xev );
				}
			}
			else
			{
				if ( obj->radio )
					fli_do_radio_push( obj, x, y, FL_MBUTTON1, xev );

				XAutoRepeatOff( flx->display );
				if ( ! obj->radio )
					fli_handle_object( obj, FL_SHORTCUT, x, y, key1, xev );
				fli_context->mouse_button = FL_SHORTCUT + key1;

				/* this is not exactly correct as shortcut might quit,
				   fl_finish will restore the keyboard state */

				if ( fl_keybdcontrol.auto_repeat_mode == AutoRepeatModeOn )
					XAutoRepeatOn( flx->display );
			}

			return 1;
		}
    }

    return 0;
}


/***************************************
 ***************************************/

int
fli_do_shortcut( FL_FORM  * form,
				 int        key,
				 FL_Coord   x,
				 FL_Coord   y,
				 XEvent   * xev )
{
    int ret = fli_do_shortcut2( form, key, x, y, xev );

    if ( ! ret )
    {
		if ( form->child )
			ret = fli_do_shortcut2( form->child, key, x, y, xev );
		if ( ! ret && form->parent )
			ret = fli_do_shortcut2( form->parent, key, x, y, xev );
    }

    return ret;
}


/***************************************
 ***************************************/

static void
fli_keyboard( FL_FORM  * form,
			 int        key,
			 FL_Coord   x,
			 FL_Coord   y,
			 void     * xev )
{
    FL_OBJECT *obj,
		      *special;

    /* Always check shortcut first */

    if ( fli_do_shortcut( form, key, x, y, xev ) )
		return;

    /* Focus policy is done as follows: Input object has the highiest
       priority. Next is the object that wants special keys which is followed
       by mouseobj that has the lowest. Focusobj == FL_INPUT OBJ */

    special = fli_find_first( form, FL_FIND_KEYSPECIAL, 0, 0 );
    obj = special ?
		      fli_find_object( special->next, FL_FIND_KEYSPECIAL, 0, 0 ) : NULL;

    /* If two or more objects that want keyboard input, none will get it and
       keyboard input will go to mouseobj instead */

    if ( obj && obj != special )
		special = fl_mouseobj;

    if ( form->focusobj )
    {
		FL_OBJECT *focusobj = form->focusobj;

		/* handle special keys first */

		if ( key > 255 )
		{
			if (    IsLeft( key )
				 || IsRight( key )
				 || IsHome( key )
				 || IsEnd( key ) )
				fli_handle_object( focusobj, FL_KEYBOARD, x, y, key, xev );
			else if (    (    IsUp( key )
						   || IsDown( key )
						   || IsPageUp( key )
						   || IsPageDown( key ) )
					  && focusobj->wantkey & FL_KEY_TAB )
				fli_handle_object( focusobj, FL_KEYBOARD, x, y, key, xev );
			else if ( special && special->wantkey & FL_KEY_SPECIAL )
			{
				/* moving the cursor in input field that does not have focus
				   looks weird */

				if ( special->objclass != FL_INPUT )
					fli_handle_object( special, FL_KEYBOARD, x, y, key, xev );
			}
			else if ( key == XK_BackSpace || key == XK_Delete )
				fli_handle_object( focusobj, FL_KEYBOARD, x, y, key, xev );
			return;
		}

		/* dispatch tab & return switches focus (for return only if not
		   FL_KEY_TAB) */

		if (    key == '\t'
			 || ( key == '\r' && ! ( focusobj->wantkey & FL_KEY_TAB ) ) )
		{
			if ( ( ( XKeyEvent * ) xev )->state & fli_context->navigate_mask )
			{
				if ( focusobj == fli_find_first( form, FL_FIND_INPUT, 0, 0 ) )
					obj = fli_find_last( form, FL_FIND_INPUT, 0, 0 );
				else
					obj = fli_find_object_backwards( focusobj->prev,
													 FL_FIND_INPUT, 0, 0 );
			}
			else
				obj = fli_find_object( focusobj->next, FL_FIND_INPUT, 0, 0 );

			if ( obj == NULL )
				obj = fli_find_first( form, FL_FIND_INPUT, 0, 0 );

			if ( obj != NULL )
			{
				fli_handle_object( focusobj, FL_UNFOCUS, x, y, 0, xev );
				fli_handle_object( obj, FL_FOCUS, x, y, 0, xev );
			}
		}
		else if ( focusobj->wantkey != FL_KEY_SPECIAL )
			fli_handle_object( focusobj, FL_KEYBOARD, x, y, key, xev );
		return;
    }

    /* keyboard input is not wanted */

    if ( ! special || special->wantkey == 0 )
		return;

    /* space is an exception for browser */

    if ( ( key > 255 || key == ' ' ) && special->wantkey & FL_KEY_SPECIAL )
		fli_handle_object( special, FL_KEYBOARD, x, y, key, xev );
    else if ( key < 255 && special->wantkey & FL_KEY_NORMAL )
		fli_handle_object( special, FL_KEYBOARD, x, y, key, xev );
    else if ( special->wantkey == FL_KEY_ALL )
		fli_handle_object( special, FL_KEYBOARD, x, y, key, xev );

#if FL_DEBUG >= ML_INFO1
    M_info( "fl_keyboard", "(%d %d)pushing %d to %s\n",
			x, y, key, special->label );
#endif
}


/***************************************
 * updates a form according to an event
 ***************************************/

void
fli_handle_form( FL_FORM * form,
				 int       event,
				 int       key,
				 XEvent  * xev )
{
    FL_OBJECT *obj = NULL;
    FL_Coord x,
		     y;

    if ( ! form || ! form->visible )
		return;

    if ( form->deactivated && event != FL_DRAW )
		return;

    if (    form->parent_obj
		 && form->parent_obj->active == DEACTIVATED
		 && event != FL_DRAW )
		return;

    if ( event != FL_STEP )
		fli_set_form_window( form );

	if ( fli_query_age > 0 )
	{
		fl_get_form_mouse( form, &fli_mousex, &fli_mousey, &fli_keymask );
		if ( event != FL_KEYBOARD )
			key = fli_xmask2key( fli_keymask );
		fli_query_age = 0;
	}

	x = fli_mousex;
	y = fli_mousey;

    if ( event != FL_STEP && event != FL_DRAW )
		obj = fli_find_last( form, FL_FIND_MOUSE, x, y );

    switch ( event )
    {
		case FL_DRAW:		/* form must be redrawn */
			fli_redraw_form_using_xevent( form, key, xev );
			break;

		case FL_ENTER:		/* Mouse did enter the form */
			fl_mouseobj = obj;
			fli_handle_object( fl_mouseobj, FL_ENTER, x, y, 0, xev );
			break;

		case FL_LEAVE:		         /* Mouse left the form */
			fli_handle_object( fl_mouseobj, FL_LEAVE, x, y, 0, xev );
			if ( fl_pushobj == fl_mouseobj )
				fl_pushobj = NULL;
			fl_mouseobj = NULL;
			break;

		case FL_PUSH:		/* Mouse was pushed inside the form */

			/* Change focus: If an input object has the focus make it lose it
			   (and thus report changes) and then set the focus to either the
			   object that got pushed (if it's an input object) or back to the
			   original one. Then we have to recheck that the object the
			   FL_PUSH was for is still active - it may have become deactivated
			   due to the handler for the object that became unfocused! */

			if ( obj && form->focusobj && form->focusobj != obj )
			{
				FL_OBJECT *old_focusobj = form->focusobj;

				fli_handle_object( form->focusobj, FL_UNFOCUS, x, y, key, xev );

				if ( ! obj->input )
					fli_handle_object( old_focusobj, FL_FOCUS, x, y, key, xev );

				if ( obj->active == DEACTIVATED )
					break;
			}

			if ( obj && obj->input )
				fli_handle_object( obj, FL_FOCUS, x, y, key, xev );

			if ( form->focusobj )
				keyform = form;

			/* Radio button only get handled on button release, other objects
			   get the button press unless focus is overriden  */

			if (    obj
				 && ! obj->radio
				 && ( ! obj->input || ( obj->input && obj->focus ) ) )
			{
				fli_handle_object( obj, FL_PUSH, x, y, key, xev );
				fl_pushobj = obj;
			}
			else if ( obj && obj->radio )
				fli_do_radio_push( obj, x, y, key, xev );
			break;

		case FL_RELEASE:		/* Mouse button was released inside the form */
			if ( fl_pushobj )
			{
				obj = fl_pushobj;
				fl_pushobj = NULL;
				fli_handle_object( obj, FL_RELEASE, x, y, key, xev );

			}
			break;

		case FL_MOTION:		     /* Mouse position changed in the form */
			/* "Pushable" objects always get FL_MOTION events. Since there's
			   no direct EnterNotify or LeaveNotify event for objects we
			   "fake" them when an object gets entered or left. */

			if ( fl_pushobj != NULL )
				fli_handle_object( fl_pushobj, FL_MOTION, x, y, key, xev );
			else if ( obj != fl_mouseobj )
			{
				fli_handle_object( fl_mouseobj, FL_LEAVE, x, y, 0, xev );
				fli_handle_object( fl_mouseobj = obj, FL_ENTER, x, y, 0, xev );
			}

			/* Objects can declare that they want FL_MOTION events even
			   though they're not "pushable" objects e.g. because they
			   have some internal structure that depends on the mouse
			   position (e.g. choice and counter objects) . */

			if ( obj != fl_pushobj && obj && obj->want_motion )
				fli_handle_object( obj, FL_MOTION, x, y, key, xev );

			break;

		case FL_KEYBOARD:		/* A key was pressed */
			fli_keyboard( form, key, x, y, xev );
			break;

		case FL_STEP:		/* A simple step */
			obj = fli_find_first( form, FL_FIND_AUTOMATIC, 0, 0 );

			if ( obj )
				fli_set_form_window( form );	/* set only if required */

			while ( obj )
			{
				fli_handle_object( obj, FL_STEP, x, y, 0, xev );
				obj = fli_find_object( obj->next, FL_FIND_AUTOMATIC, 0, 0 );
			}
			break;

		case FL_UPDATE:
			/* "Pushable" objects may request an FL_UPDATE event by an
			   artificial (but not very precise) timer.*/

			if ( fl_pushobj && fl_pushobj->want_update )
				fli_handle_object( fl_pushobj, FL_UPDATE, x, y, key, xev );
			break;

		case FL_MOVEORIGIN:
		case FL_OTHER:
			/* need to dispatch it thru all objects and monitor the status of
			   forms as it may get closed */

			for ( obj = form->first; obj && form->visible; obj = obj->next )
				if ( obj->visible )
					fli_handle_object( obj, event, x, y, key, xev );
			break;
    }
}


static long lastsec[ FLI_NTIMER ];
static long lastusec[ FLI_NTIMER ];

typedef struct
{
    long sec;
    long usec;
} TimeVal;

static TimeVal tp;


/***************************************
 * Resets the timer
 ***************************************/

void
fli_reset_time( int n )
{
	if ( n >= 0 )
	{
		n %= FLI_NTIMER;
		fl_gettime( lastsec + n, lastusec + n );
	}
}


/***************************************
 * Returns the time passed since the last call
 ***************************************/

double
fli_time_passed( int n )
{
	if ( n < 0 )
		return -1.0;

    n %= FLI_NTIMER;
    fl_gettime( &tp.sec, &tp.usec );
    return tp.sec - lastsec[ n ] + 1.0e-6 * ( tp.usec - lastusec[ n ] );
}


/***************************************
 * formevent is either FL_KEYPRESS or FL_KEYRELEASE
 ***************************************/

static void
fli_do_keyboard( XEvent * xev,
				 int      formevent )
{
    Window win = xev->xkey.window;
	KeySym keysym = 0;
	unsigned char keybuf[ 227 ];
    int kbuflen;

	fli_mousex  = xev->xkey.x;
	fli_mousey  = xev->xkey.y;
	fli_keymask = xev->xkey.state;
	fli_query_age = 0;

    /* Before doing anything save the current modifier key for the handlers */

    if ( win && ( ! keyform || fli_get_visible_forms_index( keyform ) < 0 ) )
		keyform = fl_win_to_form( win );

    /* switch keyboard input only if different top-level form */

    if ( keyform && keyform->window != win )
    {
		M_warn( "fli_do_keyboard", "pointer/keybd focus differ" );

		if (    ( keyform->child && keyform->child->window != win )
			 && ( keyform->parent && keyform->parent->window != win ) )
			keyform = fl_win_to_form( win );
    }

    if ( ! keyform )
		return;

	kbuflen = fli_XLookupString( ( XKeyEvent * ) xev, ( char * ) keybuf,
								 sizeof keybuf, &keysym );

	if ( kbuflen < 0 )
	{
		if ( kbuflen != INT_MIN )
			M_err( "fli_do_keyboard", "keyboad buffer overflow ?" );
		else
			M_err( "fli_do_keyboard", "fli_XLookupString failed ?" );

		return;
	}

	/* Ignore modifier keys as they don't cause action and are taken care
	   of by the lookupstring routine */

	if ( IsModifierKey( keysym ) )
		/* empty */ ;
	else if ( IsTab( keysym ) )
	{
		/* fake a tab key, some systems shift+tab does not generate a tab */

		fli_handle_form( keyform, formevent, '\t', xev );
	}
	else if ( IsCursorKey( keysym ) || kbuflen == 0 )
		fli_handle_form( keyform, formevent, keysym, xev );
	else
	{
		unsigned char *ch;

		/* all regular keys, including mapped strings */

		for ( ch = keybuf; ch < keybuf + kbuflen && keyform; ch++ )
			fli_handle_form( keyform, formevent, *ch, xev );
	}
}


/***************************************
 ***************************************/

FL_FORM_ATCLOSE
fl_set_form_atclose( FL_FORM         * form,
					 FL_FORM_ATCLOSE   fmclose,
					 void            * data )
{
    FL_FORM_ATCLOSE old = form->close_callback;

    if ( ! form  )
    {
		M_err( "fl_set_form_atclose", "NULL form." );
		return NULL;
    }

    form->close_callback = fmclose;
    form->close_data = data;

    return old;
}


/***************************************
 ***************************************/

FL_FORM_ATCLOSE
fl_set_atclose( FL_FORM_ATCLOSE   fmclose,
				void            * data )
{
    FL_FORM_ATCLOSE old = fli_context->atclose;

    fli_context->atclose = fmclose;
    fli_context->close_data = data;

    return old;
}


/***************************************
 * ClientMessage is intercepted if it is delete window
 ***************************************/

static void
fli_handle_ClientMessage_event( FL_FORM * form,
								void    * xev )
{
    XClientMessageEvent *xcm = xev;
    static Atom atom_protocol;
    static Atom atom_del_win;


    if ( ! atom_del_win )
    {
		atom_protocol = XInternAtom( xcm->display, "WM_PROTOCOLS", 0 );
		atom_del_win = XInternAtom( xcm->display, "WM_DELETE_WINDOW", 0 );
    }

    /* if delete top-level window, quit unless handlers are installed */

    if (    xcm->message_type == atom_protocol
		 && ( Atom ) xcm->data.l[ 0 ] == atom_del_win )
    {
		if ( form->close_callback )
		{
			if (    form->close_callback( form, form->close_data ) != FL_IGNORE
				 && form->visible == FL_VISIBLE )
				fl_hide_form( form );

			if ( form->sort_of_modal )
				fl_activate_all_forms( );
		}
		else if ( fli_context->atclose )
		{
			if ( fli_context->atclose( form,
									   fli_context->close_data ) != FL_IGNORE )
				exit( 1 );
		}
		else
			exit( 1 );
    }
    else    /* pump it thru current form */
		fli_handle_form( form, FL_OTHER, 0, xev );
}


static int fli_preemptive_consumed( FL_FORM *,
									int,
									XEvent * );

/***************************************
 * Given an X event check for which of our forms it is.
 ***************************************/

static FL_FORM *
fli_find_event_form( XEvent * xev )
{
	return fl_win_to_form( ( ( XAnyEvent * ) xev )->window );
}


/***************************************
 ***************************************/

FL_FORM *
fl_win_to_form( Window win )
{
	int i;

	if ( win == None )
		return NULL;

    for ( i = 0; i < formnumb; i++ )
		if ( forms[ i ]->window == win )
			return forms[ i ];

    return NULL;
}


/* how frequent to generate FL_STEP event, in milli-seconds. These
 * are modified if idle callback exists */

static int delta_msec = FLI_TIMER_RES;
static XEvent st_xev;

static int fli_form_event_queued( XEvent *,
								  int );


/***************************************
 ***************************************/

const XEvent *
fl_last_event( void )
{
    return &st_xev;
}


static int ignored_fake_configure;


/***************************************
 ***************************************/

static int
fli_button_is_really_down( void )
{
    FL_Coord x,
		     y;
    unsigned int km;

    fl_get_mouse( &x, &y, &km );

    return button_down( km );
}


/***************************************
 * Handle all events in the queue and flush output buffer
 ***************************************/

void
fli_treat_interaction_events( int wait_io )
{
    XEvent xev;

    /* if no event, output buffer will be flushed. If event exists,
       XNextEvent in do_interaction will flush the output buffer */

    do
		fli_do_interaction_step( wait_io );
    while ( fli_form_event_queued( &xev, QueuedAfterFlush ) );
}


/***************************************
 ***************************************/

static void
fli_do_interaction_step( int wait_io )
{
    FL_FORM *evform = NULL;
	static FL_FORM *redraw_form = NULL;


    if ( ! fli_get_next_event_or_idle( wait_io, &evform, &st_xev ) )
		return;

	/* got an event for one of the forms */

#if FL_DEBUG >= ML_WARN
	if ( st_xev.type != MotionNotify || fl_cntl.debug > 2 )
		fli_xevent_name( "MainLoop", &st_xev );
#endif

	fli_compress_event( &st_xev, evform->compress_mask );

	fli_query_age++;

	/* Run user raw callbacks for events, we're done if we get told that
	   we're not supposed to do anything else with the event */

	if ( fli_preemptive_consumed( evform, st_xev.type, &st_xev ) )
		return;

	/* Otherwise we need to handle the event... */

    switch ( st_xev.type )
    {
		case MappingNotify:
			XRefreshKeyboardMapping( ( XMappingEvent * ) &st_xev );
			break;

		case FocusIn:
			if ( evform->focusobj )
				keyform = evform;

			if ( ! fli_context->xic )
				break;

			XSetICValues( fli_context->xic,
						  XNFocusWindow, st_xev.xfocus.window,
						  XNClientWindow, st_xev.xfocus.window,
						  ( char * ) NULL );
			break;

		case FocusOut:
			keyform = NULL;
			break;

		case KeyPress:
			fli_do_keyboard( &st_xev, FL_KEYPRESS );
			break;

		case KeyRelease:
			fli_do_keyboard( &st_xev, FL_KEYRELEASE );
			break;

		case EnterNotify:
			fli_handle_EnterNotify_event( evform );
			break;

		case LeaveNotify:
			fli_handle_LeaveNotify_event( );
			break;

		case MotionNotify:
			fli_handle_MotionNotify_event( evform );
			break;

		case ButtonPress:
			fli_mousex = st_xev.xbutton.x;
			fli_mousey = st_xev.xbutton.y;
			fli_keymask =   st_xev.xbutton.state
				         | ( Button1Mask << ( st_xev.xbutton.button - 1 ) );
			fli_query_age = 0;

			fli_context->mouse_button = st_xev.xbutton.button;
			if ( metakey_down( fli_keymask ) && st_xev.xbutton.button == 2 )
				fli_print_version( 1 );
			else
				fli_handle_form( mouseform, FL_PUSH,
								 st_xev.xbutton.button, &st_xev );
			break;

		case ButtonRelease:
			fli_mousex = st_xev.xbutton.x;
			fli_mousey = st_xev.xbutton.y;
			fli_keymask =   st_xev.xbutton.state
				         & ~ ( Button1Mask << ( st_xev.xbutton.button - 1 ) );
			fli_query_age = 0;

			fli_context->mouse_button = st_xev.xbutton.button;


			if ( mouseform )
				fli_handle_form( mouseform, FL_RELEASE,
								 st_xev.xbutton.button, &st_xev );

			mouseform = evform;
			break;

		case Expose:
			fli_handle_Expose_event( evform, &redraw_form );
			break;

		case ConfigureNotify:
			fli_handle_ConfigureNotify_event( evform, &redraw_form );
			break;

		case ClientMessage:
			fli_handle_ClientMessage_event( evform, &st_xev );
			break;

		case DestroyNotify:	/* only sub-form gets this due to parent destroy */
			fl_hide_form( evform );
			break;

		case SelectionClear:
		case SelectionRequest:
		case SelectionNotify:
			if ( ! fli_handle_clipboard || fli_handle_clipboard( &st_xev ) < 0 )
				fli_handle_form( evform, FL_OTHER, 0, &st_xev );
			break;

		default:
			fli_handle_form( evform, FL_OTHER, 0, &st_xev );
			break;
    }
}


/***************************************
 ***************************************/

void
fli_handle_idling( XEvent * xev,
				   long     msec,
				   int      do_idle_cb )
{
	int i;

	/* Sleep a bit while keeping a lookout for async IO events */

	fli_watch_io( fli_context->io_rec, msec );

	/* Deal with signals */

    if ( fli_handle_signal )
		fli_handle_signal( );

	/* Make sure we have an up-to-date set of data for the mouse position
	   and the state of the keyboard and mouse buttons */

	if ( fli_query_age != 0 && mouseform )
	{
		fl_get_form_mouse( mouseform, &fli_mousex, &fli_mousey, &fli_keymask );
		fli_query_age = 0;
		st_xev.xmotion.time = CurrentTime;
	}
	else
		st_xev.xmotion.time += msec;

	/* FL_UPDATE and automatic handlers as well as idle callbacks can expect
	   a synthetic MotionNotify event, make it up, then call the handler */

	st_xev.type            = MotionNotify;
	st_xev.xany.window     = mouseform ? mouseform->window : None;
	st_xev.xany.send_event = 1;
	st_xev.xmotion.state   = fli_keymask;
	st_xev.xmotion.x       = fli_mousex;
	st_xev.xmotion.y       = fli_mousey;
	st_xev.xmotion.is_hint = 0;

	/* We need to send an FL_UPDATE while a mouse button is down to "pushable"
	   objects that want it (currently touch buttons, slider, choice, textbox
	   and counter objects) */

	if (    button_down( fli_keymask )
		 && ( fl_pushobj  && fl_pushobj->want_update )
		 && mouseform )
		fli_handle_form( mouseform, FL_UPDATE,
						 fli_xmask2key( fli_keymask ), &st_xev );

	/* Handle automatic tasks */

	if ( auto_count )
		for ( i = 0; i < formnumb; i++ )
			if ( forms[ i ]->has_auto )
				fli_handle_form( forms[ i ], FL_STEP, 0, xev );

	/* If asked to also execute user idle callbacks */

    if (    do_idle_cb && fli_context->idle_rec
		 && fli_context->idle_rec->callback )
		fli_context->idle_rec->callback( xev, fli_context->idle_rec->data );
}


/***************************************
 ***************************************/

static int
fli_get_next_event_or_idle( int        wait_io,
							FL_FORM ** form,
							XEvent   * xev )
{
    static unsigned int cnt = 0;
    long msec;

    /* Timeouts should be as precise as possible, so check them each time
	   round. Since they may dictate how long we're going to wait if there
	   is no event determine how how much time we will have to wait now */

    if ( ! wait_io )
		msec = SHORT_PAUSE;
    else if (    auto_count
			  || fl_pushobj
			  || fli_context->idle_rec )
		msec = delta_msec;
	else
		msec = FL_min( delta_msec * 3, 300 );

    if ( fli_context->timeout_rec )
		fli_handle_timeouts( &msec );

	/* Skip checking for an X event after 10 events, thus giving X events
	   a 10:1 priority over async IO, UPDATE events, automatic handlers and
	   idle callbacks etc. */

    if ( ++cnt % 11 && XEventsQueued( flx->display, QueuedAfterFlush ) )
    {
		XNextEvent( flx->display, xev );

		/* Find the form the event is for - if it's for none of "our" forms
		   it must be for e.g. a canvas window and must be put on the internal
		   event queue */

		if ( ( *form = fli_find_event_form( xev ) ) != NULL )
			return 1;

		/* Please note: we do event compression before the user ever sees the
		   events. This is a bit questionable at least for mouse movements
		   since a user may want to get all events (e.g. because s/he wants
		   to draw something exactly following the mouse movements). If this
		   is removed then care must be taken that in the mask for MotionNotify
		   PointerMotionHintMask is *not* set (see the fli_xevent_to_mask()
		   function in appwin.c) since that keeps most motion events from
		   coming through! */

		fli_compress_event( xev,
							  ExposureMask
						    | PointerMotionMask
						    | ButtonMotionMask );
		fl_XPutBackEvent( xev );

		return 0;
    }

	cnt = 0;

	fli_handle_idling( &st_xev, msec, 1 );

	return 0;
}


/***************************************
 * Handling of EnterNotiy events
 ***************************************/

static void
fli_handle_EnterNotify_event( FL_FORM * evform )
{
    Window win = st_xev.xany.window;

	fli_mousex = st_xev.xcrossing.x;
	fli_mousey = st_xev.xcrossing.y;
	fli_keymask = st_xev.xcrossing.state;
	fli_query_age = 0;

	if (    button_down( fli_keymask )
		 && st_xev.xcrossing.mode != NotifyUngrab )
		return;

	if ( mouseform )
		fli_handle_form( mouseform, FL_LEAVE,
						 fli_xmask2key( fli_keymask ), &st_xev );

	if ( evform )
	{
		mouseform = evform;

		/* this is necessary because win might be un-managed. To be
		   friendly to other applications, grab focus only if abslutely
		   necessary */

		if (    mouseform->deactivated == 0
			 && ! st_xev.xcrossing.focus && unmanaged_count > 0 )
		{
			fli_check_key_focus( "EnterNotify", win );
			fl_winfocus( win );
		}

		fli_handle_form( mouseform, FL_ENTER,
						 fli_xmask2key( fli_keymask ), &st_xev );
	}
#if FL_DEBUG >= ML_DEBUG
	else
		M_err( "fli_handle_EnterNotify_event", "Null form!" );
#endif
}


/***************************************
 * Handling of LeaveNotiy events
 ***************************************/

static void
fli_handle_LeaveNotify_event( void )
{
	fli_mousex = st_xev.xcrossing.x;
	fli_mousey = st_xev.xcrossing.y;
	fli_keymask = st_xev.xcrossing.state;
	fli_query_age = 0;

	if (    button_down( fli_keymask )
		 && st_xev.xcrossing.mode == NotifyNormal )
		return;

	/* olvwm sends LeaveNotify with NotifyGrab whenever button is clicked.
	   Ignore it. Due to Xpoup grab, (maybe Wm bug ?), end grab can also
	   generate this event. we can tell these two situations by doing a real
	   button_down test (as opposed to relying on the keymask in event) */

	if (    st_xev.xcrossing.mode == NotifyGrab
		 && fli_button_is_really_down( ) )
		return;


	if ( ! mouseform )
		return;

	fli_handle_form( mouseform, FL_LEAVE, fli_xmask2key( fli_keymask ),
					 &st_xev );
}


/***************************************
 * Handling of MotionNotify events
 ***************************************/

static void
fli_handle_MotionNotify_event( FL_FORM * evform )
{
    Window win = st_xev.xany.window;

	fli_keymask = st_xev.xmotion.state;
	fli_mousex = st_xev.xmotion.x;
	fli_mousey = st_xev.xmotion.y;
	fli_query_age = 0;

	if ( ! mouseform )
	{
		M_warn( "fli_handle_MotionNotify_event", "evwin=0x%lx", win );
		return;
	}

	if ( mouseform->window != win )
	{
		M_warn( "fli_handle_MotionNotify_event", "mousewin=0x%ld evwin=0x%ld",
				mouseform->window, win );
		fli_mousex += evform->x - mouseform->x;
		fli_mousey += evform->y - mouseform->y;
	}

	fli_handle_form( mouseform, FL_MOTION,
					 fli_xmask2key( fli_keymask ), &st_xev );
}


/***************************************
 * Handling of Expose events
 ***************************************/

static void
fli_handle_Expose_event( FL_FORM  * evform,
						 FL_FORM ** redraw_form )
{
	if ( ! evform )
		return;

	/* If 'redraw_form' is set we got a ConfigureNotify and the data from the
	   Exposure event aren't correct - set clipping to the complete area of
	   the form. */

	if ( *redraw_form == evform )
	{
		st_xev.xexpose.x = 0;
		st_xev.xexpose.y = 0;
		st_xev.xexpose.width  = evform->w;
		st_xev.xexpose.height = evform->h;
		*redraw_form = NULL;
	}
	else
	{
		if ( st_xev.xexpose.x + st_xev.xexpose.width > evform->w )
			st_xev.xexpose.width = evform->w - st_xev.xexpose.x;
		if ( st_xev.xexpose.y + st_xev.xexpose.height > evform->h )
			st_xev.xexpose.height = evform->h - st_xev.xexpose.y;
	}

	fli_set_perm_clipping( st_xev.xexpose.x, st_xev.xexpose.y,
						   st_xev.xexpose.width, st_xev.xexpose.height );
	fl_set_clipping( st_xev.xexpose.x, st_xev.xexpose.y,
					 st_xev.xexpose.width, st_xev.xexpose.height );

	/* run into trouble by ignoring configure notify */

	if ( ignored_fake_configure )
	{
		FL_Coord neww,
			     newh;

		M_warn( "fli_handle_Expose_event", "Run into trouble - correcting it" );
		fl_get_winsize( evform->window, &neww, &newh );
		fli_scale_form( evform, ( double ) neww / evform->w,
						( double ) newh / evform->h );
		ignored_fake_configure = 0;
	}

	fli_handle_form( evform, FL_DRAW, 0, &st_xev );

	fli_unset_perm_clipping( );
	fl_unset_clipping( );
	fl_unset_text_clipping( );
}


/***************************************
 * Handling of ConfigureNotify events
 ***************************************/

static void
fli_handle_ConfigureNotify_event( FL_FORM  * evform,
								  FL_FORM ** redraw_form )
{
	Window win = st_xev.xany.window;
	int old_w = evform->w;
	int old_h = evform->h;

	if ( ! evform )
		return;

	if ( ! st_xev.xconfigure.send_event )
		fl_get_winorigin( win, &evform->x, &evform->y );
	else
	{
		evform->x = st_xev.xconfigure.x;
		evform->y = st_xev.xconfigure.y;
		M_warn( "fli_handle_ConfigureNotify_event", "WMConfigure:x=%d y=%d",
				evform->x, evform->y );
	}

	/* mwm sends bogus ConfigureNotify randomly without following up with a
	   redraw event, but it does set send_event. The check is somewhat
	   dangerous, use 'ignored_fake_configure' to make sure when we got expose
	   we can respond correctly. The correct fix is always to get window
	   geometry in Expose handler, but that has a two-way traffic overhead */

	ignored_fake_configure =    st_xev.xconfigure.send_event
				             && (    st_xev.xconfigure.width  != evform->w
					              || st_xev.xconfigure.height != evform->h );

	/* Dragging the form across the screen changes its absolute x, y coords.
	   Objects that themselves contain forms should ensure that they are up to
	   date. */

	fli_handle_form( evform, FL_MOVEORIGIN, 0, &st_xev );

	if ( st_xev.xconfigure.send_event )
		return;

	/* can't just set form->{w,h}. Need to take care of obj gravity */

	fli_scale_form( evform, ( double ) st_xev.xconfigure.width  / evform->w,
					( double ) st_xev.xconfigure.height / evform->h );

	/* If both the width and the height got smaller (or one got smaller and
	   the other one is unchanged) we're not going to get an Expose event at
	   all, so we need to redraw the form. Even if only one of the lengths
	   gets smaller or remains unchanged while the other got larger, the next
	   (compressed) Expose event will only cover the added part, so in this
	   case store the forms address, so on the next Expose event we receive
	   for it it's full area can be redrawn. */

	if ( evform->w <= old_w && evform->h <= old_h )
		fl_redraw_form( evform );
	else if ( ! ( evform->w > old_w && evform->h > old_h ) ) 
		*redraw_form = evform;
}


/***************************************
 * Checks all forms. Does not wait.
 ***************************************/

FL_OBJECT *
fl_check_forms( void )
{
    FL_OBJECT *obj;

    if ( ( obj = fli_object_qread( ) ) == NULL )
    {
		fli_treat_interaction_events( 0 );
		fli_treat_user_events( );
		obj = fli_object_qread( );
    }

    return obj;
}


/***************************************
 * Same as fl_check_forms() but never returns FL_EVENT.
 ***************************************/

FL_OBJECT *
fl_check_only_forms( void )
{
    FL_OBJECT *obj;

    if ( ( obj = fli_object_qread( ) ) == NULL )
    {
		fli_treat_interaction_events( 0 );
		obj = fli_object_qread( );
    }

    return obj;
}


/***************************************
 * Checks all forms and keep checking if nothing happens.
 ***************************************/

FL_OBJECT *
fl_do_forms( void )
{
    FL_OBJECT *obj;

    while ( ( obj = fli_object_qread( ) ) == NULL )
    {
		fli_treat_interaction_events( 1 );
		fli_treat_user_events( );
    }

	return obj;
}


/***************************************
 * Same as fl_do_forms() but never returns FL_EVENT.
 ***************************************/

FL_OBJECT *
fl_do_only_forms( void )
{
    FL_OBJECT *obj;

    while ( ( obj = fli_object_qread( ) ) == NULL )
		fli_treat_interaction_events( 1 );

	if ( obj == FL_EVENT )
		M_warn( "fl_do_only_forms", "Shouldn't happen" );

	return obj;
}


/***************************************
 ***************************************/

static int
fli_form_event_queued( XEvent * xev,
					   int      mode )
{
    if ( XEventsQueued( flx->display, mode ) )
    {
		XPeekEvent( flx->display, xev );
		return fli_find_event_form( xev ) != NULL;
    }

    return 0;
}


/***************************************
 ***************************************/

void
fl_set_initial_placement( FL_FORM  * form,
						  FL_Coord   x,
						  FL_Coord   y,
						  FL_Coord   w,
						  FL_Coord   h )
{
    fl_set_form_position( form, x, y );
    fl_set_form_size( form, w, h );

    /* this alters the windowing defaults */

    fl_initial_wingeometry( form->x, form->y, form->w, form->h );
    has_initial = 1;
}


/***************************************
 * register pre-emptive event handlers
 ***************************************/

FL_RAW_CALLBACK
fl_register_raw_callback( FL_FORM         * form,
						  unsigned long     mask,
						  FL_RAW_CALLBACK   rcb )
{
    FL_RAW_CALLBACK old_rcb = NULL;
    int valid = 0;

    if ( ! form )
    {
		M_err( "fl_register_raw_callback", "Null form" );
		return NULL;
    }

    if ( ( mask & FL_ALL_EVENT ) == FL_ALL_EVENT )
    {
		old_rcb = form->all_callback;
		form->evmask = mask;
		form->all_callback = rcb;
		return old_rcb;
    }

    if ( mask & ( KeyPressMask | KeyReleaseMask ) )
    {
		form->evmask |= mask & ( KeyPressMask | KeyReleaseMask );
		old_rcb = form->key_callback;
		form->key_callback = rcb;
		valid = 1;
    }

    if ( mask & ( ButtonPressMask | ButtonReleaseMask ) )
    {
		form->evmask |= mask & ( ButtonPressMask | ButtonReleaseMask );
		old_rcb = form->push_callback;
		form->push_callback = rcb;
		valid = 1;
    }

    if ( mask & ( EnterWindowMask | LeaveWindowMask ) )
    {
		form->evmask |= mask & ( EnterWindowMask | LeaveWindowMask );
		old_rcb = form->crossing_callback;
		form->crossing_callback = rcb;
		valid = 1;
    }

    if ( mask & ( ButtonMotionMask | PointerMotionMask ) )
    {
		form->evmask |= mask & ( ButtonMotionMask | PointerMotionMask );
		old_rcb = form->motion_callback;
		form->motion_callback = rcb;
		valid = 1;
    }

    if ( ! valid )			/* unsupported mask */
		M_err( "fl_register_raw_callback", "Unsupported mask 0x%x", mask );

    return old_rcb;
}


/***************************************
 ***************************************/

static int
fli_preemptive_consumed( FL_FORM * form,
						 int       type,
						 XEvent  * xev )
{
    if ( ! form || ! form->evmask || form->deactivated )
		return 0;

    if (    ( form->evmask & FL_ALL_EVENT ) == FL_ALL_EVENT
		 && form->all_callback )
		return form->all_callback( form, xev );

    switch ( type )
    {
		case ButtonPress:
			if (    form->evmask & ButtonPressMask
				 && form->push_callback )
				return form->push_callback( form, xev );
			break;

		case ButtonRelease:
			if (    form->evmask & ButtonReleaseMask
				 && form->push_callback )
				return form->push_callback( form, xev );
			break;

		case KeyPress:
			if (    form->evmask & KeyPressMask
				 && form->key_callback )
				return form->key_callback( form, xev );
			break;

		case KeyRelease:
			if (    form->evmask & KeyRelease
				 && form->key_callback )
				return form->key_callback( form, xev );
			break;

		case EnterNotify:
			if (    form->evmask & EnterWindowMask
				 && form->crossing_callback )
				return form->crossing_callback( form, xev );
			break;

		case LeaveNotify:
			if (    form->evmask & LeaveWindowMask
				 && form->crossing_callback )
				return form->crossing_callback( form, xev );
			break;

		case MotionNotify:
			if (    form->evmask & ( ButtonMotionMask | PointerMotionMask )
				 && form->motion_callback )
				return form->motion_callback( form, xev );
    }

    return 0;
}


/***************************************
 ***************************************/

void
fl_set_form_event_cmask( FL_FORM *     form,
						 unsigned long cmask )
{
    if ( form )
		form->compress_mask = cmask;
}


/***************************************
 ***************************************/

unsigned long
fl_get_form_event_cmask( FL_FORM * form )
{
    return form ? form->compress_mask : 0UL;
}


/***************************************
 * Cleanup everything. At the moment, only need to restore the keyboard
 * settings and dealloctae memory used for the object and event queue.
 * This routine is registered as an atexit in fl_initialize in flresource.c
 ***************************************/

void
fl_finish( void )
{
    /* make sure the connection is alive */

    if ( flx->display )
    {
		XChangeKeyboardControl( flx->display, fl_keybdmask, &fl_keybdcontrol );

		fl_remove_all_signal_callbacks( );
		fl_remove_all_timeouts( );

		/* Get rid of all forms */

		while ( formnumb > 0 )
			fl_hide_form( *forms );
		while ( hidden_formnumb > 0 )
			fl_free_form( *forms );

		/* Delete the object and event queue */

		fli_obj_queue_delete( );
		fli_event_queue_delete( );

		/* Free memory allocated in xtext.c */

		fli_free_xtext_workmem( );

		/* Release memory used for symbols */

		fli_release_symbols( );

		/* Release memory used for the copy of the command line arguments */

		fli_free_cmdline_args( );

		/* Close the display */

		XCloseDisplay( flx->display );
		flx->display = fl_display = None;
    }
}


/***************************************
 * Sets the call_back routine for the form
 ***************************************/

void
fl_set_form_callback( FL_FORM            * form,
					  FL_FORMCALLBACKPTR   callback,
					  void *               d )
{
    if ( form == NULL )
    {
		M_err( "fl_set_form_callback", "NULL form." );
		return;
    }

    form->form_callback = callback;
    form->form_cb_data = d;
}


/***************************************
 * currently only a single idle callback is support
 ***************************************/

static void
fli_add_idle_callback( FL_APPEVENT_CB   cb,
					   void           * data )
{
    if ( ! fli_context->idle_rec )
    {
		fli_context->idle_rec = fl_malloc( sizeof *fli_context->io_rec );
		fli_context->idle_rec->next = NULL;
    }

    fli_context->idle_rec->callback = cb;
    fli_context->idle_rec->data = data;
}


/***************************************
 * idle callback
 ***************************************/

FL_APPEVENT_CB
fl_set_idle_callback( FL_APPEVENT_CB   callback,
					  void           * user_data )
{
    FL_APPEVENT_CB old =
		         fli_context->idle_rec ? fli_context->idle_rec->callback : NULL;

    fli_add_idle_callback( callback, user_data );

    /* if we have idle callbacks, decrease the wait time */

    delta_msec = FLI_TIMER_RES * ( callback ? 0.8 : 1.0 );
    fli_context->idle_delta = delta_msec;

    return old;
}


/***************************************
 ***************************************/

void
fl_set_idle_delta( long delta )
{
    if ( delta < 0 )
		delta = FLI_TIMER_RES;
    else if ( delta == 0 )
		delta = FLI_TIMER_RES / 10;

    delta_msec = delta;
    fli_context->idle_delta = delta;
}


/***************************************
 ***************************************/

void
fl_set_form_icon( FL_FORM * form,
				  Pixmap    p,
				  Pixmap    m )
{
    if ( form )
    {
		form->icon_pixmap = p;
		form->icon_mask = m;
		if ( form->window )
			fl_winicon( form->window, p, m );
    }
}


/***************************************
 ***************************************/

void
fl_set_app_mainform( FL_FORM * form )
{
    fli_mainform = form;
    fli_set_form_property( form, FLI_COMMAND_PROP );
}


/***************************************
 ***************************************/

FL_FORM *
fl_get_app_mainform( void )
{
    return nomainform ? NULL : fli_mainform;
}


/***************************************
 ***************************************/

void
fl_set_app_nomainform( int flag )
{
    nomainform = flag;
}


/***************************************
 * Does a rescale of a form without taking into
 * account object gravity or resize settings
 ***************************************/

static void
fli_simple_form_rescale( FL_FORM * form,
						 double    scale )
{
    FL_OBJECT *ob;

	form->w_hr *= scale;
	form->h_hr *= scale;

	form->w = FL_crnd( form->w_hr );
	form->h = FL_crnd( form->h_hr );

    for ( ob = form->first; ob; ob = ob->next )
		if ( ob->objclass != FL_BEGIN_GROUP && ob->objclass != FL_END_GROUP )
			fl_scale_object( ob, scale, scale );

    fl_redraw_form( form );
}


/***************************************
 * never shrinks a form, margin is the minimum margin to leave
 ***************************************/

void
fl_fit_object_label( FL_OBJECT * obj,
					 FL_Coord    xmargin,
					 FL_Coord    ymargin )
{
    int sw,
		sh,
		osize;
    double factor,
		   xfactor,
		   yfactor;

    if ( fl_no_connection )
		return;

    fl_get_string_dimension( obj->lstyle, obj->lsize, obj->label,
							 strlen( obj->label ), &sw, &sh );

    if (    sw <= obj->w - 2 * ( FL_abs( obj->bw ) + xmargin )
		 && sh <= obj->h - 2 * ( FL_abs( obj->bw ) + ymargin ) )
		return;

    if ( ( osize = obj->w - 2 * ( FL_abs( obj->bw ) + xmargin ) ) <= 0 )
		osize = 1;
    xfactor = ( double ) sw / osize;

    if ( ( osize = obj->h - 2 * ( FL_abs( obj->bw ) + ymargin ) ) <= 0 )
		osize = 1;
    yfactor = ( double ) sh / osize;

    factor = FL_max( xfactor, yfactor );

    if ( factor > 1.5 )
		factor = 1.5;

    /* scale all objects without taking care of gravity etc. */

	fli_simple_form_rescale( obj->form, factor );
}


/***************************************
 ***************************************/

void
fli_recount_auto_object( void )
{
    int i;

    for ( auto_count = i = 0; i < formnumb; i++ )
		if ( forms[ i ]->has_auto )
			auto_count++;
}


/***************************************
 ***************************************/

void
fl_addto_group( FL_OBJECT * group )
{
    if ( group == NULL )
    {
		M_err( "fl_addto_group", "NULL group." );
		return;
    }

    if ( group->objclass != FL_BEGIN_GROUP )
    {
		M_err( "fl_addto_group", "Parameter is not a group object." );
		return;
    }

    if ( fl_current_form && fl_current_form != group->form )
    {
		M_err( "fl_addto_group",
			   "Can't switch to a group on different form." );
		return;
    }

    if ( fl_current_group && fl_current_group != group )
    {
		M_err( "fl_addto_group", "You forgot to call fl_end_group" );
		fl_end_group( );
    }

    reopened_group = 1;
    reopened_group += fl_current_form ? 0 : 2;
    fl_current_form = group->form;
    fl_current_group = group;
}


/***************************************
 ***************************************/

int
fl_form_is_visible( FL_FORM * form )
{
    return ( form && form->window ) ? form->visible : FL_INVISIBLE;
}


/***************************************
 * similar to fit_object_label, but will do it for all objects and has
 * a smaller threshold. Mainly intended for compensation for font size
 * variations
 ***************************************/

double
fl_adjust_form_size( FL_FORM * form )
{
    FL_OBJECT *ob;
    double xfactor,
		   yfactor,
		   max_factor,
		   factor;
    int sw,
		sh,
		osize;
    double xm = 0.5,
		   ym = 0.5;
    int bw;

    if ( fl_no_connection )
		return 1.0;

    max_factor = factor = 1.0;
    for ( ob = form->first; ob; ob = ob->next )
    {
		if (    (    ob->align == FL_ALIGN_CENTER
				  || ob->align & FL_ALIGN_INSIDE
				  || ob->objclass == FL_INPUT )
			 && ! ob->is_child
			 && ob->label[ 0 ] != '\0'
			 && ob->label[ 0 ] != '@'
			 && ob->boxtype != FL_NO_BOX
			 && ( ob->boxtype != FL_FLAT_BOX || ob->objclass == FL_MENU ) )
		{
			fl_get_string_dimension( ob->lstyle, ob->lsize, ob->label,
									 strlen( ob->label ), &sw, &sh );

			bw = ( ob->boxtype == FL_UP_BOX || ob->boxtype == FL_DOWN_BOX ) ?
				 FL_abs( ob->bw ) : 1;

			if (    ob->objclass == FL_BUTTON
				 && (    ob->type == FL_RETURN_BUTTON
					  || ob->type == FL_MENU_BUTTON ) )
				sw += FL_min( 0.6 * ob->h, 0.6 * ob->w ) - 1;

			if ( ob->objclass == FL_BUTTON && ob->type == FL_LIGHTBUTTON )
				sw += FL_LIGHTBUTTON_MINSIZE + 1;

			if (    sw <= ob->w - 2 * ( bw + xm )
				 && sh <= ob->h - 2 * ( bw + ym ) )
				continue;

			if ( ( osize = ob->w - 2 * ( bw + xm ) ) <= 0 )
				osize = 1;
			xfactor = ( double ) sw / osize;

			if ( ( osize = ob->h - 2 * ( bw + ym ) ) <= 0 )
				osize = 1;
			yfactor = ( double ) sh / osize;

			if ( ob->objclass == FL_INPUT )
			{
				xfactor = 1.0;
				yfactor = ( sh + 1.6 ) / osize;
			}

			if ( ( factor = FL_max( xfactor, yfactor ) ) > max_factor )
				max_factor = factor;
		}
    }

    if ( max_factor <= 1.0 )
		return 1.0;

    max_factor = 0.01 * ( int ) ( max_factor * 100.0 );

    if ( max_factor > 1.25 )
		max_factor = 1.25;

    /* scale all objects without taking care of gravity etc. */

	fli_simple_form_rescale( form, max_factor );

    return max_factor;
}


/***************************************
 ***************************************/

long
fl_mouse_button( void )
{
    return fli_context->mouse_button;
}


/***************************************
 ***************************************/

FLI_TARGET *
fli_internal_init( void )
{
    static FLI_TARGET *default_flx;

    if ( ! default_flx )
		default_flx = fl_calloc( 1, sizeof *default_flx );

    return flx = default_flx;
}


/***************************************
 * fl_display is exposed to the outside world. Bad
 ***************************************/

void
fli_switch_target( FLI_TARGET * newtarget )
{
    flx = newtarget;
    fl_display = flx->display;
}


/***************************************
 ***************************************/

void
fli_restore_target( void )
{
    fli_internal_init( );
    fl_display = flx->display;
}


/***************************************
 ***************************************/

static int
fli_XLookupString( XKeyEvent * xkey,
				   char      * buf,
				   int         buflen,
				   KeySym    * ks )
{
    int len = INT_MIN;

    if ( ! fli_context->xic )
		len = XLookupString( xkey, buf, buflen, ks, 0 );
    else
    {
		Status status;

		if ( XFilterEvent( ( XEvent * ) xkey, None ) )
		{
			*ks = NoSymbol;
			return 0;
		}

		len = XmbLookupString( fli_context->xic, xkey, buf, buflen, ks,
							   &status );

		if ( status == XBufferOverflow )
			len = -len;
	}

    return len;
}
