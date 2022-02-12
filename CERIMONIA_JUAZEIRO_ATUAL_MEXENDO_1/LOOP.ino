void loop() {
  unsigned long currentMillis = millis();
  if(timerLedUrna.onRestart()){
    digitalWrite(CANHAO_LED_URNA,0); //apaga a luz dos canhões
  }
  SERIAL_READ();
  VERIFICA_POSICAO();
  currentMillis = millis();
  EXPOEMESA();
  GUARDAMESA();
  FECHAPORTA();
  ABREPORTA();
  entrega_cinzas();
  ESTEIRA();
  pulsa();
  if (timer_fumaca.onRestart()) {
    digitalWrite(FUMACA, LOW);
  }
  if (timer.onRestart() && debug) {
  
      Serial.println(
        (String)"S_ME=" + !digitalRead(SENSOR_ME) +
        " S_MG=" + !digitalRead(SENSOR_MG) + " S_PA=" + !digitalRead(SENSOR_PA) +
        " S_PF=" + !digitalRead(SENSOR_PF) + " ETAPA=" + button_counter + " A_P=" + abreporta  + " F_P=" + fechaporta + " G_M=" + guardamesa  +
        " E_M=" + expoemesa + " S_EST="+!digitalRead(acopl_esteira));
    
  }

if (Cerimonia_completa.onPressed()) {
//  Serial.println("Pressed");
}

  if (Cerimonia_completa.onReleased()) {
    Serial.println("COMPLETA PRESS");
    if (video_encerrado && mecanismo_parado()) {
      controle_remoto();
    }

  }
  
  if (Cerimonia_simples.onPressed())
  {
    Serial.println("ENTREGA PRESS");
  }
  
if (CEMITERIO == "VIDA"){
  if (Cerimonia_simples.onPressed())
  {
    Serial.println("ENTREGA PRESS");
    if (video_encerrado && mecanismo_parado()) {
      if (digitalRead(SENSOR_PF) == ATUADO && digitalRead(SENSOR_MG) == ATUADO) {
        PRINT("CERIMONIA_ENTREGA\n");
      }
    }
  }
}
}
