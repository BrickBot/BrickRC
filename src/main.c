/* ------------------------------------------------------------------------
 *  main.c -  brickRC
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
#include <gnome.h> 
#include <glade/glade.h>  

#include "main.h"
#include "rcx.h"
#include "motor.h"
#include "sensor.h"

/*
 * ------------------------------------------------------------------------ 
 * initialization and main loop
 * ------------------------------------------------------------------------ 
 */
int main ( int argc, char *argv[] ) {
  GladeXML *Xml = NULL;
  gchar *xml_file;
  gnome_init ( APP_NAME, APP_VERSION, argc, argv );
  glade_gnome_init ();                     
   
  xml_file = g_strjoin ( "/", DATADIR, XML_FILE, NULL );

  if ( ! rcx_init_lnp () ) {
    Xml = glade_xml_new( xml_file, "LnpError" );
    if ( ! Xml ) { 
      g_warning ( "Can't create the interface" );          
    } else { 
      glade_xml_signal_autoconnect ( Xml );                
    }
  } else {
    Xml = glade_xml_new ( xml_file, "brickRC" );   
    if ( ! Xml ) { 
      g_warning ( "Can't create the interface" );          
    } else { 
      motor_init ( Xml );
      sensor_init ( Xml );
      glade_xml_signal_autoconnect ( Xml );                
    }
  }
  gtk_main ();  
  return ( 0 );
}
