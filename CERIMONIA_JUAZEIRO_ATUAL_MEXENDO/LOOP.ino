void loop() {
  unsigned long currentMillis = millis();
  SERIAL_READ();
  VERIFICA_POSICAO();
  currentMillis = millis();
  EXPOEMESA();
  GUARDAMESA();
  FECHAPORTA();
  ABREPORTA();
  cerimonia_completa();
  entrega_cinzas();
  ESTEIRA();
  pulsa();
  if (timer_fumaca.onRestart()) {
    digitalWrite(FUMACA, LOW);
  }
  //  if (timer_pulso_controle_cortina.onRestart() && (b_c == true || s_c == true)) {
  //    b_c ? Serial.println("B_C OFF") : Serial.println("S_C OFF");
  //    b_c = false;
  //    s_c = false;
  //
  //    digitalWrite(baixa_cortina, LOW);
  //    digitalWrite(sobe_cortina, LOW);
  //
  //  }
  if (timer.onRestart() && debug) {
  
      Serial.println(
        (String)"S_ME=" + digitalRead(SENSOR_ME) +
        " S_MG=" + digitalRead(SENSOR_MG) + " S_PA=" + digitalRead(SENSOR_PA) +
        " S_PF=" + digitalRead(SENSOR_PF) + " ETAPA=" + button_counter + " A_P=" + abreporta  + " F_P=" + fechaporta + " G_M=" + guardamesa  +
        " E_M=" + expoemesa + " S_EST="+digitalRead(acopl_esteira));
    
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
