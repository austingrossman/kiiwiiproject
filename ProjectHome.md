## EE464R Research Senior Design Project ##
# Wireless Remote based Music Synthesizer (KiiWii) #
### The University of Texas at Austin, Spring 2009 ###

---

| Austin Grossman, Milton Villeda, David Liu, Bob Yu, Nabil Aounallah |
|:--------------------------------------------------------------------|
| ![http://kiiwiiproject.googlecode.com/files/kiiwiiteam.jpg](http://kiiwiiproject.googlecode.com/files/kiiwiiteam.jpg) |

## Summary ##
  * Our senior design team designed a MIDI compatible synthesizer that allows a user to manipulate the music they play through a wireless motion-sensing remote.
  * Our final design consists of three subcomponents: a motion-sensing remote control, a MIDI/remote interpreter, and a MIDI synthesizer.
  * The remote control is responsible for detecting acceleration along three axes and transmitting this data to the interpreter. The interpreter receives both incoming MIDI events from the MIDI keyboard as well as control signals from the remote. It merges this information and sends it to the synthesizer for audible wave generation.

---

## Photos of the prototype ##
| Remote with opened case of the KiiWii System | Remote with PIC18 F4520 microcontroller, MMA7260Q accelerometer, <br>and an xBee wireless module <br>
<tr><td> <img src='http://kiiwiiproject.googlecode.com/files/system_photo.jpg' /> </td><td> <img src='http://kiiwiiproject.googlecode.com/files/remote_photo.jpg' />                         </td></tr>
<tr><td> Opened case contains the Lyrtech PADK, PIC18 F8722 microcontroller, <br>and an xBee wireless module. <br>Shown with simultaneous input from MIDI keyboard and iPod </td><td> Opened case viewed from the top                                                                  </td></tr>
<tr><td> <img src='http://kiiwiiproject.googlecode.com/files/interpreter_synthesizer_photo.jpg' /> </td><td> <img src='http://kiiwiiproject.googlecode.com/files/interpreter_synthesizer_photo2.jpg' />       </td></tr></li></ul></tbody></table>

<hr />
<h2>Short video demo ##
| <a href='http://www.youtube.com/watch?feature=player_embedded&v=Djtjx90zBlA' target='_blank'><img src='http://img.youtube.com/vi/Djtjx90zBlA/0.jpg' width='640' height=480 /></a> |
|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|