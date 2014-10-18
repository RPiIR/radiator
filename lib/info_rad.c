/*
   File name: info_rad.c
   Date:      2014/10/18 07:03
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define HAVE_WIRINGPI_H
#ifdef HAVE_WIRINGPI_H
#include <wiringPi.h>
#endif

#include "info_rad.h"

#define LIGHT_PIN 29

#ifdef HAVE_WIRINGPI_H
int check_setup() {
	static int setup_st = -1;
	if (setup_st != 0) {
		setup_st = wiringPiSetup();
	}
	return setup_st;
}

int i_rad_light(int value) {
	int status = -1;
	if ((status = check_setup()) != 0) {
		return status;
	}

	pinMode(LIGHT_PIN, 1);
  	digitalWrite(LIGHT_PIN, value);
	return 0;
}

int i_rad_display(const char* str) {
	int status = -1;
	if ((status = check_setup()) != 0) {
		return status;
	}


	return 0;
}
#else
int i_rad_light(int value) {
	printf(value ? "Light ON!!!\n" : "Light OFF!!!\n");
	return 0;
}

int i_rad_display(const char* str) {
	return 0;
}
#endif



/* end of info_rad.c */
