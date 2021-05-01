SetupMode::SetupMode(uint32_t timeInterval) : Mode() 
{
    timer.setInterval(timeInterval);
}

void SetupMode::init(byte param) 
{
    _param = param;
    if (_param == 0) // time set mode
    { 
        _changeHrs = hrs;
        _changeMins = mins;
        DateTime now = rtc.now();
        _changeDay = now.day();
        _changeMonth = now.month();
        _changeYear = now.year();
        _set = &btnLeft;
        _left = &btnMiddle;
        _right = &btnRight;
    }
#if (ALARM_TYPE != 0)
    else if (_param == 1) // alarm set mode
    { 
        _changeHrs = alarmHrs;
        _changeMins = alarmMins;
        _set = &btnMiddle;
        _left = &btnLeft;
        _right = &btnRight;
    }
#endif
    else if (_param == 2) // night mode set mode
    { 
        _changeHrs = nightHrStart;
        _changeMins = nightHrEnd;
        _set = &btnRight;
        _left = &btnLeft;
        _right = &btnMiddle;
    }
    _setupStage = 0;
    _isHoursSelected = true;
    _lampState = true;
}

void SetupMode::loop() 
{
    Mode::loop();
    if (!timer.isReady()) return;
    _lampState = !_lampState;
#if (INDI_COUNT == 6)
    anodeStates[4] = false;
    anodeStates[5] = false;
#endif
    if (!_isHoursSelected) 
    {
        anodeStates[0] = true;
        anodeStates[1] = true;
        anodeStates[2] = _lampState;
        anodeStates[3] = _lampState;
    }
    else 
    {
        anodeStates[0] = _lampState;
        anodeStates[1] = _lampState;
        anodeStates[2] = true;
        anodeStates[3] = true;
    }

    if (_param == 0) // date-time setup
    { 
        if (_setupStage == 0) // time setup
        { 
            sendToIndicators(_changeHrs, _changeMins, 0, false);
#ifdef AM_PM_MODE
            if (_changeHrs > 12) setPWM(DOT, getPWM_CRT(DOT_BRIGHT));
            else setPWM(DOT, 0);
#endif
        }
        else if (_setupStage == 1) // year setup
        { 
            sendToIndicators((byte)(_changeYear / 100), (byte)(_changeYear % 100), 0, true); // year
            anodeStates[0] = _lampState;
            anodeStates[1] = _lampState;
            anodeStates[2] = _lampState;
            anodeStates[3] = _lampState;
        }
        else if (_setupStage == 2) // day, month
            sendToIndicators(_changeDay, _changeMonth, 0, true); 
    }
#if (ALARM_TYPE != 0)
    else if (_param == 1) // alarm setup
    { 
        if (_setupStage == 0) // setup alarm mode
        { 
            sendToIndicators(alarmMode, 0, 0, true);
            anodeStates[0] = _lampState;
            anodeStates[1] = _lampState;
            anodeStates[2] = false;
            anodeStates[3] = false;
        }
        else if (_setupStage == 1) // setup time
        { 
            sendToIndicators(_changeHrs, _changeMins, 0, false);
#ifdef AM_PM_MODE
            if (_changeHrs > 12) setPWM(DOT, getPWM_CRT(DOT_BRIGHT));
            else setPWM(DOT, 0);
#endif
        }
#if (ALARM_TYPE == 2)
        else if (_setupStage == 2) // setup track and volume (DFPLAYER ONLY)
            sendToIndicators(alarmTrack, alarmVolume, 0, true);
#endif
        }
#endif
#ifndef AUTO_BRIGHT
    else if (_param == 2) // night mode setup
        sendToIndicators(_changeHrs, _changeMins, 0, true);
#endif
}

void SetupMode::buttonsLoop() 
{
    if ((_setupStage == 0 && _param != 1) || (_setupStage == 1 && _param == 1) ) { // time setup
        if (_left->isClick()) 
        {
            if (_isHoursSelected)
            {
                _changeHrs--;
                if (_changeHrs < 0) _changeHrs = 23;
            }
            else 
            {
                _changeMins--;
                if (_param == 0 || _param == 1) 
                {
                    if (_changeMins < 0) _changeMins = 59; // time set or alarm time set
                }
                else
                    if (_changeMins < 0) _changeMins = 23; // day-night
            }
        }
        if (_right->isClick()) 
        {
            if (_isHoursSelected) 
            {
                _changeHrs++;
                if (_changeHrs > 23) _changeHrs = 0;
            }
            else 
            {
                _changeMins++;
                if (_param == 0 || _param == 1) 
                {
                    if (_changeMins > 59) _changeMins = 0; // time set or alarm time set
                }
                else
                    if (_changeMins > 23) _changeMins = 0; // day-night
            }
        }
    }
#if (ALARM_TYPE != 0)   
    if (_setupStage == 0 && _param == 1) // setup alarm mode
    { 
        if (_left->isClick()) 
        {
            if (alarmMode > 0) alarmMode--;
        }
        if (_right->isClick()) 
        {
            if (alarmMode < 3) alarmMode++;
        }
    }

#if (ALARM_TYPE == 2)
    if (_param == 1 && _setupStage == 2) // setup alarm mode track and volume
    { 
        if (_left->isClick()) 
        {
            myDFPlayer.stop();
            myDFPlayer.volume(alarmVolume);
            if (_isHoursSelected) 
            {
                if (alarmTrack > 0) 
                {
                    alarmTrack--;
                    myDFPlayer.play(alarmTrack);
                }
            }
            else 
            {
                if (alarmVolume > 0) alarmVolume--;
            }
        }
        if (_right->isClick()) 
        {
            myDFPlayer.stop();
            myDFPlayer.volume(alarmVolume);
            if (_isHoursSelected) {
                if (alarmTrack < ALM_TRACKS) 
                {
                    alarmTrack++;
                    myDFPlayer.play(alarmTrack);
                }
            }
            else 
            {
                if (alarmVolume < DFPLAYER_MAXVOLUME) alarmVolume++;
            }
        }
    }
#endif   
#endif //ALARM
    if (_param == 0 && _setupStage == 2) // setup day-month
    { 
        if (_left->isClick()) 
        {
            if (_isHoursSelected) _changeDay--; else _changeMonth--;
        }
        if (_right->isClick()) 
        {
            if (_isHoursSelected) _changeDay++; else _changeMonth++;
        }
        byte maxDay = getNumberOfDays(_changeYear, _changeMonth);
        if (_changeMonth > 12) _changeMonth = 1;
        if (_changeMonth < 1 ) _changeMonth = 12;
        if (_changeDay > maxDay) _changeDay = 1;
        if (_changeDay < 1) _changeDay= maxDay;
    }
    
    if (_param == 0 && _setupStage == 1) // setup year
    { 
        if (_left->isClick()) _changeYear--;
        if (_right->isClick()) _changeYear++;
        if (_changeYear < 2000) _changeYear = 2000;
        if (_changeYear > 3000) _changeYear = 3000;
    }

    if (_right->isHolded() || _left->isHolded()) {} // reset events

    if (_set->isHolded()) 
    {
        if (_param == 0) // datetime setup
        { 
            if (_setupStage < 2) 
            {
                _isHoursSelected = true;
                _setupStage++;
                return;
            }
            hrs = _changeHrs;
            mins = _changeMins;
            secs = 0;
            rtc.adjust(DateTime(_changeYear, _changeMonth, _changeDay, hrs, mins, 0));
        }
#if (ALARM_TYPE != 0)
        else if (_param == 1) // alarm
        { 
#if (ALARM_TYPE == 2)
            if (_setupStage < 2) 
            {
                _isHoursSelected = true;
#else
            if (_setupStage == 0) 
            {
#endif
                if (alarmMode > 0) 
                {
                    _setupStage++;
                    return;
                }
            }
            alarmHrs = _changeHrs;
            alarmMins = _changeMins;

            EEPROM.put(EEPROM_CELL_ALARM_HOURS, alarmHrs);
            EEPROM.put(EEPROM_CELL_ALARM_MINUTES, alarmMins);
            EEPROM.put(EEPROM_CELL_ALARM_MODE, alarmMode);

#if (ALARM_TYPE == 2)
            myDFPlayer.stop();
            EEPROM.put(EEPROM_CELL_ALARM_TRACK, alarmTrack);
            EEPROM.put(EEPROM_CELL_ALARM_VOLUME, alarmVolume);
#elif (ALARM_TYPE == 1)
            setPin(PIEZO, HIGH);
            delay(300);
            setPin(PIEZO, LOW);
#endif
        }
#endif
#ifndef AUTO_BRIGHT
        // Day-Night setup
        else if (_param == 2) 
        {
            nightHrStart = _changeHrs;
            nightHrEnd = _changeMins;
            EEPROM.put(EEPROM_CELL_NIGHT_START, nightHrStart);
            EEPROM.put(EEPROM_CELL_NIGHT_END, nightHrEnd);
        }
#endif
        modeSelector.setMode(Modes::Main, 0);
    }
    if (_set->isClick()) _isHoursSelected = !_isHoursSelected;
}
