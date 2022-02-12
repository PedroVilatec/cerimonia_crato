void controle_remoto(){
  if(iniciado_cerimonia == false){
    if (pronto() && button_counter == 2) {
      PRINT("CERIMONIA_COMPLETA\n");
    }
    if (pronto() && button_counter == 1) {
      cena_inicial_cerimonia = true;
    }
    if (expoemesa == false && abreporta == false && fechaporta == false && guardamesa == false) {

      if (pronto() && button_counter == 0) {
        button_counter ++;
        Serial.print("button_counter = ");
        Serial.println(button_counter);
      }

      if (!pronto()) {
        PRINT("REPOSICIONANDO\n");
        verifica_posicao = true;
        button_counter = 1;
      }
    }
  }
}
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
void PRINT(String a)
{
  Serial.print(a);
}
