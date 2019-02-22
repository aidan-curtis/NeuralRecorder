---
# rfpackettx

---

Example Summary
---------------
The Packet TX example illustrates how to do simple packet transmission using
the RF driver. This example is meant to be used with the Packet RX
example or SmartRF Studio. For every packet transmitted, Board_PIN_LED1 is
toggled.  The frequency and other RF settings can be modified using SmartRF
Studio.

Peripherals Exercised
---------------------
* `Board_PIN_LED1` - Toggled when data is transmitted over the RF interface


Resources & Jumper Settings
---------------------------
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Example Usage
-------------
Run the example. On another board, run the Packet RX example.
Board_PIN_LED1 is toggled when data is transmitted.

Application Design Details
--------------------------
This examples consists of a single task and the exported SmartRF Studio radio
settings.

The default frequency is 868.0 MHz. In order to change frequency, modify the
smartrf_settings file. This can be done using the code export feature in
Smart RF Studio, or directly in the file.

The default frequency is 868.0 MHz, in order to change RF settings please
modify the smartrf_settings.c file.
This can be done either by exporting from Smart RF Studio or directly in the
file.

When the task is executed it:

1. Configures the radio for Proprietary mode
2. Gets access to the radio via the RF drivers RF_open
3. Sets up the radio using CMD_PROP_RADIO_DIV_SETUP command
4. Set the output power to 14 dBm (requires that CCFG_FORCE_VDDR_HH = 1 in
ccfg.c)
5. Sets the frequency using CMD_FS command
6. Create packet (with increasing sequence number and random content)
7. Set absolute TX time to utilize automatic power management
8. Transmit packet using CMD_PROP_TX command with blocking RF driver call
9. Toggle Board_PIN_LED1 to indicate packet transmitted
10. Transmit packets forever by repeating step 6-9

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.
