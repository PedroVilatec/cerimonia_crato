void VERIFICA_POSICAO() {
  if (verifica_posicao) {
    if (!pronto()) {

      if (digitalRead(SENSOR_ME) == ATUADO && digitalRead(SENSOR_PF) == NAO_ATUADO && mecanismo_parado()) {
        fechaporta = true;
        acionado = true;
      }

      if (digitalRead(SENSOR_ME) == NAO_ATUADO && digitalRead(SENSOR_PA) == NAO_ATUADO && mecanismo_parado()) {
        abreporta = true;
        acionado = true;
      }
      if (digitalRead(SENSOR_PA) == ATUADO && digitalRead(SENSOR_ME) == NAO_ATUADO && mecanismo_parado()) {
        expoemesa = true;
        acionado = true;
      }
    }
  }
}
bool pronto() {
  if (digitalRead(SENSOR_PF) == ATUADO && digitalRead(SENSOR_ME) == ATUADO) {
    verifica_posicao = false;
    return true;

  }
  else {
    return false;
  }
}
bool mecanismo_parado() {
  if (expoemesa == false && guardamesa == false && fechaporta == false && abreporta == false) {
    return true;
  }
  else {
    return false;
  }
}
