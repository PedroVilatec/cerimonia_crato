void zero_cross_detect() {
  zero_cross = true;
  for (byte i = 0; i < canais_dim; i++) {
    valor_anterior[i] = 0;
    if (dims[i] > dim_minimo  ) {
      digitalWrite(AC_pin[i], HIGH);
    }
  }
}
void dim_check() {
  if (zero_cross == true) {

    if (valor_anterior[0] >= dims[0]) {
      if (dims[0] < 128) {
        digitalWrite(AC_pin[0], LOW);
      }
      valor_anterior[0] = 0;
    }
    else {
      valor_anterior[0]++;
    }

    if (valor_anterior[1] >= dims[1]) {
      // For each one of the outputs
      if (dims[1] < 128) {
        digitalWrite(AC_pin[1], LOW);
      }
      valor_anterior[1] = 0;
    }
    else {
      valor_anterior[1]++;
    }
    if (valor_anterior[2] >= dims[2]) {
      if (dims[2] < 128) {
        digitalWrite(AC_pin[2], LOW);
      }
      valor_anterior[2] = 0;
    }
    else {
      valor_anterior[2]++;
    }
    if (valor_anterior[3] >= dims[3]) {
      if (dims[3] < 128) {
        digitalWrite(AC_pin[3], LOW);
      }
      valor_anterior[3] = 0;
    }
    else {
      valor_anterior[3]++;
    }

    if (valor_anterior[4] >= dims[4]) {
      if (dims[4] < 128) {
        digitalWrite(AC_pin[4], LOW);
      }
      valor_anterior[4] = 0;
    }
    else {
      valor_anterior[4]++;
    }
    if (valor_anterior[5] >= dims[5]) {
      if (dims[5] < 128) {
        digitalWrite(AC_pin[5], LOW);
      }
      valor_anterior[5] = 0;
    }
    else {
      valor_anterior[5]++;
    }
    if (valor_anterior[6] >= dims[6]) {
      if (dims[6] < 128) {
        digitalWrite(AC_pin[6], LOW);
      }
      valor_anterior[6] = 0;
    }
    else {
      valor_anterior[6]++;
    }
    if (valor_anterior[7] >= dims[7]) {
      if (dims[7] < 128) {
        digitalWrite(AC_pin[7], LOW);
      }
      valor_anterior[7] = 0;
    }
    else {
      valor_anterior[7]++;
    }

    if (valor_anterior[8] >= dims[8]) {
      if (dims[8] < 128) {
        digitalWrite(AC_pin[8], LOW);
      }
      valor_anterior[8] = 0;
    }
    else {
      valor_anterior[8]++;
    }
    if (valor_anterior[9] >= dims[9]) {
      if (dims[9] < 128) {
        digitalWrite(AC_pin[9], LOW);
      }
      valor_anterior[9] = 0;
    }
    else {
      valor_anterior[9]++;
    }
  }
}
void timeDimmer() {
  if (timerDim.onRestart()) {
    for (byte i = 0; i < canais_dim; i++) {
      if ( portas[i] < powers[i]) {

        portas[i]++;
        //if (i == 7 && portas[i] == 30) portas
        dims[i] = portas[i];
        en_eeprom_dim[i] = true;
      }
      if ( portas[i] > powers[i]) {
        portas[i]--;
        dims[i] = portas[i];
        en_eeprom_dim[i] = true;

      }
      if (portas[i] == powers[i] && en_eeprom_dim[i] == true) {
        if (mem_cena) {
          EEPROM.write(AC_pin[i], portas[i]);
        }
        //        Serial.print("Gravou eeprom canal: ");
        //        Serial.print(AC_pin[i]);
        //        Serial.print(" valor: ");
        //        Serial.println(portas[i]);
        en_eeprom_dim[i] = false;

      }
    }
  }
}

void atribui_dimmer() {
  int lastIndex = 0;
  byte counter = 0;
  int pieces[canais_dim];
  DataIn.remove(0, 1);
  DataIn.remove(DataIn.length() - 1, DataIn.length());
  //DataIn.remove(DataIn.length() - 1 , DataIn.length());
  for (int i = 0; i < DataIn.length(); i++) {
    if (DataIn.substring(i, i + 1) == ",") {
      pieces[counter] = DataIn.substring(lastIndex, i).toInt();
      //      Serial.println(pieces[counter]);
      if (pieces[counter] == "") {
        //        Serial.print("Algum campo vazio ");
        //        Serial.println(pieces[counter]);
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
      //      Serial.println(pieces[counter]);
      if (pieces[counter] == "") {
        DataIn = "";
        return;
      }
    }
  }
  if (counter == 10) {
    if (cena_inicial_cerimonia == true) {
      button_counter ++;
      cena_inicial_cerimonia = false;
    }
    byte led_value = 0;
    byte counter_dim = 0;
    for (byte i = 0; i < canais_dim; i++) {
      //      Serial.println((String)i + " " + pieces[i]);
      //      {painel_frontal, painel_externo, piso, laterais, sanca_meio, sanca_canto, cabine_r, cabine_g, cabine_b};
      byte led_value = map(pieces[i], 0, 100, 0, 255);
      if (i == painel_frontal_i) {
        new_rgb[painel_frontal_i] = led_value;
      }
      else if (i == painel_externo_i) {
        new_rgb[painel_externo_i] = led_value;
      }
      else if (i == piso_i) {
        new_rgb[piso_i] = led_value;
      }
      else if (i == laterais_i) {
        new_rgb[laterais_i] = led_value;
      }
      else if (i == sanca_meio_i) {
        new_rgb[sanca_meio_i] = led_value;
      }
      else if (i == sanca_canto_i) {
        new_rgb[sanca_canto_i] = led_value;
      }
      else {
        pieces[counter_dim] = constrain(pieces[i], 0, 100);
        pieces[counter_dim] = map(pieces[i], 0, 100, 0, 128);
        powers[counter_dim] = pieces[i];
        counter_dim ++;
      }
    }
  }
}

void atribui_dimmer_rapido() {
  int lastIndex = 0;
  byte counter = 0;
  int pieces[canais_dim];
  DataIn.remove(0, 1);
  DataIn.remove(DataIn.length() - 1, DataIn.length());
  //DataIn.remove(DataIn.length() - 1 , DataIn.length());
  for (int i = 0; i < DataIn.length(); i++) {
    if (DataIn.substring(i, i + 1) == ",") {
      pieces[counter] = DataIn.substring(lastIndex, i).toInt();
      //      Serial.println(pieces[counter]);
      if (pieces[counter] == "") {
        //        Serial.print("Algum campo vazio ");
        //        Serial.println(pieces[counter]);
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
      //      Serial.println(pieces[counter]);
      if (pieces[counter] == "") {
        DataIn = "";
        return;
      }
    }
  }
  if (counter == 10) {
    if (cena_inicial_cerimonia == true) {
      button_counter ++;
      cena_inicial_cerimonia = false;
    }
    byte led_value = 0;
    byte counter_dim = 0;
    for (byte i = 0; i < canais_dim; i++) {
      //      Serial.println((String)i + " " + pieces[i]);
      //      {painel_frontal, painel_externo, piso, laterais, sanca_meio, sanca_canto, cabine_r, cabine_g, cabine_b};
      byte led_value = map(pieces[i], 0, 100, 0, 255);
      if (i == painel_frontal_i) {
        new_rgb[painel_frontal_i] = led_value;
      }
      else if (i == painel_externo_i) {
        new_rgb[painel_externo_i] = led_value;
      }
      else if (i == piso_i) {
        new_rgb[piso_i] = led_value;
      }
      else if (i == laterais_i) {
        new_rgb[laterais_i] = led_value;
      }
      else if (i == sanca_meio_i) {
        new_rgb[sanca_meio_i] = led_value;
      }
      else if (i == sanca_canto_i) {
        new_rgb[sanca_canto_i] = led_value;
      }
      else {
        pieces[counter_dim] = constrain(pieces[i], 0, 100);
        pieces[counter_dim] = map(pieces[i], 0, 100, dim_minimo, dim_maximo);
        powers[counter_dim] = pieces[i];
        portas[counter_dim] = pieces[i];
        dims[counter_dim] = pieces[i];
        counter_dim ++;
      }
    }
  }
}
