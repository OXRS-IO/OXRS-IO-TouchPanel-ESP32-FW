
# OXRS-IO-TouchPanel-WIN-APP
## OXRS-IO-Touchpanel emulator for Windows

This emulator runs the `OXRS-IO-TouchPanel` SW on a Windows computer. It gives interested user the ability to try out the features of the `OXRS-TouchPanel` without having the target HW in hands. Developed configurations and flows will work later on the target panel.

**Install and start:**

Extract the included zip file to any empty folder (usb drive should work as well).

There are two executables included: 
- OXRS-IO-TouchPanel-WIN-APP_320x480.exe (simulates the WT32 SC01 / PLUS)
- OXRS-IO-TouchPanel-WIN-APP_480x480.exe (simulates the WT82 86V / 86S)
  
Launch the desired version with double click.

##### Hint: launch only one of the executables !

##### Hint: the sw reacts very laggy without the MQTT conection configured !


**Behavior:**

The App runs a HTTP server on port 80 for the REST api endpoints. This will probably raise a Windows FireWall warning on first start.

Win10 user will get a minimized CLI, Win11 user will get a CLI window on the desktop which may be manually minimized. Some internal information may be shown here.

Use the `AdminUI`, connect to the App and set the MQTT parameter. If the App and the AdminUI are running on the same computer, #localhost# can be used for IP, other wise use the IP of the Windows computer the App is running on.

Then you can configure the App as usual.

The Actions `Edit Config` and `SetUp MQTT` from the `AdminUI` are fully functional.

The screen configuration is stored in `config.json` and the MQTT configuration in `mqtt.json`. Both are written to the above created folder.

Everything looks and feels like on the physical wall panel

MQTT topics  `conf/` , `cmnd/` and `stat/` are working as with the FW.

This version is up to date with release 2.4.6 of the FW

EDIT:

This version has additional executables:
- OXRS-IO-TouchPanel-WIN-APP-SPAN_320x480
- OXRS-IO-TouchPanel-WIN-APP-SPAN_480x480

These offer the capability to span a tile over multiple tiles (cells) to make the layout more flexible.

To configure a spanned tile add the key "span" to the tile config payload:
"span":{"x": <number of tiles to span right>, "y": <number of tiles to span down>}

The tile indexes are still following the grid layout, not the larger spanned tiles. Make sure NOT to config tiles (cells) which are allocated by a spanned tile (would create cluttered screens)

Spanning tiles is not part of the AdminUI yet.





