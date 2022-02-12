//#define CEMITERIO VIDA atual
#define KRAUSE "KRAUSE"
#define VIDA_JUAZEIRO "VIDA_JUAZEIRO"
#define CEMITERIO VIDA_JUAZEIRO
#define ATRASO_PORTA_ESTEIRA 30000
#define tempo_abertura_porta 60000
#define tempo_recolhimento_mesa 38000
//#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_RX_BUFFER_SIZE 1024
bool etapa_01 = false;
bool etapa_02 = false;
bool etapa_03 = false;
bool etapa_04 = false;
bool etapa_05 = false;
bool etapa_06 = false;
bool etapa_07 = false;
bool efeito_rgb_cabine_1 = false;
bool efeito_rgb_cabine_2 = false;
bool efeito_rgb_cabine_3 = false;
bool mem_cena = true;
bool enable_pulsa = false;
bool completa = false;
bool expoemesa = false;
bool abreporta = false;
bool fechaporta = false;
bool guardamesa = false;
bool acionado = false;
bool video_encerrado = true;
bool pressionado = false;
bool porta_aberta = false;
bool porta_fechada = false;
bool mesa_exposta = false;
bool mesa_guardada = false;


bool verifica_posicao = false;
byte acionamento_sensor = 0;
#include "dim.h"
#include "led_pwm_pin.h"
#include <EEPROM.h>
#include <RBD_Button.h>
#include <RBD_Timer.h>
#define debounce_time 50
#define NAO_ATUADO HIGH
#define ATUADO LOW
unsigned long currentMillis;
unsigned long currentMillis_acionamento;
String DataIn = "";
bool debug = false;
bool fade_lamp = false;
byte conta_red = 0;
byte conta_blue = 3;
byte conta_green = 0;
bool entrega = false;
bool en_esteira = false;
bool en_rgb_geral = false;
bool efeito_1 = false;

byte button_counter = 0;

// DELAYS DURANTE A CERIMONIA

#define timeFumacaOff 10000
#define timeFumacaOn 500

//PINO BOTÃO
#define COMPLETA A15// alterado
//#define COMPLETA 33
#define SIMPLES 32
boolean muda = false;

//SENSORES FIM DE CURSO
#define SENSOR_ME 36
#define SENSOR_MG 37
#define SENSOR_PA 35
#define SENSOR_PF 34

//PINOS CABINE
#define FUMACA 38
#define LUZ_CABINE 39
#include <TimerOne.h>

//PINOS MOTOR DC PORTA
#define PINO_ABRE 52 // PINOO MOTOR DC
#define PINO_FECHA 51 // PINO MOTOR DC
#define R_EN_PORTA  14
#define L_EN_PORTA  14
bool loop_porta = false;
byte PWM_FECHA_PORTA = 255;
#define PWM_ABRE_PORTA 255

//PINOS MOTOR DC MESA
#define PINO_GUARDA 50 // PINOO MOTOR DC
#define PINO_EXPOE 49 // PINO MOTOR DC
#define R_EN_MESA  15
#define L_EN_MESA  15
#define PWM_GUARDA_MESA 255
#define PWM_EXPOE_MESA 255

/*CONTROLES DA ROTAÇÃO DO MOTOR PORTA*/
#define tempo_rampa_motor_dc 10
byte diferencaAbreFecha = 35; // - 30 NO CASO DE A PORTA ESTAR ABRINDO
byte velocidade_minima_dc = 50;
byte velocidade_maxima_dc = 100; //tirei 10
#define timeout_porta 90000  //tempo até diminuir a velocidade da mesa
#define timeout_total_porta 80000 //duração total do ciclo de abertura e fechamento da porta

//PINOS MOTOR DC ESTEIRA
byte PWM_ESTEIRA = 0;
#define PINO_LIGA_ESTEIRA 17
#define duracaoEsteira 55000
#define velocidade_esteira 25
#define tempo_inicio_esteira 5000

byte r,g,b;
//PINOS CONTROLE CORTINA
bool b_c = false;
bool s_c = false;
#define sobe_cortina 39
#define baixa_cortina 40
/*CONTROLES DA ROTAÇÃO DO MOTOR DC*/

#define timeout_mesa 60000
#define timeout_total_mesa 60000

unsigned long previousMillis[] = {0, 0, 0, 0, 0};


RBD::Button Cerimonia_completa(COMPLETA);
RBD::Button Cerimonia_simples(SIMPLES);

RBD::Timer timer_pulso_controle_cortina;
RBD::Timer timer_start_petalas_entrega;

RBD::Timer timer;
RBD::Timer timer_pulsa_sala;
RBD::Timer timer_rgb;
RBD::Timer timer_lamp;
RBD::Timer timerCortinas;
RBD::Timer timer_fumaca;
RBD::Timer timerDim;
int tempo_dimmer = 50;
RBD::Timer timerEventos;
RBD::Timer time_esteira;
RBD::Timer time_pulso;
RBD::Timer timerEsteira;
RBD::Timer timerMecanismo;
