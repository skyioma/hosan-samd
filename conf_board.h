/**
 * \file
 *
 * \brief SAM D21 Custom board configuration.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#define DEBUG_USART_MODULE             SERCOM1
#define DEBUG_USART_SERCOM_MUX_SETTING USART_RX_1_TX_0_XCK_1
#define DEBUG_USART_SERCOM_PINMUX_PAD0 PINMUX_PA00D_SERCOM1_PAD0
#define DEBUG_USART_SERCOM_PINMUX_PAD1 PINMUX_PA01D_SERCOM1_PAD1
#define DEBUG_USART_SERCOM_PINMUX_PAD2 PINMUX_UNUSED
#define DEBUG_USART_SERCOM_PINMUX_PAD3 PINMUX_UNUSED

// BTN1 @ PA02
#define BUTTON_1_PIN                   PIN_PA02
#define BUTTON_1_ACTIVE                1
#define BUTTON_1_EIC_PIN               PIN_PA02A_EIC_EXTINT2
#define BUTTON_1_EIC_MUX               MUX_PA02A_EIC_EXTINT2
#define BUTTON_1_EIC_PINMUX            PINMUX_PA02A_EIC_EXTINT2
#define BUTTON_1_EIC_LINE              2

// VBAT_ADC @ PA03 AIN[1]
#define VBAT_ADC_INPUT                 ADC_POSITIVE_INPUT_PIN1

// BTN2 @ PA07
#define BUTTON_2_PIN                   PIN_PA07
#define BUTTON_2_ACTIVE                1
#define BUTTON_2_EIC_PIN               PIN_PA07A_EIC_EXTINT7
#define BUTTON_2_EIC_MUX               MUX_PA07A_EIC_EXTINT7
#define BUTTON_2_EIC_PINMUX            PINMUX_PA07A_EIC_EXTINT7
#define BUTTON_2_EIC_LINE              7

// MISO @ PA08
// MOSI @ PA10
// SCK @ PA11
#define SPI_MODULE                     SERCOM0
#define SPI_MUX_SETTING                SPI_SIGNAL_MUX_SETTING_E
#define SPI_PINMUX_PAD0                PINMUX_PA08C_SERCOM0_PAD0
#define SPI_PINMUX_PAD1                PINMUX_UNUSED
#define SPI_PINMUX_PAD2                PINMUX_PA10C_SERCOM0_PAD2
#define SPI_PINMUX_PAD3                PINMUX_PA11C_SERCOM0_PAD3

// NRF_IRQ @ PA09
// NRF_CSN @ PA14
#define NRF_CS_PIN                     PIN_PA14

// NRF_CE @ PA15
#define NRF_CE_PIN                     PIN_PA15
#define NRF_CE_ACTIVE                  1

// EN_33VA_N @ PA19: GPIO out
#define EN_33VA_PIN                    PIN_PA19
#define EN_33VA_ACTIVE                 0

// E_CS @ PA24: GPIO out
#define E_CS_PIN                       PIN_PA24
#define E_CS_ACTIVE                    0

// E_DC @ PA25: GPIO out
#define E_DC_PIN                       PIN_PA25

// E_RST @ PA27: GPIO out
#define E_RST_PIN                      PIN_PA27
#define E_RST_ACTIVE                   0

// E_BUSY @ PA28: GPIO in
#define E_BUSY_PIN                     PIN_PA28

// SDA @ PA16
// SCL @ PA17
#define I2C_MODULE                     SERCOM3
#define I2C_SERCOM_PINMUX_PAD0         PINMUX_PA16D_SERCOM3_PAD0
#define I2C_SERCOM_PINMUX_PAD1         PINMUX_PA17D_SERCOM3_PAD1
#define I2C_SERCOM_PINMUX_PAD2         PINMUX_UNUSED
#define I2C_SERCOM_PINMUX_PAD3         PINMUX_UNUSED
#define I2C_SERCOM_DMAC_ID_TX          SERCOM3_DMAC_ID_TX
#define I2C_SERCOM_DMAC_ID_RX          SERCOM3_DMAC_ID_RX

#endif /* CONF_BOARD_H_INCLUDED */
