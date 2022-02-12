void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < canais_rgb; i++) {
    byte eepromValue = EEPROM.read(leds_rgb[i]);
    pinMode(leds_rgb[i], OUTPUT);
    if (i == cabine_r_i || i == cabine_g_i || i == cabine_b_i) {// não acende os leds da cabine
      analogWrite(leds_rgb[i], 0);
    }
    else {
      analogWrite(leds_rgb[i], eepromValue);
      //      Serial.println((String)"Led channel = "+leds_rgb[i]+" eeprom value = "+eepromValue);
      atual_rgb[i] = eepromValue;
      old_rgb[i] = eepromValue;
      new_rgb[i] = eepromValue;
    }
  }
  pinMode(R_EN_PORTA, OUTPUT);
  digitalWrite(R_EN_PORTA, LOW);
  pinMode(acopl_esteira, INPUT);
  pinMode(L_EN_PORTA, OUTPUT);
  pinMode(PINO_FECHA, OUTPUT);
  pinMode(PINO_ABRE, OUTPUT);

  pinMode(R_EN_MESA, OUTPUT);
  pinMode(L_EN_MESA, OUTPUT);
  pinMode(PINO_EXPOE, OUTPUT);
  pinMode(PINO_GUARDA, OUTPUT);
  pinMode(pwm_led_esteira, OUTPUT);
  analogWrite(pwm_led_esteira, 0 );
  pinMode(PINO_PWM_ESTEIRA, OUTPUT);

  analogWrite(PINO_PWM_ESTEIRA, 0);

  pinMode(FUMACA, OUTPUT);
  pinMode(LUZ_CABINE, OUTPUT);

  pinMode(SENSOR_PA, INPUT);
  pinMode(SENSOR_PF, INPUT);
  pinMode(SENSOR_MG, INPUT);
  pinMode(SENSOR_ME, INPUT);

  pinMode(13, OUTPUT);
  Serial.print("SISTEMA INICIADO, TOTAL DE CERIMONIAS REALIZADAS = ");
  Serial.println((EEPROM.read(1023) * 256) + EEPROM.read(1024));
  analogWrite(PINO_ABRE, 0);
  analogWrite(PINO_FECHA, 0);
  digitalWrite(PINO_GUARDA, LOW);
  digitalWrite(PINO_EXPOE, LOW);

  attachInterrupt(digitalPinToInterrupt(19), zero_cross_detect, RISING);
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);


  //setPwmFrequency(PINO_PWM_ESTEIRA, 1024); //pino, divisor
  //setPwmFrequency(PINO_PWM_PORTA, 1024); //pino, divisor
  timer_pulso_controle_cortina.setTimeout(3000);
  timer.setTimeout(1000);
  timer_rgb.setTimeout(30);
  timer_lamp.setTimeout(7000);
  timerPrint.setTimeout(1000);
  timerEventos.setTimeout(1000);
  timerEventos.restart();
  timerEsteira.setTimeout(60000);
  time_pulso.setTimeout(1000);
  timerDim.setTimeout(70);
  timerDim.restart();
  timer_fumaca.setTimeout(12000);
  timerMecanismo.setTimeout(4000);
  Cerimonia_simples.onPressed();
  Cerimonia_completa.invertReading();
  Cerimonia_simples.invertReading();
  Cerimonia_completa.onReleased();
  Cerimonia_simples.onReleased();

}
