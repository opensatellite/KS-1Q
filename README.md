# KS-1Q
Opensat first generation cubesat bus
* Official Website http://sa.kechuang.org
* License: LGPL-2.1
* Launch Vehicle: CALT CZ-11
* Launch Date: 2016/11/11
* NORAD Object: 2016-066E

# Features
* Complete Cubesat bus design: hardware + software + documents
* Payloads: camera, LED array, radiation dosimeter
* Onboard computer: STM32F4, software stack including RTOS, FS, CANbus, libcsp (Cubesat Space Protocol), navigation, logging
* Onboard communications: half duplex or full duplex on amateur VHF and UHF band (140-148MHz, 430-440MHz). Tx power 2W, Rx sensitivity -113dBm @ 10kbps. FPGA radio baseband compliant to CCSDS standard, with libcsp support. 
* Power system: MPPT solar charging, battery management, power distribution, safety switch, magnetorquer driver, sun sensor input, EED driver and RTC clock. Telemetry and protection for all critical components.
* Ground control: Qt based UI, provide display and control for all onboard modules.
* Gnuradio extension: custom OOT module gr-kcsa-ks1q for telemetry and telecommand. Contains necessary blocks for building custom satellite transceivers.
* Documents: interface control document (ICD) and safety report submit to launch service provider, including mechanical stress test and EMC test report. Brief product introduction, schematic and flowgraph for all modules.

# Latest features
* 2016/11/3: fix radio telemetry packet early-ending problem.
* 2016/11/20: fix battery under-voltage protection issues.
* 2018/6/2: gr-kcsa-ks1q pdu_to_stream2 support fractional decimation ratio.

# Project folders
* documents: system design documents, intro pages, ICD and safety report.
* firmware: embedded software for all onborad modules.
  * firmware/binary: prebuilt firmware images. 
  * firmware/(MODULE)_FW(VER)/: firmware VER, code and project files for MODULE.
  * firmware/(MODULE)_FW(VER)_flowgraph.pdf: firmware VER program flowgraph for MODULE.
  * firmware/(MODULE)_FW_ChangeLog.txt: firmware changelog for MODULE.
* hardware: module hardware design documents.
  * hardware/*block.pdf: module block diagram.
  * hardware/*connector*: connector list and pin definations.
  * hardware/mech_outline*: KS-1Q final mechanical dimensions.
  * hardware/photo: board photos.
* host: software for ground control and debug purpose
  * host/csp: libcsp preconfigured. need by KS1GCS
  * host/DM: Data manager, all telemetry and telecommand data structure definations and service routines.
  * host/NTCTest: onboard NTC temp sensor lookup table generation tool, needed if custom NTC is used.
  * host/gr-kcsa-ks1q: custom OOT module for telemetry and telecommand.
  * host/KS1GCS: ground control user interface, qt based.
  * host/WebAPI: ground control web interface, nodejs/python based.

# Hardware
* Minimal KS-1Q devkit including:
  * KS1_EPS ( electrical power system ) module
  * KS1_SOLAR ( solar panel, sun sensor and LED array )
  * TTC ( telemetry and telecommand, aka. onboard communication ) module
  * OBC ( onboard computer ) module
  * NI USRP B210 ( other SDR device like LimeSDR can be used, some software modification needed )
  * KS-1Q internal cables.
  * KS1 Debug Tool ( an interface wiring board, two stlink or jlink, one flashpro 4. )
* How to buy
  * http://deepace.net
  * http://kcsa.taobao.com ( work in progress )

# Build environment
  * Firmware: IAR EWARM 7.2+ ( commerical toolchain. gcc support work in progress )
  * FPGA: Microsemi Libero SoC 11.3+ ( it's free, see licensing section in https://www.microsemi.com/product-directory/design-resources/1750-libero-soc  )
  * Ground control: Qt 5.2+ ( it's free )
  * Radio transceiver: Ubuntu with Gnuradio 3.7.10+, UHD 003.010.001+ installed. ( USRP B210 only supported in UHD 003.010.001+, windows virtual machine won't work. )

# How to use
  * work in progress

# TODO list
  * Move all firmware to gcc toolchain
  * Web based ground control 
  * Onboard scripting support
  * OBC firmware upgrade support
  * Better half-duplex radio link.

