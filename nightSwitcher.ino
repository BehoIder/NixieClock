NightSwitcherEffect::NightSwitcherEffect(uint32_t timerInterval) : Effect() 
{
    timer.setInterval(timerInterval);
}

void NightSwitcherEffect::loop() 
{
    if (!timer.isReady() || suspended) return;
#ifndef AUTO_BRIGHT
    if ((nightHrStart > nightHrEnd && (hrs >= nightHrStart || hrs < nightHrEnd)) ||
        (nightHrStart < nightHrEnd && hrs >= nightHrStart && hrs < nightHrEnd)) 
    {
#else
    uint16_t val = analogRead(LIGHT_SENSOR);
    //Serial.println(val);
    if (val < AUTO_BRIGHT_DAY_NIGHT_THRESOLD) 
    {
#endif
        indiMaxBright = INDI_BRIGHT_N;        
        dotMaxBright = DOT_BRIGHT_N;
        backlMaxBright = BACKL_BRIGHT_N;
        backlStep = BACKL_STEP_N;
    }
    else 
    {
        indiMaxBright = INDI_BRIGHT;        
        dotMaxBright = DOT_BRIGHT;
        backlMaxBright = BACKL_BRIGHT;
        backlStep = BACKL_STEP;
    }

    for (byte i = 0; i < INDI_COUNT; i++) indiDimm[i] = indiMaxBright;
}
