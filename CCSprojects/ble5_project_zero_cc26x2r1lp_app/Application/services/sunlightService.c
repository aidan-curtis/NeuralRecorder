/**********************************************************************************************
 * Filename:       sunlightService.c
 *
 * Description:    This file contains the implementation of the service.
 *
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include <string.h>

/* This Header file contains all BLE API and icall structure definition */
#include "icall_ble_api.h"

#include <icall.h>

#include "sunlightService.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
* GLOBAL VARIABLES
*/

// sunlightService Service UUID
CONST uint8_t sunlightServiceUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(SUNLIGHTSERVICE_SERV_UUID)
};

// sunlightValue UUID
CONST uint8_t sunlightService_SunlightValueUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(SUNLIGHTSERVICE_SUNLIGHTVALUE_UUID), HI_UINT16(SUNLIGHTSERVICE_SUNLIGHTVALUE_UUID)
//   SUNLIGHTSERVICE_SUNLIGHTVALUE_UUID_BASE128(SUNLIGHTSERVICE_SUNLIGHTVALUE_UUID)

};

/*********************************************************************
 * LOCAL VARIABLES
 */

static sunlightServiceCBs_t *pAppCBs = NULL;

/*********************************************************************
* Profile Attributes - variables
*/

// Service declaration
static CONST gattAttrType_t sunlightServiceDecl = { ATT_UUID_SIZE, sunlightServiceUUID };

// Characteristic "SunlightValue" Properties (for declaration)
static uint8_t sunlightService_SunlightValueProps = GATT_PROP_READ | GATT_PROP_NOTIFY;

// Characteristic "SunlightValue" Value variable
static uint8_t sunlightService_SunlightValueVal[SUNLIGHTSERVICE_SUNLIGHTVALUE_LEN] = {0};

// Characteristic "SunlightValue" CCCD
static gattCharCfg_t *sunlightService_SunlightValueConfig;

/*********************************************************************
* Profile Attributes - Table
*/

static gattAttribute_t sunlightServiceAttrTbl[] =
{
  // sunlightService Service Declaration
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, // type
    GATT_PERMIT_READ,                         // permission
    0,                                        // handle, assigned internally
    (uint8_t *)&sunlightServiceDecl           // attribute value
  },
    // SunlightValue Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &sunlightService_SunlightValueProps  // bool
    },
      // SunlightValue Characteristic Value
      {
        { ATT_BT_UUID_SIZE, sunlightService_SunlightValueUUID },
        GATT_PERMIT_READ,
        0,
        sunlightService_SunlightValueVal
      },
      // SunlightValue CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&sunlightService_SunlightValueConfig
      },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t sunlightService_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                           uint8 *pValue, uint16 *pLen, uint16 offset,
                                           uint16 maxLen, uint8 method );
static bStatus_t sunlightService_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                            uint8 *pValue, uint16 len, uint16 offset,
                                            uint8 method );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t sunlightServiceCBs =
{
  sunlightService_ReadAttrCB,  // Read callback function pointer
  sunlightService_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*
 * SunlightService_AddService- Initializes the SunlightService service by registering
 *          GATT attributes with the GATT server.
 *
 */
bStatus_t SunlightService_AddService( void )
{
  uint8_t status;

  // Allocate Client Characteristic Configuration table
  sunlightService_SunlightValueConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * MAX_NUM_BLE_CONNS );
  if ( sunlightService_SunlightValueConfig == NULL )
  {
    return ( bleMemAllocError );
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( LINKDB_CONNHANDLE_INVALID, sunlightService_SunlightValueConfig );
  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( sunlightServiceAttrTbl,
                                        GATT_NUM_ATTRS( sunlightServiceAttrTbl ),
                                        GATT_MAX_ENCRYPT_KEY_SIZE,
                                        &sunlightServiceCBs );

  return ( status );
}

/*
 * SunlightService_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
bStatus_t SunlightService_RegisterAppCBs( sunlightServiceCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    pAppCBs = appCallbacks;

    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}

/*
 * SunlightService_SetParameter - Set a SunlightService parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t SunlightService_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case SUNLIGHTSERVICE_SUNLIGHTVALUE:
      if ( len == SUNLIGHTSERVICE_SUNLIGHTVALUE_LEN )
      {
        // accept value and update attr's value field
        memcpy(sunlightService_SunlightValueVal, value, len);

        // Try to send notification.
        GATTServApp_ProcessCharCfg( sunlightService_SunlightValueConfig, (uint8_t *)&sunlightService_SunlightValueVal, FALSE,
                                    sunlightServiceAttrTbl, GATT_NUM_ATTRS( sunlightServiceAttrTbl ),
                                    INVALID_TASK_ID,  sunlightService_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  return ret;
}


/*
 * SunlightService_GetParameter - Get a SunlightService parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t SunlightService_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  return ret;
}


/*********************************************************************
 * @fn          sunlightService_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t sunlightService_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                       uint8 *pValue, uint16 *pLen, uint16 offset,
                                       uint16 maxLen, uint8 method )
{
  bStatus_t status = SUCCESS;

  // See if request is regarding the SunlightValue Characteristic Value
if ( ! memcmp(pAttr->type.uuid, sunlightService_SunlightValueUUID, pAttr->type.len) )
  {
    if ( offset > SUNLIGHTSERVICE_SUNLIGHTVALUE_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, SUNLIGHTSERVICE_SUNLIGHTVALUE_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  else
  {
    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has READ permissions.
    *pLen = 0;
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  return status;
}


/*********************************************************************
 * @fn      sunlightService_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t sunlightService_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                        uint8 *pValue, uint16 len, uint16 offset,
                                        uint8 method )
{
  bStatus_t status  = SUCCESS;
  uint8_t   paramID = 0xFF;

  // See if request is regarding a Client Characterisic Configuration
  if ( ! memcmp(pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len) )
  {
    // Allow only notifications.
    status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                             offset, GATT_CLIENT_CFG_NOTIFY);
  }
  else
  {
    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has WRITE permissions.
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  // Let the application know something changed (if it did) by using the
  // callback it registered earlier (if it did).
  if (paramID != 0xFF)
    if ( pAppCBs && pAppCBs->pfnChangeCb )
      pAppCBs->pfnChangeCb( paramID ); // Call app function from stack task context.

  return status;
}
