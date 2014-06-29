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
 * a string. If it's not a vaid UTF8 string -1 is returned.
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
 * Converts an UTF8 character in a string to a number
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
 * Returns the number of bytes in an UTF8 character
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
 * Returns the number of bytes in the UTF8 character
 * pointed to by a char pointer (or =1 if it's no a
 * valid UTF8 character)
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
 * Returns the number of bytes in the UTF8 character
 * before the string pointed to (or -1 if it's no a
 * valid UTF8 character)
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


/***************************************
 * Sets up an UTF8 string iterator with a new string
 ***************************************/

static void
utf8_iter_init( UTF8_Iter  * this,
				const char * str )
{
	this->char_cnt = utf8_length( str );
	this->cur_index = 0;
	if ( this->char_cnt >= 0 )
		this->str = this->cur = ( const unsigned char * ) str;
}


/***************************************
 * Sets up a reverse UTF8 string iterator with a new string
 ***************************************/

static void
utf8_reverse_iter_init( UTF8_Iter  * this,
						const char * str )
{
	this->char_cnt = utf8_length( str );
	if ( this->char_cnt >= 0 )
	{
		this->str       = ( const unsigned char * ) str;
		this->cur       = this->str + strlen( str );
		this->cur_index = this->char_cnt;
	}
}


/***************************************
 * Returns the number of UTF8 characters in the iterators string
 * (or -1 if the iterator isn't initialized or the string wasn't
 * a valid UTF8 string).
 ***************************************/

static ssize_t
utf8_iter_length( UTF8_Iter  * this )
{
	return this->char_cnt;
}


/***************************************
 * Returns the next UTF8 character from the string
 ***************************************/

static FL_Char
utf8_iter_next( UTF8_Iter * this )
{
	FL_Char ret = *this->cur++;

	if ( ! ret )
	{
		this->cur--;
		return 0;
	}

	this->cur_index++;

	if ( ret <= 0x7F )
		return ret;

	if ( ( ret & 0xE0 ) == 0xC0 )
		return ( ret << 8 ) + *this->cur++;

	if ( ( ret & 0xF0 ) == 0xE0 )
	{
		ret  = ( ret << 8 ) + *this->cur++;
		return ( ret << 8 ) + *this->cur++;
	}		 

	ret  = ( ret << 8 ) + *this->cur++;
	ret  = ( ret << 8 ) + *this->cur++;
	return ( ret << 8 ) + *this->cur++;
}


/***************************************
 * Returns the previous UTF8 character from the string
 ***************************************/

static FL_Char
utf8_iter_prev( UTF8_Iter * this )
{
	FL_Char ret;

	if ( this->cur == this->str )
		return 0;

	this->cur_index--;

	if ( ( ret = *--this->cur ) <= 0x7F )
		return ret;

	ret += ( FL_Char ) *--this->cur << 8;
	if ( ( ret & 0xE000 ) == 0xC000 )
		return ret;

	ret += ( FL_Char ) *--this->cur << 16;
	if ( ( ret & 0xF00000 ) == 0xE00000 )
		return ret;

	return ret + ( ( FL_Char ) *--this->cur << 24 );
}


/***************************************
 * Resets an UTF string iterator to the start of the string
 ***************************************/

static void
utf8_iter_reset( UTF8_Iter  * this )
{
	this->cur       = this->str;
	this->cur_index = 0;
}


/***************************************
 * Resets a UTF8 string iterator to the end of the string
 ***************************************/

static void
utf8_iter_back( UTF8_Iter  * this )
{
	this->cur       = this->str + strlen( ( const char * ) this->str );
	this->cur_index = this->char_cnt;
}


/***************************************
 * Returns the position of the current UTF8 character
 ***************************************/

static size_t
utf8_iter_cur_cnt( UTF8_Iter  * this )
{
	return this->cur_index;
}


/***************************************
 * Returns the byte position of the current UTF8 character
 ***************************************/

static size_t
utf8_iter_cur_index( UTF8_Iter  * this )
{
	return this->cur - this->str;
}


/***************************************
 * Creates a new UTF8 string iterator and, if the argument
 * is not NULL, initializes it to the start of that string
 ***************************************/

UTF8_Iter *
new_UTF8_Iter( const char *str )
{
	UTF8_Iter * iter = malloc( sizeof *iter );

	iter->str = iter->cur = ( const unsigned char * ) "";
	iter->char_cnt = -1;
	iter->cur_index = 0;

	iter->init   = utf8_iter_init;
	iter->length = utf8_iter_length;
	iter->reset  = utf8_iter_reset;
	iter->back   = utf8_iter_back;
	iter->next   = utf8_iter_next;
	iter->prev   = utf8_iter_prev;
	iter->cnt    = utf8_iter_cur_cnt;
	iter->pos    = utf8_iter_cur_index;

	if ( str )
		iter->init( iter, str );

	return iter;
}


/***************************************
 * Creates a new reverse UTF8 string iterator and, if the argument
 * is not NULL, initializes it to the end of that string
 ***************************************/

UTF8_Iter *
new_reverse_UTF8_Iter( const char *str )
{
	UTF8_Iter * iter = malloc( sizeof *iter );

	iter->str       = iter->cur = ( const unsigned char * ) "";
	iter->char_cnt  = -1;
	iter->cur_index = 0;

	iter->init   = utf8_reverse_iter_init;
	iter->length = utf8_iter_length;
	iter->reset  = utf8_iter_back;
	iter->back   = utf8_iter_reset;
	iter->next   = utf8_iter_prev;
	iter->prev   = utf8_iter_next;
	iter->cnt    = utf8_iter_cur_cnt;
	iter->pos    = utf8_iter_cur_index;

	if ( str )
		iter->init( iter, str );

	return iter;
}


/***************************************
 * Deletes an UTF8 string iterator
 ***************************************/

void *
del_UTF8_Iter( UTF8_Iter * iter )
{
	if ( iter )
		free( iter );
	return NULL;
}


#if 0

/***************************************
 * Simple test of the most relevant functions
 ***************************************/

int
main( int    argc,
	  char * argv[ ] )
{
	int i, cnt, pos;

	UTF8_Iter * it  = new_UTF8_Iter( argv[ 0 ] );
	UTF8_Iter * rit = new_reverse_UTF8_Iter( argv[ 0 ] );

	for ( i = 0; i < argc; i++ )
	{
		FL_Char c;

		if ( i > 0 )
		{
			it->init( it, argv[ i ] );
			rit->init( rit, argv[ i ] );
		}

		do
		{
			cnt = it->cnt( it );
			pos = it->pos( it );
			printf( "%d (%d) 0x%lx\n", cnt, pos, c = it->next( it ) );
		} while ( c );
		printf( "\n" );

		it->back( it );
		do
		{
			cnt = it->cnt( it );
			pos = it->pos( it );
			printf( "%d (%d) 0x%lx\n", cnt, pos, c = it->prev( it ) );
		} while ( c );
		printf( "\n" );

		do
		{
			cnt = rit->cnt( rit );
			pos = rit->pos( rit );
			printf( "%d (%d) 0x%lx\n", cnt, pos, c = rit->next( rit ) );
		} while ( c );
		printf( "\n" );

		rit->back( rit );
		do
		{
			cnt = rit->cnt( rit );
			pos = rit->pos( rit );
			printf( "%d (%d) 0x%lx\n", cnt, pos, c = rit->prev( rit ) );
		} while ( c );
		printf( "\n" );

	}

	del_UTF8_Iter( rit );
	del_UTF8_Iter( it );

	return 0;
}

#endif
