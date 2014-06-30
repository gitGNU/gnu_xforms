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

int utf8_insert( FL_Char   key,
                 char    * str );

int utf8_get_byte_count( const char * str );

int utf8_get_prev_byte_count( const char * str );

char * utf8_next_char_pos( char * str );

char * utf8_prev_char_pos( char * str );


#endif

/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
