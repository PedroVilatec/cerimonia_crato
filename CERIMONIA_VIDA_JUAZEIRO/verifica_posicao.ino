void VERIFICA_POSICAO() {
  if (verifica_posicao) {
    if (!pronto()) {

      if (digitalRead(SENSOR_ME) == ATUADO && digitalRead(SENSOR_PF) == NAO_ATUADO && todos_parado()) {
        fechaporta = true;
        sincronizado = false;
        acionado = true;
      }

      if (digitalRead(SENSOR_ME) == NAO_ATUADO && digitalRead(SENSOR_PA) == NAO_ATUADO && todos_parado()) {
        abreporta = true;
        acionado = true;
      }
      if (digitalRead(SENSOR_PA) == ATUADO && digitalRead(SENSOR_ME) == NAO_ATUADO && todos_parado()) {
        expoemesa = true;
        acionado = true;
      }
    }
  }
}
bool pronto() {
  if (digitalRead(SENSOR_PF) == ATUADO && digitalRead(SENSOR_ME) == ATUADO) {
    verifica_posicao = false;
//    Serial.println(verifica_posicao);
    return true;

  }
  else {
//    Serial.println(verifica_posicao);
    return false;
  }
}
bool todos_parado() {
  if (expoemesa == false && guardamesa == false && fechaporta == false && abreporta == false) {
    return true;
  }
  else {
    return false;
  }
}
