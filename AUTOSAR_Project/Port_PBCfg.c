 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_PBcfg.c
 *
 * Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohab Kamal
 ******************************************************************************/

#include "Port.h"

/*
 * Module Version 1.0.0
 */
#define PORT_PBCFG_SW_MAJOR_VERSION              (1U)
#define PORT_PBCFG_SW_MINOR_VERSION              (0U)
#define PORT_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between Dio_PBcfg.c and Dio.h files */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between Dio_PBcfg.c and Dio.h files */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected version"
#endif

/* PB structure used with Dio_Init API */
const Port_ConfigType Port_Configuration =
{

 PortConf_LED1_PORT_NUM, PortConf_LED1_CHANNEL_NUM, PortConf_LED1_CHANNEL_DIRECTION, PortConf_LED1_CHANNEL_MODE, PortConf_LED1_CHANNEL_INITIAL_VALUE, PortConf_LED1_CHANNEL_INTERNAL_RESISTOR, PortConf_LED1_CHANNEL_Direction_Changeability, PortConf_LED1_CHANNEL_Mode_Changeability,   /* Switch 1 */

 PortConf_SW1_PORT_NUM, PortConf_SW1_CHANNEL_NUM, PortConf_SW1_CHANNEL_DIRECTION, PortConf_SW1_CHANNEL_MODE, PortConf_SW1_CHANNEL_INITIAL_VALUE, PortConf_SW1_CHANNEL_INTERNAL_RESISTOR, PortConf_SW1_CHANNEL_Direction_Changeability, PortConf_SW1_CHANNEL_Mode_Changeability  /* RED LED */
};
