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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "include/forms.h"
#include "flinternal.h"


/***************************************
 * Function returns true if a string is a valid UTF-8 encoded
 * string, otherwise false
 ***************************************/

int
utf8_is_valid( const char * str )
{
    FL_VAL first_cp;

    if ( ! str )
        return 0;

    if ( ! *str )
        return 1;

    // Note: an UTF-8 string may not start with a diacritic

    first_cp = utf8_to_cp( str );

    return    first_cp
           && ! utf8_is_diacritic( first_cp )
           && utf8_length( str, 0 ) > 0;
}


/***************************************
 * Function for determing the number of UTF-8 encoded code points in
 * a string. If 'skip_diacritics' is set, diacritic markers (or whatever
 * else is combined with the preceeding character) don't get counted.
 * If it's not a valid UTF-8 string a negative is returned.
 ***************************************/

ssize_t
utf8_length( const char * str,
             int          skip_diacritics )
{
    const unsigned char * p = ( const unsigned char * ) str;
    ssize_t cnt = 0;

    if ( ! str )
        return -1;
    else if ( ! *str )
        return 0;

    for ( ; *p; p++, cnt++ )
    {
        int len = 1;

        if ( *p <= 0x7F )
            /* empty */ ;
        else if ( ( *p & 0xE0 ) == 0xC0 )
        {
            if ( ( *++p & 0xC0 ) != 0x80 )
                return -1;
            len = 2;
        }
        else if ( ( *p & 0xF0 ) == 0xE0 )
        {
             if (    ( *++p & 0xC0 ) != 0x80
                  || ( *++p & 0xC0 ) != 0x80 )
                 return -1;
            len = 3;
        }
        else if ( ( *p & 0xF8 ) == 0xF0 )
        {
             if (    ( *++p & 0xC0 ) != 0x80
                  || ( *++p & 0xC0 ) != 0x80
                  || ( *++p & 0xC0 ) != 0x80 )
                 return -1;
            len = 4;
        }
        else
            return -1;

        if ( skip_diacritics && utf8_is_diacritic( utf8_to_cp( p - len ) ) )
            cnt--;
    }

    return cnt;
}


/***************************************
 * Converts an UTF-8 code point in a string to an integer number
 * (still UTF-8 encoded). Returns 0 if the string does not contain
 * a valid UTF-8 code point.
 ***************************************/

FL_VAL
utf8_to_num( const char * str )
{
    FL_VAL res;
    int cnt;
    int i;
    const unsigned char * p = ( const unsigned char * ) str;

    if ( *p < 0x7F )
        return *p;

    if ( ( *p & 0xE0 ) == 0xC0 )
        cnt = 2;
    else if ( ( *p & 0xF0 ) == 0xE0 )
        cnt = 3;
    else if ( ( *p & 0xF8 ) == 0xF0 )
        cnt = 4;
    else
        return 0;

    res = *p++;
    for ( i = 1; i < cnt; ++i )
    {
        if ( ( *p & 0xC0 ) != 0x80 )
            return 0;

        res = ( res << 8 ) + *p++;
    }

    return res;
}


/***************************************
 * Converts an UTF-8 encoded code point from a string to the corresponding
 * Unicode code point. Returns 0 if the string doesn't contain a valid UTF-8
 * encoded code point.
 ***************************************/

FL_VAL
utf8_to_cp( const char * str )
{
    FL_VAL res;
    int cnt;
    int i;
    const unsigned char * p = ( const unsigned char * ) str;

    if ( *p < 0x7F )
        return *p;

    if ( ( *p & 0xE0 ) == 0xC0 )
        cnt = 2;
    else if ( ( *p & 0xF0 ) == 0xE0 )
        cnt = 3;
    else if ( ( *p & 0xF8 ) == 0xF0 )
        cnt = 4;
    else
        return 0;

    res = *p++ & ( 0x7F >> cnt );
    for ( i = 1; i < cnt; ++i )
    {
        if ( ( *p & 0xC0 ) != 0x80 )
            return 0;

        res = ( res << 6 ) + ( *p++ & 0x3f );
    }

    return res;
}


/***************************************
 * Inserts the UTF-8 encoded code point, given by the integer  'key', into
 * 'str' (which must point to enough memory), encoded also as UTF-8. Returns
 * the number of bytes inserted.
 ***************************************/

int
utf8_insert( FL_VAL   key,
             char   * str )
{
    if ( key <= 0x7F )
    {
        *str = ( char ) key;
        return 1;
    }
    else if ( ( key & 0xE000 ) == 0xC000 )
    {
        str[ 0 ] = ( char ) ( ( key >> 8 ) & 0xFF );
        str[ 1 ] = ( char ) (   key        & 0xFF );
        return 2;
    }
    else if ( ( key & 0xF00000 ) == 0xE00000 )
    {
        str[ 0 ] = ( char ) ( ( key >> 16 ) & 0xFF );
        str[ 1 ] = ( char ) ( ( key >>  8 ) & 0xFF );
        str[ 2 ] = ( char ) (   key         & 0xFF );
        return 3;
    }
    else if ( ( key & 0xF8000000 ) == 0xF0000000 )
    {
        str[ 0 ] = ( char ) ( ( key >> 24 ) & 0xFF );
        str[ 1 ] = ( char ) ( ( key >> 16 ) & 0xFF );
        str[ 2 ] = ( char ) ( ( key >>  8 ) & 0xFF );
        str[ 3 ] = ( char ) (   key         & 0xFF );
        return 4;
    }

    return 0;
}


/***************************************
 * Returns the number of bytes an UTF-8 encoded code point
 * (stored as a number) consists of (or -1 if it's not a
 * valid UTF-8 code point)
 ***************************************/

int
utf8_get_char_bytes( FL_VAL c )
{
    int len = 1;

    if ( c <= 0x7F )
        return 1;

    while ( len < 4 && ( c & 0xC0 ) == 0x80 )
    {
        c >>= CHAR_BIT;
        len++;
    }

    if ( len == 2 && ( c & 0xE0 ) == 0xC0 )
        return 2;
    else if ( len == 3 && ( c & 0xF0 ) == 0xE0 )
        return 3;
    else if ( len == 4 && ( c & 0xF8 ) == 0xF0 )
        return 4;

    return -1;
}


/***************************************
 * Returns the number of bytes in the UTF-8 encoded code point
 * pointed to by a char pointer (or -1 if it isn't a valid UTF-8
 * encoded code point)
 ***************************************/

int
utf8_get_byte_count( const char * str )
{
    const unsigned char * p = ( const unsigned char * ) str;

    if ( p[ 0 ] <= 0x7F )
        return 1;

    if (    ( p[ 0 ] & 0xE0 ) == 0xC0
         && ( p[ 1 ] & 0xC0 ) == 0x80 )
        return 2;
    else if (    ( p[ 0 ] & 0xF0 ) == 0xE0
              && ( p[ 1 ] & 0xC0 ) == 0x80
              && ( p[ 2 ] & 0xC0 ) == 0x80 )
        return 3;
    else if (    ( p[ 0 ] & 0xF8 ) == 0xF0
              && ( p[ 1 ] & 0xC0 ) == 0x80
              && ( p[ 2 ] & 0xC0 ) == 0x80
              && ( p[ 3 ] & 0xC0 ) == 0x80 )
        return 4;
    
    return -1;
}


/***************************************
 * Returns the number of bytes in the UTF-8 encoded code point
 * before the string pointed to (or -1 if it's not a alid UTF-8
 * code point)
 ***************************************/

int
utf8_get_prev_byte_count( const char * str )
{
    const unsigned char * p = ( const unsigned char * ) str;
    int len = 1;

    if ( *--p <= 0x7F )
        return 1;

    while ( len < 4 && ( *p & 0xC0 ) == 0x80 )
    {
        --p;
        len++;
    }

    if ( len == 2 && ( *p & 0xE0 ) == 0xC0 )
        return 2;
    else if ( len == 3 && ( *p & 0xF0 ) == 0xE0 )
        return 3;
    else if ( len == 4 && ( *p & 0xF8 ) == 0xF0 )
        return 4;

    return -1;
}



/***************************************
 * Returns a pointer to the next UTF-8 encoded code point (or character,
 * skipping diacritic markers if 'skip_diacritics' i set) after the start
 * of an UTF-8 string
 ***************************************/

char *
utf8_next_char_pos( char * str,
                    int    skip_diacritics )
{
    str += utf8_get_byte_count( str );

    if ( ! skip_diacritics )
        return str;

    while ( *str && utf8_is_diacritic( utf8_to_cp( str ) ) )
        str += utf8_get_byte_count( str );

    return str;
}


/***************************************
 * Returns a pointer to the UTF-8 encoded code point (or character,
 * skipping diacritic markers if 'skip_diacritics' is set) before the
 * start of an UTF-8 string (this, of course, assumes that the pointer
 * passed to the function points into a valid UTF-8 string!).
 ***************************************/

char *
utf8_prev_char_pos( char * str,
                    int    skip_diacritics )
{
    str -= utf8_get_prev_byte_count( str );

    if ( ! skip_diacritics )
        return str;

    while ( utf8_is_diacritic( utf8_to_cp( str ) ) )
        str -= utf8_get_prev_byte_count( str );

    return str;
}


/***************************************
 * Returns if a Unicode code point is a diacritical marker (or
 * anything else that will be combined with the previous character)
 ***************************************/

int
utf8_is_diacritic( FL_VAL key )
{
    return    ( 0x0300 <= key && key <= 0x036F )
           || ( 0x1AB0 <= key && key <= 0x1AFF )
           || ( 0x1DC0 <= key && key <= 0x1DFF )
           || ( 0x20D0 <= key && key <= 0x20FF )
           || ( 0xFE20 <= key && key <= 0xFE2F );
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
