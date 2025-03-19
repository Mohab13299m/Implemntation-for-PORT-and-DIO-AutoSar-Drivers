 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohab Kamal
 ******************************************************************************/

#ifndef PORT_CFG_H_
#define PORT_CFG_H_



/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_OFF)

/* Pre-compile to enable / disable the use of the function Port_SetPinDirection() */
#define PORT_SET_PIN_DIRECTION_API            (STD_OFF)

/* Pre-compile to enable / disable the use of the function Port_SetPinMode() */
#define PORT_SET_PIN_MODE_API                 (STD_OFF)

/* Parameter to indicate if the direction is changeable on a port pin during runtime */
#define PORT_PIN_DIRECTION_CHANGEABLE         (STD_ON)

/* Parameter to indicate if the direction is mode on a port pin during runtime */
#define PORT_PIN_MODE_CHANGEABLE              (STD_ON)

/* Number of the configured Port Channels */
#define PORT_CONFIGURED_CHANNLES              (2U)

/* Channel Index in the array of structures in Port_PBcfg.c */
#define PortConf_LED1_CHANNEL_ID_INDEX              (uint8)0x00
#define PortConf_SW1_CHANNEL_ID_INDEX               (uint8)0x01

/* PORT Configured Port ID's  */
#define PortConf_LED1_PORT_NUM                      (Port_PortType)5 /* PORTF */
#define PortConf_SW1_PORT_NUM                       (Port_PortType)5 /* PORTF */

/* PORT Configured Channel ID's */
#define PortConf_LED1_CHANNEL_NUM                   (Port_PinType)1 /* Pin 1 in PORTF */
#define PortConf_SW1_CHANNEL_NUM                    (Port_PinType)4 /* Pin 4 in PORTF */

/* PORT Configured Channel Direction */
#define PortConf_LED1_CHANNEL_DIRECTION             (Port_PinDirectionType)PORT_PIN_OUT /* Output Direction for Pin 1 in PORTF */
#define PortConf_SW1_CHANNEL_DIRECTION              (Port_PinDirectionType)PORT_PIN_IN /* Input Direction for Pin 4 in PORTF */

/* PORT Configured Channel Mode */
#define PortConf_LED1_CHANNEL_MODE                  (Port_PinModeType)PORT_PIN_MODE_GPIO /* GPIO MODE for Pin 1 in PORTF */
#define PortConf_SW1_CHANNEL_MODE                   (Port_PinModeType)PORT_PIN_MODE_GPIO /* GPIO MODE for Pin 4 in PORTF */

/* PORT Configured INITIAL VALUE  */
#define PortConf_LED1_CHANNEL_INITIAL_VALUE               (uint8)STD_LOW /* Pin 1 initial value in PORTF */
#define PortConf_SW1_CHANNEL_INITIAL_VALUE                (uint8)STD_HIGH /* Pin 4 initial value in PORTF */

/* PORT Configured Internal Resistor  */
#define PortConf_LED1_CHANNEL_INTERNAL_RESISTOR           (Port_InternalResistor)OFF /* Disable internal resistor in Pin 1 in PORTF */
#define PortConf_SW1_CHANNEL_INTERNAL_RESISTOR            (Port_InternalResistor)PULL_UP /* Disable internal resistor in Pin 4 in PORTF */

/* PORT Configured Direction Changeability  */
#define PortConf_LED1_CHANNEL_Direction_Changeability     (uint8)PORT_PIN_DIRECTION_CHANGEABLE /* Direction Changeability in Pin 1 in PORTF */
#define PortConf_SW1_CHANNEL_Direction_Changeability      (uint8)PORT_PIN_DIRECTION_CHANGEABLE /* Direction Changeability in Pin 4 in PORTF */

/* PORT Configured Mode Changeability  */
#define PortConf_LED1_CHANNEL_Mode_Changeability     (uint8)PORT_PIN_MODE_CHANGEABLE /* Mode Changeability in Pin 1 in PORTF */
#define PortConf_SW1_CHANNEL_Mode_Changeability      (uint8)PORT_PIN_MODE_CHANGEABLE /* Mode Changeability in Pin 4 in PORTF */

#endif /* PORT_CFG_H_ */
