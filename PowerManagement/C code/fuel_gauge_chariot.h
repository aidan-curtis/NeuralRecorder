#ifndef FUEL_GAUGE_H_
#define FUEL_GAUGE_H_

/*
 * Performs the required config updates to the fuel gauge over the I2C
 * interface.
 *
 * NOT INTENDED FOR USE IN END-APPLICATION CODE.
 */
void fuel_gauge_config_update();

/*
 * Reads the current voltage of the battery using the fuel gauge I2C interface.
 *
 * Returns:
 *     A two-byte value representing the voltage in mV.
 */
uint16_t fuel_gauge_read_voltage();

/*
 * Reads the current flags of the fuel gauge using the I2C interface.
 *
 * Returns:
 *     A two-byte bit-field representing the flags.
 */
uint16_t fuel_gauge_read_flags();

/*
 * Reads the current control status register of the fuel gauge using the I2C
 * interface.
 *
 * Returns:
 *     A two-byte bit-field representing the current control status.
 */
uint16_t fuel_gauge_read_control_status();

/*
 * Reads the current chemistry ID of the fuel gauge using the I2C interface.
 *
 * NOT INTENDED FOR USE IN END-APPLICATION CODE, since this unseals the device.
 *
 * Returns:
 *     A two-byte value representing the chemistry ID.
 */
uint16_t fuel_gauge_read_chem_id();

/*
 * Instructs the fuel gauge to perform a full device reset.
 */
void fuel_gauge_full_reset();

#endif /* FUEL_GAUGE_H_ */
