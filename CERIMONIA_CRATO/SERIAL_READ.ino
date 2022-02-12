/******************************************************************
  Created with PROGRAMINO IDE for Arduino - 22.06.2018 08:14:57
  Project     :
  Libraries   :
  Author      :Pedro_Silva
  Description :Etapa de comunicação serial
******************************************************************/

void SERIAL_READ() {

  if (Serial.available() > 0)
  {

    while (Serial.available() > 0)
    {
      char characterIn = Serial.read();
      DataIn.concat(characterIn);
      if ((characterIn == '\n') || (characterIn == '\r'))
      {
        DataIn.toUpperCase();
        processaString();
        DataIn = "";
      }
    }
  }
}


void processaString() {
  if (DataIn.indexOf("MODEL") == 0)
  {
    Serial.println("MEGA");
    return;
  }

  //    if (DataIn.indexOf("STATUS") > -1)
  //  {
  //    Serial.print("Led 1 r\t Led2\tLed3");
  //  }
  if (DataIn.indexOf("ACENDER") > -1) {
    if (CEMITERIO == "VIDA") {
      digitalWrite(42, 1);
      EEPROM.write(42, 1);
    }

  }
  if (DataIn.indexOf("APAGAR") > -1)
  {
    Serial.println("APAGAR");
    for (int i = 0; i < canais_rgb; i++) {
      new_rgb[i] = 0;

    }
    for (int i = 0; i < canais_dim; i++) {
      powers[i] = 128;


    }
    if (CEMITERIO == "VIDA") {
      digitalWrite(42, 0);
      EEPROM.write(42, 0);
    }

    return;
  }
  if (DataIn.indexOf("L_DIMER") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    //int index3 = DataIn.indexOf(',', index2 + 1);
    int canal = DataIn.substring(index1 + 1, index2).toInt();
    int intensidade = DataIn.substring(index2 + 1, DataIn.length() - 1).toInt();
    //canal = constrain(canal, 1, 10);
    intensidade = constrain(intensidade, 0, 100);
    int value = map(intensidade, 0, 100, 128, 0);
    powers[canal - 1] = value;

    //    Serial.print("DIM,");
    //    Serial.print(canal);
    //    Serial.print(",");
    //    Serial.println(intensidade);
    return;
  }
  if (DataIn.indexOf("R_DIMER") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int canal = DataIn.substring(index1 + 1, index2).toInt();
    int intensidade = DataIn.substring(index2 + 1, DataIn.length() - 1).toInt();
    // canal = constrain(canal, 1, 10);
    intensidade = constrain(intensidade, 0, 100);
    int value = map(intensidade, 0, 100, 128, 0);
    dims[canal - 1] = value;
    portas[canal - 1] = value;
    powers[canal - 1] = value;

    //    Serial.print("DIM,");
    //    Serial.print(canal);
    //    Serial.print(",");
    //    Serial.println(intensidade);
    return;
  }

  if (DataIn.indexOf("L_LED") > -1)
  {
    //Serial.println(DataIn.substring(5, 6));

    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int index4 = DataIn.indexOf(',', index3 + 1);
    int channel = DataIn.substring(index1 + 1, index2).toInt() ;
    int r = DataIn.substring(index2 + 1, index3).toInt();
    int g = DataIn.substring(index3 + 1, index4).toInt();
    int b = DataIn.substring(index4 + 1, DataIn.length() - 1).toInt();
    channel = constrain(channel, 1, 3);
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);
    //    Serial.println((String)channel + " " + r + "-" + g + "-" + b);
    channel = channel * 3;
    new_rgb[channel - 3] = r;
    new_rgb[channel - 2] = g;
    new_rgb[channel - 1] = b;

    return;
  }

  if (DataIn.indexOf("R_LED") > -1)
  {
    //Serial.println(DataIn.substring(5, 6));

    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int index4 = DataIn.indexOf(',', index3 + 1);
    int channel = DataIn.substring(index1 + 1, index2).toInt() ;
    int r = DataIn.substring(index2 + 1, index3).toInt();
    int g = DataIn.substring(index3 + 1, index4).toInt();
    int b = DataIn.substring(index4 + 1, DataIn.length() - 1).toInt();
    channel = constrain(channel, 1, 3);
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);
    //    Serial.println((String)channel + " " + r + "-" + g + "-" + b);
    channel = channel * 3;
    old_rgb[channel - 3] = r;
    old_rgb[channel - 2] = g;
    old_rgb[channel - 1] = b;
    atual_rgb[channel - 3] = r;
    atual_rgb[channel - 2] = g;
    atual_rgb[channel - 1] = b;
    new_rgb[channel - 3] = r;
    new_rgb[channel - 2] = g;
    new_rgb[channel - 1] = b;

    analogWrite(leds_rgb[channel - 3], r);
    analogWrite(leds_rgb[channel - 2], g);
    analogWrite(leds_rgb[channel - 1], b);
    return;
  }
  if (DataIn.indexOf("LED_PAREDE") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int r = DataIn.substring(index1 + 1, index2).toInt();
    int g = DataIn.substring(index2 + 1, index3).toInt();
    int b = DataIn.substring(index3 + 1, DataIn.length() - 1).toInt();
    //  Serial.println((String)"LED PAREDE R" + r + " G" + g + " B" + b);
    //    Serial.println(r);
    //    Serial.println(g);
    //    Serial.println(b);
    new_rgb[parede_r_i] = r;
    new_rgb[parede_g_i] = g;
    new_rgb[parede_b_i] = b;
    return;
  }
  if (DataIn.indexOf("LED_CABINE") == 0)
  { //LED_CABINE,255,255,255,255,255,255,255,255,255
    //LED_CABINE,255,0,0,0,255,0,0,0,255
    byte index[15] = {};
    Serial.println(DataIn.length());
    index[0] = DataIn.indexOf(',');
    for (int a = 1; a < 9; a++) {
      index[a] = DataIn.indexOf(',', index[a - 1] + 1);

    }
    //    Serial.println((String)DataIn.substring(index[0] + 1, index[1]).toInt()+" "+
    //    DataIn.substring(index[1] + 1, index[2]).toInt()+" "+
    //    DataIn.substring(index[2] + 1, index[3]).toInt()+" "+
    //    DataIn.substring(index[3] + 1, index[4]).toInt()+" "+
    //    DataIn.substring(index[4] + 1, index[5]).toInt()+" "+
    //    DataIn.substring(index[5] + 1, index[6]).toInt()+" "+
    //    DataIn.substring(index[6] + 1, index[7]).toInt()+" "+
    //    DataIn.substring(index[7] + 1, index[8]).toInt()+" "+
    //    DataIn.substring(index[8] + 1, DataIn.length() - 1).toInt());
    cabine_efeito_1[0] = DataIn.substring(index[0] + 1, index[1]).toInt();
    cabine_efeito_1[1] = DataIn.substring(index[1] + 1, index[2]).toInt();
    cabine_efeito_1[2] = DataIn.substring(index[2] + 1, index[3]).toInt();
    cabine_efeito_2[0] = DataIn.substring(index[3] + 1, index[4]).toInt();
    cabine_efeito_2[1] = DataIn.substring(index[4] + 1, index[5]).toInt();
    cabine_efeito_2[2] = DataIn.substring(index[5] + 1, index[6]).toInt();
    cabine_efeito_3[0] = DataIn.substring(index[6] + 1, index[7]).toInt();
    cabine_efeito_3[1] = DataIn.substring(index[7] + 1, index[8]).toInt();
    cabine_efeito_3[2] = DataIn.substring(index[8] + 1, DataIn.length() - 1).toInt();
    new_rgb[cabine_r_i] = 1;
    new_rgb[cabine_g_i] = 1;
    new_rgb[cabine_b_i] = 1;
    efeito_rgb_cabine_1 = true;
    return;
  }

  if (DataIn.indexOf("LED_TETO") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int r = DataIn.substring(index1 + 1, index2).toInt();
    int g = DataIn.substring(index2 + 1, index3).toInt();
    int b = DataIn.substring(index3 + 1, DataIn.length() - 1).toInt();
    //   Serial.println((String)"LED TETO R" + r + " G" + g + " B" + b);
    //    Serial.println(r);
    //    Serial.println(g);
    //    Serial.println(b);
    //    new_rgb[6] = r;
    //    new_rgb[7] = g;
    //    new_rgb[8] = b;
    return;
  }

  if (DataIn.indexOf("EN_RGB_GERAL") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);

    bool state = DataIn.substring(index1 + 1, index2).toInt();
    if (state) {
      en_rgb_geral = state;
      new_rgb[parede_r_i] = EEPROM.read(100);
      new_rgb[parede_g_i] = EEPROM.read(101);
      new_rgb[parede_b_i] = EEPROM.read(102);
      new_rgb[cruz_r_i] = EEPROM.read(103);
      new_rgb[cruz_g_i] = EEPROM.read(104);
      new_rgb[cruz_b_i] = EEPROM.read(105);
      timer_rgb.setTimeout(10);
    }
    else {
      timer_rgb.setTimeout(20);
      en_rgb_geral = false;
    }

  }
  if (DataIn.indexOf("ANA") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int pin = DataIn.substring(index1 + 1, index2).toInt();
    int state = DataIn.substring(index2 + 1, DataIn.length() - 1).toInt();
    analogWrite(pin, state);
    Serial.print("PINO ");
    Serial.print(pin);
    Serial.print(" - ");
    Serial.println(state);
  }
  if (DataIn.indexOf("DIMER") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int canal = DataIn.substring(index1 + 1, index2).toInt();
    int intensidade = DataIn.substring(index2 + 1, DataIn.length() - 1).toInt();
    canal = constrain(canal, 1, 10);
    intensidade = constrain(intensidade, 0, 100);
    int value = map(intensidade, 0, 100, 128, 0);
    powers[canal - 1] = value;

    Serial.print("DIM,");
    Serial.print(canal);
    Serial.print(",");
    Serial.println(intensidade);
  }

  if (DataIn.indexOf("GPIO") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf(',', index2 + 1);
    int pin = DataIn.substring(index1 + 1, index2).toInt();
    int state = DataIn.substring(index2 + 1, DataIn.length() - 1).toInt();
    digitalWrite(pin, state);
    EEPROM.write(pin, state);
    Serial.print("PINO ");
    Serial.print(pin);
    Serial.print(" - ");
    Serial.println(digitalRead(pin));
  }


  //  if (DataIn.substring(0, 12) == "DELAY_FUMACA")
  //  {
  //    unsigned int duracao = DataIn.substring(13, 15).toInt();
  //    duracaoFumaca = duracao * 1000;
  //    EEPROM.write(1, duracao);
  //  }

  else if (DataIn.indexOf("FUM") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);

    int duracao = DataIn.substring(index1 + 1, index2).toInt();

    Serial.println((String)"FUMACA " + duracao);
    timer_fumaca.setTimeout(duracao);
    timer_fumaca.restart();
    digitalWrite(FUMACA, HIGH);

  }

  else if (DataIn.indexOf("B_C") == 0) //BAIXA CORTINAS PULSO DE UM SEGUNDO
  {
    Serial.println("B_C");
    timer_pulso_controle_cortina.restart();
    b_c = true;
    digitalWrite(baixa_cortina, HIGH);
  }
  else if (DataIn.indexOf("ZCC") == 0) //SOBE CORTINAS PULSO DE UM SEGUNDO
  {

    EEPROM.write(1023, 0);
    EEPROM.write(1024, 0);
    Serial.println("CONTADOR DE CERIMONIAS ZERADO");

  }
  else if (DataIn.indexOf("ZCE") == 0) //SOBE CORTINAS PULSO DE UM SEGUNDO
  {

    EEPROM.write(1021, 0);
    EEPROM.write(1022, 0);
    Serial.println("CONTADOR DE ENTREGAS ZERADO");

  }
  else if (DataIn.indexOf("S_C") == 0) //SOBE CORTINAS PULSO DE UM SEGUNDO
  {
    Serial.println("S_C");
    timer_pulso_controle_cortina.restart();
    s_c = true;
    digitalWrite(sobe_cortina, HIGH);
  }

  else if (DataIn.substring(0, 8) == "CONTROLE" && mecanismo_parado() && video_encerrado)
  {
    controle_remoto();
  }
  else if (DataIn.substring(0, 9) == "ENT_CINZA" && !entrega)
  {
    if (mecanismo_parado()) {
      if (digitalRead(SENSOR_PF) == ATUADO || digitalRead(SENSOR_MG) == ATUADO) {
        completa = false;
        PRINT("CERIMONIA_ENTREGA\n");
      }
    }
  }
  else if (DataIn.indexOf("T_DIM") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);

    int value = DataIn.substring(index1 + 1, index2).toInt();
    tempo_dimmer = value;
    timerDim.setTimeout(tempo_dimmer);
    Serial.print("TEMPO DIMMER = ");
    Serial.println(tempo_dimmer);

  }


  else if (DataIn.indexOf("LED_URNA") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);

    int value = DataIn.substring(index1 + 1, index2).toInt();
    new_pwm_led_esteira = value;
    Serial.print("LED urna, ");
    Serial.println(new_pwm_led_esteira);

  }

  else if (DataIn.indexOf("CHUVAPETALAS") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf('index1', DataIn.length() - 2);

    int PWM_ESTEIRA = DataIn.substring(index1 + 1, index2).toInt();
    unsigned long timer_esteira = DataIn.substring(index2 + 1, index3).toInt() * 1000;
    timerEsteira.setTimeout(timer_esteira);
    if (en_esteira == false) {
      Serial.println((String)"ESTEIRA LIGADA " + PWM_ESTEIRA + " - " + timer_esteira);

      en_esteira = true;
      timerEsteira.restart();
    }
    else {
      Serial.println("esteira ja ligada");
    }

  }

  else if (DataIn.substring(0, 9) == "SEQUENCIA")
  {
    completa = true;
    etapa_02 = true;

  }
  else if (DataIn.substring(0, 9) == "ABREPORTA" && mecanismo_parado())
  {
    abreporta = true;
    acionado = true;
  }

  else if (DataIn.substring(0, 10) == "GUARDAMESA" && mecanismo_parado())
  {

    guardamesa = true;
    acionado = true;
  }

  else if (DataIn.substring(0, 5) == "EXPOE" && mecanismo_parado())
  {

    expoemesa = true;
    acionado = true;

  }
  else if (DataIn.substring(0, 4) == "PARA")
  {
    analogWrite(PINO_ABRE, 0);
    analogWrite(PINO_FECHA, 0);
    digitalWrite(R_EN_PORTA, 0);
    digitalWrite(L_EN_PORTA, 0);

    fechaporta = false;
    abreporta = false;

    analogWrite(PINO_GUARDA, 0);
    analogWrite(PINO_EXPOE, 0);
    digitalWrite(R_EN_MESA, 0);
    digitalWrite(L_EN_MESA, 0);
    acionamento_sensor = 0;
    guardamesa = false;
    expoemesa = false;
    acionado = false;
  }

  else if (DataIn.substring(0, 5) == "FECHA" && mecanismo_parado())
  {
    fechaporta = true;
    acionado = true;
  }
  else if (DataIn.indexOf("INICIO_CERIMONIA") > -1)
  {
    Serial.println("INICIO DE CERIMONIA");
    video_encerrado = false;
  }
  else if (DataIn.indexOf("FINAL_CERIMONIA") > -1)

  {
    Serial.println("FIM DE CERIMONIA");
    video_encerrado = true;
  }
  else if (DataIn.indexOf("COMPLETA") > -1)
  {
    PRINT("completa\n");
    entrega = false;
    completa = true;
    powers[1] = 0;

  }
  else if (DataIn.indexOf("ENTREGA") > -1)
  {
    PRINT("entrega\n");
    entrega = true;
    completa = false;
    new_pwm_led_esteira = 255;
    Serial.print("LED ESTEIRA, ");
    Serial.println(new_pwm_led_esteira);
    etapa_02 = true;

  }

  else if (DataIn.substring(0, 9) == "FADE_LAMP")
  {
    fade_lamp = !fade_lamp;
  }



  else if (DataIn.substring(0, 7) == "IL_AUTO")
  {
    digitalWrite(ilum_man_cer, HIGH);
    EEPROM.write(ilum_man_cer, 1);
  }

  else if (DataIn.substring(0, 9) == "IL_MANUAL")
  {
    digitalWrite(ilum_man_cer, LOW);
    EEPROM.write(ilum_man_cer, 0);
  }

  else if (DataIn.substring(0, 5) == "DEBUG")
  {
    debug = !debug;
  }
  if ((DataIn.charAt(0) == '<') && (DataIn.charAt(DataIn.length() - 2) == '>'))  // ÚLTIMO CARACTERE + NEW LINE PODE SER NECESSÁRIO MUDAR PARA h-1
  {
    atribui_dimmer();
    return;
  }

  if ((DataIn.charAt(0) == '(') && (DataIn.charAt(DataIn.length() - 2) == ')'))  // ÚLTIMO CARACTERE + NEW LINE PODE SER NECESSÁRIO MUDAR PARA h-1
  {

    atribui_rgb();
    return;
  }

  if ((DataIn.charAt(0) == '[') && (DataIn.charAt(DataIn.length() - 2) == ']'))  // ÚLTIMO CARACTERE + NEW LINE PODE SER NECESSÁRIO MUDAR PARA h-1
  {

    set_pulso_rgb();
    return;
  }
}
