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
    if (DataIn.indexOf("INICIADO") > -1)
  {
    iniciado_cerimonia = true;
  }
    if (DataIn.indexOf("FINAL") > -1)
  {
    iniciado_cerimonia = false;
  }  
  if (DataIn.indexOf("APAGAR") > -1)
  {
    Serial.println("APAGAR");
    for (int i = 0; i < canais_rgb; i++) {
      new_rgb[i] = 0;

    }
    for (int i = 0; i < canais_dim; i++) {
      powers[i] = 0;
    }
    return;
  }
  if (DataIn.indexOf("L_DIMER") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    //int index3 = DataIn.indexOf(',', index2 + 1);
    int canal = DataIn.substring(index1 + 1, index2).toInt() - 1;
    int intensidade = DataIn.substring(index2 + 1, DataIn.length() - 1).toInt();
    //canal = constrain(canal, 1, 10);
    intensidade = constrain(intensidade, 0, 100);
    Serial.print("DIM,");
    Serial.print(canal);
    Serial.print(",");
    Serial.println(intensidade);

    byte led_value = map(intensidade, 0, 100, 0, 255);
    if (canal == painel_frontal_i) {
      new_rgb[painel_frontal_i] = led_value;
    }
    else if (canal == painel_externo_i) {
      new_rgb[painel_externo_i] = led_value;
    }
    else if (canal == piso_i) {
      new_rgb[piso_i] = led_value;
    }
    else if (canal == laterais_i) {
      new_rgb[laterais_i] = led_value;
    }
    else if (canal == sanca_meio_i) {
      new_rgb[sanca_meio_i] = led_value;
    }
    else if (canal == sanca_canto_i) {
      new_rgb[sanca_canto_i] = led_value;
    }

    else {
      int value = map(intensidade, 0, 100, dim_minimo, dim_maximo);
      powers[canal] = value;
    }
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
    byte led_value = map(intensidade, 0, 100, 0, 255);
    if (canal == painel_frontal_i) {
      new_rgb[painel_frontal_i] = led_value;
      atual_rgb[painel_frontal_i] = led_value;
      old_rgb[painel_frontal_i] = led_value;
      analogWrite(painel_frontal, led_value);      
    }
    else if (canal == painel_externo_i) {
      new_rgb[painel_externo_i] = led_value;
      atual_rgb[painel_externo_i] = led_value;
      old_rgb[painel_externo_i] = led_value;
      analogWrite(painel_externo, led_value);       
      
    }
    else if (canal == piso_i) {
      new_rgb[piso_i] = led_value;
      atual_rgb[piso_i] = led_value;
      old_rgb[piso_i] = led_value;
      analogWrite(piso, led_value);      
    }
    else if (canal == laterais_i) {
      new_rgb[laterais_i] = led_value;
      atual_rgb[laterais_i] = led_value;
      old_rgb[laterais_i] = led_value;
      analogWrite(laterais, led_value);      
    }
    else if (canal == sanca_meio_i) {
      new_rgb[sanca_meio_i] = led_value;
      atual_rgb[sanca_meio_i] = led_value;
      old_rgb[sanca_meio_i] = led_value;
      analogWrite(sanca_meio, led_value);      
    }
    else if (canal == sanca_canto_i) {
      new_rgb[sanca_canto_i] = led_value;
      atual_rgb[sanca_canto_i] = led_value;
      old_rgb[sanca_canto_i] = led_value;
      analogWrite(sanca_canto, led_value);
    }

    else {
      canal -= 5;
      int value = map(intensidade, 0, 100, dim_minimo, dim_maximo);
      dims[canal - 1] = value;
      portas[canal - 1] = value;
      powers[canal - 1] = value;
    }
  }
  if (DataIn.indexOf("LED_CABINE") > -1)
  {
    byte valor[15] = {0};
    int ind = -1;
    byte idx[15];
    byte counter = 0;
    for (int a = 0; a < DataIn.length(); a++) {
      valor[a] = DataIn.indexOf(",", ind + 1);
      ind = valor[a];
      if (counter > 0) {
        idx[a - 1] = constrain(DataIn.substring(valor[a - 1] + 1, valor[a]).toInt(), 0, 255);
        //        Serial.println((String)(a - 1) + " " + idx[a - 1]);
      }
      counter ++;
      if (valor[a] == DataIn.lastIndexOf(",")) {
        idx[a] = constrain(DataIn.substring(valor[a] + 1, DataIn.length() - 1).toInt(), 0, 255);
        //        Serial.println((String)a + " " + idx[a]);
        break;
      }
    }


    byte alterna1 = 0;//alterna canal rgb
    byte alterna2 = 0; //alterna r,g,b
    if (counter > 2) {
      if (counter <= 3) {
        for (int a = 0; a < counter; a++) { //counter inicia em 2 para omitir o indexador
          Serial.println((String)"Canal " + idx[0] + " " +  alterna1 + " " + alterna2 + " " + idx[a]);
          Serial.println((String)"Canal " + idx[0] + " " +  (alterna1 + 1) + " " + alterna2 + " " + idx[a]);
          color_led_cabine[0][alterna1][alterna2] = idx[a];
          color_led_cabine[0][alterna1  + 1][alterna2] = idx[a];
          if (alterna2 ==  2) {
            alterna2 = 0;
            alterna1 ++;
          }
          else {
            alterna2 ++;
          }
          //            if (a == 3)break; // break independente da quantidade de numeros
        }
      }

      else {
        // MANTÉM A COR ESTÁTICA SE VIER APENAS AS TRES CORES
        for (int a = 0; a < counter; a++) { //counter inicia em 2 para omitir o indexador
          //            Serial.println((String)" " +  alterna1 + " " + alterna2 + " " + idx[a]);


          color_led_cabine[0][alterna1][alterna2] = idx[a];
          if (alterna2 ==  2) {
            alterna2 = 0;
            alterna1 ++;
          }
          else {
            alterna2 ++;
          }
          if (a == 6) {

            break; // break independente da quantidade de numeros
          }
        }
      }

      Serial.println();

      new_rgb[cabine_r_i] = color_led_cabine[0][0][0];
      new_rgb[cabine_g_i] = color_led_cabine[0][0][1];
      new_rgb[cabine_b_i] = color_led_cabine[0][0][2];
      en_rgb_cabine = true;
      for (int a = 0; a < sizeof(color_led_cabine) / sizeof(color_led_cabine[0]); a++) {
        Serial.println((String)"Status led cabine " + color_led_cabine[a][0][0] + " " + color_led_cabine[a][0][1] + " " + color_led_cabine[a][0][2] + " "
                       + color_led_cabine[a][1][0] + " " + color_led_cabine[a][1][1] + " " + color_led_cabine[a][1][2]);
      }
    }
    else {
      color_led_cabine[0][0][0] = 0;
      color_led_cabine[0][0][1] = 0;
      color_led_cabine[0][0][2] = 0;
      color_led_cabine[0][1][0] = 0;
      color_led_cabine[0][1][1] = 0;
      color_led_cabine[0][1][2] = 0;
      //      en_rgb_cabine = false;
    }
    return;
  }

    if (DataIn.indexOf("FADE") > -1)
  {
    byte valor[15] = {0};
    int ind = -1;
    byte idx[15];
    byte counter = 0;
    for (int a = 0; a < DataIn.length(); a++) {
      valor[a] = DataIn.indexOf(",", ind + 1);
      ind = valor[a];
      if (counter > 0) {
        idx[a - 1] = constrain(DataIn.substring(valor[a - 1] + 1, valor[a]).toInt(), 0, 255);
        //        Serial.println((String)(a - 1) + " " + idx[a - 1]);
      }
      counter ++;
      if (valor[a] == DataIn.lastIndexOf(",")) {
        idx[a] = constrain(DataIn.substring(valor[a] + 1, DataIn.length() - 1).toInt(), 0, 255);
        //        Serial.println((String)a + " " + idx[a]);
        break;
      }
    }


    byte alterna1 = 0;//alterna canal rgb
    byte alterna2 = 0; //alterna r,g,b
    if (counter > 2) {
      if (counter <= 3) {
        for (int a = 0; a < counter; a++) { //counter inicia em 2 para omitir o indexador
          Serial.println((String)"Canal " + idx[0] + " " +  alterna1 + " " + alterna2 + " " + idx[a]);
          Serial.println((String)"Canal " + idx[0] + " " +  (alterna1 + 1) + " " + alterna2 + " " + idx[a]);
          color_led_cabine[0][alterna1][alterna2] = idx[a];
          color_led_cabine[0][alterna1  + 1][alterna2] = idx[a];
          if (alterna2 ==  2) {
            alterna2 = 0;
            alterna1 ++;
          }
          else {
            alterna2 ++;
          }
          //            if (a == 3)break; // break independente da quantidade de numeros
        }
      }

      else {
        // MANTÉM A COR ESTÁTICA SE VIER APENAS AS TRES CORES
        for (int a = 0; a < counter; a++) { //counter inicia em 2 para omitir o indexador
          //            Serial.println((String)" " +  alterna1 + " " + alterna2 + " " + idx[a]);


          color_led_cabine[0][alterna1][alterna2] = idx[a];
          if (alterna2 ==  2) {
            alterna2 = 0;
            alterna1 ++;
          }
          else {
            alterna2 ++;
          }
          if (a == 6) {

            break; // break independente da quantidade de numeros
          }
        }
      }

      Serial.println();

      new_rgb[cabine_r_i] = color_led_cabine[0][0][0];
      new_rgb[cabine_g_i] = color_led_cabine[0][0][1];
      new_rgb[cabine_b_i] = color_led_cabine[0][0][2];
      en_rgb_cabine = true;
      for (int a = 0; a < sizeof(color_led_cabine) / sizeof(color_led_cabine[0]); a++) {
        Serial.println((String)"Status led cabine " + color_led_cabine[a][0][0] + " " + color_led_cabine[a][0][1] + " " + color_led_cabine[a][0][2] + " "
                       + color_led_cabine[a][1][0] + " " + color_led_cabine[a][1][1] + " " + color_led_cabine[a][1][2]);
      }
    }
    else {
      color_led_cabine[0][0][0] = 0;
      color_led_cabine[0][0][1] = 0;
      color_led_cabine[0][0][2] = 0;
      color_led_cabine[0][1][0] = 0;
      color_led_cabine[0][1][1] = 0;
      color_led_cabine[0][1][2] = 0;
      //      en_rgb_cabine = false;
    }
    return;
  }

  if (DataIn.indexOf("ANA") > -1)
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
    byte led_value = map(intensidade, 0, 100, 0, 255);
    if (canal == painel_frontal_i) {
      new_rgb[painel_frontal_i] = led_value;
    }
    else if (canal == painel_externo_i) {
      new_rgb[painel_externo_i] = led_value;
    }
    else if (canal == piso_i) {
      new_rgb[piso_i] = led_value;
    }
    else if (canal == laterais_i) {
      new_rgb[laterais_i] = led_value;
    }
    else if (canal == sanca_meio_i) {
      new_rgb[sanca_meio_i] = led_value;
    }
    else if (canal == sanca_canto_i) {
      new_rgb[sanca_canto_i] = led_value;
    }

    else {
      int value = map(intensidade, 0, 100, dim_minimo, dim_maximo);
      powers[canal - 1] = value;
      Serial.print("DIM,");
      Serial.print(canal);
      Serial.print(",");
      Serial.println(intensidade);
    }
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
  else if (DataIn.substring(0, 8) == "CONTROLE" && !cerimonia)
  {
    controle_remoto();
  }


  else if (DataIn.indexOf("T_RGB") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int value = DataIn.substring(index1 + 1, index2).toInt();
    tempo_dimmer = value;
    timer_rgb.setTimeout(tempo_dimmer);
    Serial.print("TEMPO RGB = ");
    Serial.println(tempo_dimmer);

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
    Serial.println("LED_URNA");

  }

  else if (DataIn.indexOf("CHUVAPETALAS") == 0)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', index1 + 1);
    int index3 = DataIn.indexOf('index1', DataIn.length() - 2);
    long time_petalas = DataIn.substring(index2 + 1, index3).toInt();
    time_petalas = time_petalas * 1000;
    esteira.setTimeout(time_petalas);
    Serial.println("CHUVAPETALAS");
    en_esteira = true;
    esteira.restart();
  }

  //  else if (DataIn.substring(0, 9) == "SEQUENCIA")
  //  {
  //    gatilho_sequencia = true;
  //    completa = true;
  //    sincronizado = true;
  //    etapa_02 = true;
  //
  //  }
  else if (DataIn.substring(0, 9) == "ABREPORTA")
  {
    abreporta = true;
    acionado = true;
  }
  else if (DataIn.substring(0, 5) == "START")
  {

    completa = true;
    sincronizado = true;
    //gatilho_cerimonia = true;
    //    if (digitalRead(SENSOR_MG) == NAO_ATUADO) {
    //      Serial.println("STARTED");
    //      // Completa();
    //      Serial.flush();
    //    }
    //    else {
    //      Serial.println("NAO PRONTO");
    //    }
  }
  else if (DataIn.substring(0, 10) == "GUARDAMESA")
  {
    guardamesa = true;
    acionado = true;
  }

  else if (DataIn.substring(0, 5) == "EXPOE")
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

  else if (DataIn.substring(0, 5) == "FECHA")
  {
    fechaporta = true;
    acionado = true;
  }

  else if (DataIn.substring(0, 9) == "FADE_LAMP")
  {
    fade_lamp = !fade_lamp;
  }

  else if (DataIn.indexOf("DEBUG") > -1)
  {
    int index1 = DataIn.indexOf(',');
    int index2 = DataIn.indexOf(',', DataIn.length() - 2);
    long dbg = DataIn.substring(index1 + 1, index2).toInt();
    dbg = constrain(dbg, 0, 1);
    debug = dbg;
  }
    if ((DataIn.charAt(0) == '{') && (DataIn.charAt(DataIn.length() - 2) == '}'))  // ÚLTIMO CARACTERE + NEW LINE PODE SER NECESSÁRIO MUDAR PARA h-1
  {
    atribui_dimmer_rapido();
    return;
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
