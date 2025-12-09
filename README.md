This 2.25-inch, 76x284 pixel ST7789 TFT is popular, but no simple driver was readily available. The standard libraries consume around 16K of flash, which is often too much. This version—without demo code—requires only about 2K (including text functions) or as little as 1300 bytes for the core driver.

It has been tested on the AVR Arduino Mini, WCH CH32V003, and CH32V006.

Regarding the CH32V00x series:
On these chips, the driver itself uses about 10K of flash. On the CH32V003, roughly 6K remains free, which is still usable. The CH32V006 has ample space, but it is surprisingly poorly supported by Arduino. The only non-official core that successfully compiles code for the V006 is the CH32VM00X core by Maxint-RD, available here:
github.com/maxint-rd/arduino_core_ch32/tree/support-CH32V002F4-CH32V006F8/variants/CH32VM00X.
 Find it here https://github.com/openwch/arduino_core_ch32/pull/200, author Maxint-RD, select CH32VM00X core. Link : https://github.com/maxint-rd/arduino_core_ch32/tree/support-CH32V002F4-CH32V006F8/variants/CH32VM00X
For uploading to the CH32V006 https://github.com/maxint-rd/arduino_core_ch32/blob/support-CH32V002F4-CH32V006F8/variants/CH32VM00X/README.md#updating-openocd. Update OpenOCD section.

Why the V006 isn't more popular is puzzling. While the V003 and V006 are priced the same, the V006 is far superior in Flash memory, RAM, and peripheral features. There’s little reason for the V003 to receive so much more software support. Hopefully, a future Arduino IDE update will resolve this disparity.
