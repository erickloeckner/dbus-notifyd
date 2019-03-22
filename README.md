# dbus-notifyd
A Linux daemon which listens for desktop notifications on DBus and triggers a Bash script. Using code from dbus-monitor, I made this modified version to watch specifically for desktop notifications and then trigger a script if it matches a text string.

The purpose is to run a script when a call comes in from the software phone system we use at my tech support job. We use USB headsets, and since Chrome can only use one audio device at a time the ring tone comes through the headphones only. I made this program so we can trigger Linux commands in a script when a call comes in, such as using aplay to trigger an audio file through the computer speakers. Aplay has flags to designate the device to use, so we can have a ring tone of our choosing play through the external speakers.

# INSALLATION

sudo wget -O /usr/local/bin/dbus-notifyd https://github.com/erickloeckner/dbus-notifyd/raw/master/dbus-notifyd
sudo chmod +x /usr/local/bin/dbus-notifyd
sudo mkdir /etc/dbus-notifyd/
sudo wget -O /etc/dbus-notifyd/dbus-notifyd_command.sh https://github.com/erickloeckner/dbus-notifyd/raw/master/etc/dbus-notifyd_command.sh
sudo chmod +x /etc/dbus-notifyd/dbus-notifyd_command.sh
sudo wget -O /etc/dbus-notifyd/ring.wav https://github.com/erickloeckner/dbus-notifyd/blob/master/etc/ring.wav?raw=true
