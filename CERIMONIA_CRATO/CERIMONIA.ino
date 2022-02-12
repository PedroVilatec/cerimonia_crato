void entrega_cinzas() {
  if (entrega) {
    if (etapa_02) {

      abreporta = true;
      acionado = true;
      etapa_03 = true;
    }

    if (etapa_03) {
      etapa_02 = false;
      if (porta_aberta) {
        porta_aberta = false;
        expoemesa = true;
        acionado = true;
        etapa_03 = false;
        etapa_04 = true;
        timer_start_petalas_entrega.setTimeout(25000);
        timer_start_petalas_entrega.restart();
      }
    }
    if (etapa_04) {
      if (timer_start_petalas_entrega.onRestart()) {
          Serial.println("ESTEIRA LIGADA");
          en_esteira = true;
          timerEsteira.restart();
          timer_start_petalas_entrega.stop();
      }

        if (mesa_exposta) {
          mesa_exposta = false;
          fechaporta = true;
          acionado = true;
          etapa_04 = false;
          etapa_05 = true;
          //        powers[9] = 128; //apaga led foco urna
        }
      }
      if (etapa_05) {
        if (porta_fechada) {
          porta_fechada = false;
          //############## APAGA O LED DA CABINE #############
          new_rgb[cabine_r_i] = 0;
          new_rgb[cabine_g_i] = 0;
          new_rgb[cabine_b_i] = 0;
          efeito_rgb_cabine_1 = false;
          efeito_rgb_cabine_2 = false;
          efeito_rgb_cabine_3 = false;
          //##################################################
          etapa_05 = false;
          etapa_06 = true;
          completa = false; // encerra o loop da funcao
          entrega = false;
          button_counter = 0;
          mem_cena = true;
          //##################CONTADOR DE ENTREGAS REALIZADAS#############
          long valor_original = (EEPROM.read(1021) * 256) + EEPROM.read(1022);
          valor_original ++;
          EEPROM.write(1021, valor_original / 256);
          EEPROM.write(1022, valor_original % 256);
          Serial.print("TOTAL DE ENTREGAS DE CINZAS REALIZADAS = ");
          Serial.println((EEPROM.read(1021) * 256) + EEPROM.read(1022));
          //################################################################
        }
      }
    }
  }

void cerimonia_completa() {
  if (completa) {

    if (etapa_02) {

      abreporta = true;
      acionado = true;
      etapa_03 = true;
      efeito_1 = true;
    }

    if (etapa_03) {
      etapa_02 = false;
      if (porta_aberta) {
        porta_aberta = false;
        guardamesa = true;
        acionado = true;
        etapa_03 = false;
        etapa_04 = true;
      }
    }
    if (etapa_04) {
      if (mesa_guardada) {
        mesa_guardada = false;
        fechaporta = true;
        acionado = true;
        etapa_04 = false;
        etapa_05 = true;
      }
    }
    if (etapa_05) {
      if (porta_fechada) {
        porta_fechada = false;
        //############## APAGA O LED DA CABINE #############
        new_rgb[cabine_r_i] = 0;
        new_rgb[cabine_g_i] = 0;
        new_rgb[cabine_b_i] = 0;
          efeito_rgb_cabine_1 = false;
          efeito_rgb_cabine_2 = false;
          efeito_rgb_cabine_3 = false;        
        //##################################################
        etapa_05 = false;
        etapa_06 = true;
        completa = false; // encerra o loop da funcao
        entrega = false;
        button_counter = 0;
        mem_cena = true;
        //##################CONTADOR DE CERIMONIAS REALIZADAS#############
        long valor_original = (EEPROM.read(1023) * 256) + EEPROM.read(1024);
        valor_original ++;
        EEPROM.write(1023, valor_original / 256);
        EEPROM.write(1024, valor_original % 256);
        Serial.print("TOTAL DE CERIMONIAS REALIZADAS = ");
        Serial.println((EEPROM.read(1023) * 256) + EEPROM.read(1024));
        //################################################################
      }
    }
  }
}
