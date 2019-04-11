

//*********************************************************************************
// These settings have been generated for use with TI-RTOS and cc26xxware
//
// Generated by SmartRF Studio version 2.3.0
// Tested with TI-RTOS version tirtos_simplelink_2_16_xx_xx
//
//*********************************************************************************


//*********************************************************************************
// Parameter summary
// Address: aa-bb
// Frequency: 2445.00000 MHz
// Data Format: Serial mode disable
// Deviation: 50.000 kHz
// Packet Length Config: Variable
// Max Packet Length: 128
// Packet Length: 30
// RX Filter BW: 220 kHz
// Symbol Rate: 50.00000 kBaud
// Sync Word Length: 32 Bits
// TX Power: 5 dBm
// Whitening: No whitening

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_prop_cmd.h)

#include <ti/drivers/rf/RF.h>
#include "smartrf_settings.h"

// TI-RTOS RF Mode Object
RF_Mode RF_prop =
{
    .rfMode = RF_MODE_PROPRIETARY_2_4,
//    .rfMode = RF_MODE_BLE,
    .cpePatchFxn = 0, //&rf_patch_cpe_genfsk,
    .mcePatchFxn = 0, //&rf_patch_mce_genfsk,
    .rfePatchFxn = 0, //&rf_patch_rfe_genfsk,
};


uint32_t shape[] = {0x00000000, 0x00000000, 0x00000000, 0x360c0100, 0xacaba076, 0xacacacac};


// Overrides for CMD_PROP_RADIO_SETUP
uint32_t pOverrides[] = {
 // override_agama_48_mhz_crystal.xml
 // Calculate the frequencies for synth based on 48MHz crystal (default is 24MHz).
 (uint32_t)0x00400403,
 // override_turn_off_dcdc_option.xml
 // Set DCDCCTL5[3:0]=0x0, Turn off optimization of DCDC regulator in TX
 (uint32_t)0x000088D3,
 // Set DCDCCTL5[3:0]=0x0, Turn off optimization of DCDC regulator in RX
 (uint32_t)0x000088C3,
 (uint32_t)0xFFFFFFFF,
};

// CMD_PROP_RADIO_SETUP
rfc_CMD_PROP_RADIO_SETUP_t RF_cmdPropRadioDivSetup =
{
    .commandNo = 0x3806,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .modulation.modType = 0x1,
    .modulation.deviation = 200,
    .symbolRate.preScale = 15,
    .symbolRate.rateWord = 131072,
    .rxBw = 7,
    .preamConf.nPreamBytes = 0x8,
    .preamConf.preamMode = 0x0,
    .formatConf.nSwBits = 16,
    .formatConf.bBitReversal = 0x0,
    .formatConf.bMsbFirst = 0x1,
    .formatConf.fecMode = 0x0,
    .formatConf.whitenMode = 0x0,
    .config.frontEndMode = 0x0,
    .config.biasMode = 0x0,
    .config.analogCfgMode = 0x0,
    .config.bNoFsPowerUp = 0x0,
    .txPower = 0x9324,
    .pRegOverride = pOverrides,
};

// CMD_FS
rfc_CMD_FS_t RF_cmdFs =
{
    .commandNo = 0x0803,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .frequency = 2440,
    .fractFreq = 0x0000,
    .synthConf.bTxMode = 1,
    .synthConf.refFreq = 0x0,
};

// CMD_PROP_TX
rfc_CMD_PROP_TX_t RF_cmdPropTx =
{
    .commandNo = 0x3801,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bVarLen = 0x1,
    .pktLen = 0x7d, // SET APPLICATION PAYLOAD LENGTH CHANGED!!
    .syncWord = 0xD391D391,
    .pPkt = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
};

// CMD_PROP_RX
rfc_CMD_PROP_RX_t RF_cmdPropRx =
{
    .commandNo = 0x3802,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bRepeatOk = 0x0,
    .pktConf.bRepeatNok = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bVarLen = 0x1,
    .pktConf.bChkAddress = 0x0,
    .pktConf.endType = 0x0,
    .pktConf.filterOp = 0x0,
    .rxConf.bAutoFlushIgnored = 0x0,
    .rxConf.bAutoFlushCrcErr = 0x0,
    .rxConf.bIncludeHdr = 0x1,
    .rxConf.bIncludeCrc = 0x0,
    .rxConf.bAppendRssi = 0x0,
    .rxConf.bAppendTimestamp = 0x0,
    .rxConf.bAppendStatus = 0x1,
    .syncWord = 0xD391D391,
    .maxPktLen = 0x7d, // MAKE SURE DATA ENTRY IS LARGE ENOUGH
//    .pktConf.addressMode = 0, // CHANGE!
    .address0 = 0xaa,
    .address1 = 0xbb,
    .endTrigger.triggerType = 0x1,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000,
    .pQueue = 0, // INSERT APPLICABLE POINTER: (dataQueue_t*)&xxx
    .pOutput = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
};

// CMD_TX_TEST
rfc_CMD_TX_TEST_t RF_cmdTxTest =
{
        .commandNo = 0x0808,
        .status = 0x0000,
        .pNextOp = 0, /*INSERT APPLICABLE POINTER: (uint8_t*)&xxx */
        .startTime = 0x00000000,
        .startTrigger.triggerType = 0x0,
        .startTrigger.bEnaCmd = 0x0,
        .startTrigger.triggerNo = 0x0,
        .startTrigger.pastTrig = 0x0,
        .condition.rule = 0x1,
        .condition.nSkip = 0x0,
        .config.bUseCw = 0x0,
        .config.bFsOff = 0x0,
        .config.whitenMode = 0x2,
        .__dummy0 = 0x00,
        .txWord = 0xabcd,
        .__dummy1 = 0x00,
        .endTrigger.triggerType = 0x1,
        .endTrigger.bEnaCmd = 0x0,
        .endTrigger.triggerNo = 0x0,
        .endTrigger.pastTrig = 0x0,
        .syncWord = 0x930b51de,
        .endTime = 0x00000000,
};