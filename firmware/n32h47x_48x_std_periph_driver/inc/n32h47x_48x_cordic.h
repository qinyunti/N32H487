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
*\*\file n32h47x_48x_cordic.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2023, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32H47X_CORDIC_H__
#define __N32H47X_CORDIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32h47x_48x.h"
#include "n32h47x_48x_rcc.h"


/*** CORDIC Structure Definition Start ***/

/** CORDIC initialize configuration structure definition **/
typedef struct
{
    uint32_t Function;   /* Select function */
    uint32_t Precision;  /* Set the number of iterations */
    uint32_t Scale;      /* Select Scaling Factor */
    uint32_t NbWrite;    /* Select the number of CORDIC_WDAT register to write */
    uint32_t NbRead;     /* Select the number of CORDIC_RDAT register to read */
    uint32_t InSize;     /* Select input data width */
    uint32_t OutSize;    /* Select output data width */
    uint32_t InSelect;   /* Select floating-point input or fixed-point input */
    uint32_t OutSelect;  /* Select floating-point output or fixed-point output */
    uint32_t CodinLimit; /* Enables or disables Coordinate Limit */
    uint32_t PhaseLimit; /* Enables or disables Phase Limit */
} CORDIC_InitType;

/*** CORDIC Structure Definition End ***/


/*** CORDIC Macro Definition Start ***/

/** CORDIC Coordinate Limit definition **/
#define CORDIC_CODIN_LIMIT_ENABLE  ((uint32_t)CORDIC_CTRLSTS_CODINLIMIT)
#define CORDIC_CODIN_LIMIT_DISABLE ((uint32_t)0x00000000U)

/** CORDIC Phase Limit definition **/
#define CORDIC_PHASE_LIMIT_ENABLE  ((uint32_t)CORDIC_CTRLSTS_PHASELIMIT)
#define CORDIC_PHASE_LIMIT_DISABLE ((uint32_t)0x00000000U)

/** CORDIC Output Select definition **/
#define CORDIC_OUTPUT_FLOAT ((uint32_t)CORDIC_CTRLSTS_FLOATOUT)
#define CORDIC_OUTPUT_FIX   ((uint32_t)0x00000000U)

/** CORDIC Input Select definition **/
#define CORDIC_INPUT_FLOAT ((uint32_t)CORDIC_CTRLSTS_FLOATIN)
#define CORDIC_INPUT_FIX   ((uint32_t)0x00000000U)

/** CORDIC Output width definition **/
#define CORDIC_OUTSIZE_16BITS ((uint32_t)CORDIC_CTRLSTS_OUTSIZE) /* 16 bits output data size (Q1.15 format) */
#define CORDIC_OUTSIZE_32BITS ((uint32_t)0x00000000U)            /* 32 bits output data size (Q1.31 format) */

/** CORDIC Input width definition **/
#define CORDIC_INSIZE_16BITS ((uint32_t)CORDIC_CTRLSTS_INSIZE) /* 16 bits input data size (Q1.15 format) */
#define CORDIC_INSIZE_32BITS ((uint32_t)0x00000000U)           /* 32 bits input data size (Q1.31 format) */

/** CORDIC Read register Number definition **/
#define CORDIC_NBREAD_2 ((uint32_t)CORDIC_CTRLSTS_NUMREAD) /* Two 32-bit Data containing two 32-bits data \
                                                              output (Q1.31 format) */
#define CORDIC_NBREAD_1 ((uint32_t)0x00000000U)            /* One 32-bits read containing either only one \
                                                              32-bit data output (Q1.31 format), or two   \
                                                              16-bit data output (Q1.15 format) packed    \
                                                              in one 32 bits Data */

/** CORDIC Write register Number definition **/
#define CORDIC_NBWRITE_2 ((uint32_t)CORDIC_CTRLSTS_NUMWRITE) /* Two 32-bit write containing two 32-bits      \
                                                                data input (Q1.31 format) */
#define CORDIC_NBWRITE_1 ((uint32_t)0x00000000U)             /* One 32-bits write containing either only one \
                                                                32-bit data input (Q1.31 format), or two     \
                                                                16-bit data input (Q1.15 format) packed      \
                                                                in one 32 bits Data */

/** CORDIC Scaling Factor definition **/
#define CORDIC_SCALE_0 ((uint32_t)0x00000000U)
#define CORDIC_SCALE_1 ((uint32_t)(CORDIC_CTRLSTS_SCALE_0))
#define CORDIC_SCALE_2 ((uint32_t)(CORDIC_CTRLSTS_SCALE_1))
#define CORDIC_SCALE_3 ((uint32_t)(CORDIC_CTRLSTS_SCALE_1 \
                                 | CORDIC_CTRLSTS_SCALE_0))
#define CORDIC_SCALE_4 ((uint32_t)(CORDIC_CTRLSTS_SCALE_2))
#define CORDIC_SCALE_5 ((uint32_t)(CORDIC_CTRLSTS_SCALE_2 \
                                 | CORDIC_CTRLSTS_SCALE_0))
#define CORDIC_SCALE_6 ((uint32_t)(CORDIC_CTRLSTS_SCALE_2 \
                                 | CORDIC_CTRLSTS_SCALE_1))
#define CORDIC_SCALE_7 ((uint32_t)(CORDIC_CTRLSTS_SCALE_2 \
                                 | CORDIC_CTRLSTS_SCALE_1 \
                                 | CORDIC_CTRLSTS_SCALE_0))

/** CORDIC Precision(Number of iterations) definition **/
#define CORDIC_PRECISION_1CYCLE   ((uint32_t)(CORDIC_CTRLSTS_PRECISION_0))
#define CORDIC_PRECISION_2CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_1))
#define CORDIC_PRECISION_3CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_1 \
                                            | CORDIC_CTRLSTS_PRECISION_0))
#define CORDIC_PRECISION_4CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_2))
#define CORDIC_PRECISION_5CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_2 \
                                            | CORDIC_CTRLSTS_PRECISION_0))
#define CORDIC_PRECISION_6CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_2 \
                                            | CORDIC_CTRLSTS_PRECISION_1))
#define CORDIC_PRECISION_7CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_2 \
                                            | CORDIC_CTRLSTS_PRECISION_1 \
                                            | CORDIC_CTRLSTS_PRECISION_0))
#define CORDIC_PRECISION_8CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3))
#define CORDIC_PRECISION_9CYCLES  ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3 \
                                            | CORDIC_CTRLSTS_PRECISION_0))
#define CORDIC_PRECISION_10CYCLES ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3 \
                                            | CORDIC_CTRLSTS_PRECISION_1))
#define CORDIC_PRECISION_11CYCLES ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3 \
                                            | CORDIC_CTRLSTS_PRECISION_1 \
                                            | CORDIC_CTRLSTS_PRECISION_0))
#define CORDIC_PRECISION_12CYCLES ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3 \
                                            | CORDIC_CTRLSTS_PRECISION_2))
#define CORDIC_PRECISION_13CYCLES ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3 \
                                            | CORDIC_CTRLSTS_PRECISION_2 \
                                            | CORDIC_CTRLSTS_PRECISION_0))
#define CORDIC_PRECISION_14CYCLES ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3 \
                                            | CORDIC_CTRLSTS_PRECISION_2 \
                                            | CORDIC_CTRLSTS_PRECISION_1))
#define CORDIC_PRECISION_15CYCLES ((uint32_t)(CORDIC_CTRLSTS_PRECISION_3 \
                                            | CORDIC_CTRLSTS_PRECISION_2 \
                                            | CORDIC_CTRLSTS_PRECISION_1 \
                                            | CORDIC_CTRLSTS_PRECISION_0))

/** CORDIC Function definition **/
#define CORDIC_FUNCTION_COSINE      ((uint32_t)0x00000000U)             /* Cosine */
#define CORDIC_FUNCTION_SINE        ((uint32_t)(CORDIC_CTRLSTS_FUNC_0)) /* Sine */
#define CORDIC_FUNCTION_PHASE       ((uint32_t)(CORDIC_CTRLSTS_FUNC_1)) /* Phase */
#define CORDIC_FUNCTION_MODULUS     ((uint32_t)(CORDIC_CTRLSTS_FUNC_1 \
                                              | CORDIC_CTRLSTS_FUNC_0)) /* Modulus */
#define CORDIC_FUNCTION_ARCTANGENT  ((uint32_t)(CORDIC_CTRLSTS_FUNC_2)) /* Arctangent */
#define CORDIC_FUNCTION_HCOSINE     ((uint32_t)(CORDIC_CTRLSTS_FUNC_2 \
                                              | CORDIC_CTRLSTS_FUNC_0)) /* Hyperbolic Cosine */
#define CORDIC_FUNCTION_HSINE       ((uint32_t)(CORDIC_CTRLSTS_FUNC_2 \
                                              | CORDIC_CTRLSTS_FUNC_1)) /* Hyperbolic Sine */
#define CORDIC_FUNCTION_HARCTANGENT ((uint32_t)(CORDIC_CTRLSTS_FUNC_2 \
                                              | CORDIC_CTRLSTS_FUNC_1 \
                                              | CORDIC_CTRLSTS_FUNC_0)) /* Hyperbolic Arctangent */
#define CORDIC_FUNCTION_NATURALLOG  ((uint32_t)(CORDIC_CTRLSTS_FUNC_3)) /* Natural Logarithm */
#define CORDIC_FUNCTION_SQUAREROOT  ((uint32_t)(CORDIC_CTRLSTS_FUNC_3 \
                                              | CORDIC_CTRLSTS_FUNC_0)) /* Square Root */

/** CORDIC Interrupt definition **/
#define CORDIC_INT            ((uint32_t)CORDIC_CTRLSTS_INTEN)
#define CORDIC_INT_INOVERFLOW ((uint32_t)CORDIC_CTRLSTS_INOVINTEN)

/** CORDIC on DMA transfer direction definition **/
#define CORDIC_AS_DMA_DSTADDR ((uint32_t)0x00000001U) /* CORDIC register address as Destination address for DMA */
#define CORDIC_AS_DMA_SRCADDR ((uint32_t)0x00000000U) /* CORDIC register address as Source address for DMA */

/** CORDIC flag definition **/
#define CORDIC_FLAG_RRF     ((uint32_t)CORDIC_CTRLSTS_RRF)    /* Result ready flag */
#define CORDIC_FLAG_INOVF   ((uint32_t)CORDIC_CTRLSTS_INOVF)  /* Input parameter overflow flag */
#define CORDIC_FLAG_DMAWENF ((uint32_t)CORDIC_CTRLSTS_DMAWEN) /* DMA Write Request Enable Flag */
#define CORDIC_FLAG_DMARENF ((uint32_t)CORDIC_CTRLSTS_DMAREN) /* DMA Read Request Enable Flag */
#define CORDIC_FLAG_INTENF  ((uint32_t)CORDIC_CTRLSTS_INTEN)  /* CORDIC Interrupt Enable Flag */


/*** CORDIC Macro Definition End ***/



/*** CORDIC Driving Functions Declaration ***/
void CORDIC_DeInit(void);
void CORDIC_Init(CORDIC_InitType* CORDIC_InitParam);
void CORDIC_StructInit(CORDIC_InitType* CORDIC_InitParam);
void CORDIC_InterruptCmd(uint32_t Interrupt, FunctionalStatus Cmd);
void CORDIC_DMAReadRequestCmd(FunctionalStatus Cmd);
void CORDIC_DMAWriteRequestCmd(FunctionalStatus Cmd);
uint32_t CORDIC_GetRegisterAddr(uint32_t Direction);
void CORDIC_WriteData(uint32_t *pInData);
void CORDIC_ReadData(uint32_t *pOutData);
FlagStatus CORDIC_GetFlagStatus(uint32_t Flag);
void CORDIC_ClearStatusFlag(uint32_t Flag);

/*** CORDIC Driving Functions Declaration End ***/



#ifdef __cplusplus
}
#endif

#endif /*__N32H47X_CORDIC_H__ */
