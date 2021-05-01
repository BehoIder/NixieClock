MainMode::MainMode(uint32_t timeInterval) : Mode() 
{
    timer.setInterval(timeInterval);
}

void MainMode::init(byte param) 
{
    _needRefreshTime = true;
    _showMode = false;
}

void MainMode::loop() 
{
    Mode::loop();
    if (_needRefreshTime) 
    {
        for (byte i = 0; i < INDI_COUNT; i++) anodeStates[i] = true;
        _needRefreshTime = false;
        syncRTC();
        sendToIndicators(hrs, mins, secs, false);
    }
    if (timer.isReady()) 
    {
#if (ALARM_TYPE != 0) // обработка срабатывания будильника
        byte dow = rtc.now().dayOfTheWeek();
        if (hrs == alarmHrs && mins == alarmMins && secs == 0 && 
            (
                (alarmMode == 1 || alarmMode == 2) || 
                (alarmMode == 3 && dow != 1 && dow != 7)
            )) 
                modeSelector.setMode(Modes::Alarm, 0);
#endif
#ifdef DHT_PIN
        if (secs >= dhtSecond && secs <= dhtSecond + dhtDuration)
        {
            flip.resume();
            modeSelector.setMode(Modes::Weather, 0);
        }
#endif
        if (_showMode) 
        {
            if (_showModeSecCounter > SHOW_EFFECTS_MODE_TIMEOUT*2) 
            {
                _showMode = false;
                _needRefreshTime = true;
                glitch.resume();
                flip.resume();
            }
            else 
            {
                anodeStates[0] = false;
                anodeStates[1] = false;
                anodeStates[2] = false;
                anodeStates[3] = true;
#if (INDI_COUNT == 6)
                anodeStates[4] = false;
                anodeStates[5] = false;
#endif
                _showModeSecCounter++;
                sendToIndicators(0, _modeValue, 0, true);
            }
        }
        else if (secs >= 59) 
        {
            setNewTime();
            newTimeFlag = true;
            _needRefreshTime = true;
        }
#if (INDI_COUNT == 6)
        else _needRefreshTime = true;
#endif
    }
}

void MainMode::buttonsLoop() 
{
    Effect* effect = NULL;

    if (btnRight.isClick())  effect = &flip;
    if (btnMiddle.isClick()) effect = &backlight;
    if (btnLeft.isClick()) effect = &glitch;

    if (effect != NULL) 
    {
        effect->switchMode();
        _modeValue = effect->mode();
        glitch.suspend();
        flip.suspend();
        _showModeSecCounter = 0;
        _showMode = true;
    }

    if (btnLeft.isHolded()) modeSelector.setMode(Modes::Setup, 0); // date-time setup
#if (ALARM_TYPE != 0)
    if (btnMiddle.isHolded()) modeSelector.setMode(Modes::Setup, 1); // alarm setup
#endif
#ifndef AUTO_BRIGHT
    if (btnRight.isHolded()) modeSelector.setMode(Modes::Setup, 2); // day-night setup
#endif
}
