void loop() {
  SERIAL_READ();
  VERIFICA_POSICAO();
  timeDimmer();
  currentMillis = millis();
  EXPOEMESA();
  GUARDAMESA();
  FECHAPORTA();
  ABREPORTA();
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

  if (timer_lamp.onRestart() && fade_lamp) {

    if (powers[0] < 100 || powers[0] > 100 ) {
      for (byte i = 0; i < canais_dim; i++) {
        powers[i] = 100;
      }
    }

    else {
      for (byte i = 0; i < canais_dim; i++) {
        powers[i] = 0;
      }
    }
  }
  if (timer.onRestart()) {
    if (cena_inicial_cerimonia == true) {
      Serial.println("CONFIGURAR CENARIO");
    }
    if (Cerimonia_simples.onPressed()) {
      PRINT("BTN_SIMPLES\n");
    }
    if (Cerimonia_completa.onPressed()) {
      PRINT("BTN_COMPLETA\n");
    }
    if (debug) {

      Serial.println(
        (String)"S_ME=" + digitalRead(SENSOR_ME) +
        " S_MG=" + digitalRead(SENSOR_MG) + " S_PA=" + digitalRead(SENSOR_PA) +
        " S_PF=" + digitalRead(SENSOR_PF) + " ETAPA=" + button_counter + " A_P=" + abreporta  + " F_P=" + fechaporta + " G_M=" + guardamesa  +
        " E_M=" + expoemesa + " ZC=" + zero_cross + " S_EST="+digitalRead(acopl_esteira));
    }

  }



  if (Cerimonia_completa.onReleased() && !cerimonia) {
    controle_remoto();
  }

  if (Cerimonia_simples.onReleased()) {
    controle_remoto();
  }
}
