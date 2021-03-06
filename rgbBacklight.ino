#ifdef RGB_BACKLIGHT
RGBBacklightEffect::RGBBacklightEffect(uint32_t timerInterval) : Effect()
{
    EEPROM.get(EEPROM_CELL_BACKLIGHT_MODE, currentMode);
    timer.setInterval(timerInterval);
}

void RGBBacklightEffect::setMode(byte mode)
{
    currentMode = mode;
    if (currentMode == 0) setPin(BACKL, 0);
    EEPROM.put(EEPROM_CELL_BACKLIGHT_MODE, currentMode);
}

void RGBBacklightEffect::switchMode()
{
    byte mode = currentMode + 1;
    if (mode > 2) mode = 0;
    setMode(mode);
}

void RGBBacklightEffect::suspend()
{
    Effect::suspend();
    strip.setBrightness(0);
}

void RGBBacklightEffect::loop()
{
    if (!timer.isReady() || suspended) return;
    for (byte i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, strip.Color(R, G, B));
    if (currentMode == 1) strip.setBrightness(backlMaxBright);
    else if (currentMode == 2 && backlMaxBright > 0)
    {
        if (!effectExecution)
        {
            effectExecution = true;
            timer.setInterval(2000L / (backlMaxBright / backlStep));
        }
        if (_directionToBright)
        {
            _backlBrightCounter += backlStep;
            if (_backlBrightCounter >= backlMaxBright)
            {
                _directionToBright = false;
                _backlBrightCounter = backlMaxBright;
            }
        }
        else
        {
            _backlBrightCounter -= backlStep;
            if (_backlBrightCounter <= BACKL_MIN_BRIGHT)
            {
                _directionToBright = true;
                _backlBrightCounter = BACKL_MIN_BRIGHT;
                timer.setInterval(BACKL_PAUSE);
                effectExecution = false;
            }
        }
        strip.setBrightness(_backlBrightCounter);
    }
    else strip.setBrightness(0);
    strip.show();
}
#endif