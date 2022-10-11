/* USER CODE BEGIN Header */
//=============================================================================
//              nobreak controle
//
// Jonny                                                            04/08/2021
// Popovicz '-'                                                     19/07/2022
//=============================================================================
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "socket.h"
#include "dhcp.h"
#include "dns.h"
#include <stdio.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "httpServer.h"
#include "httpUtil.h"
#include "ZEZ-24CXX.h"
#include "DHT.h"
#include "math.h"
#include "stm32g0xx_ll_utils.h"
#include "ping.h"

#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM14_Init(void);
/* USER CODE BEGIN PFP */
//************************************************************************************************
//      Macros
//************************************************************************************************

#define ledvd_on   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
#define ledvd_off  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
#define ledvm_off  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
#define ledvm_on   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
#define rele_on    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
#define rele_off   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
#define inv_on     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
#define inv_off    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
#define wiz_off    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
#define wiz_on     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);

#define b1         HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_8)
#define b2         HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_7)
#define opt        HAL_GPIO_ReadPin (GPIOC, GPIO_PIN_14)
#define fan1       HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_0)
#define fan2       HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_8)
#define prot_curto HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_0) //Curto = 1;

static DHT_sensor livingRoom = {GPIOB, GPIO_PIN_2, DHT11, 0};

#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define HTTP_SOCKET     2

#define MMIO16(addr)  (*(volatile uint16_t *)(addr))
#define MMIO32(addr)  (*(volatile uint32_t *)(addr))
#define U_ID           0x1FFF75E0

//MEMORIA IP LOCAL
#define ADD_IPL1 0x01       //192
#define ADD_IPL2 0x02       //168
#define ADD_IPL3 0x03	    //004
#define ADD_IPL4 0x04       //010

//MEMORIA GATEWAY
#define ADD_GW1 0x05        //192
#define ADD_GW2 0x06	    //168
#define ADD_GW3 0x07        //004
#define ADD_GW4 0x08        //001

//MEMORIA NETMASK
#define ADD_MK1 0x09        //255
#define ADD_MK2 0x0A        //255
#define ADD_MK3 0x0B        //255
#define ADD_MK4 0x0C        //000

//MEMORIA POTA LOCAL (2 partes)
#define ADD_PORTL1 0x0D     //092
#define ADD_PORTL2 0x0F     //091

//MEMORIA IDT (2 partes)
#define ADD_IDT1 0x10       //012
#define ADD_IDT2 0x11       //034

//MEMORIA KEY (2 partes)
#define ADD_KEY1 0x12       //012
#define ADD_KEY2 0x13       //034

//MEMORIA IP DO SERVIDOR
#define ADD_IPS1 0x14       //177
#define ADD_IPS2 0x15       //159
#define ADD_IPS3 0x16       //233
#define ADD_IPS4 0x17       //213

//MEMORIA IP DO SERVIDOR BKP
#define ADD_DNS1 0x18      //177
#define ADD_DNS2 0x19      //159
#define ADD_DNS3 0x1A      //233
#define ADD_DNS4 0x1B      //214

//MEMORIA PORTA DO SERVIDOR
#define ADD_PORTS1 0x1C     //050
#define ADD_PORTS2 0x1D     //007

//DOMINIO
#define ADD_DOM1    0x1F    //U
#define ADD_DOM2    0x20    //P
#define ADD_DOM3    0x21    //S
#define ADD_DOM4    0x22    //C
#define ADD_DOM5    0x23    //B
#define ADD_DOM6    0x24    //A
#define ADD_DOM7    0x25    //L
#define ADD_DOM8    0x26    //L
#define ADD_DOM9    0x27    //L
#define ADD_DOMA    0x28    //L
#define ADD_DOMB    0x29    //.
#define ADD_DOMC    0x2A    //S
#define ADD_DOMD    0x2B    //E
#define ADD_DOME    0x2C    //L
#define ADD_DOMF    0x2D    //P
#define ADD_DOMG    0x2E    //R
#define ADD_DOMH    0x2F    //O
#define ADD_DOMI    0x30    //M
#define ADD_DOMJ    0x31    //L
#define ADD_DOMK    0x32    //L
#define ADD_DOML    0x33    //L
#define ADD_DOMM    0x34    //.
#define ADD_DOMN    0x35    //C
#define ADD_DOMO    0x36    //O
#define ADD_DOMP    0x37    //M
#define ADD_DOMQ    0x38    //.
#define ADD_DOMR    0x39    //B
#define ADD_DOMS    0x3A    //R

//Tamanho do dominio
#define ADD_TDOM    0x3B    //10

#define ADD_ISD     0x3C    //BOOL
#define ADD_TIMEOUT 0x3D    //20s
#define ADD_BOOT    0x3F

#define Tfiltro 5000;

//************************************************************************************************
//      var globais
//************************************************************************************************
uint8_t UART1_rxBuffer[6] = {0};

//LCD
uint8_t tela=0;
char pp0[]= {"----"};
char pp1[]= {"----"};
char msg1[6];
char msg2[6];
char msg3[6];
char msg4[6];
char dht11[6];

char mm0[]= {"---"};
char mm1[]= {"---"};
char mm2[]= {"---"};
char mm3[]= {"---"};

char mn0[]= {"---"};
char mn1[]= {"---"};
char mn2[]= {"---"};
char mn3[]= {"---"};

char kk0[]= {"---"};
char kk1[]= {"---"};
char kk2[]= {"---"};
char kk3[]= {"---"};

char gg0[]= {"---"};
char gg1[]= {"---"};
char gg2[]= {"---"};
char gg3[]= {"---"};

float m1=0;
float m2=0;
float m3=0;

char msg5[100] = {"---"};
char energia[] = {"        "};

char mensage[60];
char testezin[13];

//NOBREAK
uint8_t  ad[4];
uint8_t  volta      = 5;
uint8_t  amp_carga  = 0;
uint8_t  amp_rede   = 0;
uint8_t  vot_bat    = 0;
uint8_t  bat_full   = 0;
uint8_t  bat_med    = 0;
uint8_t  vot_rede   = 0;
uint8_t  temp       = 0;
uint16_t dut_carga  = 0;
uint32_t tp_ativo   = 0;
char     energ      = 'X';
bool     rede;
bool     carga      = 0;
bool     fan1_stat 	= 0;
bool     fan2_stat 	= 0;
uint8_t boot;

//TCP/IP

uint8_t  ip_s[4]       = {192,168,4,2};                    // Ip do servidor
uint16_t port_s        = 9291;                             // Porta do servidor
uint8_t  dns_server[4] = {8,8,8,8};                        // DNS server

uint8_t  ip_l[4] = {192,168,4,1};                          // Ip local
uint8_t  mk_l[4] = {255,255,255,0};                        // NetMask
uint8_t  gw_l[4] = {192,168,4,254};                        // Gateway
uint16_t port_l  = 9292;                                   // Porta Local

uint8_t  ip_R[4] = {192,168,0,181};                        // Ultimo ip em que recebeu pacote
uint16_t port_R  = 2112;                                   // Ultima porta que recebeu pacote

uint8_t mac[6]   = { 0xEA, 0x11, 0x22, 0x33, 0x44, 0xEA }; // MAC address

uint8_t adominio[40] = {0x1F, 0x20, 0x21, 0x22, 0x23,      //Endereços de memória do Domínio
                        0x24, 0x25, 0x26, 0x27, 0x28,
                        0x29, 0x2A, 0x2B, 0x2C, 0x2D,
                        0x2E, 0x2F, 0x30, 0x31, 0x32,
                        0x33, 0x34, 0x35, 0x36, 0x37,
                        0x38, 0x39, 0x3A};

char    dominio[60];
uint8_t dns_buffer[256];
uint8_t tam_dominio    = 0;                   // Tamanho do dominio
uint8_t len;                                  // Tamanho do pacote recebido
uint8_t timeOut        = 10;
uint8_t timeOutLen     = 20;
bool    isdom          = 0;
bool    send_to_server = 0;


int idt = 1234;                           // IDT do dispositivo
int key = 1234;                           // Key do dispositivo


//PING
uint8_t ping_fail = 0;
bool    pinga          = 0;
PINGMSGR PingRequest;
PINGMSGR PingReply;
static uint16_t RandomID = 0x1234;
static uint16_t RandomSeqNum = 0x4321;
uint8_t ping_reply_received = 0;


//VARs gerais

uint16_t dnstimeout = 65000;
uint8_t dnscont;
uint8_t tryDNS;

uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2 , 2};

char x;                                   // CONTADOR DO FOR
char data;                                // byte que chegou pela udp
unsigned char ser_ptm;
unsigned char ser_bfm[48];
bool ser_pcm;
bool ser_rcm;
bool modo_prog    = 0;                    // Indica se o nobreak irá aceitar configurações
uint8_t fan1_cont = 5;
uint8_t fan2_cont = 5;
uint32_t filtro = Tfiltro;
uint8_t cont_ping = 0;
uint16_t cont_bat = 0;
uint8_t dead_bat = 0;
uint32_t cont_dead = 0;
uint8_t thrash = 0;
uint8_t modo_teste = 0;
uint16_t modo_teste_tempo = 10;
uint8_t modo_teste_cont = 0;
uint8_t curtoci = 0;
uint8_t statusTELA = 0;


float version = 1.3;

const unsigned char tbh[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};  //Tabela HEX

const float amp[240] = {0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.01,0.03,0.05,0.11,0.18,0,20,
						0.25,0.30,0.33,0.35,0.40,0.43,0.45,0.50,0.53,0.55,0.60,0.63,0.65,0.70,0.73,0.75,0.80,0.83,0.85,0.90,
						0.95,0.99,1.02,1.08,1.12,1.15,1.20,1.23,1.25,1.30,1.33,1.35,1.40,1.43,1.45,1.50,1.53,1.55,1.60,1.63,
						1.65,1.70,1.73,1.75,1.80,1.83,1.85,1.90,1.93,1.95,2.00,2.03,2.05,2.10,2.13,2.15,2.20,2.23,2.25,2.30,
						2.33,2.35,2.40,2.45,2.50,2.55,2.60,};

//DEVICE INFO
uint8_t aShowDeviceID[30]    = {0};
uint8_t aShowRevisionID[30]  = {0};
uint8_t aShowCoordinate[40]  = {0};
uint8_t aShowWaferNumber[30] = {0};
uint8_t aShowLotNumber[30]   = {0};


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//***********************************************************************************************
//           usart tx
//***********************************************************************************************
void UART_Printf(const char* fmt, ...) {
    char buff[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, sizeof(buff), fmt, args);
    HAL_UART_Transmit(&huart1, (uint8_t*)buff, strlen(buff),HAL_MAX_DELAY);
    va_end(args);
}

//***********************************************************************************************
//           Faz o crc do array_in e coloca no array_out
//***********************************************************************************************
void docrc(char *array_out[], char *array_in[], int len)
{
	int  crcdec;
	char crcstring[4];

	crcdec = HAL_CRC_Calculate(&hcrc, array_in, len);
	sprintf(crcstring, "%04X", crcdec);

	for(x=0; x<4; x++){
		strncat(array_out, &crcstring[x], 1);
	}

}
//***********************************************************************************************
//           Envia a mensagem + $# com o crc $mensagemcrc# para o servidor
//***********************************************************************************************
void enviaUDP(char *mensagem[],char *crc[], int len)
{
	char pacotePronto[60];

	sprintf(pacotePronto, "$%s%s#", mensagem, crc);
	sendto(1,pacotePronto,len +6,ip_s,port_s);

}

//***********************************************************************************************
//           Converte decimal em hex de 2 bytes e concatena no array
//***********************************************************************************************
void concathex2(char *array[], unsigned char bt)
{
	char hex[2];

	hex[0] = tbh[ bt/16];
	hex[1] = tbh[ bt%16];

	strncat(array, &hex[0], 1);
	strncat(array, &hex[1], 1);
}
//***********************************************************************************************
//           Converte decimal em hex de 4 bytes e concatena no array
//***********************************************************************************************
void concathex4(char *array[], uint16_t v)
{
	unsigned char bt;
	char hex[4];

	bt = (v/256);
	hex[0] = tbh[ bt>>4];
	hex[1] = tbh[ bt & 0xf];
	bt = v&0xff;
	hex[2] = tbh[ bt/16];
	hex[3] = tbh[ bt%16];

	for(x=0; x<4; x++){
		strncat(array, &hex[x], 1);
	}

}
//***********************************************************************************************
//           Converte hex em byte retornando o int decimal hextochar('0', '0', 'C', '0') -> 192
//***********************************************************************************************
int hextochar(char c1, char c2, char c3, char c4)
{
	char hex[4];
	long long decimal = 0, base = 1;
	int i = 0, length;
	hex[0] = c1;
	hex[1] = c2;
	hex[2] = c3;
	hex[3] = c4;
	length = strlen(hex);

	    for(i = length--; i >= 0; i--)
	    {
	        if(hex[i] >= '0' && hex[i] <= '9')
	        {
	            decimal += (hex[i] - 48) * base;
	            base *= 16;
	        }
	        else if(hex[i] >= 'A' && hex[i] <= 'F')
	        {
	            decimal += (hex[i] - 55) * base;
	            base *= 16;
	        }
	        else if(hex[i] >= 'a' && hex[i] <= 'f')
	        {
	            decimal += (hex[i] - 87) * base;
	            base *= 16;
	        }
	    }
 return decimal;
}

//***********************************************************************************************
//              dht
// **********************************************************************************************
void dht(void){

    DHT_data d = DHT_getData(&livingRoom);
    //sprintf(msg4, "T %d°C,  H %d%%", (uint8_t)d.temp, (uint8_t)d.hum);
    //sprintf(dht11, "T %dC  H %d%%", (uint8_t)d.temp, (uint8_t)d.hum);
    //sprintf(dht11, "%dC", (uint8_t)d.temp);
    temp=(uint8_t)d.temp;

}



//***********************************************************************************************
//              fan
// **********************************************************************************************
void trata_fan1(void){

    if(rede==0 || temp>=55){
      TIM3->CCR4 = 999;
      }else TIM3->CCR4 = 0;

    if(fan1_cont == 0){
    	fan1_stat = 0;
    }else{
    	fan1_stat = 1;
    }
}

//***********************************************************************************************
//              fan
// **********************************************************************************************
void trata_fan2(void){

    if(temp>41){
      TIM3->CCR1 = 999;
      }else TIM3->CCR1 = 0;

    if(fan2_cont == 0){
    	fan2_stat = 0;
    }else{
    	fan2_stat = 1;
    }
}

//***********************************************************************************************
//          carga
//***********************************************************************************************
void trata_carga(void)
{

	amp_carga=ad[1];
	if(carga == 1){
	if (amp_carga < 126) {dut_carga++;}
	if (amp_carga > 126 && (dut_carga > 1)) {dut_carga--;}
	if (amp_carga >= 160)
	{
		TIM3->CCR2 = 0;
		dut_carga=0;
	}
	TIM3->CCR2 = dut_carga;
	}else {
		TIM3->CCR2 = 0;
		dut_carga=0;
	}

}

void trata_carga_down(void)
{

	amp_carga=ad[1];
	if(carga == 1){
	if (amp_carga > 126 && (dut_carga > 1)) {dut_carga--;}
	TIM3->CCR2 = dut_carga;
	}else {
		TIM3->CCR2 = 0;
		dut_carga=0;
	}

}


//***********************************************************************************************
//           spi sel
//***********************************************************************************************
void W5500_Select(void) {
    HAL_GPIO_WritePin(w5500_CS_GPIO, w5500_CS_PIN, GPIO_PIN_RESET);
}

void W5500_Unselect(void) {
    HAL_GPIO_WritePin(w5500_CS_GPIO, w5500_CS_PIN, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Transmit(&hspi1, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}

//***********************************************************************************************
//           TELA LCD
//***********************************************************************************************
void telas(void){

	  switch(tela)
	  {
			case 0:
				  m2=(vot_rede*255.0/255);
			      sprintf(msg4, "%1.1fv  ", m2);
			      sprintf(msg3, "%06hus", tp_ativo);
			      sprintf(testezin, "%d", boot);

				  SSD1306_GotoXY (2, 0);
				  SSD1306_Puts ("Status", &Font_7x10, 1);
				  //SSD1306_Puts (testezin, &Font_7x10, 1);
				  SSD1306_GotoXY (70, 0);
				  SSD1306_Puts (energia, &Font_7x10, 1);
				  SSD1306_DrawLine (0, 10, 129, 10, SSD1306_COLOR_WHITE);
				  SSD1306_GotoXY (35, 20);
				  SSD1306_Puts (msg4, &Font_11x18, 1);
				  SSD1306_DrawLine (0, 43, 129, 43, SSD1306_COLOR_WHITE);
				  SSD1306_GotoXY (1, 50);
				  SSD1306_Puts ("Tempo ativo", &Font_7x10, 1);
				  SSD1306_GotoXY (80, 50);
				  SSD1306_Puts (msg3, &Font_7x10, 1);
				  SSD1306_DrawLine (0, 63, 129, 63, SSD1306_COLOR_WHITE);
				  SSD1306_UpdateScreen();
			break;

			case 1:
				  m1=(vot_bat*25.5/255);
				  m2=(vot_rede*255.0/255);
				  m3=(amp_rede*0.055);

				  sprintf(msg1, "%2.1fV", m1);
				  sprintf(msg2, "%2.2f", amp[amp_carga]);
				  //sprintf(msg2, "%dA", amp_carga);
				  sprintf(msg3, "%2.1fA ", m3);
				  sprintf(msg4, "%1.1fV", m2);

				  sprintf(dht11, "%02dc", temp);

				  SSD1306_GotoXY (35, 0);
				  SSD1306_Puts ("Sensores", &Font_7x10, 1);

				  SSD1306_DrawLine (0, 10, 129, 10, SSD1306_COLOR_WHITE);
				  SSD1306_GotoXY (5, 13);
				  SSD1306_Puts ("V Bateria", &Font_7x10, 1);
				  SSD1306_GotoXY (86, 13);
				  SSD1306_Puts (msg1, &Font_7x10, 1);
				  SSD1306_GotoXY (5, 23);
				  SSD1306_Puts ("A Bateria", &Font_7x10, 1);
				  SSD1306_GotoXY (86, 23);
				  SSD1306_Puts (msg2, &Font_7x10, 1);
				  SSD1306_GotoXY (5, 33);
				  SSD1306_Puts ("A Rede", &Font_7x10, 1);
				  SSD1306_GotoXY (86, 33);
				  SSD1306_Puts (msg3, &Font_7x10, 1);
				  SSD1306_GotoXY (5, 43);
				  SSD1306_Puts ("V Rede", &Font_7x10, 1);
				  SSD1306_GotoXY (86, 43);
				  SSD1306_Puts (msg4, &Font_7x10, 1);
				  SSD1306_GotoXY (5, 53);
				  SSD1306_Puts ("Temperatura", &Font_7x10, 1);
				  SSD1306_GotoXY (86, 53);
				  SSD1306_Puts (dht11, &Font_7x10, 1);
				  SSD1306_UpdateScreen();
			break;

			case 2:
				  sprintf(mm0, "%03d.", ip_l[0]);
				  sprintf(mm1, "%03d.", ip_l[1]);
				  sprintf(mm2, "%03d.", ip_l[2]);
				  sprintf(mm3, "%03d", ip_l[3]);

				  sprintf(kk0, "%03d.", mk_l[0]);
				  sprintf(kk1, "%03d.", mk_l[1]);
				  sprintf(kk2, "%03d.", mk_l[2]);
				  sprintf(kk3, "%03d", mk_l[3]);

				  sprintf(gg0, "%03d.", gw_l[0]);
				  sprintf(gg1, "%03d.", gw_l[1]);
				  sprintf(gg2, "%03d.", gw_l[2]);
				  sprintf(gg3, "%03d", gw_l[3]);

				  sprintf(pp0, "%d", port_l);
				  SSD1306_GotoXY (35, 0);
				  SSD1306_Puts ("Local IP", &Font_7x10, 1);
				  SSD1306_DrawLine (0, 10, 129, 10, SSD1306_COLOR_WHITE);

				  SSD1306_GotoXY (2, 15);
				  SSD1306_Puts ("IP", &Font_7x10, 1);
				  SSD1306_GotoXY (22, 15);
				  SSD1306_Puts (mm0, &Font_7x10, 1);
				  SSD1306_GotoXY (50, 15);
				  SSD1306_Puts (mm1, &Font_7x10, 1);
				  SSD1306_GotoXY (78, 15);
				  SSD1306_Puts (mm2, &Font_7x10, 1);
				  SSD1306_GotoXY (105, 15);
				  SSD1306_Puts (mm3, &Font_7x10, 1);

				  SSD1306_GotoXY (2, 27);
				  SSD1306_Puts ("MK", &Font_7x10, 1);
				  SSD1306_GotoXY (22, 27);
				  SSD1306_Puts (kk0, &Font_7x10, 1);
				  SSD1306_GotoXY (50, 27);
				  SSD1306_Puts (kk1, &Font_7x10, 1);
				  SSD1306_GotoXY (78, 27);
				  SSD1306_Puts (kk2, &Font_7x10, 1);
				  SSD1306_GotoXY (106, 27);
				  SSD1306_Puts (kk3, &Font_7x10, 1);

				  SSD1306_GotoXY (2, 39);
				  SSD1306_Puts ("GW", &Font_7x10, 1);
				  SSD1306_GotoXY (22, 39);
				  SSD1306_Puts (gg0, &Font_7x10, 1);
				  SSD1306_GotoXY (50, 39);
				  SSD1306_Puts (gg1, &Font_7x10, 1);
				  SSD1306_GotoXY (78, 39);
				  SSD1306_Puts (gg2, &Font_7x10, 1);
				  SSD1306_GotoXY (106, 39);
				  SSD1306_Puts (gg3, &Font_7x10, 1);

				  SSD1306_GotoXY (2, 51);
				  SSD1306_Puts ("PT", &Font_7x10, 1);
				  SSD1306_GotoXY (22, 51);
				  SSD1306_Puts (pp0, &Font_7x10, 1);

				  SSD1306_UpdateScreen();
			break;

			case 3:

				  sprintf(mm0, "%03d.", ip_s[0]);
				  sprintf(mm1, "%03d.", ip_s[1]);
				  sprintf(mm2, "%03d.", ip_s[2]);
				  sprintf(mm3, "%03d",  ip_s[3]);

				  if(ping_fail){mensage[0]='\0';sprintf(mensage, "     ERRO");}
				  else{         mensage[0]='\0';sprintf(mensage, "Conectado");}


				  sprintf(pp1, "%05d", port_s);

				  SSD1306_GotoXY (35, 0);
				  SSD1306_Puts ("Servidor", &Font_7x10, 1);
				  SSD1306_DrawLine (0, 10, 129, 10, SSD1306_COLOR_WHITE);

				  SSD1306_GotoXY (1, 15);
				  SSD1306_Puts ("IP", &Font_7x10, 1);
				  SSD1306_GotoXY (24, 15);
				  SSD1306_Puts (mm0, &Font_7x10, 1);
				  SSD1306_GotoXY (52, 15);
				  SSD1306_Puts (mm1, &Font_7x10, 1);
				  SSD1306_GotoXY (80, 15);
				  SSD1306_Puts (mm2, &Font_7x10, 1);
				  SSD1306_GotoXY (108, 15);
				  SSD1306_Puts (mm3, &Font_7x10, 1);


				  SSD1306_GotoXY (1, 27);
				  SSD1306_Puts ("STATUS", &Font_7x10, 1);
				  SSD1306_GotoXY (65, 27);
				  SSD1306_Puts (mensage, &Font_7x10, 1);

				  SSD1306_GotoXY (1, 39);
				  SSD1306_Puts (dominio, &Font_7x10, 1);

				  SSD1306_GotoXY (1, 51);
				  SSD1306_Puts ("PORTA ", &Font_7x10, 1);
				  SSD1306_GotoXY (93, 51);
				  SSD1306_Puts (pp1, &Font_7x10, 1);

				  SSD1306_UpdateScreen();
			break;

	  }
	  //HAL_Delay(500);
}



//***********************************************************************************************
//           init udp
//***********************************************************************************************
void init_udp() {
	UART_Printf("\r\ninit() called!\r\n");

    UART_Printf("Registering W5500 callbacks...\r\n");
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);

    UART_Printf("Calling wizchip_init()...\r\n");
    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2 , 2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);

    UART_Printf("Calling Static_ip_init()...\r\n");

     wiz_NetInfo net_info = {
     .mac  = { mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] },
     .ip = {ip_l[0],ip_l[1],ip_l[2],ip_l[3]},
     .sn ={mk_l[0],mk_l[1],mk_l[2],mk_l[3]},
     .gw = {gw_l[0],gw_l[1],gw_l[2],gw_l[3]}};

         setSHAR(net_info.mac);
         setSIPR(net_info.ip);
         setSUBR(net_info.sn);
         setGAR(net_info.gw);
         setSn_PORT(1,port_l);


    socket(1, Sn_MR_UDP, port_l, 1);
	//wizchip_setnetmode(NM_PINGBLOCK);

}
//***********************************************************************************************
//         Monta Pacote Status ($B000001001A000005A319922697934D#)
//***********************************************************************************************
void monta_status(void)
{
	char mensagem[40] = "";
	char mensagemcp[40] = "";
	char crc[4] = "";

	//concathex2(mensagem, energ);
	mensagem[0] = energ;
	concathex2(mensagem, vot_bat);
	concathex2(mensagem, amp_carga);
	concathex2(mensagem, amp_rede);
	concathex2(mensagem, vot_rede);
	concathex2(mensagem, temp);
	concathex2(mensagem, fan1_stat);
	concathex2(mensagem, fan2_stat);
	concathex4(mensagem, tp_ativo);
	concathex4(mensagem, idt);
	concathex4(mensagem, key);

	strcpy(mensagemcp, mensagem);
	docrc(crc, mensagemcp, strlen(mensagemcp));

	enviaUDP(mensagem, crc, strlen(mensagem));

}
//***********************************************************************************************
//         Monta Pacote IP_Local ($LC0A800C0C0A800026DFFFF00244C6DE109252386#)
//***********************************************************************************************
void monta_ipL(void)
{
	char mensagem[46] = "";
	char mensagemcp[46] = "";
	char crc[4] = "";
	uint8_t DATA_4[4]={255,255,255,255};
	int DATA_2 = 9999;
	int vl1;
	int vl2;

	strncat(mensagem, "L", 1);

	DATA_4[0] = read_eeprom(ADD_IPL1);
	DATA_4[1] = read_eeprom(ADD_IPL2);
	DATA_4[2] = read_eeprom(ADD_IPL3);
	DATA_4[3] = read_eeprom(ADD_IPL4);

    for (x=0; x<4; x++){
    	concathex2(mensagem, DATA_4[x]);
    }

    DATA_4[0] = read_eeprom(ADD_GW1);
    DATA_4[1] = read_eeprom(ADD_GW2);
    DATA_4[2] = read_eeprom(ADD_GW3);
    DATA_4[3] = read_eeprom(ADD_GW4);

	for (x=0; x<4; x++){
    	concathex2(mensagem, DATA_4[x]);
    }

	DATA_4[0] = read_eeprom(ADD_MK1);
	DATA_4[1] = read_eeprom(ADD_MK2);
	DATA_4[2] = read_eeprom(ADD_MK3);
	DATA_4[3] = read_eeprom(ADD_MK4);

	for (x=0; x<4; x++){
    	concathex2(mensagem, DATA_4[x]);
    }

	vl1 = read_eeprom(ADD_PORTL1);
	vl2 = read_eeprom(ADD_PORTL2);
	DATA_2 = (vl1 * 256) + vl2;
	concathex4(mensagem, DATA_2);

	vl1 = read_eeprom(ADD_IDT1);
	vl2 = read_eeprom(ADD_IDT2);
	DATA_2 = (vl1 * 256) + vl2;
	concathex4(mensagem, DATA_2);

	vl1 = read_eeprom(ADD_KEY1);
	vl2 = read_eeprom(ADD_KEY2);
	DATA_2 = (vl1 * 256) + vl2;
    concathex4(mensagem, DATA_2);

    strcpy(mensagemcp, mensagem);
	docrc(crc, mensagemcp, strlen(mensagemcp));

    enviaUDP(mensagem, crc, strlen(mensagem));

}
//***********************************************************************************************
//         Monta Pacote IP_Servidor ($SD07244B01010101upscba.selprom.com.br4A49#)
//									($SI05244B01010101C0A8001DBDE9#)
//***********************************************************************************************
void monta_ipS(void)
{
	char mensagem[60] = "";
	char mensagemcp[60] = "";
	char crc[4] = "";
	uint8_t DATA_4[4]={255,255,255,255};
	int DATA_2 = 9999;
	int vl1;
	int vl2;

	strncat(mensagem, "S", 1);

	if(isdom){
		strncat(mensagem, "D", 1);

		DATA_2 = read_eeprom(ADD_TIMEOUT);
		concathex2(mensagem, DATA_2);

		vl1 = read_eeprom(ADD_PORTS1);
		vl2 = read_eeprom(ADD_PORTS2);
		DATA_2 = (vl1 * 256) + vl2;
		concathex4(mensagem, DATA_2);

		DATA_4[0] = read_eeprom(ADD_DNS1);
		DATA_4[1] = read_eeprom(ADD_DNS2);
		DATA_4[2] = read_eeprom(ADD_DNS3);
		DATA_4[3] = read_eeprom(ADD_DNS4);
		for (x=0; x<4; x++){
			concathex2(mensagem, DATA_4[x]);
		}

		tam_dominio = read_eeprom(ADD_TDOM);
		int j;
		int i = 0;
		char dat[60] = "";
		for(i; i<tam_dominio; i++){
			dat[i] = read_eeprom(adominio[i]);
			strncat(mensagem, &dat[i], 1);
		}
	}else{
		strncat(mensagem, "I", 1);

		DATA_2 = read_eeprom(ADD_TIMEOUT);
		concathex2(mensagem, DATA_2);

		vl1 = read_eeprom(ADD_PORTS1);
		vl2 = read_eeprom(ADD_PORTS2);
		DATA_2 = (vl1 * 256) + vl2;
		concathex4(mensagem, DATA_2);

		DATA_4[0] = read_eeprom(ADD_DNS1);
		DATA_4[1] = read_eeprom(ADD_DNS2);
		DATA_4[2] = read_eeprom(ADD_DNS3);
		DATA_4[3] = read_eeprom(ADD_DNS4);
		for (x=0; x<4; x++){
			concathex2(mensagem, DATA_4[x]);
		}

		DATA_4[0] = read_eeprom(ADD_IPS1);
		DATA_4[1] = read_eeprom(ADD_IPS2);
		DATA_4[2] = read_eeprom(ADD_IPS3);
		DATA_4[3] = read_eeprom(ADD_IPS4);
		for (x=0; x<4; x++){
			concathex2(mensagem, DATA_4[x]);
		}
	}

    strcpy(mensagemcp, mensagem);
	docrc(crc, mensagemcp, strlen(mensagemcp));

    enviaUDP(mensagem, crc, strlen(mensagem));

}
//***********************************************************************************************
//         Monta Pacote Info ($0x203038463336509B2B#)
//***********************************************************************************************
void monta_info(void)
{
	char mensagem[30] = "";
	char crc[4] = "";

	strcpy(mensagem, aShowLotNumber);
	docrc(crc, mensagem, strlen(mensagem));

	enviaUDP(aShowLotNumber, crc, strlen(aShowLotNumber));
}

//***********************************************************************************************
//         Monta Pacote TESTE ($R07D205E6C0A80051C0A80002244C4414#)
//***********************************************************************************************
void monta_teste(void)
{

	modo_prog = 1;
	char mensage[60] = "";
	char mensagecp[60] = "";
	char crc[4] = "";
	uint8_t DATA_4[4]={255,255,255,255};
	int DATA_2 = 9999;
	int vl1;
	int vl2;

	mensage[0] = energ;
	concathex4(mensage, idt);
	concathex4(mensage, key);
	DATA_4[0] = read_eeprom(ADD_IPL1);
	DATA_4[1] = read_eeprom(ADD_IPL2);
	DATA_4[2] = read_eeprom(ADD_IPL3);
	DATA_4[3] = read_eeprom(ADD_IPL4);

    for (x=0; x<4; x++){
    	concathex2(mensage, DATA_4[x]);
    }

    DATA_4[0] = read_eeprom(ADD_GW1);
    DATA_4[1] = read_eeprom(ADD_GW2);
    DATA_4[2] = read_eeprom(ADD_GW3);
    DATA_4[3] = read_eeprom(ADD_GW4);

	for (x=0; x<4; x++){
    	concathex2(mensage, DATA_4[x]);
    }

	vl1 = read_eeprom(ADD_PORTL1);
	vl2 = read_eeprom(ADD_PORTL2);
	DATA_2 = (vl1 * 256) + vl2;
	concathex4(mensage, DATA_2);

    strcpy(mensagecp, mensage);
	docrc(crc, mensagecp, strlen(mensagecp));

	char pacotePronto[100] = "";
	sprintf(pacotePronto, "$%s%s#", mensage, crc);
	sendto(1,pacotePronto,strlen(pacotePronto),ip_R,9292);
	modo_prog = 0;


}

//***********************************************************************************************
//         Verifica mensagem recebida
//***********************************************************************************************
void tratapergunta(uint8_t * buf){
	if(!strcmp(buf, "?")){
		monta_status();
	}
	if(!strcmp(buf, "?L")){
		monta_ipL();
	}
	if(!strcmp(buf, "?S")){
		monta_ipS();
	}
	if(!strcmp(buf, "?I")){
		monta_info();
	}
	if(!strcmp(buf, "?T")){
		monta_teste();
	}

}

void trataconfig(char * buf, int len){
	int tam = len-2;
	int i = 0;
	int a = 0;

	uint8_t DATA_4[4]={255,255,255,255};
	uint16_t DATA_2 = 9999;
	//sendto(1,buf,strlen(buf),ip_s,port_s);

	switch(buf[0]){
	case 'L':
		if(tam != 37){
			return;
		}
		//ip
		i = 1;
		a = 2;
		for(x=0; x<4; x++){
			DATA_4[x] = hextochar('0', '0', buf[i], buf[a]);
			i = i+2;
			a = a+2;
		}
		write_eeprom(ADD_IPL1,DATA_4[0]);
		write_eeprom(ADD_IPL2,DATA_4[1]);
		write_eeprom(ADD_IPL3,DATA_4[2]);
		write_eeprom(ADD_IPL4,DATA_4[3]);
		//gateway
		i = 9;
		a = 10;
		for(x=0; x<4; x++){
			DATA_4[x] = hextochar('0', '0', buf[i], buf[a]);
			i = i+2;
			a = a+2;
		}
		write_eeprom(ADD_GW1,DATA_4[0]);
		write_eeprom(ADD_GW2,DATA_4[1]);
		write_eeprom(ADD_GW3,DATA_4[2]);
		write_eeprom(ADD_GW4,DATA_4[3]);
		//netmask
		i = 17;
		a = 18;
		for(x=0; x<4; x++){
			DATA_4[x] = hextochar('0', '0', buf[i], buf[a]);
			i = i+2;
			a = a+2;
		}
		write_eeprom(ADD_MK1,DATA_4[0]);
		write_eeprom(ADD_MK2,DATA_4[1]);
		write_eeprom(ADD_MK3,DATA_4[2]);
		write_eeprom(ADD_MK4,DATA_4[3]);
		//port
		DATA_2 = hextochar(buf[25], buf[26], buf[27], buf[28]);
		write_eeprom(ADD_PORTL1, DATA_2/256);
		write_eeprom(ADD_PORTL2, DATA_2%256);
		//idt
		DATA_2 = hextochar(buf[29], buf[30], buf[31], buf[32]);
		write_eeprom(ADD_IDT1, DATA_2/256);
		write_eeprom(ADD_IDT2, DATA_2%256);
		//key
		DATA_2 = hextochar(buf[33], buf[34], buf[35], buf[36]);
		write_eeprom(ADD_KEY1, DATA_2/256);
		write_eeprom(ADD_KEY2, DATA_2%256);

	    ledvd_on;
		ledvm_off;
		HAL_Delay(500);
		ledvd_off;
		ledvm_off;

	break;
	case 'S':
		if (buf[1] == 'D'){
			//TIMEOUT
			DATA_2 = hextochar('0', '0', buf[2], buf[3]);
			write_eeprom(ADD_TIMEOUT, DATA_2);

			//port
			DATA_2 = hextochar(buf[4], buf[5], buf[6], buf[7]);
			write_eeprom(ADD_PORTS1, DATA_2/256);
			write_eeprom(ADD_PORTS2, DATA_2%256);
			//DNS SERVER
			i = 8;
			a = 9;
			for(x=0; x<4; x++){
				DATA_4[x] = hextochar('0', '0', buf[i], buf[a]);
				i = i+2;
				a = a+2;
			}
			write_eeprom(ADD_DNS1,DATA_4[0]);
			write_eeprom(ADD_DNS2,DATA_4[1]);
			write_eeprom(ADD_DNS3,DATA_4[2]);
			write_eeprom(ADD_DNS4,DATA_4[3]);
			//DOMINIO
			int j;
			i = 0;
			for(j=16; j<tam; j++){
				write_eeprom(adominio[i],buf[j]);
				i++;
			}
			tam_dominio = i;
			write_eeprom(ADD_TDOM, tam_dominio);

			write_eeprom(ADD_ISD, 1);
			isdom = 1;

			ledvd_off;
			ledvm_on;
			HAL_Delay(500);
			ledvd_off;
			ledvm_off;
		}else if (buf[1] == 'I'){
			//TIMEOUT
			DATA_2 = hextochar('0', '0', buf[2], buf[3]);
			write_eeprom(ADD_TIMEOUT, DATA_2);

			//port
			DATA_2 = hextochar(buf[4], buf[5], buf[6], buf[7]);
			write_eeprom(ADD_PORTS1, DATA_2/256);
			write_eeprom(ADD_PORTS2, DATA_2%256);
			//DNS SERVER
			i = 8;
			a = 9;
			for(x=0; x<4; x++){
				DATA_4[x] = hextochar('0', '0', buf[i], buf[a]);
				i = i+2;
				a = a+2;
			}
			write_eeprom(ADD_DNS1,DATA_4[0]);
			write_eeprom(ADD_DNS2,DATA_4[1]);
			write_eeprom(ADD_DNS3,DATA_4[2]);
			write_eeprom(ADD_DNS4,DATA_4[3]);

			//IP S1
			i = 16;
			a = 17;
			for(x=0; x<4; x++){
				DATA_4[x] = hextochar('0', '0', buf[i], buf[a]);
				i = i+2;
				a = a+2;
			}
			write_eeprom(ADD_IPS1,DATA_4[0]);
			write_eeprom(ADD_IPS2,DATA_4[1]);
			write_eeprom(ADD_IPS3,DATA_4[2]);
			write_eeprom(ADD_IPS4,DATA_4[3]);

			write_eeprom(ADD_ISD, 0);
			isdom = 0;

			ledvd_off;
			ledvm_on;
			HAL_Delay(500);
			ledvd_off;
			ledvm_off;
		}

	break;
	default:
		sendto(1,"erro",4,ip_s,port_s);
	break;
	}

}

void tratateste(uint8_t * buf)
{
	if(!modo_teste)
	{
		uint8_t len = strlen(buf);

		uint8_t i = 4;
		char mensagem[30];
		char tempohex[4];
		uint16_t tempodec = 0;
		uint8_t pcigorfoda[4] = {192,168,0,29};

		for(uint8_t x=0; x<len-5; x++){
			mensagem[x] = buf[x+1];
		}

		if(!strcmp(mensagem, "62617465726961"))
		{
			for(uint8_t x=0;x<4;x++){
				tempohex[x] = buf[len-i];
				i--;
			}

			tempodec = hextochar(tempohex[0], tempohex[1], tempohex[2], tempohex[3]);
			modo_teste_tempo = tempodec;
			modo_teste = 1;
		}
	}
}


//***********************************************************************************************
//          Lê a eeprom e atribui os valores nas variáveis
//***********************************************************************************************

void le_eep(void)
{
	  int vl1;
	  int vl2;

	  ip_l[0] = read_eeprom(ADD_IPL1);
	  ip_l[1] = read_eeprom(ADD_IPL2);
	  ip_l[2] = read_eeprom(ADD_IPL3);
	  ip_l[3] = read_eeprom(ADD_IPL4);

	  gw_l[0] = read_eeprom(ADD_GW1);
	  gw_l[1] = read_eeprom(ADD_GW2);
	  gw_l[2] = read_eeprom(ADD_GW3);
	  gw_l[3] = read_eeprom(ADD_GW4);

	  mk_l[0] = read_eeprom(ADD_MK1);
	  mk_l[1] = read_eeprom(ADD_MK2);
	  mk_l[2] = read_eeprom(ADD_MK3);
	  mk_l[3] = read_eeprom(ADD_MK4);

	  ip_s[0] = read_eeprom(ADD_IPS1);
	  ip_s[1] = read_eeprom(ADD_IPS2);
	  ip_s[2] = read_eeprom(ADD_IPS3);
	  ip_s[3] = read_eeprom(ADD_IPS4);

	  dns_server[0] = read_eeprom(ADD_DNS1);
	  dns_server[1] = read_eeprom(ADD_DNS2);
	  dns_server[2] = read_eeprom(ADD_DNS3);
	  dns_server[3] = read_eeprom(ADD_DNS4);

	  vl1 = read_eeprom(ADD_PORTL1);
	  vl2 = read_eeprom(ADD_PORTL2);
	  port_l = (vl1 * 256) + vl2;

	  vl1 = read_eeprom(ADD_PORTS1);
	  vl2 = read_eeprom(ADD_PORTS2);
	  port_s = (vl1 * 256) + vl2;

	  tam_dominio = read_eeprom(ADD_TDOM);

	  vl1 = read_eeprom(ADD_IDT1);
	  vl2 = read_eeprom(ADD_IDT2);
	  idt = (vl1 * 256) + vl2;

	  vl1 = read_eeprom(ADD_KEY1);
	  vl2 = read_eeprom(ADD_KEY2);
	  key = (vl1 * 256) + vl2;

	  isdom = read_eeprom(ADD_ISD);
	  timeOutLen = read_eeprom(ADD_TIMEOUT);
}

//***********************************************************************************************
//          Pega o número de serie
//***********************************************************************************************
void GetMCUInfo(void)
{
  register uint32_t size_string = 0, read_info = 0, read_info2 = 0;


  read_info = LL_GetUID_Word1();
  read_info2 = LL_GetUID_Word2();
  sprintf((char*)aShowWaferNumber,"Wafer NB = 0x%X", (uint8_t)read_info);


  size_string = sprintf((char*)aShowLotNumber,"0x%lX", read_info2);

  sprintf((char*)aShowLotNumber+size_string,"%lX", (read_info >> 8));
}

void trataDNS(void)
{

	  	  uint8_t ret;
	  	  uint8_t addr[4];
		  DNS_init(DNS_SOCKET, dns_buffer);
		  int j;
		  int i = 0;
		  for(i; i<tam_dominio; i++){
			  dominio[i] = read_eeprom(adominio[i]);
		  }

		  if(dnscont < dnstimeout){
			  if ((ret = DNS_run(dns_server, (uint8_t*)&dominio, addr)) > 0)
			  {
				  if (ret == 1){
					  ip_s[0] = addr[0];
					  ip_s[1] = addr[1];
					  ip_s[2] = addr[2];
					  ip_s[3] = addr[3];
					  dominio[0] = '\0';
					  char tempo[20];
					  sprintf(tempo, "Time out      %03ds", timeOutLen);
					  strcat(dominio, tempo);
					  isdom = 1;
					  tryDNS = 0;
				  }
			  }
			  init_udp();
		  }else if(dnscont >= dnstimeout){
			  tryDNS = 0;
		  }
		  dnscont++;
}

void reboot_wiz(void)
{
    ledvm_on;
    ledvd_on;
	wiz_off;
	close(0);
	close(1);
	HAL_Delay(200);
	wiz_on;
	MX_SPI1_Init();
	init_udp();
	IINCHIP_WRITE(Sn_PROTO(0), IPPROTO_ICMP);
	ping_fail = 1;
    ledvm_on;
    ledvd_on;
}

uint8_t ping_count(uint8_t s, uint16_t pCount, uint8_t *addr){

	uint16_t rlen, cnt,i;
	cnt = 0;
	uint8_t req=0;
	uint8_t rep=0;
	ping_reply_received = 0;

	for(i=0; i<pCount+1;i++){

	if(i!=0){
		/* Output count number */
			printf( "\r\nNo.%d\r\n",   (i-1));
	}

  	switch(getSn_SR(s))
		{
			case SOCK_CLOSED:
				close(s);

				/* Create Socket */
				IINCHIP_WRITE(Sn_PROTO(s), IPPROTO_ICMP);
				if(socket(s,Sn_MR_IPRAW,3000,0)!=s){
					printf( "\r\n socket %d fail r\n",   (s)) ;
#ifdef PING_DEBUG
					return SOCKET_ERROR;
#endif
				}
				while(getSn_SR(s)!=SOCK_IPRAW);

				HAL_Delay(500); // wait 1000ms
				break;

			case SOCK_IPRAW:
				 ping_request(s, addr);
				 req++;
				while(1){
						if ( (rlen = getSn_RX_RSR(s) ) > 0){
							ping_reply(s, addr, rlen);
							rep++;
							if (ping_reply_received && !thrash)  break;


						}

						if ( (cnt > 100) ) {
							printf( "\r\nRequest Time out. \r\n") ;
							cnt = 0;
							break;
						}else {
							cnt++;
							HAL_Delay(50);
						}
				     }

				break;

			default:
				break;

       }
#ifdef PING_DEBUG
  		if(req>=pCount)
  		   	{
  		  		printf("Ping Request = %d, PING_Reply = %d\r\n",req,rep);

  		  		if(rep == req){
  		  			return SUCCESS;
  		  		}
  		  		else{
  		  			return REPLY_ERROR;
  		  		}

  		  	}
#endif
   }


}

uint8_t ping_request(uint8_t s, uint8_t *addr){
    uint16_t i;

	//ping_reply_received = 0;
	PingRequest.Type = PING_REQUEST;
	PingRequest.Code = CODE_ZERO;
	PingRequest.ID = htons(RandomID++);
	PingRequest.SeqNum =htons(RandomSeqNum++);

  	for(i = 0 ; i < BUF_LEN; i++){
		PingRequest.Data[i] = (i) % 8;
	}

	PingRequest.CheckSum = 0;
	PingRequest.CheckSum = htons(checksum((uint8_t*)&PingRequest,sizeof(PingRequest)));

	if(sendto(s,(uint8_t *)&PingRequest,sizeof(PingRequest),addr,3000)==0){
	  	 printf( "\r\n Fail to send ping-reply packet  r\n") ;
	}else{
	 	  printf( "Send Ping Request  to Destination (") ;
          printf( "%d.%d.%d.%d )",   (addr[0]),  (addr[1]),  (addr[2]),  (addr[3])) ;
		  printf( " ID:%x  SeqNum:%x CheckSum:%x\r\n",   htons(PingRequest.ID),  htons(PingRequest.SeqNum),  htons(PingRequest.CheckSum)) ;
	}
	return 0;
}

uint8_t ping_reply(uint8_t s, uint8_t *addr,  uint16_t rlen){

	 uint16_t tmp_checksum;
	 uint16_t len;
	 uint16_t i;
	 uint8_t data_buf[128];
	 uint16_t port = 3000;
	 PINGMSGR PingReply;
	 //ping_reply_received = 0;

		  	len = recvfrom(s, (uint8_t *)data_buf,rlen,ip_R,&port);
		  		if(data_buf[0] == PING_REPLY) {
					PingReply.Type 		 = data_buf[0];
					PingReply.Code 		 = data_buf[1];
					PingReply.CheckSum   = (data_buf[3]<<8) + data_buf[2];
					PingReply.ID 		 = (data_buf[5]<<8) + data_buf[4];
					PingReply.SeqNum 	 = (data_buf[7]<<8) + data_buf[6];

					for(i=0; i<len-8 ; i++)
					{
						PingReply.Data[i] = data_buf[8+i];
					}
					tmp_checksum = ~checksum(&data_buf,len);
					if(tmp_checksum != 0xffff){
						printf("tmp_checksum = %x\r\n",tmp_checksum);
						ping_reply_received = 0;
					}
					if(ip_R != ip_s)
					{
						ping_reply_received = 0;
						thrash = 1;
					}
					else{
						ping_reply_received = 1;
						thrash = 0;
					}
				}


			return 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  //uint8_t ping = 0;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_CRC_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ad, 4);

  dht();
  HAL_UART_Receive_IT(&huart1, (uint8_t *)UART1_rxBuffer, 40);

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  GetMCUInfo();

  ledvd_off;
  ledvm_off;

  sprintf(msg3, "V.%2.1fA ", version);

  SSD1306_Init();
  SSD1306_GotoXY (0,15);
  SSD1306_Puts (" NOBREAK ", &Font_11x18, 1);
  SSD1306_GotoXY (10, 38);
  SSD1306_Puts ("SELPROM", &Font_11x18, 1);
  SSD1306_GotoXY (93, 53);
  SSD1306_Puts (msg3, &Font_7x10, 1);
  SSD1306_UpdateScreen();

  HAL_Delay(200);

  TIM3->CCR1 = 0;
  TIM3->CCR2 = 0;
  TIM3->CCR4 = 0;

  HAL_Delay (1000);

  TIM3->CCR1 = 0;
  TIM3->CCR2 = 0;
  TIM3->CCR4 = 0;

  timeOut = timeOutLen;
  if(!b2){
	  modo_prog = 1;
	  HAL_Delay(500);
	  init_udp();
	  isdom = read_eeprom(ADD_ISD);
  }else {
	  le_eep();
	  modo_prog = 0;
	  init_udp();
	  //if(isdom){tryDNS=1;}
	  dominio[0] = '\0';
	  char tempo[20];
	  sprintf(tempo, "Time out      %03ds", timeOutLen);
	  strcat(dominio, tempo);
  }

  SSD1306_Clear();

  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim14);
  HAL_Delay(1000);
  curtoci = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //Eniva o pacote para o servidor;
	  if (send_to_server && !ping_fail){
		  monta_status();
		  send_to_server = 0;
	  }

	  //Verifica a conecção com o servidor;
	  if(!modo_prog){
		  if(pinga){
			  pinga = 0;
			  ping_reply_received = 0;
			  IINCHIP_WRITE(Sn_PROTO(0), IPPROTO_ICMP); //Muda o W5500 para modo ICMP;
			  uint8_t ping = ping_count(0, 3, &ip_s); //Tenta o ping 3x;
			  if(ping == SUCCESS){ //Se conseguir, volta para modo IP;
				  ping_fail = 0;
				  IINCHIP_WRITE(Sn_PROTO(0), IPPROTO_IP);
			  }else{ //Se falhar, reinicia o W5500;
				  ping_fail = 1;
				  reboot_wiz();
			  }
			  pinga = 0;
		  }
	  }

	  if(!fan1){
		  fan1_cont = 3;
	  }
	  if(!fan2){
	  	  fan2_cont = 3;
	  }

	  if(prot_curto)
	  {
		  curtoci = 1;
		  statusTELA = 3;
	  }else
	  {
		  if(energ == 'B' && modo_teste)
		  {
			  statusTELA = 1;
		  }else
		  if(energ == 'B')
		  {
			  statusTELA = 4;
		  }
		  if(energ == 'R')
		  {
	  		  statusTELA = 2;
		  }
	  }

	  //Recebe Pacotes UDP;
	  		if(getSn_IR(1) & Sn_IR_RECV)
		      {
		  	  setSn_IR(1, Sn_IR_RECV);
			  }
			if((len=getSn_RX_RSR(1))>0)
			  {
			 memset(msg5,1,len+1);
			 len = recvfrom(1,msg5, len, ip_R,&port_R);
			 if(len < 50){

			 char mensagem[60] = "";
			 char mensagemcp[60] = "";
			 uint8_t crc[4] = "";
			 uint8_t ccrc[4] = "";
			 int i;
			 for(x=0; x<len-6; x++){
				 mensagem[x] = msg5[x+1];
			 }
			 i = 5;
			 for(x=0;x<4;x++){
				 ccrc[x] = msg5[len-i];
				 i--;
			 }

			 strcpy(mensagemcp, mensagem);
			 docrc(crc, mensagemcp, strlen(mensagemcp));


			 sendto(1,crc,4,ip_s,port_s);
			 sendto(1,ccrc,4,ip_s,port_s);

			 if(crc[0] == ccrc[0] && crc[1] == ccrc[1] && crc[2] == ccrc[2] && crc[3] == ccrc[3]){
				 if(msg5[0] == '$' && msg5[len-1] == '#'){
					 if(msg5[1] == '?'){
						 tratapergunta(mensagem);
					 }
					 else if(msg5[1] == '%'){
						 tratateste(mensagem);
					 }
					 else if(modo_prog){
						 trataconfig(mensagem, len-4);
					 }
				 }
			 }

			 }
		   }


  switch(statusTELA){
  case 1:
	  energia[0]=' ';
	  energia[1]=' ';
	  energia[2]=' ';
	  energia[3]='T';
	  energia[4]='E';
	  energia[5]='S';
	  energia[6]='T';
	  energia[7]='E';
	 break;
  case 2:
  	  energia[0]=' ';
  	  energia[1]=' ';
  	  energia[2]=' ';
  	  energia[3]=' ';
  	  energia[4]='r';
  	  energia[5]='e';
  	  energia[6]='d';
  	  energia[7]='e';
  	 break;
  case 3:
	  energia[0]=' ';
	  energia[1]=' ';
	  energia[2]=' ';
	  energia[3]='C';
	  energia[4]='U';
	  energia[5]='R';
	  energia[6]='T';
	  energia[7]='O';
	 break;
  case 4:
  	  energia[0]='i';
  	  energia[1]='n';
  	  energia[2]='v';
  	  energia[3]='e';
  	  energia[4]='r';
  	  energia[5]='s';
  	  energia[6]='o';
  	  energia[7]='r';
  	 break;
  }

  trata_fan1();
  dht();
  trata_fan2();

  }

}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//************************************************************************************************
//      timer  100ms
//************************************************************************************************

void TIM6_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim6);

  trata_carga();

  if(modo_prog){
	  if(opt == 0){
		  rele_on;
		  inv_on;
		  rede=0;
		  energ = 'B';
		  carga=0;
		  volta=5;
	      }

	  if(opt == 1 && (volta == 0)){
		  rele_off;
	      inv_off;
	      rede=1;
	      energ = 'R';
	      carga=1;
	      }


  }else{
	  if(curtoci)
	  {
			inv_off;
	  		rele_off;
	  	    rede=1;
	  	    energ = 'F';
	  	    carga=1;
	  	    ledvm_off;
	  	    ledvd_on;
	  	    modo_teste = 0;
	  	    modo_teste_cont = 0;
	  }else
	  {
		  if(opt == 0 && (!dead_bat) || modo_teste && (vot_bat > 105)){
		  		  if(vot_bat > 105)
		  		  {
		  			  rele_on;
		  			  inv_on;
		  			  ledvm_on;
		  			  ledvd_off;
		  			  rede=0;
		  			  energ = 'B';
		  			  carga=0;
		  			  volta=5;
		  		  }else
		  		  {
		  			  dead_bat = 1;
		  			  inv_off;
		  			  rele_off;
		  			  ledvm_off;
		  			  ledvd_on;
		  			  rede=1;
		  			  energ = 'F';
		  			  carga=1;
		  		  }

		  	   }

		  	  if(opt == 1 && (volta == 0)){
		  		  rele_off;
		  	      inv_off;
		  	      rede=1;
		  	      energ = 'R';
		  	      carga=1;
		  	      ledvm_off;
		  	      ledvd_on;
		  	      modo_teste = 0;
		  	      modo_teste_cont = 0;
		  	      }

	  }
}

}

//************************************************************************************************
//      timer  1000ms
//************************************************************************************************

void TIM7_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim7);

  if(fan1_cont > 0){fan1_cont--;}
  if(fan2_cont > 0){fan2_cont--;}


    if(modo_teste)
    {
	    modo_teste_cont++;
	    if(modo_teste_cont > modo_teste_tempo)
	    {
		    modo_teste = 0;
		    modo_teste_cont = 0;
	    }
    }

  	cont_ping++;
	if(cont_ping >= 60)
	{
		cont_ping = 0;
		pinga = 1;
		mensage[0] = '\0';
	}

      if(opt == 1 && (volta > 0)){
    	volta--;
        }
      if(rede==0){
    	tp_ativo = tp_ativo+1;
        }

      //Se deve enviar o pacote ao servidor (>0);
      if(!modo_prog){
          if(timeOutLen != 0){
        	  if(timeOut == 0){
				timeOut = timeOutLen;
        		send_to_server = 1;
        	  }
        	  timeOut--;
          }
      }
      vot_bat =ad[0];
      amp_rede=ad[2];
      vot_rede=ad[3];
      telas();

      if(dead_bat)
      {
    	  cont_dead++;
    	  if(cont_dead >= 65000 || (vot_bat >= 130))
    	  {
    		  dead_bat = 0;
    	  }
      }

}

//************************************************************************************************
//      timer  1000ms  (cuida dos botões)
//************************************************************************************************

void TIM14_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim14);

	while(!b1) //esquerdo;
  	{
  		  if(filtro != 0)
  		  {
  			  filtro --;
  			  if(filtro == 0)
  			  {
  				  if(tela < 4)
  				  {
  					  tela++;
  					  if(tela>3){tela=0;}
  					  SSD1306_Clear();
  				  }
  			  }
  		  }
  	  }

	  while(!b2) //direito;
	  {
	  		  if(filtro != 0)
	  		  {
	  			  filtro --;
	  			  if(filtro == 0)
	  			  {
	  				  if(tela > 0)
	  				  {
	  					  tela--;
	  					  SSD1306_Clear();
	  				  }else if(tela == 0)
	  				  {
	  					  tela = 3;
	  					  SSD1306_Clear();
	  				  }
	  	          }
	           }
	  }

  	  filtro = Tfiltro;
}


//************************************************************************************************
//      usart1 RX
//************************************************************************************************
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);

	data = huart1.Instance->RDR;

  if (data=='@')
    {
     ser_ptm=0;
     ser_rcm=1;
     ser_pcm=0;
    }
 else if (data=='#')
    {
     if (ser_rcm)
        {
  	  if (ser_ptm==8) ser_pcm=1;
        };
        ser_rcm=0;
        ser_ptm=0;
        }
     else
        {
        if (ser_rcm) {ser_bfm[ser_ptm++]= data; if (ser_ptm>10) {ser_ptm=0; ser_rcm=0;}};
        }

}
  /* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV9;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLADC;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
  hcrc.Init.GeneratingPolynomial = 0x1021;
  hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
  hcrc.Init.InitValue = 0xFFFF;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00602173;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 5;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 99;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 63999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 63999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 99;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 63999;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(wiz_reset_GPIO_Port, wiz_reset_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB9 PB2 PB3 PB4
                           PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB7 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : wiz_reset_Pin */
  GPIO_InitStruct.Pin = wiz_reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(wiz_reset_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
