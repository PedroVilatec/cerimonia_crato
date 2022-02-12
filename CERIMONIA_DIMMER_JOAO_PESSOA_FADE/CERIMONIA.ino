//
//void sequencia() {
//  if (completa && sincronizado && gatilho_sequencia && !entrega) {
//
//    if (etapa_02) {
//
//      abreporta = true;
//      acionado = true;
//      etapa_03 = true;
//      cerimonia = true;
//      en_rgb_cabine = true;
//      efeito_1 = true;
//    }
//
//    if (etapa_03) {
//      etapa_02 = false;
//      if (porta_aberta) {
//        porta_aberta = false;
//        guardamesa = true;
//        acionado = true;
//        etapa_03 = false;
//        etapa_04 = true;
//      }
//    }
//    if (etapa_04) {
//      if (mesa_guardada) {
//        mesa_guardada = false;
//        fechaporta = true;
//        acionado = true;
//        etapa_04 = false;
//        etapa_05 = true;
//        //        powers[9] = 128; //apaga led foco urna
//      }
//    }
//    if (etapa_05) {
//      if (porta_fechada) {
//        porta_fechada = false;
//        //############## APAGA O LED DA CABINE #############
//        en_rgb_cabine = false;
//        new_rgb[cabine_r_i] = 0;
//        new_rgb[cabine_g_i] = 0;
//        new_rgb[cabine_b_i] = 0;
//        //##################################################
//        etapa_05 = false;
//        etapa_06 = true;
//        completa = false; // encerra o loop da funcao
//        entrega = false;
//        sincronizado = false; // colocar na ultima etapa
//        cerimonia = false;//colocar na ultima etapa
//        gatilho_sequencia = false;
//        button_counter = 0;
//        mem_cena = true;
//        //##################CONTADOR DE CERIMONIAS REALIZADAS#############
//        long valor_original = (EEPROM.read(1023) * 256) + EEPROM.read(1024);
//        valor_original ++;
//        EEPROM.write(1023, valor_original / 256);
//        EEPROM.write(1024, valor_original % 256);
//        //################################################################
//      }
//    }
//  }
//}
//
