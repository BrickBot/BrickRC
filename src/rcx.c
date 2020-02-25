/* ------------------------------------------------------------------------
 *  rcx.c - Communication with the LEGO brick
 * ------------------------------------------------------------------------ 
 * (C) 2003 by Bodo Bauer <bb@bb-zone.com>
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details. 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
 * MA 02111-1307, USA. 
 * ------------------------------------------------------------------------ 
 * $Revision: 1.2 $ 
 * ------------------------------------------------------------------------ 
 */

#include <liblnp.h>
#include "rcx.h"

#define MY_PORT   7
#define DEST_HOST 0
#define DEST_PORT 2
#define DEST_ADDR ( DEST_HOST << 4 | DEST_PORT )

static void packet_handler ( const unsigned char* data,unsigned char length, unsigned char src );
static void (*cb_sensor)(rcx_sensor_data event, void *user_data) = (void *)0;
static void *cb_user_data = (void *)0;
static rcx_sensor_data rcx_sensor[3];

int rcx_init_lnp ( void ) {
  int p;

  if ( lnp_init ( 0, 0, 0, 0, 0 ) ) {
    return 0;
  } 

  for ( p=0; p<3; p++ ) {
    rcx_sensor[p].type           = RCX_S_TOUCH;
    rcx_sensor[p].enabled        = 0;
    rcx_sensor[p].raw_value      = 0;
    rcx_sensor[p].sensor_reading = 0;
  }

  rcx_sensor[0].port = RCX_P_1;
  rcx_sensor[1].port = RCX_P_2;
  rcx_sensor[2].port = RCX_P_3;

  lnp_addressing_set_handler( MY_PORT, packet_handler);
  return 1;
}

void rcx_set_sensor_cb ( void (*cb)(rcx_sensor_data event, void *user_data), void *user_data  ) {
  cb_sensor = cb;
  cb_user_data = user_data;
}

void packet_handler ( const unsigned char* data, unsigned char length, unsigned char src ) {
  if ( data[0] == 'S' ) {
    rcx_sensor_data sensor;
    switch ( data[1] ) {
    case 0:
      sensor.port = RCX_P_1;
      break;
    case 1:
      sensor.port = RCX_P_2;
      break;
    case 2:
      sensor.port = RCX_P_3;
      break;
    }

    sensor = rcx_sensor[(int)data[1]];
    sensor.raw_value      = ((int)data[2] >> 8 ) + data[3];
    sensor.sensor_reading = ((int)data[4] >> 8 ) + data[5];

    if ( cb_sensor ) {
      (*cb_sensor)( sensor, cb_user_data );
    }
  }
}

void rcx_motor ( rcx_port port, rcx_motor_dir dir, char data ) {
  char msg[5];

  msg[0] = 'M'; 
  msg[1] = port;
  msg[2] = dir; 
  msg[3] = data;  
  msg[4] = 0;     
  lnp_addressing_write( msg, 4, DEST_ADDR, MY_PORT); 
}

void rcx_sensor_set ( rcx_sensor_data sensor ) {
  char msg[5];
  int p;

  switch ( sensor.port ) {
  case RCX_P_1:
    p = 0;
    break;
  case RCX_P_2:
    p = 1;
    break;
  case RCX_P_3:
    p = 2;
    break;
  default:
    p = 0;
    break;
  }

  rcx_sensor[p].type    = sensor.type;
  rcx_sensor[p].enabled = sensor.enabled;

  msg[0] = 'S';            
  msg[1] = sensor.port;    
  msg[2] = sensor.enabled; 
  msg[3] = sensor.type;
  msg[4] = 0;
  lnp_addressing_write( msg, 4, DEST_ADDR, MY_PORT); 
}
