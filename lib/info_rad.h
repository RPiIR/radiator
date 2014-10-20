/*
   File name: info_rad.h
   Date:      2014/10/18 06:58
   Author:    

   Copyright (C) 2002 

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   in a file called COPYING along with this program; if not, write to
   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
   02139, USA.
*/


#ifndef INFO_RAD_H
#define INFO_RAD_H

#ifdef __cplusplus
extern "C" {
#endif

int i_rad_light(int value);
int i_rad_display(const char* str);

#ifdef __cplusplus
}
#endif

#endif

/* end of info_rad.h */
