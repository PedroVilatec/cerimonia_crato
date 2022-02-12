void controle_remoto() {

  if (pronto() && button_counter == 2) {
    PRINT("CERIMONIA_COMPLETA\n");
    button_counter = 1;


  }
  if (pronto() && button_counter == 1) {
    mem_cena = false;

//    for (byte i = 0; i < canais_rgb; i++) {
//      for (byte f = 0; f < 3; f++) {
//      new_rgb[i][f] = 0;
//      }
//    }
//    for (byte i = 0; i < canais_dim; i++) {
//      if (i == 1) {
//        powers[i] = 0;
//      }
//      else {
//        powers[i] = 128;
//      }
//    }
    button_counter ++;
    Serial.print("button_counter = ");
    Serial.println(button_counter);
  }
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
//void setPwmFrequency(int pin, int divisor) {
//  byte mode;
//  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
//    switch (divisor) {
//      case 1: mode = 0x01; break;
//      case 8: mode = 0x02; break;
//      case 64: mode = 0x03; break;
//      case 256: mode = 0x04; break;
//      case 1024: mode = 0x05; break;
//      default: return;
//    }
//    if (pin == 5 || pin == 6) {
//      TCCR0B = TCCR0B & 0b11111000 | mode;
//    } else {
//      TCCR1B = TCCR1B & 0b11111000 | mode;
//    }
//  } else if (pin == 3 || pin == 11) {
//    switch (divisor) {
//      case 1: mode = 0x01; break;
//      case 8: mode = 0x02; break;
//      case 32: mode = 0x03; break;
//      case 64: mode = 0x04; break;
//      case 128: mode = 0x05; break;
//      case 256: mode = 0x06; break;
//      case 1024: mode = 0x07; break;
//      default: return;
//    }
//    TCCR2B = TCCR2B & 0b11111000 | mode;
//  }
//}
void PRINT(String a)
{
  Serial.print(a);
}
