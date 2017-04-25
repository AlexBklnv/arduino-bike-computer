#include "iarduino_RTC.h"
/**	пользовательские функции **/
			iarduino_RTC::iarduino_RTC(uint8_t RTC_selected_module){
				RTC_uint_MODULE = RTC_selected_module;		//	сохраняем выбранный модуль
			}

//			инициализация модуля
void		iarduino_RTC::begin(){
			I2C_mass_STATUS[0] = RTC_array_BUS[1];
			I2C_func_begin();
			RTC_func_START();	//	устанавливаем флаги управления и состояния модуля (в соответствии с настройками из массива RTC_array_STR)
			gettime();			//	читаем время в переменные
}

//			вывод даты
char*		iarduino_RTC::gettime(char* RTC_enter_string){
uint8_t		RTC_var_I=0; RTC_uint_RETURN=0;
//			читаем данные из модуля, или если не истёк RTC_uint_PERIOD, то пересчитываем время
			if(RTC_uint_PERIOD==0){RTC_func_READ_TIME();}else{if(RTC_uint_REQUEST > millis()){RTC_uint_REQUEST=0;} if(RTC_uint_REQUEST == 0 || (RTC_uint_PERIOD*60000+RTC_uint_REQUEST <= millis())){RTC_func_READ_TIME();}else{RTC_func_CALC_TIME();}}
			RTC_func_SET_MORE_TIME();	//	преобразуем переменные не читаемые из модуля
//			заполняем строку вывода
			while(RTC_enter_string[RTC_var_I] > 0 && RTC_var_I < 255){
				switch (RTC_enter_string[RTC_var_I]){
				//	case 's': if(RTC_uint_BLINK==1&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(2,seconds );} break;
					case 'i': if(RTC_uint_BLINK==2&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(2,minutes );} break;
				//	case 'h': if(RTC_uint_BLINK==3&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(2,hours   );} break;
					case 'H': if(RTC_uint_BLINK==3&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(2,Hours   );} break;
					case 'd': if(RTC_uint_BLINK==4&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(2,day     );} break;
				//	case 'w': if(RTC_uint_BLINK==7&&seconds%2){RTC_func_FILL_RESULTS(5,1);}else{RTC_func_FILL_RESULTS(1,weekday );} break;
				//case 'D': if(RTC_uint_BLINK==7&&seconds%2){RTC_func_FILL_RESULTS(5,3);}else{RTC_func_FILL_RESULTS(4,weekday );} break;
					case 'm': if(RTC_uint_BLINK==5&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(2,month   );} break;
				//	case 'M': if(RTC_uint_BLINK==5&&seconds%2){RTC_func_FILL_RESULTS(5,3);}else{RTC_func_FILL_RESULTS(4,month+6 );} break;
					case 'Y': if(RTC_uint_BLINK==6&&seconds%2){RTC_func_FILL_RESULTS(5,4);}else{RTC_func_FILL_RESULTS(0,year    );} break;
				//	case 'y': if(RTC_uint_BLINK==6&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(2,year    );} break;
				//	case 'a': if(RTC_uint_BLINK==8&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(3,midday  );} break;
				//	case 'A': if(RTC_uint_BLINK==8&&seconds%2){RTC_func_FILL_RESULTS(5,2);}else{RTC_func_FILL_RESULTS(3,midday+2);} break;
					default:  RTC_char_RETURN[RTC_uint_RETURN]=RTC_enter_string[RTC_var_I]; RTC_uint_RETURN++;
				}	RTC_var_I++;
			}		RTC_char_RETURN[RTC_uint_RETURN]='\0';
			return	RTC_char_RETURN;
}

//			установка даты
void		iarduino_RTC::settime(int RTC_i1, int RTC_i2, int RTC_i3, int RTC_i4, int RTC_i5, int RTC_i6, int RTC_i7){
			RTC_func_WRITE_TIME(RTC_i1,RTC_i2,RTC_i3,RTC_i4,RTC_i5,RTC_i6,RTC_i7);
			RTC_func_READ_TIME();
			RTC_func_SET_MORE_TIME();
}

//			вывод даты без отправки строки
void		iarduino_RTC::gettime(){gettime("");}
//			установка минимального периода опроса модуля
//void		iarduino_RTC::period(uint8_t RTC_i){RTC_uint_PERIOD=RTC_i;}
//			мигание одним из параметров времени
void		iarduino_RTC::blinktime(uint8_t RTC_i){RTC_uint_BLINK=RTC_i;}

/** внутренние функции для работы с модулями **/
//			установка флагов управления и состояния модуля
void		iarduino_RTC::RTC_func_START(){
uint8_t		RTC_var_I=0, RTC_var_J=0;
			if(RTC_array_STR[0]){
				for(int i=1; i<RTC_array_STR[0]*2; i+=2){
					RTC_var_J=0; RTC_var_I=RTC_func_READ_REG(RTC_array_STR[i]);
					for(int j=1; j<=15; j+=2){if(RTC_array_STR[i+1] & _BV(j)){if(bitRead(RTC_var_I, j/2)!=bitRead(RTC_array_STR[i+1], j-1)){bitWrite(RTC_var_I, j/2, bitRead(RTC_array_STR[i+1], j-1)); RTC_var_J=1;}}}
					if(RTC_var_J){RTC_func_WRITE_REG(RTC_array_STR[i], RTC_var_I);}
				}
			}	
}

//			чтение даты и времени из регистров модуля
void		iarduino_RTC::RTC_func_READ_TIME(){
			seconds = RTC_func_READ_TIME_INDEX(0);
			minutes = RTC_func_READ_TIME_INDEX(1);
			Hours   = RTC_func_READ_TIME_INDEX(2);
			day     = RTC_func_READ_TIME_INDEX(3);
			month   = RTC_func_READ_TIME_INDEX(4);
			year    = RTC_func_READ_TIME_INDEX(5);
		//	weekday = RTC_func_READ_TIME_INDEX(6); weekday--; // в регистре значение weekday не 0...6, а 1...7
			RTC_uint_REQUEST = millis();
}

//			запись даты и времени в регистры модуля
void		iarduino_RTC::RTC_func_WRITE_TIME(int RTC_i1, int RTC_i2, int RTC_i3, int RTC_i4, int RTC_i5, int RTC_i6, int RTC_i7){
			if(RTC_i1<=60 && RTC_i1>=0){RTC_func_WRITE_TIME_INDEX(0, RTC_i1   );}
			if(RTC_i2<=60 && RTC_i2>=0){RTC_func_WRITE_TIME_INDEX(1, RTC_i2   );}
			if(RTC_i3<=23 && RTC_i3>=0){RTC_func_WRITE_TIME_INDEX(2, RTC_i3   );}
			if(RTC_i4<=31 && RTC_i4>=1){RTC_func_WRITE_TIME_INDEX(3, RTC_i4   );}
			if(RTC_i5<=12 && RTC_i5>=1){RTC_func_WRITE_TIME_INDEX(4, RTC_i5   );}
			if(RTC_i6<=99 && RTC_i6>=0){RTC_func_WRITE_TIME_INDEX(5, RTC_i6   );}
			if(RTC_i7<= 6 && RTC_i7>=0){RTC_func_WRITE_TIME_INDEX(6,(RTC_i7+1));} // в регистре значение RTC_i7 не 0...6, а 1...7
}

//			чтение байта из регистра модуля
uint8_t		iarduino_RTC::RTC_func_READ_REG(uint8_t RTC_byte_address){
uint8_t		RTC_var_I=0xFF;
			
					I2C_func_START();													// отправляем команду START
					I2C_func_SEND_ID(RTC_array_BUS[2],0);				// отправляем адрес модуля и бит RW=0
					I2C_func_WRITE_BYTE(RTC_byte_address);								// отправляем адрес регистра
					I2C_func_RESTART();													// отправляем команду RESTART
					I2C_func_SEND_ID(RTC_array_BUS[2],1);				// отправляем адрес модуля и бит RW=1
					RTC_var_I = I2C_func_READ_BYTE(false);								// читаем байт из регистра с отправкой бита NACK
					I2C_func_STOP();													// отправляем команду STOP
					if(!I2C_mass_STATUS[2]){RTC_var_I=1;}								// если произошла ошибка при чтении, то выводим 1 (чтоб не вывести: 45 апреля 255 часов 127 минут и 200 секунд)

			return RTC_var_I;
}

//			запись байта в регистр модуля
void		iarduino_RTC::RTC_func_WRITE_REG(uint8_t RTC_byte_address, uint8_t RTC_byte_data){
			RTC_byte_address+=RTC_array_BUS[7];						// добавляем к адресу регистра слагаемое из ячейки 7 массива RTC_array_BUS
					I2C_func_START();													// отправляем команду START
					I2C_func_SEND_ID(RTC_array_BUS[2],0);				// отправляем адрес модуля и бит RW=0
					I2C_func_WRITE_BYTE(RTC_byte_address);								// отправляем адрес регистра
					I2C_func_WRITE_BYTE(RTC_byte_data);									// отправляем байт данных
					I2C_func_STOP();													// отправляем команду STOP
}

//			заполнение строки вывода результата
void		iarduino_RTC::RTC_func_FILL_RESULTS(uint8_t RTC_type, uint8_t RTC_data){
			switch (RTC_type){
			/* 4 знака	*/	case 0:	RTC_char_RETURN[RTC_uint_RETURN]=(RTC_uint_CENTURY-1)/10+48; RTC_uint_RETURN++; RTC_char_RETURN[RTC_uint_RETURN]=(RTC_uint_CENTURY-1)%10+48; RTC_uint_RETURN++; RTC_char_RETURN[RTC_uint_RETURN]=RTC_data/10+48; RTC_uint_RETURN++; RTC_char_RETURN[RTC_uint_RETURN]=RTC_data%10+48; RTC_uint_RETURN++; break;
			/* 1 знак	*/	case 1:	RTC_char_RETURN[RTC_uint_RETURN]=RTC_data+48; RTC_uint_RETURN++; break;
			/* 2 знака	*/	case 2:	RTC_char_RETURN[RTC_uint_RETURN]=RTC_data/10+48; RTC_uint_RETURN++; RTC_char_RETURN[RTC_uint_RETURN]=RTC_data%10+48; RTC_uint_RETURN++; break;
			/* AM / PM	*/	//case 3: if(RTC_data< 4){for(int i=0; i<2; i++){RTC_char_RETURN[RTC_uint_RETURN]=RTC_char_MIDDAY[RTC_data][i]; RTC_uint_RETURN++;}} break;
			/* дн / мес	*/	case 4: if(RTC_data<19){for(int i=0; i<3; i++){/*RTC_char_RETURN[RTC_uint_RETURN]=RTC_char_DayMon[RTC_data][i]; */RTC_uint_RETURN++;}} break;
			/* пробелы	*/	case 5: for(int i=0; i<RTC_data; i++){RTC_char_RETURN[RTC_uint_RETURN]=32; RTC_uint_RETURN++;} break;
			}
}

//			преобразование двоично-десятичного кода в число
uint8_t		iarduino_RTC::RTC_func_CONVER_CODE_NUM(uint8_t RTC_byte){return (RTC_byte >> 4)*10+(RTC_byte & 0x0F);}
//			преобразование числа в двоично-десятичный код
uint8_t		iarduino_RTC::RTC_func_CONVER_NUM_CODE(uint8_t RTC_byte){return ((RTC_byte/10) << 4)+(RTC_byte%10);}
//			преобразование переменных не читаемых из модуля
void		iarduino_RTC::RTC_func_SET_MORE_TIME(){hours=(Hours%12)?(Hours%12):12; /*midday=(Hours<12)?0:1;*/}
//			вспомогательная функция для RTC_func_WRITE_TIME()
void		iarduino_RTC::RTC_func_WRITE_TIME_INDEX(uint8_t RTC_byte_index, uint8_t RTC_byte_data){RTC_func_WRITE_REG(RTC_array_REG[RTC_byte_index], (RTC_func_CONVER_NUM_CODE(RTC_byte_data) | (~RTC_ignor_BIT[RTC_byte_index] & RTC_buff_TIMEREG[RTC_byte_index])) & (RTC_ignor_BIT[RTC_byte_index] | RTC_buff_TIMEREG[RTC_byte_index]));}
//			вспомогательная функция для RTC_func_READ_TIME()
uint8_t		iarduino_RTC::RTC_func_READ_TIME_INDEX(uint8_t RTC_byte_index){RTC_buff_TIMEREG[RTC_byte_index] = RTC_func_READ_REG(RTC_array_REG[RTC_byte_index]); return RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[RTC_byte_index] & RTC_ignor_BIT[RTC_byte_index]);}
//			увеличение переменных времени на разницу между последним и текущим запросами
void		iarduino_RTC::RTC_func_CALC_TIME(){
uint32_t	RTC_uint_this_request=millis();
uint8_t		RTC_j=(RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[4] & RTC_ignor_BIT[4])==2)?29:31+(((RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[4] & RTC_ignor_BIT[4])>7)?RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[4] & RTC_ignor_BIT[4])+1:RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[4] & RTC_ignor_BIT[4]))%2);
			RTC_uint_this_request-=RTC_uint_REQUEST; RTC_uint_this_request/=1000;
			RTC_uint_this_request+=RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[0] & RTC_ignor_BIT[0]); seconds = RTC_uint_this_request%60;		RTC_uint_this_request/=60;
			RTC_uint_this_request+=RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[1] & RTC_ignor_BIT[1]); minutes = RTC_uint_this_request%60;		RTC_uint_this_request/=60;
			RTC_uint_this_request+=RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[2] & RTC_ignor_BIT[2]); Hours   = RTC_uint_this_request%24;		RTC_uint_this_request/=24;
			                                                                                                        /*  weekday = RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[6] & RTC_ignor_BIT[6])+RTC_uint_this_request; if(weekday>6){weekday=0;}*/
			RTC_uint_this_request+=RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[3] & RTC_ignor_BIT[3]); day     = RTC_uint_this_request%RTC_j;	RTC_uint_this_request/=RTC_j;	day+=RTC_uint_this_request;
			RTC_uint_this_request+=RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[4] & RTC_ignor_BIT[4]); month   = RTC_uint_this_request%13;		RTC_uint_this_request/=13;		month+=RTC_uint_this_request;
			RTC_uint_this_request+=RTC_func_CONVER_CODE_NUM(RTC_buff_TIMEREG[5] & RTC_ignor_BIT[5]); year    = RTC_uint_this_request%100;
}

/** внутренние функции для работы с шинами I2C, SPI, SI3 **/
void		iarduino_RTC::I2C_func_START			()												{int I2C_var_I=0; I2C_mass_STATUS[2]=1; TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA); while(!(TWCR & _BV(TWINT))){I2C_var_I++; if(I2C_var_I>I2C_mass_STATUS[1]){I2C_mass_STATUS[2]=0; break;}} I2C_mass_STATUS[3] = TWSR & 0xF8; if(I2C_mass_STATUS[3]!=0x08){I2C_mass_STATUS[2]=0;}}
void		iarduino_RTC::I2C_func_RESTART			()												{int I2C_var_I=0;if(I2C_mass_STATUS[2]){ TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA); while(!(TWCR & _BV(TWINT))){I2C_var_I++; if(I2C_var_I>I2C_mass_STATUS[1]){I2C_mass_STATUS[2]=0; break;}} I2C_mass_STATUS[3] = TWSR & 0xF8; if(I2C_mass_STATUS[3]!=0x10){I2C_mass_STATUS[2]=0;}}}
void		iarduino_RTC::I2C_func_SEND_ID			(uint8_t I2C_byte_ID, bool I2C_bit_RW)			{int I2C_var_I=0;if(I2C_mass_STATUS[2]){ TWDR = (I2C_byte_ID<<1)+I2C_bit_RW; TWCR = _BV(TWINT) | _BV(TWEN); while(!(TWCR & _BV(TWINT))){I2C_var_I++; if(I2C_var_I>I2C_mass_STATUS[1]){I2C_mass_STATUS[2]=0; break;}} I2C_mass_STATUS[3] = TWSR & 0xF8; if(I2C_bit_RW){ if(I2C_mass_STATUS[3]!=0x40){I2C_mass_STATUS[2]=0;}}else{if(I2C_mass_STATUS[3]!=0x18){I2C_mass_STATUS[2]=0;}}}}
void		iarduino_RTC::I2C_func_WRITE_BYTE		(uint8_t I2C_byte_DATA)							{int I2C_var_I=0;if(I2C_mass_STATUS[2]){ TWDR = I2C_byte_DATA; TWCR = _BV(TWINT) | _BV(TWEN); while(!(TWCR & _BV(TWINT))){I2C_var_I++; if(I2C_var_I>I2C_mass_STATUS[1]){I2C_mass_STATUS[2]=0; break;}} I2C_mass_STATUS[3] = TWSR & 0xF8; if(I2C_mass_STATUS[3]!=0x28){I2C_mass_STATUS[2]=0;}}}
uint8_t		iarduino_RTC::I2C_func_READ_BYTE		(bool I2C_bit_ACK)								{int I2C_var_I=0;if(I2C_mass_STATUS[2]){ TWCR = _BV(TWINT) | _BV(TWEN) | I2C_bit_ACK<<TWEA; while(!(TWCR & _BV(TWINT))){I2C_var_I++; if(I2C_var_I>I2C_mass_STATUS[1]){I2C_mass_STATUS[2]=0; break;}} I2C_mass_STATUS[3] = TWSR & 0xF8; if(I2C_bit_ACK){if(I2C_mass_STATUS[3]!=0x50){I2C_mass_STATUS[2]=0;}}else{if(I2C_mass_STATUS[3]!=0x58){I2C_mass_STATUS[2]=0;}} return TWDR;}else{return 0xFF;}}
void		iarduino_RTC::I2C_func_STOP				()												{int I2C_var_I=0; TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO); while(!(TWCR & _BV(TWSTO))){I2C_var_I++; if(I2C_var_I>I2C_mass_STATUS[1]){I2C_mass_STATUS[2]=0; break;}} delayMicroseconds(20);}
void		iarduino_RTC::I2C_func_begin			()												{    I2C_mass_STATUS[2]=1; TWBR=((F_CPU/(I2C_mass_STATUS[0]*1000))-16)/2; if(TWBR<10){TWBR=10;} TWSR&=(~(_BV(TWPS1)|_BV(TWPS0)));}

