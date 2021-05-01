#pragma once

#define GLITCH_START_SECOND 7
#define GLITCH_END_SECOND 55

class Effect 
{
protected:
	boolean effectExecution = false, suspended = false; 
	TimerMinim timer = TimerMinim(); 
	byte currentMode;
public:
	Effect() { }
	virtual void loop() {}
	virtual void setMode(byte mode) {}
	virtual void switchMode() {}
	virtual void suspend() { suspended = true; }
	virtual void resume() { suspended = false; }
	byte mode() { return currentMode; }
};

class BacklightEffect : public Effect 
{
private:
	boolean _directionToBright = true;
	byte _backlBrightCounter = 0;
public:
	BacklightEffect(uint32_t timerInterval);
	void setMode(byte mode);
	void switchMode();
	void loop();
};

class RGBBacklightEffect : public Effect
{
private:
	boolean _directionToBright = true;
	byte _backlBrightCounter = 0;
public:
	RGBBacklightEffect(uint32_t timerInterval);
	void setMode(byte mode);
	void switchMode();
	void loop();
	void suspend();
};

class DotBlinkingEffect : public Effect 
{
private:
	byte _dotBrightStep, _dotBrightCounter = 0;
	uint32_t _resetCounter, _timerInterval;
	boolean _directionToBright = true;
public:
	DotBlinkingEffect(uint32_t timerInterval);
	void loop();
	void suspend();
};


class GlitchEffect : public Effect 
{
private:
	boolean _indiState; 
	byte _glitchCounter, _glitchMax, _glitchIndic;
	uint32_t _timerInterval;
	uint32_t _glitchMinSeconds = GLITCH_MIN, _glitchMaxSeconds = GLITCH_MAX;

public:
	GlitchEffect(uint32_t timerInterval);
	void setMode(byte mode);
	void switchMode();
	void loop();
};


class FlipEffect : public Effect 
{
private:
	byte _currentLamp, _flipEffectStages = 0, _indiBrightCounter;
	byte _startCathode[4], _endCathode[4];
	boolean _trainLeaving;
	boolean _directionToBright = true;
public:
	FlipEffect();
	void setMode(byte mode);
	void switchMode();
	void loop();
};

class NightSwitcherEffect : public Effect 
{
private:
public:
	NightSwitcherEffect(uint32_t timerInterval);
	void loop();
};

class LampBurningEffect : public Effect 
{
private:
public:
	LampBurningEffect(uint32_t timerInterval);
	void loop();
};