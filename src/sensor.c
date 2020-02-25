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
#include <stdio.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>  

#include "sensor.h"
#include "rcx.h"

static rcx_sensor_data sensor[3];
static void sensor_cb ( rcx_sensor_data sensor, void *data );

void sensor_init ( GladeXML *xml ) {
  int i;
  
  sensor[0].port = RCX_P_1;
  sensor[1].port = RCX_P_2;
  sensor[2].port = RCX_P_3;

  for ( i=0; i<3; i++ ) {
    sensor[i].enabled = FALSE;
    sensor[i].type    = RCX_S_TOUCH;
    rcx_sensor_set ( sensor[i] );
  }

  rcx_set_sensor_cb ( &sensor_cb, (void*)xml );
}

void sensor_cb ( rcx_sensor_data sensor, void *data ) {
  GladeXML *xml     = (GladeXML*)data;
  gchar *raw_name   = g_strdup ( "S1_RAW" );
  gchar *value_name = g_strdup ( "S1_VALUE" );
  gchar buffer[10];

  switch ( sensor.port ) {
  case RCX_P_2:
    raw_name[1]   = '2';
    value_name[1] = '2';
    break;
  case RCX_P_3:
    raw_name[1]   = '3';
    value_name[1] = '3';
    break;
  default:
    break;
  }

  switch ( sensor.type ) {
  case RCX_S_TOUCH:
    if ( sensor.sensor_reading ) {
      strcpy ( buffer, "ON     " );
    } else {
      strcpy ( buffer, "OFF    " );
    }
    break;
  default:
    sprintf ( buffer, "%06d", sensor.sensor_reading );
    break;
  }
  gtk_label_set_text ( GTK_LABEL (  glade_xml_get_widget( xml, value_name ) ), buffer );

  sprintf ( buffer, "%06d", sensor.raw_value );
  gtk_label_set_text ( GTK_LABEL (  glade_xml_get_widget( xml, raw_name ) ), buffer );

  g_free ( raw_name );
  g_free ( value_name );
}

void sensor_toggle ( GtkToggleButton *button, gpointer user_data) {
  gchar *sensor_name = gtk_widget_get_name ( GTK_WIDGET ( button ) );
  gint  sensor_num = ( sensor_name[1] - '0' ) - 1;
  
  sensor[sensor_num].enabled = sensor[sensor_num].enabled ? FALSE : TRUE;
  rcx_sensor_set ( sensor[sensor_num] );
}

void sensor_type ( GtkToggleButton *button, gpointer user_data) {
  gchar *sensor_name = gtk_widget_get_name ( GTK_WIDGET ( button ) );
  gint  sensor_num   = ( sensor_name[1] - '0' ) - 1;
  rcx_sensor_type  sensor_t = RCX_S_TOUCH;

  switch ( sensor_name[3] ) {
  case 'T':
    sensor_t = RCX_S_TOUCH;
    break;
  case 'L':
    sensor_t = RCX_S_LIGHT;
    break;
  case 'R':
    sensor_t = RCX_S_ROTATION;
    break;
  }

  if ( sensor_t != sensor[sensor_num].type ) {
    sensor[sensor_num].type = sensor_t;
    rcx_sensor_set ( sensor[sensor_num] );
  }
}
