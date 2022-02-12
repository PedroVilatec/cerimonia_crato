void EXPOEMESA() {
  if (expoemesa == true && abreporta == false && fechaporta == false && guardamesa == false) {
    if (acionado) {
      acionamento_sensor = 0;
      delay(500);
      acionado = false;
      currentMillis_acionamento = millis();
      PRINT("EXPOE MESA\n");
      if (digitalRead(SENSOR_PA) == NAO_ATUADO) {
        PRINT("SENSOR_PA NAO ATUADO\n");
        mesa_exposta = false;
        expoemesa = false;
        return;
      }
      if (digitalRead(SENSOR_ME) == ATUADO) {
        PRINT("MESA JA EXPOSTA\n");
        mesa_exposta = true;
        mesa_guardada = false;
        expoemesa = false;
        return;
      }
      digitalWrite(PINO_GUARDA, 0);
      digitalWrite(PINO_EXPOE, 1);
      digitalWrite(R_EN_MESA, 1);
      digitalWrite(L_EN_MESA, 1);
    }


    if (digitalRead(SENSOR_ME) == NAO_ATUADO && millis() - currentMillis_acionamento < timeout_mesa) {
    }
    else {
      acionamento_sensor++;
      delay(20);
      if (acionamento_sensor >= 5) {
        if (digitalRead(SENSOR_ME) == ATUADO) {
          mesa_exposta = true;
          mesa_guardada = false;
          PRINT("EXPONDO MESA, TEMPO PREVISTO: 40851\n");
          PRINT("TEMPO DECORRIDO :");
          PRINT(String(millis() - currentMillis_acionamento));
          PRINT("\n");
        }
        else {
          mesa_exposta = false;
          PRINT("EXPONDO MESA, TEMPO LIMITE "+String(timeout_mesa /1000)+" SEGUNDOS EXCEDIDO\n");
        }

        digitalWrite(PINO_GUARDA, 0);
        digitalWrite(PINO_EXPOE, 0);
        digitalWrite(R_EN_MESA, 0);
        digitalWrite(L_EN_MESA, 0);
        acionamento_sensor = 0;
        expoemesa = false;
      }

    }
  }
}
void GUARDAMESA() {
  if (expoemesa == false && abreporta == false && fechaporta == false && guardamesa == true) {
    if (acionado) {
      acionamento_sensor = 0;
      delay(500);
      acionado = false;
      currentMillis_acionamento = millis();
      PRINT("GUARDAMESA\n");
      if (digitalRead(SENSOR_PA) == NAO_ATUADO) {
        PRINT("SENSOR_PA NAO ATUADO\n");
        mesa_guardada = false;
        guardamesa = false;
        return;
      }
      if (digitalRead(SENSOR_MG) == ATUADO) {
        PRINT("MESA JA GUARDADA\n");
        mesa_guardada = true;
        mesa_exposta = false;
        guardamesa = false;
        return;
      }
      digitalWrite(PINO_GUARDA, 1);
      digitalWrite(PINO_EXPOE, 0);
      digitalWrite(R_EN_MESA, 1);
      digitalWrite(L_EN_MESA, 1);
    }


    if (digitalRead(SENSOR_MG) == NAO_ATUADO && millis() - currentMillis_acionamento < timeout_mesa) // tempo até o motor iniciar a fechar lentamente
    {
      if (timer_fumaca.onRestart() && video_encerrado == false) {
        if (digitalRead(FUMACA) == LOW) {
          timer_fumaca.setTimeout(timeFumacaOn);
          timer_fumaca.restart();
        }
        else {
          timer_fumaca.setTimeout(timeFumacaOff);
          timer_fumaca.restart();
        }

        digitalWrite(FUMACA, !digitalRead(FUMACA));
      }

    }
    else {
      acionamento_sensor++;
      delay(20);
      if (acionamento_sensor >= 5) {
        if (digitalRead(SENSOR_MG) == ATUADO) {
          mesa_guardada = true;
          mesa_exposta = false;
          PRINT("GUARDANDO MESA, TEMPO PREVISTO: 39675\n");
          PRINT("TEMPO DECORRIDO :");
          PRINT(String(millis() - currentMillis_acionamento));
          PRINT("\n");
        }
        else {
          mesa_guardada = true;
          PRINT("GUARDANDO MESA, TEMPO LIMITE "+String(timeout_mesa/1000)+" SEGUNDOS EXCEDIDO\n");
        }

        analogWrite(PINO_GUARDA, 0);
        analogWrite(PINO_EXPOE, 0);
        digitalWrite(R_EN_MESA, 0);
        digitalWrite(L_EN_MESA, 0);
        acionamento_sensor = 0;
        guardamesa = false;
      }
    }
  }
}
