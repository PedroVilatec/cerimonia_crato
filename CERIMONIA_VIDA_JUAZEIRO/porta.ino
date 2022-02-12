void FECHAPORTA() {
  if (expoemesa == false && abreporta == false && fechaporta == true && guardamesa == false) {
    //Serial.println("passando_fecha");
    if (acionado) {
      acionamento_sensor = 0;
      delay(500);
      acionado = false;
      currentMillis_acionamento = millis();
      PRINT("FECHA PORTA\n");
      if (digitalRead(SENSOR_PF) == ATUADO) {
        PRINT("PORTA JA FECHADA\n");
        porta_fechada = true;
        porta_aberta = false;
        fechaporta = false;
        return;
      }
      if (digitalRead(SENSOR_MG) == NAO_ATUADO && digitalRead(SENSOR_ME) == NAO_ATUADO)
      {
        PRINT("SENSORES DA MESA NAO ATUADOS\n");
        porta_fechada = false;
        fechaporta = false;
        return;
      }

      analogWrite(PINO_ABRE, 0);
      analogWrite(PINO_FECHA, PWM_FECHA_PORTA);
      digitalWrite(R_EN_PORTA, 1);
      digitalWrite(L_EN_PORTA, 1);
    }


    if (digitalRead(SENSOR_PF) == NAO_ATUADO && millis() - currentMillis_acionamento < timeout_porta) // tempo até o motor iniciar a fechar lentamente
    {
      if (timerMecanismo.onRestart()) {

        if (alterna_analog_porta) {
          timerMecanismo.setTimeout(40);
          timerMecanismo.restart();
          alterna_analog_porta = !alterna_analog_porta;
          analogWrite(PINO_FECHA, PWM_FECHA_PORTA);
        }
        else {
          timerMecanismo.setTimeout(5);
          timerMecanismo.restart();
          alterna_analog_porta = !alterna_analog_porta;
          analogWrite(PINO_FECHA, 255);
        }
      }
    }
    else {
      if (acionamento_sensor < 5) {
        acionamento_sensor++;
        delay(20);
      }

      if (acionamento_sensor == 5) {

        if (digitalRead(SENSOR_PF) == ATUADO) {
          PRINT((String)"PORTA FECHADA, TEMPO PREVISTO: " + tempo_fechamento_porta + " TEMPO DECORRIDO : " + (millis() - currentMillis_acionamento) + "\n");
          porta_fechada = true;
          porta_aberta = false;
          analogWrite(PINO_ABRE, 0);
          analogWrite(PINO_FECHA, 0);
          digitalWrite(R_EN_PORTA, 0);
          digitalWrite(L_EN_PORTA, 0);
          acionamento_sensor = 0;
          fechaporta = false;         
        }
        else if (millis() - currentMillis_acionamento > timeout_porta) {
          porta_fechada = false;
          PRINT("FECHA PORTA, TEMPO LIMITE " + String(timeout_porta / 1000) + " SEGUNDOS EXCEDIDO\n");
          analogWrite(PINO_ABRE, 0);
          analogWrite(PINO_FECHA, 0);
          digitalWrite(R_EN_PORTA, 0);
          digitalWrite(L_EN_PORTA, 0);
          fechaporta = false;

          return;
        }
      }
    }
  }
}

void ABREPORTA() {
  if (expoemesa == false && abreporta == true && fechaporta == false && guardamesa == false) {

    if (acionado) {
      acionamento_sensor = 0;
      delay(500);
      acionado = false;
      currentMillis_acionamento = millis();

      if (digitalRead(SENSOR_PA) == ATUADO) {
        PRINT("PORTA JA ABERTA\n");
        porta_fechada = false;
        porta_aberta = true;
        abreporta = false;
        return;
      }
      PRINT("ABRINDO A PORTA\n");
      analogWrite(PINO_ABRE, PWM_ABRE_PORTA);
      analogWrite(PINO_FECHA, 0);
      digitalWrite(R_EN_PORTA, 1);
      digitalWrite(L_EN_PORTA, 1);
    }
    if (digitalRead(SENSOR_PA) == NAO_ATUADO && millis() - currentMillis_acionamento < timeout_porta) // tempo até o motor iniciar a fechar lentamente
    {
      //      if (timerPrint.onRestart()) {
      //        for (byte a = 0; a < 10; a++) {
      //          Serial.write('\b');
      //        }
      //        Serial.print(String((millis() - currentMillis_acionamento) / 1000));
      //      }
    }
    else {
      acionamento_sensor++;
      delay(10);
      if (acionamento_sensor >= 5) {
        if (digitalRead(SENSOR_PA) == ATUADO) {
          porta_aberta = true;
          porta_fechada = false;
          PRINT("TEMPO ESTIMADO 24958,TEMPO DECORRIDO :");
          PRINT(String(millis() - currentMillis_acionamento));
          PRINT("\n");
          analogWrite(PINO_ABRE, 0);
          analogWrite(PINO_ABRE, 0);
          analogWrite(PINO_FECHA, 0);
          digitalWrite(R_EN_PORTA, 0);
          digitalWrite(L_EN_PORTA, 0);
          acionamento_sensor = 0;
          abreporta = false;
          return;
        }
        else if (millis() - currentMillis_acionamento > timeout_porta) {
          porta_fechada = false;
          PRINT("ABRE PORTA, TEMPO LIMITE " + String(timeout_porta / 1000) + " SEGUNDOS EXCEDIDO\n");
          analogWrite(PINO_ABRE, 0);
          analogWrite(PINO_FECHA, 0);
          digitalWrite(R_EN_PORTA, 0);
          digitalWrite(L_EN_PORTA, 0);
          fechaporta = false;
          return;
        }
      }
    }
  }
}
