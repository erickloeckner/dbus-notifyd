# dbus-notifyd
A Linux daemon which listens for desktop notifications on DBus and triggers a Bash script. Using code from dbus-monitor, I made this modified version to watch specifically for desktop notifications and then trigger a script if it matches a text string.

The purpose is to run a script when a call comes in from the software phone system we use at my tech support job. We use USB headsets, and since Chrome can only use one audio device at a time the ring tone comes through the headphones only. I made this program so we can trigger Linux commands in a script when a call comes in, such as using aplay to trigger an audio file through the computer speakers. Aplay has flags to designate the device to use, so we can have a ring tone of our choosing play through the external speakers.

# INSALLATION

```
sudo wget -O /usr/local/bin/dbus-notifyd https://github.com/erickloeckner/dbus-notifyd/raw/master/dbus-notifyd
```

```
sudo chmod +x /usr/local/bin/dbus-notifyd
```

```
sudo mkdir /etc/dbus-notifyd/
```

```
sudo wget -O /etc/dbus-notifyd/dbus-notifyd_command.sh https://github.com/erickloeckner/dbus-notifyd/raw/master/etc/dbus-notifyd_command.sh
```

```
sudo chmod +x /etc/dbus-notifyd/dbus-notifyd_command.sh
```

```
sudo wget -O /etc/dbus-notifyd/ring.wav https://github.com/erickloeckner/dbus-notifyd/blob/master/etc/ring.wav?raw=true
```

# USAGE

Simply let the command ```dbus-notifyd``` run in a spare terminal. The script at /etc/dbus-notifyd/dbus-notifyd_command.sh can be modified to perform a different action, use another audio file of your choosing, etc. After installing, run that script to insure it is playing from the correct audio device. The "-D hw:0,0" sets the device as card 0, device 0. To find the correct settings for your particular setup, run ```aplay -l``` to list the available ouptuts. Here is the output in CentOS 7.6:
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
