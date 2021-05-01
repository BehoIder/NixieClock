#pragma once
#include "mode.h"

class ModeSelector 
{
private:
	Mode* _currentMode = NULL;
	Mode* _setupMode = new SetupMode(500L);
	Mode* _mainMode = new MainMode(500L);
#if (ALARM_TYPE != 0)
	Mode* _alarmMode = new AlarmMode(500L);
#endif
#ifdef DHT_PIN
	Mode* _dhtMode = new WeatherMode(1000L);
#endif
public:
	ModeSelector() {}
	ModeSelector(Modes mode) { setMode(mode, 0); }
	void setMode(Modes mode, byte param);
	Mode* getMode() { return _currentMode; }
};
