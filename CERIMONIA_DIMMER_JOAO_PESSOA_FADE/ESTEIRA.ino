void ESTEIRA() {
  if (en_esteira) {
    if  (!esteira.onExpired()) {
      if (timerMecanismo.onRestart()) {
        if (digitalRead(PINO_PWM_ESTEIRA) == LOW) {
          timerMecanismo.setTimeout(400);
          timerMecanismo.restart();
          digitalWrite(PINO_PWM_ESTEIRA, HIGH);
        }
        else {
          digitalWrite(PINO_PWM_ESTEIRA, LOW);
          timerMecanismo.setTimeout(200);
          timerMecanismo.restart();
        }
      }

  }
  else {
    //      Serial.print("ESTEIRA DESLIGADA");
    digitalWrite(PINO_PWM_ESTEIRA, 0); // VELOCIDADE 0 A 255
    esteira.stop();
    en_esteira = false;
  }
}
}
