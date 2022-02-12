void pulsa() {
  //[0,128,128,128,50,5,0,5,5,5,5,5] opcao 1
  //[0,128,100,128,50,0,0,10,10,10,20,0] opcao 2
  //[0,0,60,0,0,60,0,0,10,0,0,10]
  //[255,0,100,100,100,100,10,10,10,10,10,10]
  //<0,0,0,0,0,0,0,0,0,0>
  //<100,100,100,100,100,100,100,100,100,100>
  // branco (255,255,255,255,255,255,255,255,255)
  // vermelho (255,0,0,255,0,0,255,0,0)
  // verde (0,255,0,0,255,0,0,255,0)
  // azul  (0,0,255,0,0,255,0,0,255)
  //(100,100,100,100,100,100,100,100,100)
  //(10,10,10,10,10,10,10,10,10)
  //(1,1,1,1,1,1,1,1,1)
  //(0,0,0,0,0,0,0,0,0)

  if (timer_rgb.onRestart()) {
    if ( pwm_atual_led_esteira < new_pwm_led_esteira) {
      pwm_atual_led_esteira ++;
      analogWrite(pwm_led_esteira, pwm_atual_led_esteira);
    }
    if ( pwm_atual_led_esteira > new_pwm_led_esteira) {
      pwm_atual_led_esteira --;
      analogWrite(pwm_led_esteira, pwm_atual_led_esteira);

    }


    for (byte i = 0; i < canais_rgb; i++) {
      if ( old_rgb[i] < new_rgb[i]) {
        old_rgb[i]++;
        atual_rgb[i] = old_rgb[i];
        analogWrite(leds_rgb[i], atual_rgb[i]);
        en_eeprom_rgb[i] = true;
        //        Serial.println(atual_rgb[i]);
      }
      else if ( old_rgb[i] > new_rgb[i]) {
        old_rgb[i]--;
        atual_rgb[i] = old_rgb[i];
        analogWrite(leds_rgb[i], atual_rgb[i]);
        en_eeprom_rgb[i] = true;
        //        Serial.println(atual_rgb[i]);
      }
      else if (old_rgb[i] == new_rgb[i]) {
        if (i <= 5) {
          if (fade_1[i] != fade_2[i]) {
            if (new_rgb[i] != fade_1[i]) new_rgb[i] = fade_1[i];
            else if (new_rgb[i] != fade_2[i]) new_rgb[i] = fade_2[i];
          }
        }
      }

      if (old_rgb[i] == new_rgb[i] && en_eeprom_rgb[i] == true) {

        if (i == cabine_r_i || i == cabine_g_i || i == cabine_b_i) {
          // não salva o status se o led for o da cabine
          //          if (en_rgb_cabine) {
          if (old_rgb[cabine_r_i] == new_rgb[cabine_r_i] && old_rgb[cabine_g_i] == new_rgb[cabine_g_i] && old_rgb[cabine_b_i] == new_rgb[cabine_b_i]) {
            if (efeito_1) {
              new_rgb[cabine_r_i] = color_led_cabine[0][0][0];
              new_rgb[cabine_g_i] = color_led_cabine[0][0][1];
              new_rgb[cabine_b_i] = color_led_cabine[0][0][2];
              efeito_1 = false;
              //              Serial.println((String)"Status led cabine false " + new_rgb[cabine_r_i] + " " + new_rgb[cabine_g_i] + " " + new_rgb[cabine_b_i]);

            }
            else {
              new_rgb[cabine_r_i] = color_led_cabine[0][1][0];
              new_rgb[cabine_g_i] = color_led_cabine[0][1][1];
              new_rgb[cabine_b_i] = color_led_cabine[0][1][2];
              efeito_1 = true;
              //              Serial.println((String)"Status led cabine true " + new_rgb[cabine_r_i] + " " + new_rgb[cabine_g_i] + " " + new_rgb[cabine_b_i]);
            }
          }
          //          }


        }
        else {

          if (mem_cena) {
//            EEPROM.write(leds_rgb[i], old_rgb[i]);
            //                        Serial.println((String)"pino = " +leds_rgb[i] + " Valor = "+old_rgb[i]);
          }
          //####################################################################
          if (en_rgb_geral == true) {
            if (old_rgb[painel_frontal_i] == new_rgb[painel_frontal_i] && old_rgb[painel_externo_i] == new_rgb[painel_externo_i] && old_rgb[piso_i] == new_rgb[piso_i] &&
                old_rgb[laterais_i] == new_rgb[laterais_i] && old_rgb[sanca_meio_i] == new_rgb[sanca_meio_i] && old_rgb[sanca_canto_i] == new_rgb[sanca_canto_i]) {
              //          if (efeito_1) {
              //            new_rgb[0] = 100;
              //            new_rgb[1] = 0;
              //            new_rgb[2] = 0;
              //            new_rgb[6] = 100;
              //            new_rgb[7] = 0;
              //            new_rgb[8] = 0;
              //            efeito_1 = false;
              //          }
              //          else {
              //            new_rgb[0] = 10;
              //            new_rgb[1] = 0;
              //            new_rgb[2] = 0;
              //            new_rgb[6] = 10;
              //            new_rgb[7] = 0;
              //            new_rgb[8] = 0;
              //            efeito_1 = true;
              //          }

              if (efeito_1) {
                //timer_rgb.setTimeout(40);// caso altere o valor para menos
                new_rgb[painel_frontal_i] = fade_1[painel_frontal_i];
                new_rgb[painel_externo_i] = fade_1[painel_externo_i];
                new_rgb[piso_i] = fade_1[piso_i];
                new_rgb[laterais_i] = fade_1[laterais_i];
                new_rgb[sanca_meio_i] = fade_1[sanca_meio_i];
                new_rgb[sanca_canto_i] = fade_1[sanca_canto_i];
                efeito_1 = false;
              }
              else {
                new_rgb[painel_frontal_i] = fade_2[painel_frontal_i];
                new_rgb[painel_externo_i] = fade_2[painel_externo_i];
                new_rgb[piso_i] = fade_2[piso_i];
                new_rgb[laterais_i] = fade_2[laterais_i];
                new_rgb[sanca_meio_i] = fade_2[sanca_meio_i];
                new_rgb[sanca_canto_i] = fade_2[sanca_canto_i];
                efeito_1 = true;
              }

            }
          }
          //##########################################################################
          //          Serial.print("Gravou eeprom canal: ");
          //          Serial.print(leds_rgb[i]);
          //          Serial.print(" valor: ");
          //          Serial.println(old_rgb[i]);
        }
        en_eeprom_rgb[i] = false;
      }
    }
  }
}

void atribui_rgb() {
  int lastIndex = 0;
  byte counter = 0;
  int pieces[canais_rgb];
  DataIn.remove(0, 1);
  DataIn.remove(DataIn.length() - 1, DataIn.length());
  //DataIn.remove(DataIn.length() - 1 , DataIn.length());
  for (int i = 0; i < DataIn.length(); i++) {
    if (DataIn.substring(i, i + 1) == ",") {
      pieces[counter] = DataIn.substring(lastIndex, i).toInt();
      //Serial.println(pieces[counter]);
      if (pieces[counter] == "") {
        Serial.println("Algum campo vazio");
        DataIn = "";
        return;
      }
      lastIndex = i + 1;
      counter++;
    }
    if (i == DataIn.length() - 1) {
      // Grab the last part of the string from the lastIndex to the end
      pieces[counter] = DataIn.substring(lastIndex, i).toInt();
      counter++;
      //Serial.println(pieces[counter]);
      if (pieces[counter] == "") {
        DataIn = "";
        return;
      }
    }
  }

  if (counter == 9) {
    for (byte i = 0; i < canais_rgb; i++) {
      pieces[i] = constrain(pieces[i], 0, 255);
      //      if (i != 3 && i != 4 && i != 5) {
      new_rgb[i] = pieces[i];
      //Serial.println(new_rgb[i]);
      //      }
    }
  }
}

void set_pulso_rgb() {
  Serial.println("pulso_rgb");
  en_rgb_geral = true;
  int lastIndex = 0;
  byte counter = 0;
  int pieces[15];
  DataIn.remove(0, 1);
  // Serial.println(DataIn);
  DataIn.remove(DataIn.length() - 1, DataIn.length());

  //DataIn.remove(DataIn.length() - 1 , DataIn.length());
  // Serial.printlvn(DataIn);
  for (int i = 0; i < DataIn.length(); i++) {
    if (DataIn.substring(i, i + 1) == ",") {
      pieces[counter] = DataIn.substring(lastIndex, i).toInt();
      //Serial.println(pieces[counter]);
      if (pieces[counter] == "") {
        //Serial.println("Algum campo vazio");
        DataIn = "";
        return;
      }
      lastIndex = i + 1;
      counter++;
      //Serial.print("counter ");
      //Serial.println(counter);
    }
    if (i == DataIn.length() - 1) {
      // Grab the last part of the string from the lastIndex to the end
      //String dados = DataIn.substring(lastIndex, i);
      // Serial.print("dados ");
      // Serial.println(dados);


      pieces[counter] = DataIn.substring(lastIndex, i).toInt();
      counter ++;
      //Serial.println(pieces[counter]);

      if (pieces[counter] == "") {
        DataIn = "";
        Serial.println("Dados invalidos");
        return;
      }
    }
  }
  //  Serial.print("counter ");
  //  Serial.println(counter);
  if (counter  == 12) {

    for (byte i = 0; i < counter; i++) {
      //      Serial.println(i);
      //      pieces[i] = constrain(pieces[i], 0, 255);
      //      Serial.println(pieces[i]);
      if (i < 6) {
        fade_1[i] = pieces[i];
        new_rgb[i] = pieces[i];
        //        Serial.println((String)"Indice baixo "+ i +" valor "+ pieces[i]);
      }
      else if (i > 5) {
        fade_2[i - 6] = pieces[i];
        //        Serial.println((String)"Indice alto "+ (i - 6) +" valor "+ pieces[i]);
      }
      if (i == 11) {
        //      Serial.println("Break");
        break;
      }
    }
  }
}
