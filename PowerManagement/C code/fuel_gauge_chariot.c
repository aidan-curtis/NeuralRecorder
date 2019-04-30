#include <msp430.h>
#include <stdint.h>
#include "p7_i2c.h"

#define I2C_ADDR 0x55

/*
 * Define function call addresses:
 */
#define FUNC_CONTROL          0x00
#define FUNC_VOLTAGE          0x04
#define FUNC_FLAGS            0x06
#define FUNC_BLOCK_DATA_CHKSM 0x60
#define FUNC_BLOCK_DATA_CTL   0x61
#define FUNC_DATA_CLASS       0x3E
#define FUNC_DATA_OFFSET      0x3F

static uint8_t CMD_UNSEAL[] = {FUNC_CONTROL, 0x00, 0x80};
static uint8_t CMD_CFGUPD[] = {FUNC_CONTROL, 0x13, 0x00};
static uint8_t CMD_FLAGS[] = {FUNC_FLAGS};
static uint8_t CMD_EN_BLOCK_DATA_CTL[] = {FUNC_BLOCK_DATA_CTL, 0x00};
static uint8_t CMD_DATA_CLASS[] = {FUNC_DATA_CLASS, 0x52};
static uint8_t CMD_DATA_OFFSET[] = {FUNC_DATA_OFFSET, 0x00};
static uint8_t CMD_DATA_CHKSM[] = {FUNC_BLOCK_DATA_CHKSM};
static uint8_t CMD_READ_OLD_CAP_HIGH[] = {0x4A};
static uint8_t CMD_READ_OLD_CAP_LOW[] = {0x4B};
static uint8_t CMD_WRITE_CAP_HIGH[] = {0x4A, 0x00};
static uint8_t CMD_WRITE_CAP_LOW[] = {0x4B, 110};
static uint8_t CMD_WRITE_CHKSM[] = {FUNC_BLOCK_DATA_CHKSM, 0x00};
static uint8_t CMD_SOFTRESET[] = {FUNC_CONTROL, 0x42, 0x00};
static uint8_t CMD_SEAL[] = {FUNC_CONTROL, 0x20, 0x00};
static uint8_t CMD_CHEM_B[] = {FUNC_CONTROL, 0x31, 0x00};
static uint8_t CMD_LOAD_CHEM_ID[] = {FUNC_CONTROL, 0x08, 0x00};
static uint8_t CMD_READ_CONTROL[] = {FUNC_CONTROL};

void fuel_gauge_config_update() {
    uint8_t success;
    uint8_t *res;
    uint8_t old_chksm, old_cap_high, old_cap_low, temp;

    // Unseal the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_UNSEAL, 3, 0);
    success = p7_i2c_write(I2C_ADDR, CMD_UNSEAL, 3, 0);

    // Load the current CHEM ID into the CONTROL register:
    success = p7_i2c_write(I2C_ADDR, CMD_LOAD_CHEM_ID, 3, 0);

    // Read the current CHEM ID:
    success = p7_i2c_write(I2C_ADDR, CMD_READ_CONTROL, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    // Set the fuel gauge to config update mode:
    success = p7_i2c_write(I2C_ADDR, CMD_CFGUPD, 3, 0);

    // Confirm that the fuel gauge is in config update mode:
    __delay_cycles(16000000);
    success = p7_i2c_write(I2C_ADDR, CMD_FLAGS, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);
    while (!((*res) & BIT4)) {
        __delay_cycles(16000000);
        success = p7_i2c_write(I2C_ADDR, CMD_FLAGS, 1, 1);
        res = p7_i2c_read(I2C_ADDR, 2);
    }

    // Enable block data control for fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_EN_BLOCK_DATA_CTL, 2, 0);

    // Set the data class to be accessed:
    success = p7_i2c_write(I2C_ADDR, CMD_DATA_CLASS, 2, 0);

    // Set the data offset to be accessed:
    success = p7_i2c_write(I2C_ADDR, CMD_DATA_OFFSET, 2, 0);

    // Read the current checksum:
    success = p7_i2c_write(I2C_ADDR, CMD_DATA_CHKSM, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 1);
    old_chksm = *res;

    // Read the current capacity:
    success = p7_i2c_write(I2C_ADDR, CMD_READ_OLD_CAP_HIGH, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);
    old_cap_high = *res;
    old_cap_low = *(res + 1);

    // Write the new capacity:
    success = p7_i2c_write(I2C_ADDR, CMD_WRITE_CAP_HIGH, 2, 0);
    success = p7_i2c_write(I2C_ADDR, CMD_WRITE_CAP_LOW, 2, 0);

    // Compute the new checksum:
    temp = (255 - old_chksm - old_cap_high - old_cap_low) & 0xFF;
    temp = (temp + CMD_WRITE_CAP_HIGH[1] + CMD_WRITE_CAP_LOW[1]) & 0xFF;
    CMD_WRITE_CHKSM[1] = 255 - temp;

    // Write the new checksum to the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_WRITE_CHKSM, 2, 0);

    // Update the chemistry profile to CHEM_B:
    success = p7_i2c_write(I2C_ADDR, CMD_CHEM_B, 3, 0);

    // Exit CFGUPDATE using a soft reset:
    success = p7_i2c_write(I2C_ADDR, CMD_SOFTRESET, 3, 0);

    // Confirm that the fuel gauge is not in config update mode:
    __delay_cycles(16000000);
    success = p7_i2c_write(I2C_ADDR, CMD_FLAGS, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);
    while ((*res) & BIT4) {
        __delay_cycles(16000000);
        success = p7_i2c_write(I2C_ADDR, CMD_FLAGS, 1, 1);
        res = p7_i2c_read(I2C_ADDR, 2);
    }

    // Load the current CHEM ID into the CONTROL register:
    success = p7_i2c_write(I2C_ADDR, CMD_LOAD_CHEM_ID, 3, 0);

    // Read the current CHEM ID:
    success = p7_i2c_write(I2C_ADDR, CMD_READ_CONTROL, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    // Re-seal the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_SEAL, 3, 0);
}

static uint8_t CMD_CONTROL_STATUS[] = {FUNC_CONTROL, 0x00, 0x00};

uint16_t fuel_gauge_read_control_status() {
    uint8_t success;
    uint8_t *res;

    // Load the current control status into the CONTROL register:
    success = p7_i2c_write(I2C_ADDR, CMD_CONTROL_STATUS, 3, 0);

    // Read the current CHEM ID:
    success = p7_i2c_write(I2C_ADDR, CMD_READ_CONTROL, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    // Unseal the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_UNSEAL, 3, 0);
    success = p7_i2c_write(I2C_ADDR, CMD_UNSEAL, 3, 0);

    // Load the current control status into the CONTROL register:
    success = p7_i2c_write(I2C_ADDR, CMD_CONTROL_STATUS, 3, 0);

    // Read the current CHEM ID:
    success = p7_i2c_write(I2C_ADDR, CMD_READ_CONTROL, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    // Re-seal the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_SEAL, 3, 0);

    // Load the current control status into the CONTROL register:
    success = p7_i2c_write(I2C_ADDR, CMD_CONTROL_STATUS, 3, 0);

    // Read the current CHEM ID:
    success = p7_i2c_write(I2C_ADDR, CMD_READ_CONTROL, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    return (*res << 8) + *(res + 1);
}

uint16_t fuel_gauge_read_chem_id() {
    uint8_t success;
    uint8_t *res;

    // Unseal the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_UNSEAL, 3, 0);
    success = p7_i2c_write(I2C_ADDR, CMD_UNSEAL, 3, 0);

    // Load the current CHEM ID into the CONTROL register:
    success = p7_i2c_write(I2C_ADDR, CMD_LOAD_CHEM_ID, 3, 0);

    // Read the current CHEM ID:
    success = p7_i2c_write(I2C_ADDR, CMD_READ_CONTROL, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    // Re-seal the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_SEAL, 3, 0);

    return (*res << 8) + *(res + 1);
}

static uint8_t CMD_READ_VOLTAGE[] = {FUNC_VOLTAGE};

uint16_t fuel_gauge_read_voltage() {
    uint8_t success;
    uint8_t *res;

    success = p7_i2c_write(I2C_ADDR, CMD_READ_VOLTAGE, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    return (*(res + 1) << 8) + *res;
}

uint16_t fuel_gauge_read_flags() {
    uint8_t success;
    uint8_t *res;

    success = p7_i2c_write(I2C_ADDR, CMD_FLAGS, 1, 1);
    res = p7_i2c_read(I2C_ADDR, 2);

    return (*(res + 1) << 8) + *res;
}

static uint8_t CMD_FULLRESET[] = {FUNC_CONTROL, 0x41, 0x00};

void fuel_gauge_full_reset() {
    uint8_t success;

    // Re-seal the fuel gauge:
    success = p7_i2c_write(I2C_ADDR, CMD_FULLRESET, 3, 0);
}

