LampBurningEffect::LampBurningEffect(uint32_t timerInterval) : Effect() 
{
    timer.setInterval(timerInterval);
}

void LampBurningEffect::loop() {
    if (!timer.isReady() || suspended) return;
    for (byte k = 0; k < BURN_LOOPS; k++) 
    {
        for (byte d = 0; d < 10; d++) 
        {
            for (byte i = 0; i < INDI_COUNT; i++) 
            {
                indiDigits[i]--;
                if (indiDigits[i] < 0) indiDigits[i] = 9;
            }
            delay(BURN_TIME);
        }
    }
}
