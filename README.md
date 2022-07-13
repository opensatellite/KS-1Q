# KS-1Q
Opensat first generation cubesat bus
* Official Website http://sa.kechuang.org
* License: LGPL-2.1
* Launch Vehicle: CALT CZ-11
* Launch Date: 2016/11/11
* NORAD Object: 2016-066E

<img src="hardware/photo/KS-1Q testing.jpg">

# Features
* Complete Cubesat bus design: hardware + software + documents
* Payloads: camera, LED array, radiation dosimeter
* Onboard computer: STM32F4, software stack including RTOS, FS, CANbus, libcsp (Cubesat Space Protocol), navigation, logging
* Onboard communications: half duplex or full duplex on amateur VHF and UHF band (140-148MHz, 430-440MHz). Tx power 2W, Rx sensitivity -113dBm @ 10kbps, both tx and rx support 2k~500kbps rate. FPGA radio baseband compliant to CCSDS standard, with libcsp support. With USRP B210, downlink rx sensitivity -126dBm @ 10kbps.
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
* FPGA: if you want to recompile fpga logics, install Microsemi Libero SoC 11.3+ ( it's free, see licensing section in https://www.microsemi.com/product-directory/design-resources/1750-libero-soc  )
* Ground control: Qt 5.2+ ( it's free )
* Radio transceiver: Ubuntu with Gnuradio 3.7.10+, UHD 003.010.001+ installed. ( USRP B210 only supported in UHD 003.010.001+, windows virtual machine won't work. )
* STLink utility: if using stlink debugger, need this for firmware hex download. ( download link https://www.st.com/en/development-tools/stsw-link004.html )
* J-Flash: if using jlink debugger, need this for firmware hex download. ( shipped with your jlink )

# How to use
* Preparing - how to load mcu firmware
  * if build with source, configure debugger in IAR `Project Settings`, then use `Download and debug` button.
  * if loading prebuilt image, use STLink utility or J-Flash.
* Preparing - how to load fpga image
  * if build with source, use `Program Device` in Libero SoC. ( https://www.eecs.umich.edu/courses/eecs373/labs/LiberoRefGuides/projectflow_SoC.html )
  * if loading prebuilt image, use FlashPro software, user guide here http://coredocs.s3.amazonaws.com/Libero/11_7_0/Tool/flashpro_ug.pdf
* Preparing - how to install gnuradio and usrp driver
  * if using Ubuntu 18.04, use `sudo apt install gnuradio-dev uhd-host` is the easist way
  * if using Ubuntu 16.04, compile and install gnuradio from source ( http://github.com/gnuradio/gnuradio ) by yourself, usrp driver must use latest PPA ( https://files.ettus.com/manual/page_install.html )
    * `sudo add-apt-repository ppa:ettusresearch/uhd`
    * `sudo apt-get update`
    * `sudo apt-get install libuhd-dev libuhd003 uhd-host`
  * other linux distribution not tested, compile on your own is suggested.
* Install host software
  * Install ubuntu linux
  * Install gnuradio and usrp driver
  * Build and install libcsp
    * `cd KS-1Q/host/csp`
    * `./build_csp.sh`
    * `sudo ./install_csp.sh`
  * Build KS1GCS
    * launch QtCreator, open project KS-1Q/host/KS1GCS/KS1GCS.pro
    * press `Build` button ( the hammer icon near left-bottom )
* Load bootloader / firmware
  * Step 1: Connect KS1 debug tool to KS-1Q
  * Step 2: Connect stlink or jlink to EPS SWD port
  * Step 3: Connect li-ion battery charger to CHARGE port
  * Step 4: Set EPS_RBF switch to `UNLOCK` position
  * Step 5: Load KS1_EPS_FW03 (firmware) to KS1_EPS module
    * if failed, check STM32 on KS1_EPS have power supply
    * possible reason: RBF in `LOCK` position or battery rans out.
  * Step 6: Set TTC_DEBUG_SELECT switch to `JTAG` position
  * Step 7: Load TTC_V04_FPGA (fpga bitstream) to TTC module
    * if failed, check TTC module have power supply.
    * possible reason: KS1_EPS no output.
    * RBF unlocked ? KS1_EPS firmware loaded ?
  * Step 8: Set TTC_DEBUG_SELECT switch to `SWD` position
  * Step 9: Load TTC_V04_FW00 to TTC module
    * if failed, check TTC module have power supply.
  * Step 10: connect USRP to host USB3.0 port
  * Step 11: Run gnuradio to establish wireless connection to KS-1Q.
	* cd KS-1Q/host/KS1GCS/
    * python ccsds-halfduplex-tcpserver2.py
	* to edit flowgraph, run gnuradio-companion and open KS-1Q/host/gr-kcsa-ks1q/examples/ccsds-halfduplex-tcpserver2.grc
  * Step 12: Run KS1GCS, then click `Connect` button.
  * Step 13: Send telecommand to enable OBC power. click `EPS` page in KS1GCS, then click `OBC on` button. if success, EPS telemetry PwrOBC will change to nonzero value after ~10seconds.
    * Now OBC have power supply
  * Step 14: Load OBC_2D_FW03 to OBC module.
    * if failed, check OBC module have power supply.
    * use an external power supply ( equal to battery voltage, 7.2~8.4V ) can power on OBC directly. here step 10 - 13 is not required.

# TODO list
  * Move all firmware to gcc toolchain
  * Web based ground control
  * Onboard scripting support
  * OBC firmware upgrade support
  * Better half-duplex radio link.
