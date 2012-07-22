Raspbercopter
-------------
Hub started 7/22/12
=============

/*****************************************************************************
//All documents, files and code in this hub
//Are copyright (C) 2012 Eric Middleton

//This file is part of Raspbercopter
//Raspbercopter is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

The Raspbercopter is an autonomous quadrocopter that uses an Arduino Mega 2560 for low-level flight controls 
and a Raspberry Pi for control via cellular internet access.

Because the Raspbercopter uses two different processors, there are two sections in this github:

--------------------------------------------------------------------------------------------------------------

Arduino:
  The arduino code is written in the dialect of C++ that the Arduino IDE supports.
  
  The code is implimented as a library for simplicity.
  
  The code uses a Communicator class to communicate with the Raspberry Pi via UART.
    The user can control the quadrocopter without a raspberry pi by manually sending command messages 
    to the main Quadrotor class
  
--------------------------------------------------------------------------------------------------------------

Raspberry Pi:
  The Raspberry Pi code is written in C++ and tested with g++ on the current official Raspbian release.
  
  The code communicates with remote computers via cellular data networks to receive commands and send status updates.
  
  The code is designed to run without X and, for the most part, in a headless configuration.
  
  It is possible to recompile and upload Arduino code from the Raspberry Pi.
    You must connect a monitor and keyboard to the Raspberry Pi, connect the Arduino via USB, and 
    run the Arduino IDE in X.

  TODO:
  There will eventually be support for a secondary control link, possibly using XBee modules. This system will be 
  for short-distance point-to-point communication with a controller or 'base unit,' possibly another Raspberry 
  Pi. Communications over this link will take priority over the cellular link.