#!/bin/bash

# This script is run every time a call comes in. Test the script by running it to insure the audio is coming from the correct 
# output. The audio device with the aplay command is set with "plughw:0,0", the second sound card is "plughw:1,0", etc. Use
# only one of the following commands, whichever one works obviously.

# This works with most distros. Use "aplay -l" to list the devices. Find the desired device. "card X:" is the first number,
# and "device X:" is the second number. My workstation shows "card 1: SB [HDA ATI SB], device 0: ALC889 Analog [ALC889 Analog]"
# so it would be "-D plughw:1,0" to use the built in soundcard.
aplay -q -D plughw:0,0 ./ring.wav &

# Uncomment and use this on new workstations with fedora if aplay is not available. The device name should be correct for the
# current hardware:
#
# gst-launch-1.0 -q filesrc location=./ring.wav ! wavparse ! audioconvert ! audioresample ! pulsesink device ="alsa_output.pci-0000_0b_00.6.analog-stereo" &
