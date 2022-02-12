#define ilum_man_cer 42
#define ilum_man_aux 43
#define rele_som 40
#define acopl_esteira 41
#define painel_frontal_i 0
#define painel_externo_i 1
#define piso_i 2
#define laterais_i 3
#define sanca_meio_i 4
#define sanca_canto_i 5

#define cabine_r_i 6
#define cabine_g_i 7
#define cabine_b_i 8

#define painel_frontal 5
#define painel_externo 7
#define piso 6
#define laterais 3
#define sanca_meio 4
#define sanca_canto 2
//#define laterais 4
//#define sanca_meio 3
//#define sanca_canto 2
#define cabine_r 4
#define cabine_g 3
#define cabine_b 2


byte led_esteira[] = {17,8};
byte pwm_atual_led_esteira = 0;
byte new_pwm_led_esteira = 0;
#define pwm_led_esteira 9
//byte led_parede[] = {7, 6, 5};
//byte led_sanca[] = {4, 3, 2};
byte led_cabine[] = {44, 45, 46};
byte leds_rgb[] = {painel_frontal, painel_externo, piso, laterais, sanca_meio, sanca_canto, cabine_r, cabine_g, cabine_b,};
byte atual_rgb[]  = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
byte old_rgb[] = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
byte new_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool en_eeprom_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
byte fade_1[]  = {0, 0, 0, 0, 0, 0};
byte fade_2[]  = {0, 0, 0, 0, 0, 0};
byte canais_rgb = 9;
