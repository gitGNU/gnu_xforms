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


#if ! defined _UTF8_H_
#define _UTF8_H_

#include <stdlib.h>
#include <string.h>


ssize_t utf8_length( const char * str );

int utf8_get_char_bytes( FL_Char c );

FL_Char utf8_to_num( const char * str );

int utf8_get_byte_count( const char * str );

int utf8_get_prev_byte_count( const char * str );

char * utf8_next_char_pos( char * str );

char * utf8_prev_char_pos( char * str );


/*------------------------------------------*
 * Iterator object for UTF8 strings
 *
 * A new object is to be created by calling new_UTF8_Iter() or
 * new_UTF8_revere_Iter(). Delete them with del_UTF8_Iter().
 *
 * If the new_xxx() function receives a pointer to a string it
 * already is initialized to iterate over that string. Otherwise
 * call the init() object method. This also allows to reuse the
 * iterator for another string.
 *
 * After having initialized the iterator (either by passing a
 * string tp the new_xxx() function or calling the init()
 * method you may inspect the 'char_cnr' member, which
 * contains the number of UTF8 characters in the string
 * or -1 if the string was not a valid UTF8 string.
 *
 * The methods reset() and back() can be used to set the
 * iterator to the start or the end of the string (vice versa
 * for reverse iterators).
 *
 * The next() and prev() methods return the current UTF8
 * character and move the iterator either to the right or
 * the left (vice versa for reverse iterators). If it's at the
 * end of the string (or the start for prev()) the returned
 * character is '\0'.
 *
 * The cnt() method returns the current character position of
 * the iterator in the string, pos() the current byte position.
 * At the start both (for a normal iterator) are 0. A call of
 * next() will move them one character to the right, a call of
 * prev() one character left (unless the start or end of the string
 * has been reached).
 *------------------------------------------*/

typedef struct _UTF8_Iter {
	const unsigned char * str;
	const unsigned char * cur;
	ssize_t               char_cnt;
	size_t                cur_index;

	void                  ( * init )( struct _UTF8_Iter * this,
									  const char        * str );
	ssize_t               ( * length )( struct _UTF8_Iter * this );
	void                  ( * reset )( struct _UTF8_Iter * this );
	void                  ( * back )( struct _UTF8_Iter * this );
	FL_Char               ( * next )( struct _UTF8_Iter * this );
	FL_Char               ( * prev )( struct _UTF8_Iter * this );
	size_t                ( * cnt )( struct _UTF8_Iter * this );
	size_t                ( * pos )( struct _UTF8_Iter * this );
} UTF8_Iter;

UTF8_Iter * new_UTF8_Iter( const char *str );

UTF8_Iter * new_reverse_UTF8_Iter( const char *str );

void * del_UTF8_Iter( UTF8_Iter * iter );


/* Some macros to help avoid the somewhat clunky syntax */

#define IterInit( iter, str )  ( iter )->init( ( iter ), ( str ) )
#define IterLenght( iter )     ( iter )->lenght( ( iter ) )
#define IterReset( iter )      ( iter )->reset( ( iter ) )
#define IterBack( iter )       ( iter )->back( ( iter ) )
#define IterNext( iter )       ( iter )->next( ( iter ) )
#define IterPrev( iter )       ( iter )->prev( ( iter ) )
#define IterCnt( iter )        ( iter )->cnt( ( iter ) )
#define IterPos( iter )        ( iter )->pos( ( iter ) )

#endif
