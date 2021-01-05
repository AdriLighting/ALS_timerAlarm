
alarm_weeklyEnd::alarm_weeklyEnd (int _pos) : alarm_base() {
	fsprintf("\n[alarm_weeklyEnd::alarm_weeklyEnd]\n");
	objPos = _pos;
}
alarm_weeklyEnd::~alarm_weeklyEnd(){
	fsprintf("\n[alarm_weeklyEnd::destructor]\n");
	delete timerUpdated;
}
void alarm_weeklyEnd::setup (
	String _name, 
	time_t _value, 
	time_t _endValue, 
	OnTick_t _onTickHandler, 
	timeDayOfWeek_t _dayOfWeek, 
	OnTick_t _endTickHandler
	) {
	fsprintf("\n[alarm_weeklyEnd::setup]\n");
	createTime     	= now();
	o_endValue 		= _endValue;
	isEnabled 		= true;
	name 			= _name;
	onTickHandler 	= _onTickHandler;
	endTickHandler 	= _endTickHandler;
	period			= dtWeeklyEndAlarm;
	dayOfWeek		= _dayOfWeek;
	endValue 		= (_dayOfWeek-1) * SECS_PER_DAY + (_value + _endValue);
	value 			= (_dayOfWeek-1) * SECS_PER_DAY + _value;
	startTime 		= _value;
	nextTrigger 	= _value;
	timerUpdated 	= new adri_timer(1000,"",true);

}
void alarm_weeklyEnd::edit(time_t _value, time_t _endValue){
		o_endValue	= _endValue;
		endValue	= (dayOfWeek-1) * SECS_PER_DAY + (_value + _endValue);
		value		= (dayOfWeek-1) * SECS_PER_DAY + _value;
		startTime	= _value;
		nextTrigger	= _value;
		if ( (endValue + previousSunday(now())) <= now()) { 
			Serial.printf("\n[calendar_createWeekly] : up to tomorrow\n");
			nextTrigger = _value + nextSunday(now());
			
		}	
}
int alarm_weeklyEnd::updateNextTrigger(){

	time_t time = now();

	if ( nextTrigger <= time) {
		
		if ( (value + previousSunday(now())) <= time) {
			if (isOneShot){
				isOneShot= false;
				return 1;
			}			
			if ( (endValue + previousSunday(now())) <= time) { 
				#ifdef DEBUG
					Serial.printf("\n\t[2 alarm_weeklyEnd] [%s <= %s] %s -> upd: time has passed then set for the next week.\n", 
						timerAlarmPtr->timertoString(endValue + previousSunday(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + nextSunday(time)).c_str());						
				#endif
				nextTrigger = value + nextSunday(time);
				isOneShot = true;
				return 2;
			}
			
		} else {
			#ifdef DEBUG
				Serial.printf("\n\t[0 alarm_weeklyEnd] [%s >= %s] %s -> upd: set the date to today and add the time given in value\n", 
					timerAlarmPtr->timertoString(value + previousSunday(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + previousSunday(time)).c_str());
			#endif
			nextTrigger = value + previousSunday(time);
			isOneShot = true;
			return 0;
		}
	}

	return -1;
}
void alarm_weeklyEnd::loop(){
	if (!isEnabled) return;

	if (timerUpdated->loop_stop()) {
		isUpdated = true;
		fsprintf("\n[alarm_weeklyEnd::loop] [isUpdated: %d]\n", isUpdated);
	}

	if ( (now() >= nextTrigger)) {
		int upd = updateNextTrigger();
		if (!isUpdated) return;
		if ((upd == 1 ) && (onTickHandler != NULL)) {
			(*onTickHandler)(0);     // call the handler
		}
		if ((upd == 2 ) && (endTickHandler != NULL)) {
			(*endTickHandler)(1);
		}	
	}	
}

void alarm_weeklyEnd::print(){
	if (!isUpdated) return;
	fsprintf("\n[alarm_weeklyEnd::print]\n");
	String s_period 		= timerAlarmPtr->period_to_string(period);
	String s_startTime 		= timerAlarmPtr->timertoString(startTime);
	String s_createTime		= timerAlarmPtr->timertoString(createTime);	
	String s_value			= timerAlarmPtr->timertoString(value);	
	String s_endValue		= timerAlarmPtr->timertoString(endValue);	
	String s_o_endValue		= timerAlarmPtr->timertoString(o_endValue);	
	String s_nextTrigger 	= timerAlarmPtr->timertoString(nextTrigger);	
	String triggerDay		= dayStr(day(value));

	fsprintf("[%-15s][%s]\n", fsget(ta_str_period).c_str(), 		s_period.c_str());
	fsprintf("[%-15s][%d]\n", fsget(ta_str_isEnabled).c_str(), 		isEnabled);
	fsprintf("[%-15s][%d]\n", fsget(ta_str_isOneShot).c_str(), 		isOneShot);
	fsprintf("[%-15s][%s]\n", fsget(ta_str_createTime).c_str(), 	s_createTime.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_startTime).c_str(), 		s_startTime.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_value).c_str(), 			s_value.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_endValue).c_str(),		s_endValue.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_o_endValue).c_str(),		s_o_endValue.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_nextTrigger).c_str(),	s_nextTrigger.c_str());
	fsprintf("[%-15s][%s - %d]\n", fsget(ta_str_triggerDay).c_str(), triggerDay.c_str(), day(nextTrigger));

}


void alarm_weeklyEnd::lapse_toString		(String & ret) {ret = String(o_endValue);}
void alarm_weeklyEnd::repeat_toString 		(String & ret) {ret = String("-1");}
void alarm_weeklyEnd::start_toString		(String & ret) {ret = String(startTime);}
void alarm_weeklyEnd::isEnabled_toString 	(String & ret) {ret = String(isEnabled);}
void alarm_weeklyEnd::period_toString 		(String & ret) {ret = timerAlarmPtr->period_to_string(period);}