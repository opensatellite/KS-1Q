# KS-1Q
开源卫星第一代立方星平台
* 官方网站 http://sa.kechuang.org
* 授权协议: LGPL-2.1
* 运载火箭: 中国运载火箭研究院 长征11号
* 发射日期: 2016/11/11
* NORAD空间物体编号: 2016-066E

# 功能特性
* 完整的立方星平台设计方案，提供完整的硬件、软件和文档
* 载荷: 相机，LED阵列，辐射剂量计
* 星载计算机: 硬件基于STM32F4, 软件提供了实时操作系统(RTOS),文件系统,CAN总线驱动, libcsp (Cubesat Space Protocol)移植,导航算法,日志功能
* 星上测控收发机: 支持半双工/全双工方式, 工作在VHF和UHF业余无线电频段 (140-148MHz, 430-440MHz). 发射功率2W, 接收机灵敏度-113dBm @ 10kbps数据速率. 基于FPGA的基带处理器兼容CCSDS标准, 兼容libcsp.
* 电源系统: MPPT (最大功率点跟踪) 太阳能充电, 电池管理, 电源分配, 安全开关, 磁力矩器驱动, 太阳敏感器采集, 火工品驱动, RTC (实时时钟). 所有的关键部件均带有遥测和保护功能.
* 地面控制站: Qt开发的图形界面, 可显示所有星上设备的遥测量或发送遥控指令.
* Gnuradio扩展包: 用于卫星遥测遥控的扩展模块gr-kcsa-ks1q, 包括多种常用模块，可用于构建自定义的卫星测控收发机
* 文档: 提供给发射服务商的接口控制文档(ICD)和安全性报告, 包括机械应力测试和电磁兼容测试(EMC)报告. 提供产品简介文档, 所有模块的电路原理图和软件流程图.

# 最新增加的功能

* 2016/11/3: 修复了星上遥测发射数据包过早结束的问题.
* 2016/11/20: 修复了电池欠压保护问题.
* 2018/6/2: gr-kcsa-ks1q: pdu_to_stream2 模块增加了小数倍采样率抽取的支持.

# 目录结构
* documents: 总体设计文档, 产品介绍, ICD和安全性报告.
* firmware: 所有星上模块的嵌入式软件.
  * firmware/binary: 预编译好的二进制固件文件. 
  * firmware/(MODULE)_FW(VER)/: 模块MODULE的工程文件和代码文件, 固件版本=VER.
  * firmware/(MODULE)_FW(VER)_flowgraph.pdf: 模块MODULE的固件程序流程图, 固件版本=VER.
  * firmware/(MODULE)_FW_ChangeLog.txt: 模块MODULE的固件修改记录.
* hardware: 所有星上模块的硬件设计资料
  * hardware/*block.pdf: 模块的构成框图.
  * hardware/*connector*: 连接器列表和针脚功能定义.
  * hardware/mech_outline*: KS-1Q机械外形三视图
  * hardware/photo: 板卡实物照片
* host: 地面控制软件和调试软件
  * host/csp: 预先配置好的libcsp库, 用于KS1GCS
  * host/DM: 数据管理器(Data Manager), 所有的遥测、遥控数据接口和服务程序.
  * host/NTCTest: NTC热敏电阻查找表数据生成工具, 更换NTC热敏电阻规格时需要使用.
  * host/gr-kcsa-ks1q: 用于卫星遥测遥控的Gnuradio扩展模块
  * host/KS1GCS: 地面控制软件界面, 使用Qt开发

# 硬件
* KS-1Q最小开发套件包括:
  * KS1_EPS ( 电源系统 ) 模块
  * KS1_SOLAR ( 太阳帆板, 太阳敏感器和LED灯阵列 )
  * TTC ( 测控收发机 ) 模块
  * OBC ( 星载计算机 ) 模块
  * NI USRP B210 ( 其他的SDR收发机例如LimeSDR也可以使用, 需要进行一些软件修改 )
  * KS-1Q电缆束
  * KS1_DEBUG_TOOL ( 调试工具, 包括一个接线板卡, 2个stlink或jlink, 1个flashpro4 )
* 购买渠道
  * http://deepace.net
  * http://kcsa.taobao.com ( 建设中 )

# 编译环境
  * 固件: IAR EWARM 7.2+ ( 商用工具链, 正在移植到gcc工具链 )
  * FPGA: Microsemi Libero SoC 11.3+ ( 免费, 在此处申请license: https://www.microsemi.com/product-directory/design-resources/1750-libero-soc  )
  * 地面控制软件: Qt 5.2以上 ( 免费 )
  * 软件无线电收发: Ubuntu系统, 安装了Gnuradio 3.7.10+, UHD 003.010.001+ ( 只有UHD 003.010.001以上版本支持USRP B210, 无法使用Windows系统的虚拟机运行. )
  * STLink utility: ST-Link调试器使用这个软件进行HEX固件下载。 ( 软件下载链接 https://www.st.com/en/development-tools/stsw-link004.html )
  * J-Flash: JLink调试器使用这个软件进行HEX固件下载 ( JLink购买处附带 )

# 使用方法
* 准备工作 - 下载MCU固件
  * 使用源码编译下载: 在IAR的`Project Settings`菜单, 选择你使用的Debugger, 然后点击`Download and debug`按钮。
  * 载入预先编译好的固件: 使用STLink utility或者J-Flash.
* 准备工作 - 下载FPGA固件
  * 使用源码编译下载: 使用Libero SoC的`Program Device`功能页面. ( 教程页面 https://www.eecs.umich.edu/courses/eecs373/labs/LiberoRefGuides/projectflow_SoC.html )
  * 载入预先编译好的固件: 安装FlashPro软件, 使用说明书: http://coredocs.s3.amazonaws.com/Libero/11_7_0/Tool/flashpro_ug.pdf
* 准备工作 - 安装Gnuradio和USRP驱动程序
  * 如果系统是Ubuntu 18.04, 最简单的方法是运行命令`sudo apt install gnuradio-dev uhd-host`
  * 如果系统是Ubuntu 16.04, 需要从源码编译安装Gnuradio ( 源码链接 http://github.com/gnuradio/gnuradio ), USRP驱动程序使用最新的PPA源安装 ( 教程页面 https://files.ettus.com/manual/page_install.html )
    `sudo add-apt-repository ppa:ettusresearch/uhd
    `sudo apt-get update
    `sudo apt-get install libuhd-dev libuhd003 uhd-host
  * 其他Linux发行版还没有测试过,推荐的方法是自行从源码编译安装
* 安装地面控制软件
  * 安装Ubuntu系统
  * 安装安装Gnuradio和USRP驱动程序
  * 编译安装libcsp
    # cd KS-1Q/host/csp
    # ./build_csp.sh && sudo ./install_csp.sh
  * 安装KS1GCS
    运行QtCreator, 打开工程文件 KS-1Q/host/KS1GCS/KS1GCS.pro
    点击`Build` ( 靠近左下角的锤子图标 )
* 载入bootloader/固件
  * 步骤1: 将KS1 Debug Tool连接到KS-1Q
  * 步骤2: 将STLink或者JLink连接到EPS SWD端口
  * 步骤3: 将锂电池充电器连接到CHARGE端口
  * 步骤4: EPS_RBF开关切换到`UNLOCK`
  * 步骤5: 下载KS1_EPS_FW03固件到KS1_EPS模块
            -> 如果下载失败,检查KS1_EPS内部的STM32供电是否正常
            -> 可能的原因: RBF开关设置到`LOCK`位置, KS1_EPS没有安装电池，或者电池已经耗尽。
  * 步骤6: TTC_DEBUG_SELECT开关切换到`JTAG`位置
  * 步骤7: 下载TTC_V04_FPGA (FPGA位流) 到TTC模块
            -> 如果下载失败,检查TTC模块供电是否正常.
            -> 可能的原因: KS1_EPS没有为TTC模块供电. 
            -> RBF开关设置到`Unlock`位置? KS1_EPS固件已经装载? 
  * 步骤8: TTC_DEBUG_SELECT开关切换到`SWD`位置
  * 步骤9: 下载TTC_V04_FW00到TTC模块
            -> 如果下载失败,检查TTC模块供电是否正常.
  * 步骤10: 将USRP连接到主机的USB3.0端口
  * 步骤11: 通过Gnuradio与KS-1Q建立测控链路
	    # cd KS-1Q/host/KS1GCS/
            # python ccsds-halfduplex-tcpserver2.py
	    若要编辑Gnuradio的处理流图, 运行gnuradio-companion命令, 然后打开KS-1Q/host/gr-kcsa-ks1q/examples/ccsds-halfduplex-tcpserver2.grc文件
  * 步骤12: 运行KS1GCS, 点击`连接`按钮
  * 步骤13: 发送遥控指令开启OBC电源. 点击KS1GCS的`EPS`页面, 然后点击`OBC开启`按钮. 成功后,EPS遥测量PwrOBC会在大约10秒之后变为一个非零数值
            -> 现在, OBC电源已经开启
  * 步骤14: 下载OBC_2D_FW03到OBC模块
            -> 如果下载失败,检查OBC模块供电是否正常.
            -> 使用一个外部电源(与电池电压相同, 7.2～8.4V之间)可以直接启动OBC. 这样做可以省去步骤10-13.

# 待做事项
  * 所有的固件转移到gcc工具链
  * 网页版地面控制软件 
  * 星上脚本程序支持
  * 星上固件自动升级
  * 更好的半双工无线链路

