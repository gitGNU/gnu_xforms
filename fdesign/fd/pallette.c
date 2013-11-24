/* Form definition file generated by fdesign on Mon Nov 25 00:37:17 2013 */

#include <stdlib.h>
#include "pallette.h"


/***************************************
 ***************************************/

FD_pmain *
create_form_pmain( void )
{
    FL_OBJECT *obj;
    FD_pmain *fdui = ( FD_pmain * ) fl_malloc( sizeof *fdui );

    int old_bw = fl_get_border_width( );
    fl_set_border_width( -1 );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->pmain = fl_bgn_form( FL_NO_BOX, 380, 210 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 380, 210, "" );

    fdui->folder = obj = fl_add_tabfolder( FL_TOP_TABFOLDER, 5, 5, 370, 175, "" );
    fl_set_object_resize( obj, FL_RESIZE_ALL );

    obj = fl_add_button( FL_NORMAL_BUTTON, 311, 183, 62, 23, " Dismiss " );
    fl_set_object_callback( obj, dismiss_pallete, 0 );

    fl_end_form( );

    fdui->pmain->fdui = fdui;
    fl_set_border_width( old_bw );

    return fdui;
}


/***************************************
 ***************************************/

FD_buttform *
create_form_buttform( void )
{
    FL_OBJECT *obj;
    FD_buttform *fdui = ( FD_buttform * ) fl_malloc( sizeof *fdui );

    int old_bw = fl_get_border_width( );
    fl_set_border_width( -1 );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->buttform = fl_bgn_form( FL_NO_BOX, 360, 150 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 360, 150, "" );

    obj = fl_add_frame( FL_ENGRAVED_FRAME, 10, 10, 345, 130, "" );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 15, 15, 70, 40, "Button" );
    fl_set_button_shortcut( obj, "B#B", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_BUTTON );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 101, 15, 70, 40, "LightButton" );
    fl_set_button_shortcut( obj, "L#L", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_LIGHTBUTTON );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 188, 15, 70, 40, "RoundButton" );
    fl_set_button_shortcut( obj, "R#R", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_ROUNDBUTTON );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 275, 15, 70, 40, "Round3DButton" );
    fl_set_button_shortcut( obj, "3#3", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_ROUND3DBUTTON );
    fl_set_pixmapbutton_align( obj, FL_ALIGN_TOP | FL_ALIGN_INSIDE, 3, 3 );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 58, 80, 70, 40, "CheckButton" );
    fl_set_button_shortcut( obj, "C#C", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_CHECKBUTTON );

    obj = fl_add_bitmapbutton( FL_RADIO_BUTTON, 145, 80, 70, 40, "BitmapButton" );
    fl_set_button_shortcut( obj, "B#B", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_BITMAPBUTTON );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 231, 80, 70, 40, "PixmapButton" );
    fl_set_button_shortcut( obj, "P#P", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_PIXMAPBUTTON );

    fl_end_form( );

    fdui->buttform->fdui = fdui;
    fl_set_border_width( old_bw );

    return fdui;
}


/***************************************
 ***************************************/

FD_staticform *
create_form_staticform( void )
{
    FL_OBJECT *obj;
    FD_staticform *fdui = ( FD_staticform * ) fl_malloc( sizeof *fdui );

    int old_bw = fl_get_border_width( );
    fl_set_border_width( -1 );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->staticform = fl_bgn_form( FL_NO_BOX, 360, 150 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 360, 150, "" );

    obj = fl_add_frame( FL_ENGRAVED_FRAME, 10, 10, 345, 130, "" );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 30, 20, 65, 40, "Box" );
    fl_set_button_shortcut( obj, "B#B", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_BOX );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 110, 20, 65, 40, "Text" );
    fl_set_button_shortcut( obj, "T#T", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_TEXT );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 190, 20, 65, 40, "Frame" );
    fl_set_button_shortcut( obj, "F#F", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_LABELFRAME );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 270, 20, 65, 40, "Chart" );
    fl_set_button_shortcut( obj, "C#C", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_CHART );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 30, 80, 65, 40, "Clock" );
    fl_set_button_shortcut( obj, "o#O", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_CLOCK );

    obj = fl_add_bitmapbutton( FL_RADIO_BUTTON, 110, 80, 65, 40, "Bitmap" );
    fl_set_button_shortcut( obj, "B#B", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_BITMAP );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 210, 80, 65, 40, "Pixmap" );
    fl_set_button_shortcut( obj, "P#P", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_PIXMAP );

    fl_end_form( );

    fdui->staticform->fdui = fdui;
    fl_set_border_width( old_bw );

    return fdui;
}


/***************************************
 ***************************************/

FD_valuatorform *
create_form_valuatorform( void )
{
    FL_OBJECT *obj;
    FD_valuatorform *fdui = ( FD_valuatorform * ) fl_malloc( sizeof *fdui );

    int old_bw = fl_get_border_width( );
    fl_set_border_width( -1 );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->valuatorform = fl_bgn_form( FL_NO_BOX, 360, 150 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 360, 150, "" );

    obj = fl_add_frame( FL_ENGRAVED_FRAME, 10, 8, 345, 130, "" );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 30, 16, 70, 40, "Slider" );
    fl_set_button_shortcut( obj, "S#S", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_SLIDER );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 183, 16, 70, 40, "Scrollbar" );
    fl_set_button_shortcut( obj, "b#B", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_SCROLLBAR );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 260, 16, 70, 40, "Dial" );
    fl_set_button_shortcut( obj, "D#D", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_DIAL );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 30, 76, 70, 40, "Counter" );
    fl_set_button_shortcut( obj, "C#C", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_COUNTER );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 105, 76, 70, 40, "Positioner" );
    fl_set_button_shortcut( obj, "P#P", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_POSITIONER );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 106, 16, 70, 40, "Valslider" );
    fl_set_button_shortcut( obj, "#V", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_VALSLIDER );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 185, 76, 70, 40, "Thumbwheel" );
    fl_set_button_shortcut( obj, "t#T", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_THUMBWHEEL );

    fl_end_form( );

    fdui->valuatorform->fdui = fdui;
    fl_set_border_width( old_bw );

    return fdui;
}


/***************************************
 ***************************************/

FD_choiceform *
create_form_choiceform( void )
{
    FL_OBJECT *obj;
    FD_choiceform *fdui = ( FD_choiceform * ) fl_malloc( sizeof *fdui );

    int old_bw = fl_get_border_width( );
    fl_set_border_width( -1 );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->choiceform = fl_bgn_form( FL_NO_BOX, 360, 150 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 360, 150, "" );

    obj = fl_add_frame( FL_ENGRAVED_FRAME, 10, 10, 345, 130, "" );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 30, 20, 60, 60, "Menu" );
    fl_set_button_shortcut( obj, "M#M", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_MENU );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 100, 20, 60, 60, "Choice" );
    fl_set_button_shortcut( obj, "C#C", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_CHOICE );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 170, 20, 60, 60, "Browser" );
    fl_set_button_shortcut( obj, "B#B", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_BROWSER );

    fl_end_form( );

    fdui->choiceform->fdui = fdui;
    fl_set_border_width( old_bw );

    return fdui;
}


/***************************************
 ***************************************/

FD_miscform *
create_form_miscform( void )
{
    FL_OBJECT *obj;
    FD_miscform *fdui = ( FD_miscform * ) fl_malloc( sizeof *fdui );

    int old_bw = fl_get_border_width( );
    fl_set_border_width( -1 );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->miscform = fl_bgn_form( FL_NO_BOX, 360, 150 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 360, 150, "" );

    obj = fl_add_frame( FL_ENGRAVED_FRAME, 10, 10, 345, 130, "" );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 30, 16, 70, 45, "Timer" );
    fl_set_button_shortcut( obj, "T#T", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_TIMER );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 103, 16, 70, 45, "XYPlot" );
    fl_set_button_shortcut( obj, "X#X", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_XYPLOT );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 176, 16, 70, 45, "Canvas" );
    fl_set_button_shortcut( obj, "C#C", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_CANVAS );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 250, 16, 70, 45, "GLCanvas" );
    fl_set_button_shortcut( obj, "G#G", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_GLCANVAS );

    obj = fl_add_pixmapbutton( FL_RADIO_BUTTON, 30, 82, 70, 40, "Tabfolder" );
    fl_set_button_shortcut( obj, "f#F", 1 );
    fl_set_object_color( obj, FL_COL1, FL_YELLOW );
    fl_set_object_callback( obj, pallette_entry_callback, FL_NTABFOLDER );

    fl_end_form( );

    fdui->miscform->fdui = fdui;
    fl_set_border_width( old_bw );

    return fdui;
}
