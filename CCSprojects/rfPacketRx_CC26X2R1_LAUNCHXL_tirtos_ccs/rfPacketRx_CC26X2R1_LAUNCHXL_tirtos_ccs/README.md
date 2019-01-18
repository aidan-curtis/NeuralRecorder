---
# rfpacketrx

---

Example Summary
---------------
The Packet RX example illustrates how to do simple packet RX using the
RF driver. This example is meant to be used with the Packet TX example or
SmartRF Studio. For every packet received, Board_PIN_LED2 is toggled.
The frequency and other RF settings can be modified using SmartRF Studio.

Peripherals Exercised
---------------------
* `Board_PIN_LED2` - Toggled when data is received over the RF interface


Resources & Jumper Settings
---------------------------
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Example Usage
-------------
Run the example. On another board, run the EasyLink TX example.
Board_PIN_LED2 is toggled when data with CRC OK is received.

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
4. Defines a data queue to handle received data
5. Sets the frequency using CMD_FS command
6. Sends the CMD_PROP_RX command to start receiving data
7. Once data with CRC OK is received we toggle the
   Board_PIN_LED2 and re-enter RX with the CMD_PROP_RX command

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.
