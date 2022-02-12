//PINO 44 - 255 r
//PINO 45 - 255 g  CRUZ
//PINO 46 - 255 b

//PINO 4 r
//PINO 3 g  CABINE
//PINO 2 b
//
//PINO 7 r
//PINO 6 g  Sanca teto 1
//PINO 5 b

//PINO 8 r
//PINO 9 g  Sanca teto 2
//PINO 10 b

//PINO 11 r
//PINO 12 g  Paredes
//PINO 13 b

void pulsa() {
//(0,0,255,0,0,255,0,0,255,0,0,255,0,0,255)

//(255,0,0,255,0,0,255,0,0,255,0,0,255,0,0)
//(0,255,0,0,255,0,0,255,0,0,255,0,0,255,0)
//(128,128,128,128,128,128,128,128,128,128,128,128,128,128,128)
//(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
//(50,50,50,50,50,50,50,50,50,50,50,50,50,50,50)
//(40,40,40,40,40,40,40,40,40,40,40,40,40,40,40)
//(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255)
//(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14)
  //[100,0,0,100,0,0,10,0,0,10,0,0]
  //<0,0,0,0,0,0,0,0,0,0>
  //<100,100,100,100,100,100,100,100,100,100>
  //<75,75,75,75,75,75,75,75,75,75>
  //<95,95,95,95,95,95,95,95,95,95>
  //<90,90,90,90,90,90,90,90,90,90>
  //<40,40,40,40,40,40,40,40,40,40>
  //<20,20,20,20,20,20,20,20,20,20>
  //<10,10,10,10,10,10,10,10,10,10>
  //<6,6,6,6,6,6,6,6,6,6>
  // branco (255,255,255,255,255,255,255,255,255)
  // vermelho (255,0,0,255,0,0,255,0,0)
  // verde (0,255,0,0,255,0,0,255,0)
  // azul  (0,0,255,0,0,255,0,0,255)
  //(100,100,100,100,100,100,100,100,100)
  //(10,10,10,10,10,10,10,10,10)
  //(1,1,1,1,1,1,1,1,1)
  //(0,0,0,0,0,0,0,0,0)

  if (timer_rgb.onRestart()) {
    //    if ( pwm_atual_led_esteira < new_pwm_led_esteira) {
    //      pwm_atual_led_esteira ++;
    //      if (pwm_atual_led_esteira > 0) {
    //        digitalWrite(en_led_esteira, 1);
    //
    //      }
    //      else {
    //        digitalWrite(en_led_esteira, 0);
    //      }
    //      analogWrite(pwm_led_esteira, pwm_atual_led_esteira);
    //    }
    //    if ( pwm_atual_led_esteira > new_pwm_led_esteira) {
    //      pwm_atual_led_esteira --;
    //      if (pwm_atual_led_esteira > 0) {
    //        digitalWrite(en_led_esteira, 1);
    //      }
    //      else {
    //        digitalWrite(en_led_esteira, 0);
    //      }
    //      analogWrite(pwm_led_esteira, pwm_atual_led_esteira);
    //
    //    }


    for (byte i = 0; i < canais_rgb; i++) {

      for (byte f = 0; f < 3; f++) {
       //Serial.println((String)"["+i+"]["+f+"]");
        if ( old_rgb[i][f] < new_rgb[i][f]) {
          old_rgb[i][f]++;
          atual_rgb[i][f] = old_rgb[i][f];
          analogWrite(leds_rgb[i][f], atual_rgb[i][f]);
          en_eeprom_rgb[i][f] = true;
          //Serial.println(atual_rgb[i][f]);
        }
        if ( old_rgb[i][f] > new_rgb[i][f]) {
          old_rgb[i][f]--;
          atual_rgb[i][f] = old_rgb[i][f];
          analogWrite(leds_rgb[i][f], atual_rgb[i][f]);
          en_eeprom_rgb[i][f] = true;
          //Serial.println(atual_rgb[i][f]);
        }


        if (old_rgb[i][f] == new_rgb[i][f] && en_eeprom_rgb[i][f] == true) {

          if (leds_rgb[i][f] == cabine_r || leds_rgb[i][f] == cabine_g || leds_rgb[i][f] == cabine_b) {
            // não salva o status se o led for o da cabine
            if (old_rgb[i][0] == new_rgb[i][0] && old_rgb[i][1] == new_rgb[i][1] && old_rgb[i][2] == new_rgb[i][2]) {//new_rgb e old_rgb -> cabine
              if (efeito_rgb_cabine_1) {
                //Serial.println((String)cabine_efeito_1[0] + " " + cabine_efeito_1[1] + " " + cabine_efeito_1[2]);
                new_rgb[i][0] = cabine_efeito_1[0];
                new_rgb[i][1] = cabine_efeito_1[1];
                new_rgb[i][2] = cabine_efeito_1[2];
                efeito_rgb_cabine_1 = false;
                efeito_rgb_cabine_2 = true;
              }
              else if (efeito_rgb_cabine_2) {
                //Serial.println((String)cabine_efeito_2[0] + " " + cabine_efeito_2[1] + " " + cabine_efeito_2[2]);
                new_rgb[i][0] = cabine_efeito_2[0];
                new_rgb[i][1] = cabine_efeito_2[1];
                new_rgb[i][2] = cabine_efeito_2[2];
                efeito_rgb_cabine_2 = false;
                efeito_rgb_cabine_3 = true;
              }
              else if (efeito_rgb_cabine_3) {
                //Serial.println((String)cabine_efeito_3[0] + " " + cabine_efeito_3[1] + " " + cabine_efeito_3[2]);
                new_rgb[i][0] = cabine_efeito_3[0];
                new_rgb[i][1] = cabine_efeito_3[1];
                new_rgb[i][2] = cabine_efeito_3[2];
                efeito_rgb_cabine_3 = false;
                efeito_rgb_cabine_1 = true;
              }
            }



          }
          else {

            if (mem_cena) {
              EEPROM.write(leds_rgb[i][f], old_rgb[i][f]);
//              Serial.print("Gravou eeprom canal: ");
//              Serial.print(leds_rgb[i][f]);
//              Serial.print(" valor: ");
//              Serial.println(old_rgb[i][f]);
            }
            //####################################################################
            if (en_rgb_geral == true) {
              bool todos_iguais = false;
              bool breaker = false;
              for (byte g = 0; g < canais_rgb; g++) {
                if (breaker) {
                  todos_iguais = false;
                  break;
                }
                for (byte h = 0; h < 3; h++) {
                  if (old_rgb[g][h] != new_rgb[g][h]) {
                    breaker = true;
                    break;
                  }
                }
                todos_iguais = true;
              }
              if (todos_iguais) {

                if (efeito_1) {
                  //timer_rgb.setTimeout(40);// caso altere o valor para menos
                  new_rgb[2][0] = EEPROM.read(100);//parede
                  new_rgb[2][1] = EEPROM.read(101);//parede
                  new_rgb[2][2] = EEPROM.read(102);//parede
                  new_rgb[3][0] = EEPROM.read(103);//cruz
                  new_rgb[3][1] = EEPROM.read(104);//cruz
                  new_rgb[3][2] = EEPROM.read(105);//cruz
                  efeito_1 = false;
                }
                else {
                  new_rgb[2][0] = EEPROM.read(106);//parede
                  new_rgb[2][1] = EEPROM.read(107);//parede
                  new_rgb[2][2] = EEPROM.read(108);//parede
                  new_rgb[3][0] = EEPROM.read(109);//cruz
                  new_rgb[3][1] = EEPROM.read(110);//cruz
                  new_rgb[3][2] = EEPROM.read(111);//cruz
                  efeito_1 = true;
                }

              }
            }
            //##########################################################################

          }
          en_eeprom_rgb[i][f] = false;
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
    }// fim do for
  }

}

void atribui_rgb() {
  //Serial.println("Atribui rgb");
  int lastIndex = 0;
  byte counter = 0;
  int pwm_value = 0;
  DataIn.remove(0, 1);
  DataIn.remove(DataIn.length() - 1, DataIn.length());
  byte Loop = 0;
  for (int i = 0; i < DataIn.length(); i++) {
    if (DataIn.substring(i, i + 1) == ",") {
      pwm_value = DataIn.substring(lastIndex, i).toInt();
      pwm_value = constrain(pwm_value, 0, 255);
      //Serial.println(String(counter/3) + "  "+ String(Loop));
      new_rgb[counter/3][Loop] = pwm_value;
      lastIndex = i + 1;
      counter++;
      if (Loop == 2){
        Loop = 0;
      }
      else
      {
        Loop ++;
      }
      
    }
    if (i == DataIn.length() - 1) {
      //Serial.println(String(counter/3) + "  "+ String(Loop));
      pwm_value = DataIn.substring(lastIndex, i).toInt();
      pwm_value = constrain(pwm_value, 0, 255);      
      new_rgb[counter/3][Loop] = pwm_value;
      counter++;
    }
  }
  //if(CEMITERIO == "KRAUSE"){
  //  if (counter == canais_rgb) {
  //    for (byte i = 0; i < 3; i++) {
  //      pieces[i] = constrain(pieces[i], 0, 255);
  //      //      if (i != 3 && i != 4 && i != 5) {
  //      new_rgb[i] = pieces[i];
  //      //Serial.println(new_rgb[i]);
  //      //      }
  //    }
  //  }
  //}

  //if(CEMITERIO == "VIDA"){
  //    if (counter == 9) {
  //    for (byte i = 0; i < canais_rgb; i++) {
  //      pieces[i] = constrain(pieces[i], 0, 255);
  //      //      if (i != 3 && i != 4 && i != 5) {
  //      new_rgb[i] = pieces[i];
  //      //Serial.println(new_rgb[i]);
  //      //      }
  //    }
  //  }
  //}
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
        Serial.println("Algum campo vazio");
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
      EEPROM.write(100 + i, pieces[i]);
      Serial.println((String)"EEPROM.read " + EEPROM.read(100 + i));
      //Serial.println(new_rgb[i]);
      //      }
    }
  }
}
