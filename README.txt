ArithMax Overview
=================

概述
ArithMax是由nbzwt在cnCalc论坛发起的一项自制开源图形计算器活动，最先的名字是OurCalc，不排除将来继续改名（比如就叫cnCalc）的可能性。其初衷是为大家提供一个开放的，具有高度可定制性且价格低廉的图形计算器。大家知道，作为学生用计算器的market leader——TI，其nspire计算器不仅价格昂贵（直到最近几个月，洋垃圾的价格才开始下降）而且十分封闭（虽然有ndless将其和谐），但是由于其高性能和高分辨率，仍然吸引了不少人选购，取代了9860成了本论坛no.1的计算器。ArithMax就是在拥有nspire这些硬件优点的同时，发挥开源和国产的优势，把价格降到最低，方便广大学生朋友购买和使用。

Hardware
========

硬件
ArithMax的硬件经过好几次修改，未来可能会继续修改，下面是目前计划的ArithMax配置情况。
CPU：i.MX233 来自Freescale的ARM9处理器，主频可达454MHz，秒杀一切
RAM：DDR 64MB 使用一片HY5DU121622D，16bit总线，DDR333
ROM：待定
屏幕：2.6英寸 640*480 307ppi 电阻触摸 背光200nit 型号TD026TTEA1，LTPS TFT LCD
按键：支持外部连接8*8矩阵键盘或通过i2c、spi总线连接键盘。会提供连接fx-82es plus键盘和nspire touchpad的示例
IO：调试接口（SJTAG，串口和复合视频输出），TF卡接口，USB接口。

Software
========

软件
会以FreeRTOS或RT-Thread为系统内核，上层界面由自己开发，核心计算软件移植eigenmath。

Price
=====

售价
价格会低于9860GII，发售日期有待商讨。

Development
===========

开发进度
参考日志的进展汇报。

Website
=======

网站
http://zweb.clanmark.com

Join us
=======

加入我们
目前硬件开发组主要人员为nbzwt和Dram，软件开发组主要人员为nbzwt，前台MM为朔雪X忆。加入请email我nbzwt@126.com。