// динамическая индикация в прерывании таймера 2
ISR(TIMER2_COMPA_vect) 
{
	indiCounter[curIndi]++;             // счётчик индикатора
	if (indiCounter[curIndi] >= indiDimm[curIndi])  // если достигли порога диммирования
		setPin(opts[curIndi], 0);         // выключить текущий индикатор

	if (indiCounter[curIndi] > 25) // достигли порога в 25 единиц
	{    
		indiCounter[curIndi] = 0;         // сброс счетчика лампы
		if (++curIndi >= INDI_COUNT) curIndi = 0;  // смена лампы закольцованная

		// отправить цифру из массива indiDigits согласно типу лампы
		if (indiDimm[curIndi] > 0) 
		{
			setPin(opts[curIndi], anodeStates[curIndi]);    // включить анод на текущую лампу
			byte thisDig = digitMask[indiDigits[curIndi]];
			setPin(DECODER0, bitRead(thisDig, 0));
			setPin(DECODER1, bitRead(thisDig, 1));
			setPin(DECODER2, bitRead(thisDig, 2));
			setPin(DECODER3, bitRead(thisDig, 3));
		}
	}
}