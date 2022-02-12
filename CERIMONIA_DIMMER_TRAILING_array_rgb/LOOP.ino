void loop() {
  unsigned long currentMillis = millis();
//  if (timerCortinas.onRestart()) {
//    
//
//    bool cortina_acionada = false;
//    for (int a = 0; a < 5; a++) {
//      if (currentMillis - previousMillis[a] >= 30000) {
//        previousMillis[a] = currentMillis;
//        digitalWrite(cortinas[a][0], HIGH);
//        digitalWrite(cortinas[a][1], HIGH);
//      }
//      if (digitalRead(cortinas[a][0]) ==  LOW || digitalRead(cortinas[a][1]) ==  LOW) {
//        cortina_acionada = true;
//        digitalWrite(69, 1);
//      }
//      if (cortina_acionada == false) digitalWrite(69, 0);
//    }
//}
SERIAL_READ();
  VERIFICA_POSICAO();
  timeDimmer();
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
  if (timer.onRestart() && debug) {
    if(zero_cross){
      Serial.println("zero cross");
      zero_cross = false;
    }
      for (byte i = 0; i < canais_dim; i++) {

    digitalWrite(AC_pin[i], ! digitalRead(AC_pin[i]));
    delay(500);

  }
    // PRINT("ONLINE\n");
    // Serial.println(millis() / 1000);
    if (digitalRead(SENSOR_ME) == ATUADO) {
      PRINT("SENSOR_ME\n");
    }

    if (digitalRead(SENSOR_MG) == ATUADO) {
      PRINT("SENSOR_MG\n");
    }

    if (digitalRead(SENSOR_PF) == ATUADO) {
      PRINT("SENSOR_PF\n");
    }

    if (digitalRead(SENSOR_PA) == ATUADO) {
      PRINT("SENSOR_PA\n");
    }
    if (digitalRead(acopl_esteira) == ATUADO) {
      PRINT("ACO_ESTEIRA\n");
      Serial.println(pwm_atual_led_esteira);
    }
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
