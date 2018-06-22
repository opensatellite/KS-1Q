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
  * KS1调试工具 ( 包括一个接线板卡, 2个stlink或jlink, 1个flashpro 4 )
* 购买渠道
  * http://deepace.net
  * http://kcsa.taobao.com ( 建设中 )

# 编译环境
  * 固件: IAR EWARM 7.2+ ( 商用工具链, 正在移植到gcc工具链 )
  * FPGA: Microsemi Libero SoC 11.3+ ( 免费, 在此处申请license: https://www.microsemi.com/product-directory/design-resources/1750-libero-soc  )
  * 地面控制软件: Qt 5.2以上 ( 免费 )
  * 软件无线电收发: Ubuntu系统, 安装了Gnuradio 3.7.10+, UHD 003.010.001+ ( 只有UHD 003.010.001以上版本支持USRP B210, 无法使用Windows系统的虚拟机运行. )

# 使用方法
  * 完善中

# 待做事项
  * 所有的固件转移到gcc工具链
  * 网页版地面控制软件 
  * 星上脚本程序支持
  * 星上固件自动升级
  * 更好的半双工无线链路

