#!/bin/bash

# This script is run every time a call comes in. Test the script by running it to insure the audio is coming from the correct 
# output. The audio device with the aplay command is set with "hw:0,0", the second sound card is "hw:1,0", etc. Use only one of 
# the following commands, whichever one works obviously.

# This works with most distros:
aplay -q -D hw:0,0 ./ring.wav &

# Uncomment and use this on new workstations with fedora if aplay is not available:
# gst-launch-1.0 -q filesrc location=./ring.wav ! wavparse ! audioconvert ! audioresample ! pulsesink device ="alsa_output.pci-0000_0b_00.6.analog-stereo" &
