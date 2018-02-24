/*! @file TX_RADIO_config.h
 * @brief This file contains the automatically generated
 * configurations.
 *
 * @n WDS GUI Version: 3.2.10.0
 * @n Device: Si4463 Rev.: C2                                 
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2015 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_TX_CONFIG_H_
#define RADIO_TX_CONFIG_H_

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
// Crys_freq(Hz): 26000000    Crys_tol(ppm): 2    IF_mode: 2    High_perf_Ch_Fil: 1    OSRtune: 0    Ch_Fil_Bw_AFC: 0    ANT_DIV: 0    PM_pattern: 15    
// MOD_type: 3    Rsymb(sps): 20000    Fdev(Hz): 5000    RXBW(Hz): 150000    Manchester: 0    AFC_en: 0    Rsymb_error: 0.0    Chip-Version: 2    
// TX_RF Freq.(MHz): 436.5    API_TC: 29    fhst: 20000    inputBW: 0    BERT: 0    RAW_dout: 0    D_source: 1    Hi_pfm_div: 1    
// API_ARR_Det_en: 0    Fdev_error: 0    API_ETSI: 0    
// 
// # RX IF frequency is  -406250 Hz
// # WB filter 5 (BW =  32.15 kHz);  NB-filter 5 (BW = 32.15 kHz)
// 
// Modulation index: 0.5
*/


// CONFIGURATION PARAMETERS
#define TX_RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ                     26000000L
#define TX_RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER                    0x00
#define TX_RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH               0x00
#define TX_RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP        0x03
#define TX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       0xF000



// CONFIGURATION COMMANDS

/*
// Command:                  TX_RF_POWER_UP
// Description:              Command to power-up the device and select the operational mode and functionality.
*/
#define TX_RF_POWER_UP 0x02, 0x81, 0x01, 0x01, 0x8C, 0xBA, 0x80

/*
// Command:                  TX_RF_GPIO_PIN_CFG
// Description:              Configures the GPIO pins.
*/
#define TX_RF_GPIO_PIN_CFG 0x13, 0x13, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00

/*
// Set properties:           TX_RF_GLOBAL_XO_TUNE_2
// Number of properties:     2
// Group ID:                 0x00
// Start ID:                 0x00
// Default values:           0x40, 0x00, 
// Descriptions:
//   GLOBAL_XO_TUNE - Configure the internal capacitor frequency tuning bank for the crystal oscillator.
//   GLOBAL_CLK_CFG - Clock configuration options.
*/
#define TX_RF_GLOBAL_XO_TUNE_2 0x11, 0x00, 0x02, 0x00, 0x00, 0x00

/*
// Set properties:           TX_RF_GLOBAL_CONFIG_1
// Number of properties:     1
// Group ID:                 0x00
// Start ID:                 0x03
// Default values:           0x20, 
// Descriptions:
//   GLOBAL_CONFIG - Global configuration settings.
*/
#define TX_RF_GLOBAL_CONFIG_1 0x11, 0x00, 0x01, 0x03, 0x20

/*
// Set properties:           TX_RF_INT_CTL_ENABLE_1
// Number of properties:     1
// Group ID:                 0x01
// Start ID:                 0x00
// Default values:           0x04, 
// Descriptions:
//   INT_CTL_ENABLE - This property provides for global enabling of the three interrupt groups (Chip, Modem and Packet Handler) in order to generate HW interrupts at the NIRQ pin.
*/
#define TX_RF_INT_CTL_ENABLE_1 0x11, 0x01, 0x01, 0x00, 0x00

/*
// Set properties:           TX_RF_FRR_CTL_A_MODE_4
// Number of properties:     4
// Group ID:                 0x02
// Start ID:                 0x00
// Default values:           0x01, 0x02, 0x09, 0x00, 
// Descriptions:
//   FRR_CTL_A_MODE - Fast Response Register A Configuration.
//   FRR_CTL_B_MODE - Fast Response Register B Configuration.
//   FRR_CTL_C_MODE - Fast Response Register C Configuration.
//   FRR_CTL_D_MODE - Fast Response Register D Configuration.
*/
#define TX_RF_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00

/*
// Set properties:           TX_RF_PREAMBLE_CONFIG_1
// Number of properties:     1
// Group ID:                 0x10
// Start ID:                 0x04
// Default values:           0x21, 
// Descriptions:
//   PREAMBLE_CONFIG - General configuration bits for the Preamble field.
*/
#define TX_RF_PREAMBLE_CONFIG_1 0x11, 0x10, 0x01, 0x04, 0x21

/*
// Set properties:           TX_RF_MODEM_MOD_TYPE_12
// Number of properties:     12
// Group ID:                 0x20
// Start ID:                 0x00
// Default values:           0x02, 0x80, 0x07, 0x0F, 0x42, 0x40, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x06, 
// Descriptions:
//   MODEM_MOD_TYPE - Selects the type of modulation. In TX mode, additionally selects the source of the modulation.
//   MODEM_MAP_CONTROL - Controls polarity and mapping of transmit and receive bits.
//   MODEM_DSM_CTRL - Miscellaneous control bits for the Delta-Sigma Modulator (DSM) in the PLL Synthesizer.
//   MODEM_DATA_RATE_2 - Unsigned 24-bit value used to determine the TX data rate
//   MODEM_DATA_RATE_1 - Unsigned 24-bit value used to determine the TX data rate
//   MODEM_DATA_RATE_0 - Unsigned 24-bit value used to determine the TX data rate
//   MODEM_TX_NCO_MODE_3 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_TX_NCO_MODE_2 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_TX_NCO_MODE_1 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_TX_NCO_MODE_0 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_FREQ_DEV_2 - 17-bit unsigned TX frequency deviation word.
//   MODEM_FREQ_DEV_1 - 17-bit unsigned TX frequency deviation word.
*/
#define TX_RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x0B, 0x00, 0x07, 0x0C, 0x35, 0x00, 0x05, 0x8C, 0xBA, 0x80, 0x00, 0x01

/*
// Set properties:           TX_RF_MODEM_FREQ_DEV_0_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x0C
// Default values:           0xD3, 
// Descriptions:
//   MODEM_FREQ_DEV_0 - 17-bit unsigned TX frequency deviation word.
*/
#define TX_RF_MODEM_FREQ_DEV_0_1 0x11, 0x20, 0x01, 0x0C, 0x93

/*
// Set properties:           TX_RF_MODEM_TX_RAMP_DELAY_12
// Number of properties:     12
// Group ID:                 0x20
// Start ID:                 0x18
// Default values:           0x01, 0x00, 0x08, 0x03, 0xC0, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00, 0x4B, 
// Descriptions:
//   MODEM_TX_RAMP_DELAY - TX ramp-down delay setting.
//   MODEM_MDM_CTRL - MDM control.
//   MODEM_IF_CONTROL - Selects Fixed-IF, Scaled-IF, or Zero-IF mode of RX Modem operation.
//   MODEM_IF_FREQ_2 - the IF frequency setting (an 18-bit signed number).
//   MODEM_IF_FREQ_1 - the IF frequency setting (an 18-bit signed number).
//   MODEM_IF_FREQ_0 - the IF frequency setting (an 18-bit signed number).
//   MODEM_DECIMATION_CFG1 - Specifies three decimator ratios for the Cascaded Integrator Comb (CIC) filter.
//   MODEM_DECIMATION_CFG0 - Specifies miscellaneous parameters and decimator ratios for the Cascaded Integrator Comb (CIC) filter.
//   MODEM_DECIMATION_CFG2 - Specifies miscellaneous decimator filter selections.
//   MODEM_IFPKD_THRESHOLDS - 
//   MODEM_BCR_OSR_1 - RX BCR/Slicer oversampling rate (12-bit unsigned number).
//   MODEM_BCR_OSR_0 - RX BCR/Slicer oversampling rate (12-bit unsigned number).
*/
#define TX_RF_MODEM_TX_RAMP_DELAY_12 0x11, 0x20, 0x0C, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20, 0x00, 0xE8, 0x00, 0x51

/*
// Set properties:           TX_RF_MODEM_BCR_NCO_OFFSET_2_12
// Number of properties:     12
// Group ID:                 0x20
// Start ID:                 0x24
// Default values:           0x06, 0xD3, 0xA0, 0x06, 0xD3, 0x02, 0xC0, 0x00, 0x00, 0x23, 0x83, 0x69, 
// Descriptions:
//   MODEM_BCR_NCO_OFFSET_2 - RX BCR NCO offset value (an unsigned 22-bit number).
//   MODEM_BCR_NCO_OFFSET_1 - RX BCR NCO offset value (an unsigned 22-bit number).
//   MODEM_BCR_NCO_OFFSET_0 - RX BCR NCO offset value (an unsigned 22-bit number).
//   MODEM_BCR_GAIN_1 - The unsigned 11-bit RX BCR loop gain value.
//   MODEM_BCR_GAIN_0 - The unsigned 11-bit RX BCR loop gain value.
//   MODEM_BCR_GEAR - RX BCR loop gear control.
//   MODEM_BCR_MISC1 - Miscellaneous control bits for the RX BCR loop.
//   MODEM_BCR_MISC0 - Miscellaneous RX BCR loop controls.
//   MODEM_AFC_GEAR - RX AFC loop gear control.
//   MODEM_AFC_WAIT - RX AFC loop wait time control.
//   MODEM_AFC_GAIN_1 - Sets the gain of the PLL-based AFC acquisition loop, and provides miscellaneous control bits for AFC functionality.
//   MODEM_AFC_GAIN_0 - Sets the gain of the PLL-based AFC acquisition loop, and provides miscellaneous control bits for AFC functionality.
*/
#define TX_RF_MODEM_BCR_NCO_OFFSET_2_12 0x11, 0x20, 0x0C, 0x24, 0x06, 0x4D, 0x32, 0x03, 0x29, 0x00, 0xC2, 0x00, 0x54, 0x23, 0x80, 0xAA

/*
// Set properties:           TX_RF_MODEM_AFC_LIMITER_1_3
// Number of properties:     3
// Group ID:                 0x20
// Start ID:                 0x30
// Default values:           0x00, 0x40, 0xA0, 
// Descriptions:
//   MODEM_AFC_LIMITER_1 - Set the AFC limiter value.
//   MODEM_AFC_LIMITER_0 - Set the AFC limiter value.
//   MODEM_AFC_MISC - Specifies miscellaneous AFC control bits.
*/
#define TX_RF_MODEM_AFC_LIMITER_1_3 0x11, 0x20, 0x03, 0x30, 0x01, 0xAC, 0x80

/*
// Set properties:           TX_RF_MODEM_AGC_CONTROL_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x35
// Default values:           0xE0, 
// Descriptions:
//   MODEM_AGC_CONTROL - Miscellaneous control bits for the Automatic Gain Control (AGC) function in the RX Chain.
*/
#define TX_RF_MODEM_AGC_CONTROL_1 0x11, 0x20, 0x01, 0x35, 0xE0

/*
// Set properties:           TX_RF_MODEM_AGC_WINDOW_SIZE_12
// Number of properties:     12
// Group ID:                 0x20
// Start ID:                 0x38
// Default values:           0x11, 0x10, 0x10, 0x0B, 0x1C, 0x40, 0x00, 0x00, 0x2B, 0x0C, 0xA4, 0x03, 
// Descriptions:
//   MODEM_AGC_WINDOW_SIZE - Specifies the size of the measurement and settling windows for the AGC algorithm.
//   MODEM_AGC_RFPD_DECAY - Sets the decay time of the TX_RF peak detectors.
//   MODEM_AGC_IFPD_DECAY - Sets the decay time of the IF peak detectors.
//   MODEM_FSK4_GAIN1 - Specifies the gain factor of the secondary branch in 4(G)FSK ISI-suppression.
//   MODEM_FSK4_GAIN0 - Specifies the gain factor of the primary branch in 4(G)FSK ISI-suppression.
//   MODEM_FSK4_TH1 - 16 bit 4(G)FSK slicer threshold.
//   MODEM_FSK4_TH0 - 16 bit 4(G)FSK slicer threshold.
//   MODEM_FSK4_MAP - 4(G)FSK symbol mapping code.
//   MODEM_OOK_PDTC - Configures the attack and decay times of the OOK Peak Detector.
//   MODEM_OOK_BLOPK - Configures the slicing reference level of the OOK Peak Detector.
//   MODEM_OOK_CNT1 - OOK control.
//   MODEM_OOK_MISC - Selects the detector(s) used for demodulation of an OOK signal, or for demodulation of a (G)FSK signal when using the asynchronous demodulator.
*/
#define TX_RF_MODEM_AGC_WINDOW_SIZE_12 0x11, 0x20, 0x0C, 0x38, 0x11, 0x12, 0x12, 0x80, 0x1A, 0x20, 0x00, 0x00, 0x28, 0x0C, 0x84, 0x23

/*
// Set properties:           TX_RF_MODEM_RAW_CONTROL_5
// Number of properties:     5
// Group ID:                 0x20
// Start ID:                 0x45
// Default values:           0x02, 0x00, 0xA3, 0x02, 0x80, 
// Descriptions:
//   MODEM_RAW_CONTROL - Defines gain and enable controls for raw / nonstandard mode.
//   MODEM_RAW_EYE_1 - 11 bit eye-open detector threshold.
//   MODEM_RAW_EYE_0 - 11 bit eye-open detector threshold.
//   MODEM_ANT_DIV_MODE - Antenna diversity mode settings.
//   MODEM_ANT_DIV_CONTROL - Specifies controls for the Antenna Diversity algorithm.
*/
#define TX_RF_MODEM_RAW_CONTROL_5 0x11, 0x20, 0x05, 0x45, 0x8F, 0x00, 0x80, 0x01, 0x00

/*
// Set properties:           TX_RF_MODEM_RSSI_JUMP_THRESH_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x4B
// Default values:           0x0C, 
// Descriptions:
//   MODEM_RSSI_JUMP_THRESH - Configures the RSSI Jump Detection threshold.
*/
#define TX_RF_MODEM_RSSI_JUMP_THRESH_1 0x11, 0x20, 0x01, 0x4B, 0x06

/*
// Set properties:           TX_RF_MODEM_RSSI_CONTROL2_2
// Number of properties:     2
// Group ID:                 0x20
// Start ID:                 0x4D
// Default values:           0x00, 0x40, 
// Descriptions:
//   MODEM_RSSI_CONTROL2 - RSSI Jump Detection control.
//   MODEM_RSSI_COMP - RSSI compensation value.
*/
#define TX_RF_MODEM_RSSI_CONTROL2_2 0x11, 0x20, 0x02, 0x4D, 0x18, 0x40

/*
// Set properties:           TX_RF_MODEM_RAW_SEARCH2_2
// Number of properties:     2
// Group ID:                 0x20
// Start ID:                 0x50
// Default values:           0x00, 0x08, 
// Descriptions:
//   MODEM_RAW_SEARCH2 - Defines and controls the search period length for the Moving Average and Min-Max detectors.
//   MODEM_CLKGEN_BAND - Select PLL Synthesizer output divider ratio as a function of frequency band.
*/
#define TX_RF_MODEM_RAW_SEARCH2_2 0x11, 0x20, 0x02, 0x50, 0x94, 0x0A

/*
// Set properties:           TX_RF_MODEM_SPIKE_DET_2
// Number of properties:     2
// Group ID:                 0x20
// Start ID:                 0x54
// Default values:           0x00, 0x00, 
// Descriptions:
//   MODEM_SPIKE_DET - Configures the threshold for (G)FSK Spike Detection.
//   MODEM_ONE_SHOT_AFC - Configures parameters for th e One Shot AFC function and for BCR timing/acquisition.
*/
#define TX_RF_MODEM_SPIKE_DET_2 0x11, 0x20, 0x02, 0x54, 0x03, 0x07

/*
// Set properties:           TX_RF_MODEM_RSSI_MUTE_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x57
// Default values:           0x00, 
// Descriptions:
//   MODEM_RSSI_MUTE - Configures muting of the RSSI to avoid false RSSI interrupts.
*/
#define TX_RF_MODEM_RSSI_MUTE_1 0x11, 0x20, 0x01, 0x57, 0x00

/*
// Set properties:           TX_RF_MODEM_DSA_CTRL1_5
// Number of properties:     5
// Group ID:                 0x20
// Start ID:                 0x5B
// Default values:           0x00, 0x00, 0x00, 0x00, 0x00, 
// Descriptions:
//   MODEM_DSA_CTRL1 - Configures parameters for the Signal Arrival Detection circuit block and algorithm.
//   MODEM_DSA_CTRL2 - Configures parameters for the Signal Arrival Detection circuit block and algorithm.
//   MODEM_DSA_QUAL - Configures parameters for the Eye Opening qualification m ethod of the Signal Arrival Detection algorithm.
//   MODEM_DSA_RSSI - Signal Arrival Detect RSSI Qualifier Config
//   MODEM_DSA_MISC - Miscellaneous detection of signal arrival bits.
*/
#define TX_RF_MODEM_DSA_CTRL1_5 0x11, 0x20, 0x05, 0x5B, 0x40, 0x04, 0x04, 0x78, 0x20

/*
// Set properties:           TX_RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12
// Number of properties:     12
// Group ID:                 0x21
// Start ID:                 0x00
// Default values:           0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01, 
// Descriptions:
//   MODEM_CHFLT_RX1_CHFLT_COE13_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE12_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE11_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE10_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE9_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE8_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE7_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE6_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE5_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE4_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE3_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE2_7_0 - Filter coefficients for the first set of RX filter coefficients.
*/
#define TX_RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0x7E, 0x64, 0x1B, 0xBA, 0x58, 0x0B, 0xDD, 0xCE, 0xD6, 0xE6, 0xF6, 0x00

/*
// Set properties:           TX_RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12
// Number of properties:     12
// Group ID:                 0x21
// Start ID:                 0x0C
// Default values:           0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 
// Descriptions:
//   MODEM_CHFLT_RX1_CHFLT_COE1_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE0_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM1 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM2 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM3 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE13_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE12_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE11_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE10_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE9_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE8_7_0 - Filter coefficients for the second set of RX filter coefficients.
*/
#define TX_RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0x03, 0x03, 0x15, 0xF0, 0x3F, 0x00, 0x7E, 0x64, 0x1B, 0xBA, 0x58, 0x0B

/*
// Set properties:           TX_RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12
// Number of properties:     12
// Group ID:                 0x21
// Start ID:                 0x18
// Default values:           0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 
// Descriptions:
//   MODEM_CHFLT_RX2_CHFLT_COE7_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE6_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE5_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE4_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE3_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE2_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE1_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE0_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM1 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM2 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM3 - Filter coefficients for the second set of RX filter coefficients.
*/
#define TX_RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0xDD, 0xCE, 0xD6, 0xE6, 0xF6, 0x00, 0x03, 0x03, 0x15, 0xF0, 0x3F, 0x00

/*
// Set properties:           TX_RF_PA_MODE_4
// Number of properties:     4
// Group ID:                 0x22
// Start ID:                 0x00
// Default values:           0x08, 0x7F, 0x00, 0x5D, 
// Descriptions:
//   PA_MODE - Selects the PA operating mode, and selects resolution of PA power adjustment (i.e., step size).
//   PA_PWR_LVL - Configuration of PA output power level.
//   PA_BIAS_CLKDUTY - Configuration of the PA Bias and duty cycle of the TX clock source.
//   PA_TC - Configuration of PA ramping parameters.
*/
#define TX_RF_PA_MODE_4 0x11, 0x22, 0x04, 0x00, 0x08, 0x05, 0x00, 0x1D

/*
// Set properties:           TX_RF_SYNTH_PFDCP_CPFF_7
// Number of properties:     7
// Group ID:                 0x23
// Start ID:                 0x00
// Default values:           0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03, 
// Descriptions:
//   SYNTH_PFDCP_CPFF - Feed forward charge pump current selection.
//   SYNTH_PFDCP_CPINT - Integration charge pump current selection.
//   SYNTH_VCO_KV - Gain scaling factors (Kv) for the VCO tuning varactors on both the integrated-path and feed forward path.
//   SYNTH_LPFILT3 - Value of resistor R2 in feed-forward path of loop filter.
//   SYNTH_LPFILT2 - Value of capacitor C2 in feed-forward path of loop filter.
//   SYNTH_LPFILT1 - Value of capacitors C1 and C3 in feed-forward path of loop filter.
//   SYNTH_LPFILT0 - Bias current of the active amplifier in the feed-forward loop filter.
*/
#define TX_RF_SYNTH_PFDCP_CPFF_7 0x11, 0x23, 0x07, 0x00, 0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03

/*
// Set properties:           TX_RF_FREQ_CONTROL_INTE_8
// Number of properties:     8
// Group ID:                 0x40
// Start ID:                 0x00
// Default values:           0x3C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0xFF, 
// Descriptions:
//   FREQ_CONTROL_INTE - Frac-N PLL Synthesizer integer divide number.
//   FREQ_CONTROL_FRAC_2 - Frac-N PLL fraction number.
//   FREQ_CONTROL_FRAC_1 - Frac-N PLL fraction number.
//   FREQ_CONTROL_FRAC_0 - Frac-N PLL fraction number.
//   FREQ_CONTROL_CHANNEL_STEP_SIZE_1 - EZ Frequency Programming channel step size.
//   FREQ_CONTROL_CHANNEL_STEP_SIZE_0 - EZ Frequency Programming channel step size.
//   FREQ_CONTROL_W_SIZE - Set window gating period (in number of crystal reference clock cycles) for counting VCO frequency during calibration.
//   FREQ_CONTROL_VCOCNT_RX_ADJ - Adjust target count for VCO calibration in RX mode.
*/
#define TX_RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08, 0x00, 0x42, 0x09, 0x3B, 0x13, 0x06, 0x4D, 0x20, 0xFE


// AUTOMATICALLY GENERATED CODE! 
// DO NOT EDIT/MODIFY BELOW THIS LINE!
// --------------------------------------------

#ifndef FIRMWARE_LOAD_COMPILE
#define TX_RADIO_CONFIGURATION_DATA_ARRAY { \
        0x08,0x04,0x21,0x71,0x4B,0x00,0x00,0xBA,0x9E, \
0x08,0x05,0x48,0x23,0x2E,0x2B,0x90,0xB1,0x4E, \
0x08,0xEA,0x3F,0xB9,0xE8,0x8B,0xA9,0xCA,0xD6, \
0x08,0x05,0xD2,0xE5,0xBE,0xD1,0x27,0x55,0x82, \
0x08,0xE5,0x56,0x2A,0x3B,0x76,0x76,0x96,0x48, \
0x08,0x05,0x8E,0x26,0xD8,0x5D,0x01,0xA7,0x88, \
0x08,0xE2,0x89,0xCC,0x63,0x79,0x95,0x00,0x4B, \
0x08,0x05,0xE0,0x75,0xCD,0xA4,0xB9,0x46,0xBC, \
0x08,0xEA,0xD3,0x37,0xD2,0x9A,0x89,0x82,0xEA, \
0x08,0x05,0x0C,0xAE,0x4C,0xF5,0xF6,0x3C,0xB3, \
0x08,0xE9,0xA7,0x70,0xDF,0xF1,0x14,0x4F,0x04, \
0x08,0x05,0xFE,0x5B,0xDF,0x47,0x0A,0x7C,0x5B, \
0x08,0xE2,0xFB,0x3E,0x21,0xA2,0x1B,0xAA,0x93, \
0x08,0x05,0xBF,0xFD,0xAB,0x69,0x6C,0xA8,0x5A, \
0x08,0xE2,0x66,0xB7,0x2E,0x2C,0x45,0x2D,0xFB, \
0x08,0x05,0x0D,0x55,0xBD,0xC2,0x37,0x00,0x72, \
0x08,0xE2,0xFF,0x57,0x4D,0x7C,0x6C,0x00,0x2C, \
0x08,0x05,0x9E,0xF2,0x46,0xFD,0xD3,0x16,0x1B, \
0x08,0xEA,0x16,0x7F,0x67,0x4D,0xE5,0xE2,0xC8, \
0x08,0x05,0x37,0x33,0x1C,0xFA,0xBB,0xEE,0xEF, \
0x08,0xEA,0x00,0x5F,0xBE,0xA4,0xFC,0xBF,0xC1, \
0x08,0x05,0x95,0x12,0x2F,0x0A,0xCF,0x55,0x8C, \
0x08,0xE7,0x70,0xC2,0xD4,0xF0,0x81,0x95,0xC8, \
0x08,0xE7,0x72,0x00,0xF9,0x8D,0x15,0x01,0xA3, \
0x08,0xE7,0x18,0xE5,0x6C,0x51,0x1F,0x86,0x9F, \
0x08,0xE7,0xDD,0x37,0x59,0x4B,0xAD,0xB0,0x9C, \
0x08,0xE7,0xC8,0xE8,0x84,0xCD,0x55,0x41,0x83, \
0x08,0xEF,0x4F,0x8E,0x38,0xCB,0x37,0x02,0x87, \
0x08,0xE7,0xF5,0x00,0x88,0x4C,0x09,0x65,0xCE, \
0x08,0xEF,0xDD,0xBC,0x65,0x62,0xAC,0x75,0x62, \
0x08,0xE7,0xC0,0xF1,0x5D,0x98,0xB0,0xDD,0x43, \
0x08,0xE7,0x19,0xB4,0xF8,0x9F,0x6D,0x8C,0xCB, \
0x08,0xE1,0xDE,0x63,0xC2,0x32,0xC6,0xE4,0x2F, \
0x08,0x05,0xF4,0x33,0xB7,0x2E,0x72,0x9A,0xF9, \
0x08,0xE7,0x65,0xD9,0x38,0xB8,0xFE,0x31,0x16, \
0x08,0xE7,0xF3,0x06,0x2D,0xF5,0xFE,0x0C,0x38, \
0x08,0xE7,0x70,0x4F,0xE7,0x49,0xB4,0x58,0x39, \
0x08,0xEF,0xF1,0x46,0xA9,0x23,0x38,0x64,0xC0, \
0x08,0xE7,0x09,0x4E,0x04,0xD3,0x46,0x15,0x02, \
0x08,0xEF,0x8D,0xC7,0x20,0xC3,0x90,0x87,0x4D, \
0x08,0xEF,0x00,0xAB,0x7F,0x27,0x02,0xC6,0xA0, \
0x08,0xE7,0x23,0xA6,0xA6,0xA4,0x27,0x11,0x7D, \
0x08,0xEF,0xB3,0xF1,0x9E,0x6A,0xB3,0x19,0xAF, \
0x08,0xE7,0xAB,0xF5,0x15,0x78,0x5E,0x48,0xF8, \
0x08,0xEF,0x5B,0xB1,0x2E,0xAF,0x2A,0xFF,0x16, \
0x08,0xE7,0x30,0x62,0x5C,0x82,0x7A,0x3F,0x83, \
0x08,0xEF,0x91,0xA7,0xD3,0x1B,0x64,0x85,0xBE, \
0x08,0xE7,0x4D,0x81,0x94,0xE4,0xAA,0xE8,0xDB, \
0x08,0xEF,0xA0,0xCC,0x4A,0x23,0xA5,0x7E,0x36, \
0x08,0xEF,0x0C,0x72,0x4C,0xFB,0x26,0x5A,0xEC, \
0x08,0xEF,0x0E,0x42,0xFA,0xAF,0x49,0xA0,0xA8, \
0x08,0xE7,0x6D,0x12,0xDF,0x2B,0x0C,0x61,0x58, \
0x08,0xEA,0xB6,0x9B,0xDE,0x81,0xB9,0xFF,0xFF, \
0x08,0x05,0x04,0xEB,0xD8,0x12,0xD6,0x8D,0xE0, \
0x08,0xEC,0x29,0x66,0x4B,0xDE,0xB7,0xDE,0x36, \
0x08,0x05,0x0D,0x28,0xB9,0x0A,0x89,0x31,0x1A, \
        0x07, TX_RF_POWER_UP, \
        0x08, TX_RF_GPIO_PIN_CFG, \
        0x06, TX_RF_GLOBAL_XO_TUNE_2, \
        0x05, TX_RF_GLOBAL_CONFIG_1, \
        0x05, TX_RF_INT_CTL_ENABLE_1, \
        0x08, TX_RF_FRR_CTL_A_MODE_4, \
        0x05, TX_RF_PREAMBLE_CONFIG_1, \
        0x10, TX_RF_MODEM_MOD_TYPE_12, \
        0x05, TX_RF_MODEM_FREQ_DEV_0_1, \
        0x10, TX_RF_MODEM_TX_RAMP_DELAY_12, \
        0x10, TX_RF_MODEM_BCR_NCO_OFFSET_2_12, \
        0x07, TX_RF_MODEM_AFC_LIMITER_1_3, \
        0x05, TX_RF_MODEM_AGC_CONTROL_1, \
        0x10, TX_RF_MODEM_AGC_WINDOW_SIZE_12, \
        0x09, TX_RF_MODEM_RAW_CONTROL_5, \
        0x05, TX_RF_MODEM_RSSI_JUMP_THRESH_1, \
        0x06, TX_RF_MODEM_RSSI_CONTROL2_2, \
        0x06, TX_RF_MODEM_RAW_SEARCH2_2, \
        0x06, TX_RF_MODEM_SPIKE_DET_2, \
        0x05, TX_RF_MODEM_RSSI_MUTE_1, \
        0x09, TX_RF_MODEM_DSA_CTRL1_5, \
        0x10, TX_RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, \
        0x10, TX_RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, \
        0x10, TX_RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, \
        0x08, TX_RF_PA_MODE_4, \
        0x0B, TX_RF_SYNTH_PFDCP_CPFF_7, \
        0x0C, TX_RF_FREQ_CONTROL_INTE_8, \
        0x00 \
 }
#else
#define TX_RADIO_CONFIGURATION_DATA_ARRAY { 0 }
#endif

// DEFAULT VALUES FOR CONFIGURATION PARAMETERS
#define TX_RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT                     30000000L
#define TX_RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT                    0x00
#define TX_RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT               0x10
#define TX_RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT        0x01
#define TX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT       0x1000

#define TX_RADIO_CONFIGURATION_DATA_RADIO_PATCH_INCLUDED                      0x00
#define TX_RADIO_CONFIGURATION_DATA_RADIO_PATCH_SIZE                          0x00
#define TX_RADIO_CONFIGURATION_DATA_RADIO_PATCH                               {  }

#ifndef TX_RADIO_CONFIGURATION_DATA_ARRAY
#error "This property must be defined!"
#endif

#ifndef TX_RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ
#define TX_RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ          TX_RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT 
#endif

#ifndef TX_RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER
#define TX_RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER         TX_RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT 
#endif

#ifndef TX_RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH
#define TX_RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH    TX_RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT 
#endif

#ifndef TX_RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP
#define TX_RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP   TX_RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT 
#endif

#ifndef TX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET
#define TX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET  TX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT 
#endif

#define TX_RADIO_CONFIGURATION_DATA { \
                            TX_RADIO_Configuration_Data_Array,                            \
                            TX_RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER,                   \
                            TX_RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH,              \
                            TX_RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP,       \
                            TX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       \
                            }

#endif /* TX_RADIO_CONFIG_H_ */
