/**
*     Copyright (c) 2023, Nations Technologies Inc.
* 
*     All rights reserved.
*
*     This software is the exclusive property of Nations Technologies Inc. (Hereinafter 
* referred to as NATIONS). This software, and the product of NATIONS described herein 
* (Hereinafter referred to as the Product) are owned by NATIONS under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NATIONS does not grant any license under its patents, copyrights, trademarks, or other 
* intellectual property rights. Names and brands of third party may be mentioned or referred 
* thereto (if any) for identification purposes only.
*
*     NATIONS reserves the right to make changes, corrections, enhancements, modifications, and 
* improvements to this software at any time without notice. Please contact NATIONS and obtain 
* the latest version of this software before placing orders.

*     Although NATIONS has attempted to provide accurate and reliable information, NATIONS assumes 
* no responsibility for the accuracy and reliability of this software.
* 
*     It is the responsibility of the user of this software to properly design, program, and test 
* the functionality and safety of any application made of this information and any resulting product. 
* In no event shall NATIONS be liable for any direct, indirect, incidental, special,exemplary, or 
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NATIONS Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property 
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NATIONS and hold NATIONS 
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not 
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NATIONS, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NATIONS products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
* User shall comply with any applicable export control laws and regulations promulgated and administered by 
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/

/**
*\*\file n32h47x_48x_wwdg.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2023, Nations Technologies Inc. All rights reserved.
**/
#ifndef __N32H47X_WWDG_H__
#define __N32H47X_WWDG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32h47x_48x.h"

#define WWDG_PRESCALER_DIV1             ((uint32_t)0x00000000)
#define WWDG_PRESCALER_DIV2             ((uint32_t)WWDG_CFG_TIMERB0)
#define WWDG_PRESCALER_DIV4             ((uint32_t)WWDG_CFG_TIMERB1)
#define WWDG_PRESCALER_DIV8             ((uint32_t)(WWDG_CFG_TIMERB1 | WWDG_CFG_TIMERB0))


/** EWINT bit **/
#define EARLY_WAKEUP_INT                (WWDG_CFG_EWINT)
#define EARLY_WAKEUP_FLAG               (WWDG_STS_EWINTF)

/** CTRL register bit mask **/
#define CTRL_ACTB_SET                   ((uint32_t)WWDG_CTRL_ACTB)

/* CFG register bit mask **/
#define CFG_TIMERB_MASK                 ((uint32_t)0xFFFF3FFFU)
#define CFG_W_MASK                      ((uint32_t)0xFFFFC000U)
#define BIT_MASK                        ((uint16_t)0x3FFF)


void WWDG_DeInit(void);
void WWDG_SetPrescalerDiv(uint32_t WWDG_Prescaler);
void WWDG_SetWValue(uint16_t WindowValue);
void WWDG_EnableInt(void);
void WWDG_SetCnt(uint16_t Counter);
void WWDG_Enable(uint16_t Counter);
FlagStatus WWDG_GetEWINTF(void);
void WWDG_ClrEWINTF(void);

#ifdef __cplusplus
}
#endif

#endif /* __N32H47X__WWDG_H */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
