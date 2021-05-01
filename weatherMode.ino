#ifdef DHT_PIN
WeatherMode::WeatherMode(uint32_t timeInterval) : Mode()
{
    timer.setInterval(timeInterval);
}

void WeatherMode::init(byte param) 
{ 
    _temp = dht.readTemperature();
    _hum = dht.readHumidity();
    if (_hum > 99) _hum = 99;
}

void WeatherMode::loop()
{
    Mode::loop();
    if (!timer.isReady()) return;
    syncRTC();
    if (secs >= dhtSecond + dhtDuration)  modeSelector.setMode(Modes::Main, 0);
    anodeStates[0] = true;
    anodeStates[1] = true;
    if (INDI_COUNT > 4)
    {
        anodeStates[2] = false;
        anodeStates[3] = false;
        anodeStates[4] = true;
        anodeStates[5] = true;
        sendToIndicators(_temp, 0, _hum, true);
    }
    else
    {
        anodeStates[2] = true;
        anodeStates[3] = true;
        sendToIndicators(_temp, _hum, 0, true);
    }
}
#endif