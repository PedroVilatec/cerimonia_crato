void pulsa() {
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
        if(pwm_atual_led_esteira > 0){
          digitalWrite(en_led_esteira, 1);
          
        }
        else{
          digitalWrite(en_led_esteira, 0);
        }
        analogWrite(pwm_led_esteira, pwm_atual_led_esteira);
      }
      if ( pwm_atual_led_esteira > new_pwm_led_esteira) {
        pwm_atual_led_esteira --;
        if(pwm_atual_led_esteira > 0){
          digitalWrite(en_led_esteira, 1);
        }
        else{
          digitalWrite(en_led_esteira, 0);
        }
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
      if ( old_rgb[i] > new_rgb[i]) {
        old_rgb[i]--;
        atual_rgb[i] = old_rgb[i];
        analogWrite(leds_rgb[i], atual_rgb[i]);
        en_eeprom_rgb[i] = true;
        //        Serial.println(atual_rgb[i]);
      }


      if (old_rgb[i] == new_rgb[i] && en_eeprom_rgb[i] == true) {

        if (i == cabine_r_i || i == cabine_g_i || i == cabine_b_i) {
          // não salva o status se o led for o da cabine
            if (old_rgb[cabine_r_i] == new_rgb[cabine_r_i] && old_rgb[cabine_g_i] == new_rgb[cabine_g_i] && old_rgb[cabine_b_i] == new_rgb[cabine_b_i]) {
              if (efeito_rgb_cabine_1) {
                new_rgb[cabine_r_i] = cabine_efeito_1[0];
                new_rgb[cabine_g_i] = cabine_efeito_1[1];
                new_rgb[cabine_b_i] = cabine_efeito_1[2];
                efeito_rgb_cabine_1 = false;
                efeito_rgb_cabine_2 = true;
              }
              else if (efeito_rgb_cabine_2) {
                new_rgb[cabine_r_i] = cabine_efeito_2[0];
                new_rgb[cabine_g_i] = cabine_efeito_2[1];
                new_rgb[cabine_b_i] = cabine_efeito_2[2];
                efeito_rgb_cabine_2 = false;
                efeito_rgb_cabine_3 = true;
              }
              else if (efeito_rgb_cabine_3) {
                new_rgb[cabine_r_i] = cabine_efeito_3[0];
                new_rgb[cabine_g_i] = cabine_efeito_3[1];
                new_rgb[cabine_b_i] = cabine_efeito_3[2];
                efeito_rgb_cabine_3 = false;
                efeito_rgb_cabine_1 = true;
              }
            }
          


        }
        else {
        
          if (mem_cena) {
            EEPROM.write(leds_rgb[i], old_rgb[i]);
            

//            Serial.println((String)"pino = " +leds_rgb[i] + " Valor = "+old_rgb[i]);
          }
//####################################################################
      if (en_rgb_geral == true) {
        if (old_rgb[parede_r_i] == new_rgb[parede_r_i] && old_rgb[parede_g_i] == new_rgb[parede_g_i] && old_rgb[parede_b_i] == new_rgb[parede_b_i] &&
            old_rgb[cruz_r_i] == new_rgb[cruz_r_i] && old_rgb[cruz_g_i] == new_rgb[cruz_g_i] && old_rgb[cruz_b_i] == new_rgb[cruz_b_i]) {
          if (efeito_1) {
            //timer_rgb.setTimeout(40);// caso altere o valor para menos
            new_rgb[parede_r_i] = EEPROM.read(100);
            new_rgb[parede_g_i] = EEPROM.read(101);
            new_rgb[parede_b_i] = EEPROM.read(102);
            new_rgb[cruz_r_i] = EEPROM.read(103);
            new_rgb[cruz_g_i] = EEPROM.read(104);
            new_rgb[cruz_b_i] = EEPROM.read(105);
            efeito_1 = false;
          }
          else {
            new_rgb[parede_r_i] = EEPROM.read(106);
            new_rgb[parede_g_i] = EEPROM.read(107);
            new_rgb[parede_b_i] = EEPROM.read(108);
            new_rgb[cruz_r_i] = EEPROM.read(109);
            new_rgb[cruz_g_i] = EEPROM.read(110);
            new_rgb[cruz_b_i] = EEPROM.read(111);
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
        //        new_rgb[i] = random(256);
        //
        //        if (i % 3 == 0 && ) {
        //          if (conta_red < 3) {
        //            conta_red = ++;
        //            new_rgb[i] = 255;
        //          }
        //
        //    }
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
  int lastIndex = 0;
  byte counter = 0;
  int pieces[12];
  DataIn.remove(0, 1);
  // Serial.println(DataIn);
  DataIn.remove(DataIn.length() - 1, DataIn.length());
  
  //DataIn.remove(DataIn.length() - 1 , DataIn.length());
 // Serial.println(DataIn);
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
  Serial.print("counter ");
Serial.println(counter);
  if (counter  == 12) {
    Serial.println();
    for (byte i = 0; i < counter; i++) {
      pieces[i] = constrain(pieces[i], 0, 255);
      //      if (i != 3 && i != 4 && i != 5) {
      EEPROM.write(100 + i,pieces[i]);
      Serial.println((String)"EEPROM.read " + EEPROM.read(100 + i));
      //Serial.println(new_rgb[i]);
      //      }
    }
  }
}
