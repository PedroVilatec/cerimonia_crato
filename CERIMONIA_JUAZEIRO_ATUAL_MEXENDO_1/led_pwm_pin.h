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


#if defined(VIDA_JUAZEIRO)
  byte led_cabine[] = {4, 3, 2};
  byte canais_extra[] = {7, 6, 5, 8, 9, 10};
  #define par_sanca_frente_r 7
  #define par_sanca_frente_g 6
  #define par_sanca_frente_b 5
  #define par_sanca_atras_r 8
  #define par_sanca_atras_g 9
  #define par_sanca_atras_b 10
  
  #define cabine_r 4
  #define cabine_g 3
  #define cabine_b 2
  #define parede_r 44
  #define parede_g 45
  #define parede_b 46
  #define cruz_r 11
  #define cruz_g 12
  #define cruz_b 13
  byte leds_rgb[5][3] =
  {
    {par_sanca_frente_r, par_sanca_frente_g, par_sanca_frente_b},
    {par_sanca_atras_r, par_sanca_atras_g, par_sanca_atras_b},
    {parede_r, parede_g, parede_b},
    {cruz_r, cruz_g, cruz_b},
    {cabine_r, cabine_g, cabine_b}
  };
  byte atual_rgb[5][3]  = 
  {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}    
  };
  
  byte old_rgb[5][3] = 
    {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}    
  };
  byte new_rgb[5][3] = 
   {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}    
  };
  bool en_eeprom_rgb[5][3] = 
    {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}    
  };
  byte canais_rgb = sizeof(leds_rgb) / sizeof(leds_rgb[0]);


#elif defined(KRAUSE)
  byte cortinas[5][2] = {{68, 67}, {66, 65}, {62, 60}, {58, 59}, {57, 56}};
  byte led_cabine[] = {4, 3, 2};
  #define cabine_r 4 // krause
  #define cabine_g 3
  #define cabine_b 2
  #define parede_r 7
  #define parede_g 6
  #define parede_b 5
  #define cruz_r 4 //nao se aplica
  #define cruz_g 3 //nao se aplica
  #define cruz_b 2 //nao se aplica
  byte leds_rgb[] = {parede_r, parede_g, parede_b, cruz_r, cruz_g, cruz_b, cabine_r, cabine_g, cabine_b};
  byte atual_rgb[]  = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
  byte old_rgb[] = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
  byte new_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  bool en_eeprom_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  byte canais_rgb = 9;

#elif defined(VIDA)
  byte led_cabine[] = {44, 45, 46};
  #define cabine_r 44
  #define cabine_g 45
  #define cabine_b 46
  #define parede_r 7
  #define parede_g 6
  #define parede_b 5
  #define cruz_r 4
  #define cruz_g 3
  #define cruz_b 2
  byte leds_rgb[] = {parede_r, parede_g, parede_b, cruz_r, cruz_g, cruz_b, cabine_r, cabine_g, cabine_b};
  byte atual_rgb[]  = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
  byte old_rgb[] = {0, 0 , 0, 0, 0, 0, 0, 0, 0};
  byte new_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  bool en_eeprom_rgb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  byte canais_rgb = 9;
#endif


byte cortinas[5][2] = {{68, 67}, {66, 65}, {62, 60}, {58, 59}, {57, 56}};



byte cabine_efeito_1[] = {0, 0, 0};
byte cabine_efeito_2[] = {0, 0, 0};
byte cabine_efeito_3[] = {0, 0, 0};
