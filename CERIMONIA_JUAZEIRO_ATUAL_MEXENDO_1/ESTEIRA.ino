void ESTEIRA() {
  if (en_esteira == true ) {

    if (!timerEsteira.onRestart()) {
      if (time_pulso.onRestart()) {
        if (digitalRead(PINO_LIGA_ESTEIRA) == HIGH) {
          //          analogWrite(PINO_PWM_ESTEIRA, LOW);
          //          digitalWrite(PINO_LIGA_ESTEIRA, LOW);
          //          time_pulso.setTimeout(60);
          //
          //          time_pulso.restart();

        }
        else if (digitalRead(PINO_LIGA_ESTEIRA) == LOW) {
          //analogWrite(PINO_PWM_ESTEIRA, PWM_ESTEIRA);
          digitalWrite(PINO_LIGA_ESTEIRA, HIGH);
          //          time_pulso.setTimeout(400);
          //          time_pulso.restart();
        }
      }
    }

    else {
      en_esteira = false;
    digitalWrite(PINO_LIGA_ESTEIRA, LOW);
    digitalWrite(LED_ESTEIRA, LOW);      
      Serial.println("ESTEIRA E LED DESLIGADOS");
    }

  }

}
