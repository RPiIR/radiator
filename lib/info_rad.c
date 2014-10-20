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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

#else
int i_rad_light(int value) {
	printf(value ? "Light ON!!!\n" : "Light OFF!!!\n");
	return 0;
}

#endif

// define our pins :
#define DATA        0  // GPIO17 (WiringPi pin num 0) header pin 11
#define CLOCK       2  // GPIO27 (WiringPi pin num 2) header pin 13
#define LOAD        3  // GPIO22 (WiringPi pin num 3) header pin 15

// define max display digits (actual digits -1)
#define DIGITS      0x07

// The Max7219 Registers :

#define NO_OP         0x00
#define DECODE_MODE   0x09                       
#define INTENSITY     0x0a                        
#define SCAN_LIMIT    0x0b                        
#define SHUTDOWN      0x0c                        
#define DISPLAY_TEST  0x0f                         

// Register Values
#define MODE_B      0xFF // Table 4, datasheet, bit position enables BCD decoding
#define MODE_7SEG   0x01

static void Send16bits (unsigned short output)
{
   
  unsigned char i;

  for (i=16; i>0; i--) 
  {
    unsigned short mask = 1 << (i - 1); // calculate bitmask
  
    digitalWrite(CLOCK, 0);  // set clock to 0
    
    // Send one bit on the data pin
    
    if (output & mask)  {
#ifdef DEBUG
	putchar('1');
#endif
        digitalWrite(DATA, 1);         
    } 
    else                              
    {
#ifdef DEBUG
	putchar('0');
#endif
        digitalWrite(DATA, 0);  
    }
        
    digitalWrite(CLOCK, 1);  // set clock to 1
      
  }
#ifdef DEBUG
   putchar('\n');
#endif
}

// Take a reg numer and data and send to the max7219

static void MAX7219Send (unsigned char reg_number, unsigned char dataout)
{
	digitalWrite(LOAD, 1);  // set LOAD 1 to start
	Send16bits((reg_number << 8) + dataout);   // send 16 bits ( reg number + dataout )
	digitalWrite(LOAD, 0);  // LOAD 0 to latch
	digitalWrite(LOAD, 1);  // set LOAD 1 to finish
}

// Initialise 7219
//
// To try and ensure device initialised correctly always
// 
// Leaves device in Test mode
//
// Pass in
//   digits - number of digits to display - 1 (1 digit pass in 0)
//   mode   - display mode 7 segment (MODE_7SEG) control
//         Segment Data   bit 0 = seg A to bit 6 = seg G and bit 7 = DP
//       or decode special set (MODE_B) as below
//         Segment Data   bits 0 to 3: 0 to 15 =  0 to 9, -, E, H, L, P, and ' ' (blank)
//         note top 4 bits ignored
//   level  - intensity level (1-15)
//
//  Exits program if no wiringPi installed or invalid parameters or LOAD fails to go high
//
void Init7219( int digits, int mode, int level )
{
// Initial checks
if( wiringPiSetup () == -1 )  exit( 1 );

if( digits < 0 || digits > 7 )
{
    printf( "\nError Init7219 called with invalid number of digits of %d (should be 0 to 7)\n", digits );
    exit( 1 );
}


//if( mode != MODE_B || mode != MODE_7SEG || level < 0 || level > 15 )
if( level < 0 || level > 15 )
{
    printf( "\nError Init7219 invalid brigthness of %d!\n", level);
    exit( 1 );
}

//We need 3 output pins to control the Max7219: Data, Clock and Load
pinMode( DATA, OUTPUT );  
pinMode( CLOCK, OUTPUT );
pinMode( LOAD, OUTPUT );  

// Initial states of pins
digitalWrite( LOAD, 1 );     // set LOAD 1 to start
digitalWrite( CLOCK, 0 );     // set CLOCK to 0
digitalWrite( DATA, 0 );     // set DATA to 0

// Quick test check LOAD pin actually HIGH as test
if( digitalRead( LOAD ) != 1 )
{
  printf( "\nError Max7219 LOAD pin %u failed to go high\n", LOAD );
  exit( 2 );
}

// Ensure shift refister is all zeros no random clocks occured
// and loaded into device to synchronise shift register
MAX7219Send( NO_OP, 0 );

MAX7219Send( SCAN_LIMIT, digits );   // set up to scan number of digits
MAX7219Send( DECODE_MODE, mode );    // Set BCD decode mode
MAX7219Send( DISPLAY_TEST, 0);       // disable test mode
MAX7219Send( INTENSITY, level );     // set brightness 0 to 15
MAX7219Send( SHUTDOWN, 1 );          // Ensure in Normal mode of operation
}

////////////////////////////////////////////////////////////////////////////////

char REGISTER[DIGITS+1] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

////////////////////////////////////////////////////////////////////////////////
int i_rad_display(const char* input) {
	int status = -1;
	if ((status = check_setup()) != 0) {
		return status;
	}

	signed char i = 0; // char position
	unsigned char j; // register counter

	i = strlen(input) - 1;
	j = 0;
	for(; i >= 0; i--) {
		printf("%c ", (char) input[i]); 
		switch(input[i]) {
			case '-': REGISTER[j++] |= 0xa; break;
			case 'e':
                        case 'E': REGISTER[j++] |= 0xb; break;
			case 'h':
                        case 'H': REGISTER[j++] |= 0xc; break;
			case 'l':
                        case 'L': REGISTER[j++] |= 0xd; break;
			case 'p':
                        case 'P': REGISTER[j++] |= 0xe; break;
			case ' ': REGISTER[j++] |= 0xf; break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': REGISTER[j++] |= (input[i] - 48); break;
			case '.': REGISTER[j] = 0x80; break; // set dot
			default:
				printf("Error value '%s'!\n", input[i]);
				exit(1);
				break;
		}
	
		if( j > DIGITS + 1 ) {
			printf("Input longer then expected!\n");
			exit(1);
		}
	}


  	Init7219( DIGITS, MODE_B, 8 );

	for(i = 0; i<8; i++)
	{
		printf("REGISTER[%d] = '%#x'\n", i, REGISTER[i]);
		MAX7219Send(i+1, REGISTER[i]);
	}

	return 0;
}

