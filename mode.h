#pragma once
#include <GyverButton.h>
#include "timerMinim.h"
#include "effect.h"

#define EFFECTS_COUNT 6

enum Modes : byte 
{
	Main,
	Setup,
#if (ALARM_TYPE != 0)
	Alarm,
#endif
#ifdef DHT_PIN
	Weather
#endif
};


class Mode 
{
protected:
	virtual void buttonsLoop() {}
	TimerMinim timer = TimerMinim();
	Effect** _effects;
public:
	Mode();
	~Mode();
	virtual void loop();
	virtual void init(byte param) = 0;
};

class MainMode : public Mode 
{
private:
	boolean _needRefreshTime = false, _showMode = false;
	byte _showModeSecCounter, _modeValue;
protected:
	void buttonsLoop();
public:
	MainMode(uint32_t timeInterval);
	void loop();
	void init(byte param);
};

class SetupMode : public Mode 
{
private:
	int8_t _changeHrs, _changeMins;
	byte _changeDay, _changeMonth;
	uint16_t _changeYear;
	byte _param; // 0 = time setup, 1 = alarm setup, 2 = night mode setup
	boolean _isHoursSelected, _lampState;
	byte _setupStage = 0;
	GButton* _set;
	GButton* _left;
	GButton* _right;
protected:
	void buttonsLoop();
public:
	SetupMode(uint32_t timeInterval);
	void loop();
	void init(byte param);
};

class AlarmMode : public Mode 
{
private:
	boolean _lampState;
	uint32_t _alarmTimeCounter, _timeInterval;
	void stopAlarm();
#if (ALARM_TYPE == 2)
	byte _soundVolume;
#endif
protected:
	void buttonsLoop();
public:
	AlarmMode(uint32_t timeInterval);
	void loop();
	void init(byte param);
};

class WeatherMode : public Mode
{
private:
	byte _temp, _hum;
public:
	WeatherMode::WeatherMode(uint32_t timeInterval);
	void loop();
	void init(byte param);
};