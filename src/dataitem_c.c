/***************************************************************************
 *   Copyright (C) 2007, IBM                                               *
 *                                                                         *
 *   Maintained By:                                                        *
 *   Eric Munson and Brad Peters                                           *
 *   ebmunson@us.ibm.com, bpeters@us.ibm.com                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the Lesser GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or at your option) any later version.                        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the Lesser GNU General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "libvpd-2/dataitem.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct dataitem* new_dataitem( )
{
	return calloc( 1, sizeof( struct dataitem ) );
}

void free_dataitem( struct dataitem *freeme )
{
	struct dataitem *this = NULL;

	while (freeme) {
		if (freeme->humanName)
			free(freeme->humanName);
		if (freeme->ac)
			free(freeme->ac);
		if (freeme->dataValue)
			free(freeme->dataValue);

		this = freeme->next;
		free(freeme);
		freeme = this;
	}
}

int calc_packed_length_dataitem( struct dataitem *packme )
{
	int ret = 3;
	if( !packme )
		return 0;

	if( packme->ac )
		ret += strlen( packme->ac );
	if( packme->dataValue )
		ret += strlen( packme->dataValue );
	if( packme->humanName )
		ret += strlen( packme->humanName );

	return ret;
}

struct dataitem * unpack_dataitem( void *buffer )
{
	if (buffer == NULL)
		return NULL;

	char *buf = (char*)buffer;
	struct dataitem *ret = NULL;

	ret = new_dataitem( );
	if( !ret )
		return ret;

	ret->ac = strdup( buf );
	if (ret->ac == NULL)
		goto unpackerr;
	buf += strlen( ret->ac ) + 1;

	ret->humanName = strdup( buf );
	if (ret->humanName == NULL)
		goto unpackerr;
	buf += strlen( ret->humanName ) + 1;

	ret->dataValue = strdup( buf );
	if (ret->dataValue == NULL)
		goto unpackerr;

	return ret;

unpackerr:
	free_dataitem(ret);
	return NULL;
}

void add_dataitem( struct dataitem *head, const struct dataitem *addme )
{
	if( !head || !addme )
		return;

	while( head->next )
		head = head->next;
	head->next = (struct dataitem *) addme;
}
