// отправить на индикаторы
void sendToIndicators(byte hours, byte minutes, byte seconds, boolean notTime) 
{
#ifdef AM_PM_MODE
    if (!notTime) 
    {
        if (hours == 0) hours = 12;
        if (hours > 12) hours = hours % 12;
    }
#endif
    indiDigits[0] = hours / 10;
    indiDigits[1] = hours % 10;
    indiDigits[2] = minutes / 10;
    indiDigits[3] = minutes % 10;
    if (INDI_COUNT == 6) 
    {
        indiDigits[4] = seconds / 10;
        indiDigits[5] = seconds % 10;
    }
}

// для эффектов
void setNewTime() 
{
    int8_t newHrs = hrs, newMins = mins, newSecs;
    newSecs = secs + 1; 
    if (newSecs >= 60) { newSecs = 0; newMins++; if (newMins >= 60) { newMins = 0; newHrs++; if (newHrs >= 24) newHrs = 0; } }
#ifdef AM_PM_MODE
    if (newHrs == 0) newHrs = 12;
    if (newHrs > 12) newHrs = newHrs % 12;
#endif
    newTime[0] = (byte)newHrs / 10;
    newTime[1] = (byte)newHrs % 10;
    newTime[2] = (byte)newMins / 10;
    newTime[3] = (byte)newMins % 10;
    newTime[4] = secs;
    newTime[5] = secs;
}

void syncRTC() 
{
    DateTime now = rtc.now();
    secs = now.second();
    mins = now.minute();
    hrs = now.hour();
}

byte getNumberOfDays(uint16_t year, byte month)
{
    if (month == 2)
    {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) return 29;
        else return 28;
    }
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) return 31;
    return 30;
}
