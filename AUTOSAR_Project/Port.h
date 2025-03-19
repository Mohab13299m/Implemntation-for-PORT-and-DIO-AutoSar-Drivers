 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR
 * for example Mohamed Tarek's ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* PORT Module Id */
#define PORT_MODULE_ID    (124U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

#include "Common_Macros.h"
#include "Std_Types.h"
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* PORT Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Dio_Cfg.h and Dio.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PORT_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Dio_Cfg.h and Dio.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Dio_Cfg.h does not match the expected version"
#endif
/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Set Channel Direction*/
#define PORT_SET_PIN_DIR_SID         (uint8)0x01

/* Service ID for Refresh PORT Direction */
#define PORT_REFRESH_PORT_DIR_SID    (uint8)0x02

/* Service ID for PORT GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID    (uint8)0x03

/* Service ID for Set Channel Mode */
#define PORT_SET_PIN_MODE_SID        (uint8)0x04

/* Service ID for PORT Init Channel */
#define PORT_INIT_SID                (uint8)0x00

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port Pin ID */
#define PORT_E_PARAM_PIN                          (uint8)0x0A

/* Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE             (uint8)0x0B

/* Port_Init API service called with NULL pointer parameter */
#define PORT_E_PARAM_CONFIG                       (uint8)0x0C

/* DET code to report Invalid Mode */
#define PORT_E_PARAM_INVALID_MODE                 (uint8)0x0D

/* DET code to report Invalid Mode */
#define PORT_E_MODE_UNCHANGEABLE                  (uint8)0x0E


/*
 * The API service shall return immediately without any further action,
 * beside reporting this development error.
 */
#define PORT_E_PARAM_POINTER                      (uint8)0x10

/*
 * API service used without module initialization is reported using following
 * error code (Not exist in AUTOSAR 4.0.3 DIO SWS Document.
 */
#define PORT_E_UNINIT                             (uint8)0x0F
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

  /* Type definition for Port_PortType used by the PORT APIs (Doesn't exist in AUTOSAR 4.0.3 PORT SWS Document.*/
typedef uint8  Port_PortType;

  /* Type definition for Port_PinType used by the PORT APIs */
typedef uint8  Port_PinType;
/* Type definition for Port_PinModeType used by the PORT APIs */
typedef enum{
    PORT_PIN_MODE_GPIO,
    PORT_PIN_MODE_ADC,
    PORT_PIN_MODE_UART,
    PORT_PIN_MODE_SSI,
    PORT_PIN_MODE_I2C,
    PORT_PIN_MODE_M0PWM,
    PORT_PIN_MODE_M1PWM,
    PORT_PIN_MODE_CAN
}Port_PinModeType;

/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN,PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/* Description: Structure to configure each individual PIN:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *  3. the direction of pin --> INPUT or OUTPUT
 *  4. the internal resistor --> Disable, Pull up or Pull down
 *  5. the mode of the pin --> either it is a GPIO or not
 *  6. the intial value --> High or Low
 */
typedef struct
{
    /* Member contains the ID of the Port that this channel belongs to */
  Port_PortType port_number;

    /* Member contains the ID of the Channel*/
  Port_PinType channel_number;

    /* Member contains the direction of the Channel */
  Port_PinDirectionType direction;

    /* Member contains the Mode of the Channel */
  Port_PinModeType Mode;

    /* Member contains the Initial Value of the Channel */
  uint8 InitialValue;

    /* Member indicates whether the internal resistor is enabled as pull up or pull down or off */
  Port_InternalResistor resistor;
  /* Member indicates whether the Channel's Direction is changeable or not */
  uint8 DirectionChangeability;

  /* Member indicates whether the Channel's Mode is changeable or not */
  uint8 ModeChangeability;
}Port_ConfigChannel;

/* Data Structure required for initializing the Port Driver */
typedef struct Port_ConfigType
{
Port_ConfigChannel Channels[PORT_CONFIGURED_CHANNLES];
}Port_ConfigType;


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_SetupGpioPin
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the pin configuration:
*              - Setup the pin as Digital GPIO pin
*              - Setup the direction of the GPIO pin
*              - Setup the internal resistor for i/p pin
************************************************************************************/
//void Port_SetupGpioPin(const Port_ConfigType *ConfigPtr );

void Port_Init(const Port_ConfigType* ConfigPtr);

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction);
#endif

void Port_RefreshPortDirection(void);

#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode);
#endif

#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif
/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

  /* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;
#endif /* PORT_H */
