1、功能说明

    该测例演示了USARTx与PC间通过查询检测标识实现的基础通信。
    重定向printf函数至USARTx，并使用printf函数输出消息至终端。
    USARTx可以是USART1或USART2。


2、使用环境

    软件开发环境：KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    
    硬件开发环境：    
        N32H473系列：
        基于评估板N32H473VEL7_STB V1.0开发
        N32H474系列：
        基于评估板N32H474VEL7_STB V1.0开发
        N32H475系列：
        基于评估板N32H475UEQ7_STB V1.0开发
        N32H482系列：
        基于评估板N32H482ZEL7_STB V1.0开发
        N32H487系列：
        基于评估板N32H487ZEL7_EVB V1.0开发
        

3、使用说明
	
	/* 描述相关模块配置方法；例如:时钟，I/O等 */
    系统时钟配置：
        N32H475/N32H482/N32H487:
            SystemClock：240MHz
        N32H474/N32H473:
            SystemClock：200MHz
    
    USART配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - 硬件流控制禁用（RTS和CTS信号）
    - 接收器和发送器使能
    
    USART引脚连接如下：
    - USART3_Tx.PA9 
    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，查看串口打印信息


4、注意事项




1. Function description

     This test example demonstrates the basic communication between USARTx and PC by querying the detection flag.
     Redirect the printf function to USARTx, and use the printf function to output messages to the terminal.
     USARTx can be USART1 or USART2.


2. Use environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware development environment:
        N32H473 series:
        Developed based on the evaluation board N32H473VEL7_STB V1.0
        N32H474 series:
        Developed based on the evaluation board N32H474VEL7_STB V1.0
        N32H475 series:
        Developed based on the evaluation board N32H475UEQ7_STB V1.0
        N32H482 series:
        Developed based on the evaluation board N32H482ZEL7_STB V1.0
        N32H487 series:
        Developed based on the evaluation board N32H487ZEL7_EVB V1.0
        

3. Instructions for use

	/* Describe related module configuration methods; for example: clock, I/O, etc. */
    System Clock Configuration:
        N32H475/N32H482/N32H487:
            SystemClock：240MHz
        N32H474/N32H473:
            SystemClock：200MHz
    
     USART is configured as follows:
     - Baud rate = 115200 baud
     - Word length = 8 data bits
     - 1 stop bit
     - checksum control disabled
     - Hardware flow control disabled (RTS and CTS signals)
     - Receiver and transmitter enable
    
     The USART pins are connected as follows:
     - USART3_Tx.PA9
    
     Test steps and phenomena:
     - Demo is compiled in KEIL environment and downloaded to MCU
     - reset operation, view serial port printing information


4. Attention
    none