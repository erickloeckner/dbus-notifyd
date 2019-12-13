# dbus-notifyd
A Linux daemon which listens for desktop notifications on DBus and triggers a Bash script. Using code from dbus-monitor, I made this modified version to watch specifically for desktop notifications and then trigger a script if it matches a text string.

The purpose is to run a script when a call comes in from the software phone system we use at my tech support job. We use USB headsets, and since Chrome can only use one audio device at a time the ring tone comes through the headphones only. I made this program so we can trigger Linux commands in a script when a call comes in, such as using aplay to trigger an audio file through the computer speakers. Aplay has flags to designate the device to use, so we can have a ring tone of our choosing play through the external speakers.

# INSTALLATION

```
wget https://github.com/erickloeckner/dbus-notifyd/raw/master/dbus-notifyd.tar.gz
```

```
tar xvf dbus-notifyd.tar.gz
```

# USAGE

In a spare terminal, simply cd into the dbus-notifyd directory and let the command ```dbus-notifyd``` run. It will also print a timestamp for each incoming call:

```
[ekloeckner@ekloeckner ~]$ cd /home/ekloeckner/dbus-notifyd/
```

```
[ekloeckner@ekloeckner ~/dbus_test/dbus-notifyd]$ ./dbus-notifyd
DBus Notification Daemon
Watching for "Inbound Call"
```

The script in the main directory named dbus-notifyd_command.sh can be modified to perform a different action, use another audio file of your choosing, etc. After installing, run that script to insure it is playing from the correct audio device. The "-D hw:0,0" flag sets the device as card 0, device 0. To find the correct settings for your particular setup, run ```aplay -l``` to list the available ouptuts. Here is the output in CentOS 7.6:

```
[ekloeckner@ekloeckner ~/c/dbus-notifyd]$ aplay -l
**** List of PLAYBACK Hardware Devices ****
card 0: SB [HDA ATI SB], device 0: ALC889 Analog [ALC889 Analog]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 0: SB [HDA ATI SB], device 1: ALC889 Digital [ALC889 Digital]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 1: NVidia [HDA NVidia], device 3: HDMI 0 [HDMI 0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 1: NVidia [HDA NVidia], device 7: HDMI 0 [HDMI 0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 1: NVidia [HDA NVidia], device 8: HDMI 0 [HDMI 0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 1: NVidia [HDA NVidia], device 9: HDMI 0 [HDMI 0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 2: Headset [Logitech USB Headset], device 0: USB Audio [USB Audio]
  Subdevices: 0/1
  Subdevice #0: subdevice #0
  ```
  
  As you can see, I used "card 0: SB [HDA ATI SB], device 0: ALC889 Analog [ALC889 Analog]", which would be hw:0,0. The USB headset would be hw:2,0.
