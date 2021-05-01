AlarmMode::AlarmMode(uint32_t timeInterval) : Mode() 
{
    timer.setInterval(timeInterval);
    _timeInterval = timeInterval;
}

void AlarmMode::init(byte param) 
{
    _lampState = true;
    _alarmTimeCounter = 0;
#if (ALARM_TYPE == 2)
    _soundVolume = 0;
#endif
}

void AlarmMode::loop() 
{
    Mode::loop();
    if (!timer.isReady()) return;
    syncRTC();
    sendToIndicators(hrs, mins, secs, false);
    _alarmTimeCounter += _timeInterval;
    if (_alarmTimeCounter > ALM_TIMEOUT*60000) stopAlarm();
    _lampState = !_lampState;
    anodeStates[0] = _lampState;
    anodeStates[1] = _lampState;
    anodeStates[2] = _lampState;
    anodeStates[3] = _lampState;
#if (INDI_COUNT == 6)
    anodeStates[4] = _lampState;
    anodeStates[5] = _lampState;
#endif
    if (_lampState) return;
#if (ALARM_TYPE == 2)
    if (_soundVolume < alarmVolume) 
    {
        _soundVolume++;
        myDFPlayer.volume(_soundVolume);
    }
    if (_soundVolume == 1) 
    {
        myDFPlayer.loop(alarmTrack);
    }
#elif (ALARM_TYPE == 1)
    setPin(PIEZO, HIGH);
    delay(100);
    setPin(PIEZO, LOW);
    delay(100);
    setPin(PIEZO, HIGH);
    delay(100);
    setPin(PIEZO, LOW);
#endif
}

void AlarmMode::buttonsLoop() 
{
    if (btnMiddle.isClick() || btnLeft.isClick() || btnRight.isClick()) stopAlarm();
    if (btnMiddle.isHolded() || btnLeft.isHolded() || btnRight.isHolded()) {} // reset events
}

void AlarmMode::stopAlarm() 
{
    if (alarmMode == 1) 
    {
        alarmMode = 0;
        EEPROM.put(EEPROM_CELL_ALARM_MODE, alarmMode);
    }
#if (ALARM_TYPE == 2)
    myDFPlayer.stop();
#endif
    modeSelector.setMode(Modes::Main, 0);
}
