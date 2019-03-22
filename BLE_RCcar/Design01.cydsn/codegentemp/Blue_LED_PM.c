/*******************************************************************************
* File Name: Blue_LED.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Blue_LED.h"

static Blue_LED_BACKUP_STRUCT  Blue_LED_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Blue_LED_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Blue_LED_SUT.c usage_Blue_LED_Sleep_Wakeup
*******************************************************************************/
void Blue_LED_Sleep(void)
{
    #if defined(Blue_LED__PC)
        Blue_LED_backup.pcState = Blue_LED_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Blue_LED_backup.usbState = Blue_LED_CR1_REG;
            Blue_LED_USB_POWER_REG |= Blue_LED_USBIO_ENTER_SLEEP;
            Blue_LED_CR1_REG &= Blue_LED_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Blue_LED__SIO)
        Blue_LED_backup.sioState = Blue_LED_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Blue_LED_SIO_REG &= (uint32)(~Blue_LED_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Blue_LED_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Blue_LED_Sleep() for an example usage.
*******************************************************************************/
void Blue_LED_Wakeup(void)
{
    #if defined(Blue_LED__PC)
        Blue_LED_PC = Blue_LED_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Blue_LED_USB_POWER_REG &= Blue_LED_USBIO_EXIT_SLEEP_PH1;
            Blue_LED_CR1_REG = Blue_LED_backup.usbState;
            Blue_LED_USB_POWER_REG &= Blue_LED_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Blue_LED__SIO)
        Blue_LED_SIO_REG = Blue_LED_backup.sioState;
    #endif
}


/* [] END OF FILE */
