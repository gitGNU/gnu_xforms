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
 * Function for determing the number of (UTF-8) characters in
 * a string. If it's not a vaid UTF-8 string -1 is returned.
 ***************************************/

ssize_t
utf8_length( const char * str )
{
    const unsigned char * p = ( const unsigned char * ) str;
    ssize_t cnt = 0;

    if ( ! str )
        return -1;

    for ( ; *p; p++, cnt++ )
    {
        if ( *p <= 0x7F )
            /* empty */ ;
        else if ( ( *p & 0xE0 ) == 0xC0 )
        {
            if ( ( *++p & 0xC0 ) != 0x80 )
                return -1;
        }
        else if ( ( *p & 0xF0 ) == 0xE0 )
        {
             if (    ( *++p & 0xC0 ) != 0x80
                  || ( *++p & 0xC0 ) != 0x80 )
                 return -1;
        }
        else if ( ( *p & 0xF8 ) == 0xF0 )
        {
             if (    ( *++p & 0xC0 ) != 0x80
                  || ( *++p & 0xC0 ) != 0x80
                  || ( *++p & 0xC0 ) != 0x80 )
                 return -1;
        }
        else
            return -1;
    }

    return cnt;
}


/***************************************
 * Converts an UTF-8 character in a string to a number
 ***************************************/

FL_Char
utf8_to_num( const char * str )
{
    FL_Char ret;
    const unsigned char * p = ( const unsigned char * ) str;

    if ( *p < 0x7F )
        return *p;

    ret = *p;

    if ( ( *p & 0xE0 ) == 0xC0 )
    {
        if ( ( p[ 1 ] & 0xC0 ) != 0x80 )
            return 0;

        return ( ret << 8 ) + p[ 1 ];
    }
    else if ( ( *p & 0xF0 ) == 0xE0 )
    {
        if (    ( p[ 1 ] & 0xC0 ) != 0x80
             || ( p[ 2 ] & 0xC0 ) != 0x80 )
            return 0;

        return ( ( ( ret << 8 ) + p[ 1 ] ) << 8 ) + p[ 2 ];
    }
    else if ( ( *p & 0xF8 ) == 0xF0 )
    {
        if (    ( p[ 1 ] & 0xC0 ) != 0x80
             || ( p[ 2 ] & 0xC0 ) != 0x80
             || ( p[ 3 ] & 0xC0 ) != 0x80 )
            return 0;

        return   ( ( ( ( ( ret << 8 ) + p[ 1 ] ) << 8 ) + p[ 2 ] ) << 8 )
               + p[ 3 ];
    }

    return 0;
}


/***************************************
 * Inserts the UTF-8 character stored in 'key' into a string.
 * Returns the number of character inserted.
 ***************************************/

int
utf8_insert( FL_Char   key,
             char    * str )
{
    if ( key <= 0x7F )
    {
        *str = ( char ) key;
        return 1;
    }
    else if ( ( key & 0xE000 ) == 0xC000 )
    {
        str[ 0 ] = ( char ) ( ( key >> 8 ) & 0xFF );
        str[ 1 ] = ( char ) ( key & 0xFF );
        return 2;
    }
    else if ( ( key & 0xF00000 ) == 0xE00000 )
    {
        str[ 0 ] = ( char ) ( ( key >> 16 ) & 0xFF );
        str[ 1 ] = ( char ) ( ( key >>  8 ) & 0xFF );
        str[ 2 ] = ( char ) ( key & 0xFF );
        return 3;
    }
    else if ( ( key & 0xF8000000 ) == 0xF0000000 )
    {
        str[ 0 ] = ( char ) ( ( key >> 24 ) & 0xFF );
        str[ 1 ] = ( char ) ( ( key >>  16 ) & 0xFF );
        str[ 2 ] = ( char ) ( ( key >>  8 ) & 0xFF );
        str[ 3 ] = ( char ) ( key & 0xFF );
        return 4;
    }

    return 0;
}


/***************************************
 * Returns the number of bytes in an UTF-8 character
 * (or -1 if it's not a valid UYF8 character)
 ***************************************/

int
utf8_get_char_bytes( FL_Char c )
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
 * Returns the number of bytes in the UTF-8 character
 * pointed to by a char pointer (or =1 if it's no a
 * valid UTF-8 character)
 ***************************************/

int
utf8_get_byte_count( const char * str )
{
    const unsigned char * p = ( const unsigned char * ) str;

    if ( *p <= 0x7F )
        return 1;


    if (    ( *p & 0xE0 ) == 0xC0
         && ( p[ 1 ] & 0xC0 ) == 0x80 )
        return 2;
    else if (    ( *p & 0xF0 ) == 0xE0
              && ( p[ 1 ] & 0xC0 ) == 0x80
              && ( p[ 2 ] & 0xC0 ) == 0x80 )
        return 3;
    else if (    ( *p & 0xF8 ) == 0xF0
              && ( p[ 1 ] & 0xC0 ) == 0x80
              && ( p[ 2 ] & 0xC0 ) == 0x80
              && ( p[ 3 ] & 0xC0 ) == 0x80 )
        return 4;
    
    return -1;
}


/***************************************
 * Returns the number of bytes in the UTF-8 character
 * before the string pointed to (or -1 if it's no a
 * valid UTF-8 character)
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
 ***************************************/

char *
utf8_next_char_pos( char * str )
{
    return str + utf8_get_byte_count( str );
}


char *
utf8_prev_char_pos( char * str )
{
    return str - utf8_get_prev_byte_count( str );
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
