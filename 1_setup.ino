void setup() {
    //Serial.begin(9600);

    // ---------- RTC -----------
    rtc.begin();
    if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    #ifdef DHT_PIN
        pinMode(DHT_PIN, INPUT);
        dht.begin();
    #endif

    // настройка пинов на выход
    pinMode(DECODER0, OUTPUT);
    pinMode(DECODER1, OUTPUT);
    pinMode(DECODER2, OUTPUT);
    pinMode(DECODER3, OUTPUT);
    pinMode(KEY0, OUTPUT);
    pinMode(KEY1, OUTPUT);
    pinMode(KEY2, OUTPUT);
    pinMode(KEY3, OUTPUT);
    pinMode(DOT, OUTPUT);
    pinMode(GEN, OUTPUT);
    #if (BOARD_TYPE < 4)
    #else
        pinMode(KEY4, OUTPUT);
        pinMode(KEY5, OUTPUT);
    #endif
    pinMode(BACKL, OUTPUT);

    #if (ALARM_TYPE != 0)
        #if (ALARM_TYPE == 2)
            mySoftwareSerial.begin(9600);
            myDFPlayer.begin(mySoftwareSerial);
            myDFPlayer.volume(0);
        #elif (ALARM_TYPE == 1)
            pinMode(PIEZO, OUTPUT);
        #endif
    #endif
    #ifdef AUTO_BRIGHT
        pinMode(LIGHT_SENSOR, INPUT);
    #endif

    // задаем частоту ШИМ на 9 и 10 выводах 31 кГц
    TCCR1B = TCCR1B & 0b11111000 | 1;    // ставим делитель 1

    // включаем ШИМ
    setPWM(GEN, DUTY);

    // перенастраиваем частоту ШИМ на пинах 3 и 11 на 7.8 кГц и разрешаем прерывания COMPA
    TCCR2B = (TCCR2B & B11111000) | 2;    // делитель 8
    TCCR2A |= (1 << WGM21);   // включить CTC режим для COMPA
    TIMSK2 |= (1 << OCIE2A);  // включить прерывания по совпадению COMPA

      // EEPROM
    if (EEPROM.read(1023) != 100) 
    {   // первый запуск
        EEPROM.put(1023, 100);
        EEPROM.put(EEPROM_CELL_FLIP_MODE, 1);
        EEPROM.put(EEPROM_CELL_BACKLIGHT_MODE, 0);
        EEPROM.put(EEPROM_CELL_GLITCH_MODE, 0);
    #ifndef AUTO_BRIGHT
        EEPROM.put(EEPROM_CELL_NIGHT_START, 0);
        EEPROM.put(EEPROM_CELL_NIGHT_END, 0);
    #endif
    #if (ALARM_TYPE != 0)
        EEPROM.put(EEPROM_CELL_ALARM_HOURS, 0);
        EEPROM.put(EEPROM_CELL_ALARM_MINUTES, 0);
        EEPROM.put(EEPROM_CELL_ALARM_MODE, 0);
        #if (ALARM_TYPE == 2)
            EEPROM.put(EEPROM_CELL_ALARM_TRACK, 1);
            EEPROM.put(EEPROM_CELL_ALARM_VOLUME, alarmVolume);
        #endif
    #endif //ALARM
    }

    // установить яркость на индикаторы
    for (byte i = 0; i < INDI_COUNT; i++) indiDimm[i] = indiMaxBright;
    #ifndef AUTO_BRIGHT
        EEPROM.get(EEPROM_CELL_NIGHT_START, nightHrStart);
        EEPROM.get(EEPROM_CELL_NIGHT_END, nightHrEnd);
    #endif
    #if (ALARM_TYPE != 0)
        EEPROM.get(EEPROM_CELL_ALARM_HOURS, alarmHrs);
        EEPROM.get(EEPROM_CELL_ALARM_MINUTES, alarmMins);
        EEPROM.get(EEPROM_CELL_ALARM_MODE, alarmMode);
        #if (ALARM_TYPE == 2)
            EEPROM.get(EEPROM_CELL_ALARM_TRACK, alarmTrack);
            EEPROM.get(EEPROM_CELL_ALARM_VOLUME, alarmVolume);
        #endif
    #endif //ALARM
}
