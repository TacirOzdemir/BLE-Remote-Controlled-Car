/*******************************************************************************
* File Name: Red_LED.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Red_LED_ALIASES_H) /* Pins Red_LED_ALIASES_H */
#define CY_PINS_Red_LED_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Red_LED_0			(Red_LED__0__PC)
#define Red_LED_0_PS		(Red_LED__0__PS)
#define Red_LED_0_PC		(Red_LED__0__PC)
#define Red_LED_0_DR		(Red_LED__0__DR)
#define Red_LED_0_SHIFT	(Red_LED__0__SHIFT)
#define Red_LED_0_INTR	((uint16)((uint16)0x0003u << (Red_LED__0__SHIFT*2u)))

#define Red_LED_INTR_ALL	 ((uint16)(Red_LED_0_INTR))


#endif /* End Pins Red_LED_ALIASES_H */


/* [] END OF FILE */
