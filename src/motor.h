/* ------------------------------------------------------------------------
 *  motor.h - Motor control
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

#ifndef _MOTOR_H_
#define _MOTOR_H_

extern void motor_init ( GladeXML *xml );

/* 
 * signal handler for motor control
 *
 * The port (A,B,C) is taken from the first character of the widget name 
 * emitting the signal (sorry for this hack, but somehow glade has issues
 * with passing user data to signal functions...)
 */
extern void forw_cb  ( GtkButton *button, gpointer data ); 
extern void rev_cb   ( GtkButton *button, gpointer data ); 
extern void brake_cb ( GtkButton *button, gpointer data ); 
extern void off_cb   ( GtkButton *button, gpointer data ); 
extern void power_cb ( GtkButton *button, gpointer data ); 

extern void a_power_cb  ( GtkAdjustment *button, gpointer data ); 
extern void b_power_cb  ( GtkAdjustment *button, gpointer data ); 
extern void c_power_cb  ( GtkAdjustment *button, gpointer data ); 

#endif 
