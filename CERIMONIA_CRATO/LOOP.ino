void loop() {
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
  if (timer.onRestart()) {
    if (debug) {

      Serial.println(
        (String)"S_ME=" + ! digitalRead(SENSOR_ME) +
        " S_MG=" + ! digitalRead(SENSOR_MG) + " S_PA=" + ! digitalRead(SENSOR_PA) +
        " S_PF=" + ! digitalRead(SENSOR_PF) + " ETAPA=" + button_counter + " A_P=" + abreporta  + " F_P=" + fechaporta + " G_M=" + guardamesa  +
        " E_M=" + expoemesa + " ZC=" + zero_cross + " S_EST="+ ! digitalRead(acopl_esteira));
    }    
  }



  if (Cerimonia_completa.onPressed()) {
    Serial.println("COMPLETA PRESS");
    if (video_encerrado && mecanismo_parado()) {
      controle_remoto();
    }

  }

  if (Cerimonia_simples.onPressed())
  {
    Serial.println("ENTREGA PRESS");
    if (video_encerrado && mecanismo_parado()) {
      if (digitalRead(SENSOR_PF) == ATUADO && digitalRead(SENSOR_MG) == ATUADO) {
        PRINT("CERIMONIA_ENTREGA\n");
      }
    }
  }
//  if (timer_pulsa_sala.onRestart()){
//      if (en_rgb_geral == true) {
//        if (old_rgb[parede_r_i] == new_rgb[parede_r_i] && old_rgb[parede_g_i] == new_rgb[parede_g_i] && old_rgb[parede_b_i] == new_rgb[parede_b_i] &&
//            old_rgb[cruz_r_i] == new_rgb[cruz_r_i] && old_rgb[cruz_g_i] == new_rgb[cruz_g_i] && old_rgb[cruz_b_i] == new_rgb[cruz_b_i]) {
//          if (efeito_1) {
//            //timer_rgb.setTimeout(40);// caso altere o valor para menos
//            new_rgb[parede_r_i] = EEPROM.read(100);
//            new_rgb[parede_g_i] = EEPROM.read(101);
//            new_rgb[parede_b_i] = EEPROM.read(102);
//            new_rgb[cruz_r_i] = EEPROM.read(103);
//            new_rgb[cruz_g_i] = EEPROM.read(104);
//            new_rgb[cruz_b_i] = EEPROM.read(105);
//            efeito_1 = false;
//          }
//          else {
//            new_rgb[parede_r_i] = EEPROM.read(106);
//            new_rgb[parede_g_i] = EEPROM.read(107);
//            new_rgb[parede_b_i] = EEPROM.read(108);
//            new_rgb[cruz_r_i] = EEPROM.read(109);
//            new_rgb[cruz_g_i] = EEPROM.read(110);
//            new_rgb[cruz_b_i] = EEPROM.read(111);
//            efeito_1 = true;
//          }              
//
//        }
//      }
//  }
//  if (timer_pulsa_cabine.onRestart()){
//                if (en_rgb_cabine) {
//            if (old_rgb[cabine_r_i] == new_rgb[cabine_r_i] && old_rgb[cabine_g_i] == new_rgb[cabine_g_i] && old_rgb[cabine_b_i] == new_rgb[cabine_b_i]) {
//              if (efeito_1) {
//                new_rgb[cabine_r_i] = 128;
//                new_rgb[cabine_g_i] = 255;
//                new_rgb[cabine_b_i] = 0;
//                efeito_1 = false;
//              }
//              else {
//                new_rgb[cabine_r_i] = 255;
//                new_rgb[cabine_g_i] = 255;
//                new_rgb[cabine_b_i] = 255;
//                efeito_1 = true;
//              }
//            }
//          }
//    }
  }
