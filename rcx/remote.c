/* ------------------------------------------------------------------------
 *  remote.c - Remote controled RCX
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
 */

#include <lnp.h>
#include <conio.h>
#include <string.h>
#include <lnp-logical.h>
#include <dkey.h>
#include <dsound.h>
#include <dsensor.h>
#include <dmotor.h>

#define MY_PORT 2
#define DEST_HOST 0x8
#define DEST_PORT 0x7
#define DEST_ADDR ( DEST_HOST << 4 | DEST_PORT )

typedef enum _rcx_port { 
  RCX_P_A, 
  RCX_P_B, 
  RCX_P_C,
  RCX_P_1, 
  RCX_P_2, 
  RCX_P_3 
} rcx_port;

typedef enum _rcx_motor_dir   { 
  RCX_M_OFF,
  RCX_M_FORW,
  RCX_M_REV, 
  RCX_M_BRAKE,
  RCX_M_POWER 
} rcx_motor_dir;

typedef enum _rcx_sensor_type { 
  RCX_S_TOUCH, 
  RCX_S_LIGHT, 
  RCX_S_ROTATION 
} rcx_sensor_type;

typedef struct _rcx_sensor_data {
  rcx_port         port;
  int              enabled;
  unsigned int     raw_value;
  unsigned int     sensor_reading;
  rcx_sensor_type type;
} rcx_sensor_data;

int gNewData = 0;
char gMessagingData[10];
int gDataLength;
int done = 0;
int packet_handler_done = 0;
rcx_sensor_data sensor[3];

void motor_init ( void ) {
  motor_a_speed ( 200 );
  motor_b_speed ( 200 );
  motor_c_speed ( 200 );

  motor_a_dir ( 0 );
  motor_b_dir ( 0 );
  motor_c_dir ( 0 );
}

void sensor_init ( void ) {
  int i;
  
  for ( i=0; i<3; i++ ) {
    sensor[i].enabled = 0;
    sensor[i].type    = RCX_S_TOUCH;
  }
  sensor[0].port = RCX_P_1;
  sensor[1].port = RCX_P_2;
  sensor[2].port = RCX_P_3;

  ds_rotation_set ( &SENSOR_1, 0 );
  ds_rotation_off ( &SENSOR_1 );
  ds_passive ( &SENSOR_1 );

  ds_rotation_set ( &SENSOR_2, 0 );
  ds_rotation_off ( &SENSOR_2 );
  ds_passive ( &SENSOR_2 );

  ds_rotation_set ( &SENSOR_3, 0 );
  ds_rotation_off ( &SENSOR_3 );
  ds_passive ( &SENSOR_3 );

}

int SensorWatcher ( int num, char **argv ) {
  char msg[10];
  volatile unsigned *S = &SENSOR_1;

  wakeup_t WaitForSensor ( wakeup_t data ) {
    int retval = 0;
    volatile unsigned *s = &SENSOR_1;
    int t = 0, r = 2;

    if ( ! sensor[num].enabled ) {
      return 1;
    }
    
    switch ( sensor[num].port ) {
      case RCX_P_1:
	s = &SENSOR_1;
	r = 2;
	break;
      case RCX_P_2:
	s = &SENSOR_2;
	r = 1;
	break;
      case RCX_P_3:
	s = &SENSOR_3;
	r = 0;
	break;
      default:
	break;
    }

    sensor[num].raw_value = *s;

    switch ( sensor[num].type ) {
    case RCX_S_TOUCH:
      t = (int)(sensor[num].raw_value) < 0x8000;
      if ( sensor[num].sensor_reading != t ) {
	sensor[num].sensor_reading = t;
	retval = 1;
      }
      break;
    case RCX_S_LIGHT:
      t = 147 - ds_scale(sensor[num].raw_value)/7;
      if ( sensor[num].sensor_reading != t ) {
	sensor[num].sensor_reading = t ;
	retval = 1;
      }
      break;
    case RCX_S_ROTATION:
      t = ds_rotations[r];
      if ( sensor[num].sensor_reading != t ) {
	sensor[num].sensor_reading = t ;
	retval = 1;
      }
      break;
    default:
      sensor[num].enabled = 0;
      break;
    }
    return retval;
  }

  msg[0] = 'S';
  msg[1] = (char)num;
  msg[6] = 0;
  sensor[num].sensor_reading = -1;

  while ( sensor[num].enabled ) {
    wait_event( WaitForSensor, 0 );
    /* if ( sensor[num].enabled ) dsound_system ( DSOUND_BEEP );    /* */
    memcpy ( msg+2, &sensor[num].raw_value, 2 );
    memcpy ( msg+4, &sensor[num].sensor_reading, 2 );
    lnp_addressing_write( msg, 7, DEST_ADDR, DEST_PORT); 
    if ( sensor[num].type == RCX_S_LIGHT ) {
      /* avoid message flood for light readings */
      msleep ( 100 );
    }
  }

   
  switch ( sensor[num].port ) {
  case RCX_P_1:
    S = &SENSOR_1;
    break;
  case RCX_P_2:
    S = &SENSOR_2;
    break;
  case RCX_P_3:
    S = &SENSOR_3;
    break;
  default:
    break;
  }

  ds_rotation_set ( S, 0 );
  ds_rotation_off ( S );
  ds_passive ( S );

  return 0;
}

wakeup_t prgmKeyCheck ( wakeup_t data ) {
  return dkey == KEY_PRGM;
}

void packet_handler(const unsigned char* data,unsigned char length, unsigned char src) {
  if ( gNewData == 0 ) {
    memcpy ( gMessagingData, data, length);
    gDataLength = length;
    gNewData = 1;
  }
}


int PacketWatcher( int argc, char **argv ) {
  wakeup_t WaitForData(wakeup_t data) {
    return ( gNewData | done );
  }

  while ( !done ) {
    int dir = 0;
    int s_port;
    volatile unsigned *s = &SENSOR_1;

    wait_event( WaitForData, 0 );
    if ( done ) {
      break ;
    }

    switch ( gMessagingData[0] ) {

    /* ---------------------------------------------------------- */
    /*                  MOTOR CONTROL                             */
    /* ---------------------------------------------------------- */
    case 'M':
      switch ( gMessagingData[2] ) {
      case RCX_M_OFF:
	dir = 0;
	break;
      case RCX_M_FORW:
	dir = 1;
	break;
      case RCX_M_REV:
	dir = 2;
	break;
      case RCX_M_BRAKE:
	dir = 3;
	break;
      default:
	dir = 0;
      }
      if ( gMessagingData[2] == RCX_M_POWER ) {
	switch  ( gMessagingData[1] ) {
	case RCX_P_A:
	  motor_a_speed ( gMessagingData[3] );
	  break;
	case RCX_P_B:
	  motor_b_speed ( gMessagingData[3] );
	  break;
	case RCX_P_C:
	  motor_c_speed ( gMessagingData[3] );
	  break;
	}
      } else {
	switch  ( gMessagingData[1] ) {
	case RCX_P_A:
	  motor_a_dir ( dir );
	  break;
	case RCX_P_B:
	  motor_b_dir ( dir );
	  break;
	case RCX_P_C:
	  motor_c_dir ( dir );
	  break;
	}
      }
      break; /* M */
    /* ---------------------------------------------------------- */
    /*                 SENSOR CONTROL                             */
    /* ---------------------------------------------------------- */
    case 'S':
      switch ( gMessagingData[1] ) {
      case RCX_P_1:
	s_port = 0;
	s = &SENSOR_1;
	break;
      case RCX_P_2:
	s_port = 1;
	s = &SENSOR_2;
	break;
      case RCX_P_3:
	s_port = 2;
	s = &SENSOR_3;
	break;
      default:
	s_port=0;
	break;
      }

      ds_rotation_set ( s, 0 );
      ds_rotation_off ( s );
      ds_passive ( s );

      switch ( gMessagingData[3] ) {
      case RCX_S_TOUCH:
	break;
      case RCX_S_LIGHT:
	ds_active ( s );
	break;
      case RCX_S_ROTATION:
	ds_active ( s );
	ds_rotation_on ( s );
	break;
      default:
	break;
      }

      sensor[s_port].enabled = 0;
      msleep ( 500 );
      sensor[s_port].enabled = gMessagingData[2];
      sensor[s_port].type    = gMessagingData[3];
      execi ( SensorWatcher, s_port, NULL, PRIO_NORMAL, DEFAULT_STACK_SIZE );
      break; /* S */
    default:
      break;
    }
    gNewData = 0;
  }
  packet_handler_done = 1;
  return ( 0 );
}

wakeup_t childTest(wakeup_t dummy) {
  return ( packet_handler_done );
}

int main ( int argc, char **argv ) {
  lnp_logical_range(0);
  motor_init ();
  sensor_init ();

  cputs ( " rc " );
  lnp_addressing_set_handler( MY_PORT, packet_handler);

  execi ( PacketWatcher, 0, NULL, PRIO_NORMAL, DEFAULT_STACK_SIZE );

  wait_event ( prgmKeyCheck, 0 );

  done = 1;
  wait_event( childTest, 0 );
  motor_init();
  sensor_init();
  cputs ( "end" );

  return 0;
}
