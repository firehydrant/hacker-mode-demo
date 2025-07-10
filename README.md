# hacker-mode-demo

An IoT demo using [Firehydrant](https://firehydrant.com) Signals's Hacker Mode to flash lights in response to an alert.  

A single combined sketch to do everything was too large to fit on the board I got for this project, so these sketches ran on a pair of ESP32 controllers, a [Seed Studio XIAO ESP32C6](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/) for the zigbee code and an [ESP32C3 Super Mini](https://www.espboards.dev/esp32/esp32-c3-super-mini/) for the wifi code.

Note: When uploading the zigbee sketch, be sure to have the Partition Scheme set to "Zigbee ZCZR 4MB with spiffs" in addition to setting the Zigbee mode to "Zigbee ZBCR(coordinator/router)".  This can be done via the Arduino IDE Tools menu or see instructions for your IDE or command line upload tool for instructions on this.

As written, the wifi sketch will connect with your Hacker Mode URL and raise the voltage on pin 3 if it finds an alert.  This is set to scan every 5 seconds, which is more than sufficient for this demo and keeps us from getting rate limited.

The zigbee sketch will pair with any new lights in pairing mode it finds for the first 3 minuts.  Then it will read pin 2 and flash the lights if it reads a signal.  In this way, it is acting as both a zigbee switch and a hub/coordinator.

NOTE: In the below diagram, I've wired both boards together so that only one should be plugged in to power(5v).  DO NOT PLUG BOTH BOARDS IN WITH THIS WIRING.  Flash them separately, then complete the wiring and plug one only in for power.  Alternately, you can remove the the red wire in the below diagram (connecting the 5v pins of both boards together) and then plug the boards in separately.

## Wiring diagram

![demo wiring diagram](/images/demo_bb.png)

## Enabling hacker mode
Signals hacker mode must be enabled for your organization.  From the [Settings page](https://app.firehydrant.io/org/firehydrant/settings/organization) (the gear icon in the upper right), scroll down on the General > Organization page and make sure the toggle for Enable Signals Hacker Mode is on:

![enable signals hacker mode](/images/enable_hacker_mode.png)

After that, you'll find your personal hacker mode URL on the bottom of the [Signals Notifications tab](https://app.firehydrant.io/org/firehydrant/account/notification_preferences) of your account page:

![hacker mode url](/images/hacker_mode_url.png)

Once you have this, you're ready to go!  You can test your hacker mode URL via `curl` and see it return `no_active_alerts` if everything is clear or `has_active_alerts` if you have an alert in the opened state, whether you were targeted directly, via a Team, or via an Escalation Policy.  You can test sending yourself an alert via the Send Alert button at the top of the web application.

![send alert button](/images/send_alert.png)

Not using FireHydrant Signals yet?  Learn more about it [here](https://firehydrant.com/signals/).
