1������˵��

    �ò�����ʾ��USARTx��PC��ͨ����ѯ����ʶʵ�ֵĻ���ͨ�š�
    �ض���printf������USARTx����ʹ��printf���������Ϣ���նˡ�
    USARTx������USART1��USART2��


2��ʹ�û���

    �������������KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    
    Ӳ������������    
        N32H473ϵ�У�
        ����������N32H473VEL7_STB V1.0����
        N32H474ϵ�У�
        ����������N32H474VEL7_STB V1.0����
        N32H475ϵ�У�
        ����������N32H475UEQ7_STB V1.0����
        N32H482ϵ�У�
        ����������N32H482ZEL7_STB V1.0����
        N32H487ϵ�У�
        ����������N32H487ZEL7_EVB V1.0����
        

3��ʹ��˵��
	
	/* �������ģ�����÷���������:ʱ�ӣ�I/O�� */
    ϵͳʱ�����ã�
        N32H475/N32H482/N32H487:
            SystemClock��240MHz
        N32H474/N32H473:
            SystemClock��200MHz
    
    USART�������£�
    - ������ = 115200 baud
    - �ֳ� = 8����λ
    - 1ֹͣλ
    - У����ƽ���
    - Ӳ�������ƽ��ã�RTS��CTS�źţ�
    - �������ͷ�����ʹ��
    
    USART�����������£�
    - USART3_Tx.PA9 
    
    ���Բ���������
    - Demo��KEIL�����±����������MCU
    - ��λ���У��鿴���ڴ�ӡ��Ϣ


4��ע������




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
            SystemClock��240MHz
        N32H474/N32H473:
            SystemClock��200MHz
    
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