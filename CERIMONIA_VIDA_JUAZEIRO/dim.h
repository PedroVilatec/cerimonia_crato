volatile boolean zero_cross = 0; // Boolean to store a "switch" to tell us if we have crossed zero
#if defined VIDA 
  byte AC_pin[] = {29, 28, 27, 26, 25, 24, 22, 21, 20};
#else
  byte AC_pin[] = {29, 28, 27, 26, 25, 24, 23, 50, 21, 20};
#endif
byte canais_dim = sizeof(AC_pin) / sizeof(AC_pin[0]);
volatile int valor_anterior[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte dims[]  = {0, 0 , 0, 0, 0, 0, 0, 0, 0, 0};
byte portas[] = {0, 0 , 0, 0, 0, 0, 0, 0, 0, 0};
byte powers[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool en_eeprom_dim[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int freqStep = 65;
