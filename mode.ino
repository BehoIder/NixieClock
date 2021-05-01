void ModeSelector::setMode(Modes mode, byte param) 
{
	if (mode == Modes::Main)  _currentMode = _mainMode;
	else if (mode == Modes::Setup) _currentMode = _setupMode;
#if (ALARM_TYPE != 0)
	else if (mode == Modes::Alarm) _currentMode = _alarmMode;
#endif
#ifdef DHT_PIN
	else if (mode == Modes::Weather) _currentMode = _dhtMode;
#endif
	_currentMode->init(param);
}

Mode::Mode()
{
	_effects = new Effect*[EFFECTS_COUNT] { &glitch, & flip, & backlight, & blink, & night, & burn };
}

Mode::~Mode()
{
	delete _effects;
}

void Mode::loop() 
{
#if (BOARD_TYPE < 4)
	btnMiddle.tick();
	btnLeft.tick();
	btnRight.tick();
#else
	int analog = analogRead(7);
	btnLeft.tick(analog <= 1023 && analog > 1000);
	btnMiddle.tick(analog <= 820 && analog > 690);
	btnRight.tick(analog <= 280 && analog > 120);
#endif
	buttonsLoop();
	for (byte i = 0; i < EFFECTS_COUNT; i++) _effects[i]->loop();
};
