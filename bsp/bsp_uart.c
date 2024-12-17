/*
* 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源
* 开发板官网：www.lckfb.com
* 技术支持常驻论坛，任何技术问题欢迎随时交流学习
* 立创论坛：club.szlcsc.com
* 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
* 不靠卖板赚钱，以培养中国工程师为己任
* Change Logs:
* Date           Author       Notes
* 2023-11-02     LCKFB-yzh    first version
*/

#include "bsp_uart.h"
#include <stdio.h>

// clang-format off

static const uint32_t COM_UART[COM_UART_MAX_NUM] =
{
COM_UART0,
COM_UART1,
COM_UART2
};
static const IRQn_Type COM_UART_IRQn[COM_UART_MAX_NUM] =
{
COM_UART0_IRQn,
COM_UART1_IRQn,
COM_UART2_IRQn
};
static const rcu_periph_enum COM_UART_CLK[COM_UART_MAX_NUM] =
{
COM_UART0_CLK,
COM_UART1_CLK,
COM_UART2_CLK,
};
static const rcu_periph_enum COM_UART_TX_GPIO_CLK[COM_UART_MAX_NUM] =
{
COM_UART0_TX_GPIO_CLK,
COM_UART1_TX_GPIO_CLK,
COM_UART2_TX_GPIO_CLK
};
static const uint32_t COM_UART_TX_GPIO_AF[COM_UART_MAX_NUM] =
{
COM_UART0_TX_GPIO_AF,
COM_UART1_TX_GPIO_AF,
COM_UART2_TX_GPIO_AF
};
static const uint32_t COM_UART_TX_PORT[COM_UART_MAX_NUM] =
{
COM_UART0_TX_PORT,
COM_UART1_TX_PORT,
COM_UART2_TX_PORT
};
static const uint32_t COM_UART_TX_PIN[COM_UART_MAX_NUM] =
{
COM_UART0_TX_PIN,
COM_UART1_TX_PIN,
COM_UART2_TX_PIN
};
static const rcu_periph_enum COM_UART_RX_GPIO_CLK[COM_UART_MAX_NUM] =
{
COM_UART0_RX_GPIO_CLK,
COM_UART1_RX_GPIO_CLK,
COM_UART2_RX_GPIO_CLK
};
static const uint32_t COM_UART_RX_GPIO_AF[COM_UART_MAX_NUM] =
{
COM_UART0_RX_GPIO_AF,
COM_UART1_RX_GPIO_AF,
COM_UART2_RX_GPIO_AF
};
static const uint32_t COM_UART_RX_PORT[COM_UART_MAX_NUM] =
{
COM_UART0_RX_PORT,
COM_UART1_RX_PORT,
COM_UART2_RX_PORT
};
static const uint32_t COM_UART_RX_PIN[COM_UART_MAX_NUM] =
{
COM_UART0_RX_PIN,
COM_UART1_RX_PIN,
COM_UART2_RX_PIN
};

static const uint32_t COM_UART_BAUD[COM_UART_MAX_NUM] =
{
COM_UART0_BAUD,
COM_UART1_BAUD,
COM_UART2_BAUD
};

// clang-format on

static void bsp_uart_periph_init(uart_type_def _com_uart)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable( COM_UART_TX_GPIO_CLK[_com_uart]);
    rcu_periph_clock_enable( COM_UART_RX_GPIO_CLK[_com_uart]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_UART_CLK[_com_uart]);

    /* connect port to USARTx_Tx */
    gpio_af_set(COM_UART_TX_PORT[_com_uart], COM_UART_TX_GPIO_AF[_com_uart], COM_UART_TX_PIN[_com_uart]);

    /* connect port to USARTx_Rx */
    gpio_af_set(COM_UART_RX_PORT[_com_uart], COM_UART_RX_GPIO_AF[_com_uart], COM_UART_RX_PIN[_com_uart]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(COM_UART_TX_PORT[_com_uart], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_TX_PIN[_com_uart]);
    gpio_output_options_set(COM_UART_TX_PORT[_com_uart], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_TX_PIN[_com_uart]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(COM_UART_RX_PORT[_com_uart], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_RX_PIN[_com_uart]);
    gpio_output_options_set(COM_UART_RX_PORT[_com_uart], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_RX_PIN[_com_uart]);

    /* USART configure */
    usart_deinit(COM_UART[_com_uart]);
    usart_baudrate_set(COM_UART[_com_uart],COM_UART_BAUD[_com_uart]);
    usart_receive_config(COM_UART[_com_uart], USART_RECEIVE_ENABLE);
    usart_transmit_config(COM_UART[_com_uart], USART_TRANSMIT_ENABLE);
    usart_enable(COM_UART[_com_uart]);

    /* USART interrupt configuration */
    if (_com_uart == COM_UART_NUM0)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
    else if (_com_uart == COM_UART_NUM1)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
    else if (_com_uart == COM_UART_NUM2)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 0, 0);
    }
    else if (_com_uart == COM_UART_NUM5)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 1, 0);
    }
    else
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
}
void bsp_uart_init(void)
{
    for (uart_type_def _uart = COM_UART_NUM0; _uart < COM_UART_MAX_NUM; _uart++)
    {
        bsp_uart_periph_init(_uart);
    }
}

#if !defined(__MICROLIB)
//不使用微库的话就需要添加下面的函数
#if (__ARMCLIB_VERSION <= 6000000)
//如果编译器是AC5  就定义下面这个结构体
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(COM_UART[0], (uint8_t)ch);
    while(RESET == usart_flag_get(COM_UART[0], USART_FLAG_TBE));
    return ch;
}

