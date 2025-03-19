 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mohab Kamal
 ******************************************************************************/

#include "Port.h"
#include "Det.h"
#include "tm4c123gh6pm_registers.h"

/*******************************************************************************
 *                          STATIC Global Definitions                          *
 *******************************************************************************/

STATIC const Port_ConfigChannel * Port_PortChannels = NULL_PTR;
STATIC volatile uint8 Port_Status = PORT_NOT_INITIALIZED;
/********************************************************************************
* Function Name: void Port_Init (const Port_ConfigType*ConfigPtr)
* Inputs       : const Port_ConfigType*ConfigPtr
* Outputs      : void
* Reentrancy   : Non-Reentrant
* Synchronous  : Synchronous
* Description  : Initializes Port Module
********************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
                        PORT_E_PARAM_CONFIG);
    }
    else
#endif
    {
        Port_Status = PORT_INITIALIZED;
        Port_PortChannels = ConfigPtr -> Channels;
        uint8 counter = 0;
          for (counter = 0; counter < PORT_CONFIGURED_CHANNLES; counter++)
          {
            if((Port_PortChannels[counter].port_number == 2) && (Port_PortChannels[counter].channel_number <= 3))/* PC0 to PC3 */
            {
              /* Do Nothing ...  this is the JTAG pins */
            }
            else
            {
              volatile uint32 *Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */
              switch(Port_PortChannels[counter].port_number)
              {
              case  0: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                       break;
              case  1: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                       break;
              case  2: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                       break;
              case  3: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                       break;
              case  4: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                       break;
              case  5: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                       break;
              }
              if(((Port_PortChannels[counter].port_number == 3) && (Port_PortChannels[counter].channel_number == 7)) || ((Port_PortChannels[counter].port_number == 5) && (Port_PortChannels[counter].channel_number == 0))) /* PD7 or PF0 */
              {
                  *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
              }
              else if((Port_PortChannels[counter].port_number == 2) && (Port_PortChannels[counter].channel_number <= 3))      /* PC0 to PC3 */
              {
                /* Do Nothing ...  this is the JTAG pins */
              }
              else
              {
                /* Do Nothing ... No need to unlock the commit register for this pin */
              }

              /* INITIALIZING MODE (AMSEL - AFSEL - PMC - DEN) */
              if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_GPIO)
              {

                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);         /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)); /* Clear the PMCx bits for this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              else if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_ADC)
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);    /* Set the corresponding bit in the GPIOAMSEL register to Enable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);         /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)); /* Clear the PMCx bits for this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);   /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
              }

              else if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_UART)
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);           /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)) | (Port_PortChannels[counter].Mode));   /* Set the PMCx bits for this pin depending on the mode */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              else if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_SSI)
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);           /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)) | (Port_PortChannels[counter].Mode));   /* Set the PMCx bits for this pin depending on the mode */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              else if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_I2C)
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);           /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)) | (Port_PortChannels[counter].Mode));   /* Set the PMCx bits for this pin depending on the mode */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              else if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_M0PWM)
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);           /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)) | (Port_PortChannels[counter].Mode));   /* Set the PMCx bits for this pin depending on the mode */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              else if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_M1PWM)
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);           /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)) | (Port_PortChannels[counter].Mode));   /* Set the PMCx bits for this pin depending on the mode */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              else if(Port_PortChannels[counter].Mode == PORT_PIN_MODE_CAN)
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)) | (Port_PortChannels[counter].Mode));     /* Set the PMCx bits for this pin depending on the mode */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              else
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[counter].channel_number);  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[counter].channel_number);         /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */

                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[counter].channel_number * 4)); /* Clear the PMCx bits for this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
              }

              /* SETTING PIN DIRECTION (DIR - DATAR) */
              if(Port_PortChannels[counter].direction == PORT_PIN_OUT)
              {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[counter].channel_number);               /* Set the corresponding bit in the GPIODIR register to configure it as output pin */

                  if(Port_PortChannels[counter].InitialValue == STD_HIGH)
                  {
                      SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[counter].channel_number);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                  }
                  else
                  {
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[counter].channel_number);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                  }
              }
              else if(Port_PortChannels[counter].direction == PORT_PIN_IN)
              {
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[counter].channel_number);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */

                  if(Port_PortChannels[counter].resistor == PULL_UP)
                  {
                      SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[counter].channel_number);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                  }
                  else if(Port_PortChannels[counter].resistor == PULL_DOWN)
                  {
                      SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                  }
                  else
                  {
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[counter].channel_number);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[counter].channel_number);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                  }
              }
              else
              {
                  /* Do Nothing */
              }
            }
          }
    }
}

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction)
{
    volatile uint32 * Port_Ptr = NULL_PTR;    /* point to the required Port Registers base address */
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (PORT_PIN_DIRECTION_CHANGEABLE == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID,
                        PORT_E_DIRECTION_UNCHANGEABLE);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
    if (PORT_CONFIGURED_CHANNLES <= Pin)
    {

        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_SET_PIN_DIR_SID, PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
#endif
    if (FALSE == error)
    {
        if((Port_PortChannels[Pin].port_number == 2) && (Port_PortChannels[Pin].channel_number <= 3))/* PC0 to PC3 */
        {
          /* Do Nothing ...  this is the JTAG pins */
        }
        else
        {
            switch(Port_PortChannels[Pin].port_number)
            {
            case  0: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                     break;
            case  1: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                     break;
            case  2: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                     break;
            case  3: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                     break;
            case  4: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                     break;
            case  5: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                     break;
            }
            if(Direction == PORT_PIN_OUT)
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            }

            else
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].channel_number);    /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            }
        }
    }
}
#endif
void Port_RefreshPortDirection(void)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the Driver is initialized before using this function */
    if (PORT_NOT_INITIALIZED == Port_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_REFRESH_PORT_DIR_SID, PORT_E_UNINIT);
    }
    else
#endif
    {
        uint8 counter;
        for(counter = 0; counter < PORT_CONFIGURED_CHANNLES; counter++)
          {
            if((Port_PortChannels[counter].port_number == 2) && (Port_PortChannels[counter].channel_number <= 3))/* PC0 to PC3 */
            {
              /* Do Nothing ...  this is the JTAG pins */
            }
            else
            {
              if( (Port_PortChannels[counter].DirectionChangeability) == PORT_PIN_DIRECTION_CHANGEABLE)
              {
                /* Do Nothing  */
              }

              else
              {
                volatile uint32 * Port_Ptr = NULL_PTR;      /* point to the required Port Registers base address */
                volatile uint32 delay = 0;

                switch(Port_PortChannels[counter].port_number)
                {
                case  0: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                         break;
                case  1: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                         break;
                case  2: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                         break;
                case  3: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                         break;
                case  4: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                         break;
                case  5: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                         break;
                }
                if((Port_PortChannels[counter].direction) == PORT_PIN_OUT)
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_PortChannels[counter].channel_number);       /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                }

                else
                {
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_PortChannels[counter].channel_number);     /* Clear the corresponding bit in the GPIODIR register to configure it as Input pin */
                }
              }
            }
          }
    }
}
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode)
{
    volatile uint32 * Port_Ptr = NULL_PTR;    /* point to the required Port Registers base address */
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (PORT_PIN_DIRECTION_CHANGEABLE == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID,
                        PORT_E_DIRECTION_UNCHANGEABLE);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
    if (PORT_CONFIGURED_CHANNLES <= Pin)
    {

        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_SET_PIN_DIR_SID, PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
    if ((Mode != PORT_PIN_MODE_GPIO) || (Mode != PORT_PIN_MODE_ADC) || (Mode != PORT_PIN_MODE_UART)|| (Mode != PORT_PIN_MODE_SSI)|| (Mode != PORT_PIN_MODE_I2C)|| (Mode != PORT_PIN_MODE_M0PWM)|| (Mode != PORT_PIN_MODE_M1PWM)|| (Mode != PORT_PIN_MODE_CAN))
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_SET_PIN_DIR_SID, PORT_E_PARAM_INVALID_MODE);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
#endif
    if (FALSE == error)
    {
        if((Port_PortChannels[Pin].port_number == 2) && (Port_PortChannels[Pin].channel_number <= 3))/* PC0 to PC3 */
        {
          /* Do Nothing ...  this is the JTAG pins */
        }
        else
        {
            switch(Port_PortChannels[Pin].port_number)
            {
            case  0: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                     break;
            case  1: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                     break;
            case  2: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                     break;
            case  3: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                     break;
            case  4: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                     break;
            case  5: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                     break;
            }
            if(Mode == PORT_PIN_MODE_GPIO)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);               /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

            else if(Mode == PORT_PIN_MODE_ADC)
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);          /* Set the corresponding bit in the GPIOAMSEL register to Enable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);               /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);         /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            }

            else if(Mode == PORT_PIN_MODE_UART)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);                 /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);      /* Set the PMCx bits for this pin depending on the mode */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

            else if(Mode == PORT_PIN_MODE_SSI)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);                 /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);      /* Set the PMCx bits for this pin depending on the mode */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

            else if(Mode == PORT_PIN_MODE_I2C)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);                 /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);      /* Set the PMCx bits for this pin depending on the mode */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

            else if(Mode == PORT_PIN_MODE_M0PWM)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);                 /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);      /* Set the PMCx bits for this pin depending on the mode */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

            else if(Mode == PORT_PIN_MODE_M1PWM)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);                 /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);      /* Set the PMCx bits for this pin depending on the mode */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

            else if(Mode == PORT_PIN_MODE_CAN)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);                 /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);      /* Set the PMCx bits for this pin depending on the mode */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

            else
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);               /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */

              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));       /* Clear the PMCx bits for this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);           /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
        }
    }
}
#endif
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the Driver is initialized before using this function */
    if (PORT_NOT_INITIALIZED == Port_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
    /* Check if input pointer is not Null pointer */
    if(NULL_PTR == versioninfo)
    {
        /* Report to DET  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }

#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
    if(FALSE == error)
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
    }
}
#endif
