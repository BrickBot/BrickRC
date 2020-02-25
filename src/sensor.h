/* ------------------------------------------------------------------------
 *  sensor.c -  Sensor Control and Display
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
#ifndef _SENSOR_H_
#define _SENSOR_H_
extern void sensor_init ( GladeXML *xml );

extern void sensor_toggle ( GtkToggleButton *button, gpointer user_data);
extern void sensor_type ( GtkToggleButton *button, gpointer user_data);
#endif

