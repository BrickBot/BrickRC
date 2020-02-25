/* ------------------------------------------------------------------------
 *  motor.c -  Motor Control
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
#include <glib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>  

#include "motor.h"
#include "rcx.h"


void motor_init ( GladeXML *xml ) {
  GtkAdjustment *a_power, *b_power, *c_power;

  /* hook up rulers for power control */
  a_power = GTK_ADJUSTMENT ( gtk_adjustment_new ( 200.0, 0.0, 255.0, 1.0, 1.0, 0.0 ) );
  gtk_range_set_adjustment ( GTK_RANGE ( glade_xml_get_widget( xml, "A_SPEED" ) ), a_power );
  gtk_signal_connect ( GTK_OBJECT ( a_power ), "value-changed",  GTK_SIGNAL_FUNC ( a_power_cb ), xml );
  gtk_adjustment_value_changed ( a_power );

  b_power = GTK_ADJUSTMENT ( gtk_adjustment_new ( 200.0, 0.0, 255.0, 1.0, 1.0, 0.0 ) );
  gtk_range_set_adjustment ( GTK_RANGE ( glade_xml_get_widget( xml, "B_SPEED" ) ), b_power );
  gtk_signal_connect ( GTK_OBJECT ( b_power ), "value-changed",  GTK_SIGNAL_FUNC ( b_power_cb ), xml );
  gtk_adjustment_value_changed ( b_power );

  c_power = GTK_ADJUSTMENT ( gtk_adjustment_new ( 200.0, 0.0, 255.0, 1.0, 1.0, 0.0 ) );
  gtk_range_set_adjustment ( GTK_RANGE ( glade_xml_get_widget( xml, "C_SPEED" ) ), c_power );
  gtk_signal_connect ( GTK_OBJECT ( c_power ), "value-changed",  GTK_SIGNAL_FUNC ( c_power_cb ), xml );
  gtk_adjustment_value_changed ( c_power );

}

void a_power_cb  ( GtkAdjustment *power, gpointer data ) {
  rcx_motor ( RCX_P_A, RCX_M_POWER, (char)power->value );
}

void b_power_cb  ( GtkAdjustment *power, gpointer data ) {
  rcx_motor ( RCX_P_B, RCX_M_POWER, (char)power->value );
}

void c_power_cb  ( GtkAdjustment *power, gpointer data ) {
  rcx_motor ( RCX_P_C, RCX_M_POWER, (char)power->value );
}

void forw_cb ( GtkButton *button, gpointer data ) {
  gchar *motor = gtk_widget_get_name ( GTK_WIDGET ( button ) );
  
  switch ( motor[0] ) {
  case 'A':
    rcx_motor ( RCX_P_A, RCX_M_FORW, 0 );
    break;
  case 'B':
    rcx_motor ( RCX_P_B, RCX_M_FORW, 0 );
    break;
  case 'C':
    rcx_motor ( RCX_P_C, RCX_M_FORW, 0 );
    break;
  default:
    g_print ( "ERROR: Motor %c is not allowed\n", motor[0] );
    break;
  }
}

void rev_cb ( GtkButton *button, gpointer data ) {
  gchar *mot = gtk_widget_get_name ( GTK_WIDGET ( button ) );
  
  switch ( mot[0] ) {
  case 'A':
    rcx_motor ( RCX_P_A, RCX_M_REV, 0 );
    break;
  case 'B':
    rcx_motor ( RCX_P_B, RCX_M_REV, 0 );
    break;
  case 'C':
    rcx_motor ( RCX_P_C, RCX_M_REV, 0 );
    break;
  default:
    g_print ( "ERROR: Motor %c is not allowed\n", mot[0] );
    break;
  }
}

void brake_cb ( GtkButton *button, gpointer data ) {
  gchar *mot = gtk_widget_get_name ( GTK_WIDGET ( button ) );
  
  switch ( mot[0] ) {
  case 'A':
    rcx_motor ( RCX_P_A, RCX_M_BRAKE, 0 );
    break;
  case 'B':
    rcx_motor ( RCX_P_B, RCX_M_BRAKE, 0 );
    break;
  case 'C':
    rcx_motor ( RCX_P_C, RCX_M_BRAKE, 0 );
    break;
  default:
    g_print ( "ERROR: Motor %c is not allowed\n", mot[0] );
    break;
  }
}

void off_cb ( GtkButton *button, gpointer data ) {
  gchar *mot = gtk_widget_get_name ( GTK_WIDGET ( button ) );
  
  switch ( mot[0] ) {
  case 'A':
    rcx_motor ( RCX_P_A, RCX_M_OFF, 0 );
    break;
  case 'B':
    rcx_motor ( RCX_P_B, RCX_M_OFF, 0 );
    break;
  case 'C':
    rcx_motor ( RCX_P_C, RCX_M_OFF, 0 );
    break;
  default:
    g_print ( "ERROR: Motor %c is not allowed\n", mot[0] );
    break;
  }
}
