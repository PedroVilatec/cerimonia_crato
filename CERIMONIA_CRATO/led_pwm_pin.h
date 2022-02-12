#define ilum_man_cer 42
#define ilum_man_aux 43
#define rele_som 40
#define acopl_esteira 41
#define parede_r_i 0
#define parede_g_i 1
#define parede_b_i 2
#define cruz_r_i 3
#define cruz_g_i 4
#define cruz_b_i 5
#define cabine_r_i 6
#define cabine_g_i 7
#define cabine_b_i 8

#define parede_r 7
#define parede_g 6
#define parede_b 5
#define cruz_r 4
#define cruz_g 3
#define cruz_b 2
#define cabine_r 44
#define cabine_g 45
#define cabine_b 46

byte led_esteira[] = {17,8};
byte pwm_atual_led_esteira = 0;
byte new_pwm_led_esteira = 0;
#define en_led_esteira 17
#define pwm_led_esteira 8
//byte led_parede[] = {7, 6, 5};
//byte led_sanca[] = {4, 3, 2};
byte led_cabine[] = {44, 45, 46};
byte leds_rgb[] = {parede_r, parede_g, parede_b, cruz_r, cruz_g, cruz_b, cabine_r, cabine_g, cabine_b};
byte atual_rgb[]  = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
byte old_rgb[] = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
byte new_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool en_eeprom_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
byte canais_rgb = 9;

byte cabine_efeito_1[] = {128,255,0};
byte cabine_efeito_2[] = {255,255,255};
byte cabine_efeito_3[] = {0,255,0};
