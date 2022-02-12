void setup() {
  Serial.begin(9600);
  //Serial.println("Botao desabilitado");
  if(CEMITERIO == "KRAUSE"){
  for (int a = 0; a < 5; a++) {
    pinMode(cortinas[a][0], OUTPUT);
    pinMode(cortinas[a][1], OUTPUT);
    digitalWrite(cortinas[a][0], HIGH);
    digitalWrite(cortinas[a][1], HIGH);
  }

  digitalWrite(69, 0); // liga habilita cortinas
  }
  for (byte i = 0; i < canais_dim; i++) {

    pinMode(AC_pin[i], OUTPUT);
    byte eeprom_read = EEPROM.read(AC_pin[i]);
    dims[i] = eeprom_read;
    powers[i] = eeprom_read;
    portas[i] = eeprom_read;

  }

  for (byte i = 0; i < canais_rgb; i++) {
    for (byte f = 0; f < 3; f++) {
      byte eepromValue = EEPROM.read(leds_rgb[i][f]);
      pinMode(leds_rgb[i][f], OUTPUT);
      if (leds_rgb[i][f] == cabine_r || leds_rgb[i][f] == cabine_g || leds_rgb[i][f] == cabine_b) {// não acende os leds da cabine
        analogWrite(leds_rgb[i][f], 0);
      }
      else {
        analogWrite(leds_rgb[i][f], eepromValue);
        //      Serial.println((String)"Led channel = "+leds_rgb[i][f]+" eeprom value = "+eepromValue);
        atual_rgb[i][f] = eepromValue;
        old_rgb[i][f] = eepromValue;
        new_rgb[i][f] = eepromValue;
      }

    }
  }

  pinMode(R_EN_PORTA, OUTPUT);
  digitalWrite(R_EN_PORTA, LOW);
  pinMode(acopl_esteira, INPUT);
  pinMode(ilum_man_cer, OUTPUT);
  if (EEPROM.read(ilum_man_cer) > 1) {
    EEPROM.write(ilum_man_cer, 1);
  }
  digitalWrite(ilum_man_cer, EEPROM.read(ilum_man_cer));
  pinMode(ilum_man_aux, OUTPUT);
  digitalWrite(ilum_man_aux, EEPROM.read(ilum_man_aux));
  pinMode(rele_som, OUTPUT);
  digitalWrite(rele_som, LOW);
  pinMode(L_EN_PORTA, OUTPUT);
  pinMode(PINO_FECHA, OUTPUT);
  pinMode(PINO_ABRE, OUTPUT);
  if (CEMITERIO == "VIDA") {
    digitalWrite(42, EEPROM.read(42));
  }
  pinMode(R_EN_MESA, OUTPUT);
  pinMode(L_EN_MESA, OUTPUT);
  pinMode(PINO_EXPOE, OUTPUT);
  pinMode(PINO_GUARDA, OUTPUT);
  pinMode(en_led_esteira, OUTPUT);
  pinMode(pwm_led_esteira, OUTPUT);
  digitalWrite(en_led_esteira, 0);
  pinMode(PINO_LIGA_ESTEIRA, OUTPUT);
  digitalWrite(PINO_LIGA_ESTEIRA, 0);
  pinMode(40, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(FUMACA, OUTPUT);
  pinMode(LUZ_CABINE, OUTPUT);

  pinMode(SENSOR_PA, INPUT);
  pinMode(SENSOR_PF, INPUT);
  pinMode(SENSOR_MG, INPUT);
  pinMode(SENSOR_ME, INPUT);

  //  Serial.print("SISTEMA INICIADO, TOTAL DE CERIMONIAS REALIZADAS = ");
  //  Serial.println((EEPROM.read(1023) * 256) + EEPROM.read(1024));
  analogWrite(PINO_ABRE, 0);
  analogWrite(PINO_FECHA, 0);
  digitalWrite(PINO_GUARDA, LOW);
  digitalWrite(PINO_EXPOE, LOW);

//  attachInterrupt(digitalPinToInterrupt(19), zero_cross_detect, RISING);
//  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
//  Timer1.attachInterrupt(dim_check, freqStep);


  //setPwmFrequency(PINO_PWM_ESTEIRA, 1024); //pino, divisor
  //setPwmFrequency(PINO_PWM_PORTA, 1024); //pino, divisor
  timer_pulso_controle_cortina.setTimeout(3000);
  timer.setTimeout(1000);
  timer_rgb.setTimeout(20);
  timer_pulsa_sala.setTimeout(10);
  timer_lamp.setTimeout(7000);
  timerCortinas.setTimeout(1000);
  timerEventos.setTimeout(1000);
  timerEventos.restart();
  timerEsteira.setTimeout(60000);
  time_pulso.setTimeout(1000);
  if (CEMITERIO == "KRAUSE") {
    timerDim.setTimeout(1);
    timerDim.restart();
  }
  else {
    timerDim.setTimeout(100);
    //timerDim.setTimeout(50);
    timerDim.restart();
  }
  timer_fumaca.setTimeout(12000);
  timerMecanismo.setTimeout(4000);
  if (debug)
    PRINT("DEBUG\n");
  PRINT(" \n");
  Cerimonia_completa.onReleased();
  Cerimonia_completa.onPressed();
}
