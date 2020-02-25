/* ------------------------------------------------------------------------
 *  rcx.h - Communication with the LEGO brick
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

#ifndef _RCX_H_
#define _RCX_H_

typedef enum _rcx_port { 
  RCX_P_A, 
  RCX_P_B, 
  RCX_P_C,
  RCX_P_1, 
  RCX_P_2, 
  RCX_P_3 
} rcx_port;

typedef enum _rcx_motor_dir { 
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
  int              raw_value;
  int              sensor_reading;
  rcx_sensor_type type;
} rcx_sensor_data;

extern int rcx_init_lnp ( void );
extern void rcx_motor ( rcx_port port, rcx_motor_dir dir, char data );
extern void rcx_sensor_set ( rcx_sensor_data sensor );
extern void rcx_set_sensor_cb ( void (*cb)(rcx_sensor_data event, void *user_data), void *user_data  );
#endif
