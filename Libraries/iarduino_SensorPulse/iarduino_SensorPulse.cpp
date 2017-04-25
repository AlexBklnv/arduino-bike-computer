#include	"iarduino_SensorPulse.h"

volatile	iarduino_SensorPulse_VolatileVariableClass ISPVVC;

			iarduino_SensorPulse::iarduino_SensorPulse(uint8_t i, uint8_t j){
				ISPVVC.ISP_pins_SEN=i;																			//	Сохраняем номер аналогового входа, к которому подключён сенсор пульса
				ISPVVC.ISP_pins_BEP=j;																			//	Сохраняем номер цифрового  вывода, к которому подключён биппер
			}

//			инициализация счётчика пульса
void		iarduino_SensorPulse::begin(){
				ISP_func_SET_Timer2(1000);																		//	Запускаем 2 таймер на частоте 1 кГц
				if(ISPVVC.ISP_pins_BEP<255){																	//	Если указан вывод биппера
					ISPVVC.ISP_data_BEP=0;																		//	Устанавливаем значение для биппера = LOW
					ISPVVC.ISP_flag_BEP=1;																		//	Устанавливаем флаг, сигнализирующий таймеру, о необходимости вывода сигналов на биппер
					pinMode     (ISPVVC.ISP_pins_BEP, OUTPUT);													//	Переводим вывод в режим выхода
					digitalWrite(ISPVVC.ISP_pins_BEP, ISPVVC.ISP_data_BEP);										//	Применяем установленные значение к выводу биппера
				}
			}

//			проверка результата
uint16_t	iarduino_SensorPulse::check(uint8_t i){
				switch(i){
					case ISP_ANALOG:	return ISPVVC.ISP_data_PIN[0];	break;
					case ISP_BEEP:		return ISP_func_CheckBeep();	break;
					case ISP_PULSE:		return ISP_func_CheckPulse();	break;
					case ISP_VALID:		return ISP_func_CheckValid();	break;
				}
			}

//			проверка вершины пульса
uint16_t	iarduino_SensorPulse::ISP_func_CheckBeep(){
				return ISPVVC.ISP_time_TOP/25;																	//	выводим время прошедшее после начала пульса, в десятых долях секунды ( return 1 = 0,1 сек )
			}

//			проверка значения пульса
uint16_t	iarduino_SensorPulse::ISP_func_CheckPulse(){
				uint32_t i = (ISPVVC.ISP_time_ARR[0]+ISPVVC.ISP_time_ARR[1]+ISPVVC.ISP_time_ARR[2]+ISPVVC.ISP_time_ARR[3]+ISPVVC.ISP_time_ARR[4])/5;
				if(ISPVVC.ISP_time_ARR[0]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[1]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[2]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[3]<10){return 0;}
				if(ISPVVC.ISP_time_ARR[4]<10){return 0;}
				if(i>999){i=999;}             return i;
			}

//			проверка валидности результата
uint8_t		iarduino_SensorPulse::ISP_func_CheckValid(){
				uint8_t	i1, i2, j1=0, j2=0, j3=ISPVVC.ISP_flag_VAL;
					if(ISPVVC.ISP_data_PIN[0]-20>ISPVVC.ISP_data_PIN[1]){i1=1;}									//	i1 - вектор направления предыдущей точки графика
					if(ISPVVC.ISP_data_PIN[0]+20<ISPVVC.ISP_data_PIN[1]){i1=0;}									//	i1==1 => вверх, i1==0 => вниз
				for(uint8_t k=1; k<9; k++){
					if(ISPVVC.ISP_data_PIN[k]-20>ISPVVC.ISP_data_PIN[k+1]){i2=1;}								//	i2 - вектор направления текущей точки графика
					if(ISPVVC.ISP_data_PIN[k]+20<ISPVVC.ISP_data_PIN[k+1]){i2=0;}								//	i2==1 => вверх, i2==0 => вниз
					if(ISPVVC.ISP_data_PIN[k]>900||ISPVVC.ISP_data_PIN[k]<50){j1++;}							//	j1 - количество точек графика за пределами нормальных значений
					if(i1!=i2){j2++;} i1=i2;																	//	j2 - количество изменений вектора направления текущей точки относительно предыдущей
				}	if(j2<2){ISPVVC.ISP_flag_VAL=1;}															//	если зафиксировано до    2 изменений вектора направления графика из 10 последних точек, то считаем что показания сенсора    валидны
					if(j2>4){ISPVVC.ISP_flag_VAL=0;}															//	если зафиксировано более 3 изменений вектора направления графика из 10 последних точек, то считаем что показания сенсора не валидны
					if(j1>5){ISPVVC.ISP_flag_VAL=0;}															//	если зафиксировано более 5 точек графика, из 10 последних, за пределами нормальных показаний, то считаем что показания сенсора не валидны
					if(j3!=ISPVVC.ISP_flag_VAL)	{return ISP_CHANGED;}
					if(ISPVVC.ISP_flag_VAL)		{return ISP_CONNECTED;}
					else						{return ISP_DISCONNECTED;}
			}

//			установка таймера2
void		iarduino_SensorPulse::ISP_func_SET_Timer2(uint32_t f){
//				определение значения предделителя для 2 таймера
				uint16_t			prescaler=1024,	CS22_CS21_CS20=7;											//	Значение предделителя = prescaler, а соответствующее ему состояние битов регистра TCCR2B: CS22, CS21, CS20 = CS22_CS21_CS20
				if(F_CPU/255/  1<f){prescaler=   1; CS22_CS21_CS20=1;}else
				if(F_CPU/255/  8<f){prescaler=   8; CS22_CS21_CS20=2;}else
				if(F_CPU/255/ 32<f){prescaler=  32; CS22_CS21_CS20=3;}else
				if(F_CPU/255/ 64<f){prescaler=  64; CS22_CS21_CS20=4;}else
				if(F_CPU/255/128<f){prescaler= 128; CS22_CS21_CS20=5;}else
				if(F_CPU/255/256<f){prescaler= 256; CS22_CS21_CS20=6;}
//				установка регистров 2 таймера и его запуск
				TCCR2A	= 0<<COM2A1	| 0<<COM2A0	| 0<<COM2B1	| 0<<COM2B0	| 1<<WGM21	| 0<<WGM20;					//	биты COM2... = «0» (каналы A и B таймера отключены от выводов 3 и 11), биты WGM21 и WGM20 = «10» (таймер 2 в режиме CTC)
				TCCR2B	= 0<<FOC2A	| 0<<FOC2B	| 0<<WGM22	| CS22_CS21_CS20;									//	биты FOC2... = «0» (без принудительной установки результата сравнения), бит WGM22 = «0» (таймер 2 в режиме CTC), биты CS22, CS21, CS20 = CS22_CS21_CS20 (значение предделителя)
				OCR2A	= (uint8_t)(F_CPU/(prescaler*f))-1;														//	значение регистра сравнения OCR2A настраивается под частоту переполнения счётного регистра TCNT2=f.  f=F_CPU/(предделитель*(OCR2A+1)) => OCR2A = (F_CPU/(предделитель*f))-1
				TIMSK2	= 0<<OCIE2B	| 1<<OCIE2A	| 0<<TOIE2;														//	разрешаем прерывание по совпадению счётного регистра TCNT2 и регистра сравнения OCR2A
				SREG	= 1<<7;																					//	устанавливаем флаг глобального разрешения прерываний в регистре SREG
			}

/* ISR */	ISR(TIMER2_COMPA_vect){
				ISPVVC.ISP_time_CNT++;																			//	увеличиваем счетчик прерываний
				if (ISPVVC.ISP_time_CNT%4==0){																	//	заходим в таймер 1 раз в 4 прерывания
					if (ISPVVC.ISP_time_TOP<0xFFFF){ISPVVC.ISP_time_TOP++;}										//	увеличиваем время прошедшее после того, как более 7 точек графика вошли в вершину пульса
					ISPVVC.ISP_data_TOP=0;																		//	сброс количества точек графика находящихся возле вершины пульса
					for(uint8_t i=9; i>0; i--){																	//	проход по всем элементам массива, ранее прочитанных данных
						ISPVVC.ISP_data_PIN[i]=ISPVVC.ISP_data_PIN[i-1];										//	сдвиг всех ранее прочитанных данных: 19=18, 18=17, 17=16, ... 1=0
						if(ISPVVC.ISP_data_PIN[i]>ISPVVC.ISP_data_CEN){ISPVVC.ISP_data_TOP++;}					//	подсчет количества точек графика находящихся возле вершины пульса
					}	ISPVVC.ISP_data_PIN[0]=analogRead(ISPVVC.ISP_pins_SEN);									//	считываем показания сенсора
					if (ISPVVC.ISP_time_CNT>=2000){ISPVVC.ISP_time_CNT=0;										//	если произошло переполнение счетчика прерываний (более 2 секунд), то ...
						ISPVVC.ISP_data_CEN=ISPVVC.ISP_data_MIN+(ISPVVC.ISP_data_MAX-ISPVVC.ISP_data_MIN)*2/3;	//	определяем 2/3 части от минимального до максимального значения за предыдущие 2 секунды
						ISPVVC.ISP_data_MAX=0; ISPVVC.ISP_data_MIN=1024;										//	сбрасываем максимальное и минимальное значение сенсора за текущие 2 секунды
					}
					if (ISPVVC.ISP_data_MAX<ISPVVC.ISP_data_PIN[0]){ISPVVC.ISP_data_MAX=ISPVVC.ISP_data_PIN[0];}//	обновляем максимальное значение
					if (ISPVVC.ISP_data_MIN>ISPVVC.ISP_data_PIN[0]){ISPVVC.ISP_data_MIN=ISPVVC.ISP_data_PIN[0];}//	обновляем минимальное  значение
					if (ISPVVC.ISP_data_TOP>7){																	//	если более 7 точек графика находятся возле вершины пульса
						if(ISPVVC.ISP_flag_TOP==0){																//	если до этого возле вершины пульса находились менее 3 точек графика
							for(uint8_t i=4; i>0; i--){ISPVVC.ISP_time_ARR[i]=ISPVVC.ISP_time_ARR[i-1];}		//	сдвиг всех ранее прочитанных данных о промежутках времени между пульсами: 4=3, 3=2, 2=1, 1=0
							ISPVVC.ISP_time_WAS=ISPVVC.ISP_time_NOW; ISPVVC.ISP_time_NOW=millis();				//	фиксируем текущее время
							if(ISPVVC.ISP_time_WAS>=ISPVVC.ISP_time_NOW){ISPVVC.ISP_time_WAS=0;}				//	избавляемся от переполнения millis()
							ISPVVC.ISP_time_ARR[0]=ISPVVC.ISP_time_NOW-ISPVVC.ISP_time_WAS;						//	фиксируем промежуток времени между последними пульсами
							if(ISPVVC.ISP_time_ARR[0]>0){ISPVVC.ISP_time_ARR[0]=60000/ISPVVC.ISP_time_ARR[0];}	//	переводим зафиксированный промежуток времени из мс в кол/мин
							ISPVVC.ISP_time_TOP=0;																//	сброс времени входа более 7 точек графика в вершину пульса
						}	ISPVVC.ISP_flag_TOP=1;																//	фиксируем вход в вершину пульса
					}
					if (ISPVVC.ISP_data_TOP<3){ISPVVC.ISP_flag_TOP=0;}											//	фиксируем уход с вершины пульса
				}
				if (ISPVVC.ISP_flag_BEP){ if (ISPVVC.ISP_flag_VAL){ if (ISPVVC.ISP_time_TOP>0){ if (ISPVVC.ISP_time_TOP<25){ ISPVVC.ISP_data_BEP=ISPVVC.ISP_data_BEP==1?0:1; digitalWrite(ISPVVC.ISP_pins_BEP, ISPVVC.ISP_data_BEP); }}}}
			}
