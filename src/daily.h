alarm_daily::alarm_daily(int _pos) : alarm_base() {
	fsprintf("\n[alarm_daily::alarm_daily]\n");
	objPos = _pos;
}
alarm_daily::~alarm_daily(){
	fsprintf("\n[alarm_daily::destructor]\n");
	delete timerUpdated;

}
void alarm_daily::setup(
		String _name, 
		time_t _value, 
		OnTick_t _onTickHandler, 
		OnTick_t _endTickHandler
	) {
	fsprintf("\n[alarm_daily::setup]\n");
	createTime     	= now();
	isEnabled 		= true;
	name 			= _name;
	onTickHandler 	= _onTickHandler;
	endTickHandler 	= _endTickHandler;
	period			= dtDailyAlarm;
	startTime 		= _value;
	if (_value + previousMidnight(now()) <= now()) {
			value 	= _value + nextMidnight(now());
	} else 	value 	= _value;		
	nextTrigger 	= value;
	timerUpdated 	= new adri_timer(1000,"",true);
}
void alarm_daily::edit(time_t _value){
	startTime 		= _value;
	if (_value + previousMidnight(now()) <= now()) {
			value 	= _value + nextMidnight(now());
	} else 	value 	= _value;		
	nextTrigger 	= value;
}
int alarm_daily::updateNextTrigger(){
	time_t time = now();
	if (dtIsAlarm(period) && nextTrigger <= time) {
		if ((value + previousMidnight(now())) <= time) {
			#ifdef DEBUG
				Serial.printf("\n\t[1 alarm_daily] [%s <= %s] %s -> upd: time has passed then set for tomorrow.\n", 
					timerAlarmPtr->timertoString(value + previousMidnight(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + nextMidnight(time)).c_str());						
			#endif
			nextTrigger = value + nextMidnight(time);
			return 1;									
		}	
		else {
			#ifdef DEBUG
				Serial.printf("\n\t[0 alarm_daily] [%s >= %s] %s -> upd: set the date to today and add the time given in value\n", 
					timerAlarmPtr->timertoString(value + previousMidnight(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + previousMidnight(time)).c_str());
			#endif					
			nextTrigger = value + previousMidnight(time);
			return 0;
		}
	}

	return -1;
}
void alarm_daily::loop(){
	if (!isEnabled) return;	

	if (timerUpdated->loop_stop()) {
		isUpdated = true;
		fsprintf("\n[alarm_weeklyEnd::loop] [isUpdated: %d]\n", isUpdated);
	}

	if ( (now() >= nextTrigger)) {
		int upd = updateNextTrigger();
		if (!isUpdated) return;
		if ((upd == 1 ) && (onTickHandler != NULL)) {
			(*onTickHandler)(objPos);
			(*endTickHandler)(objPos);						
		}	
	}	
}
void alarm_daily::print(){
	if (!isUpdated) return;
	fsprintf("\n[alarm_daily::print]\n");
	String s_period 		= timerAlarmPtr->period_to_string(period);
	String s_startTime 		= timerAlarmPtr->timertoString(startTime);
	String s_createTime		= timerAlarmPtr->timertoString(createTime);	
	String s_value			= timerAlarmPtr->timertoString(value);	
	// String s_endValue		= timerAlarmPtr->timertoString(endValue);	
	// String s_o_endValue		= timerAlarmPtr->timertoString(o_endValue);	
	String s_nextTrigger 	= timerAlarmPtr->timertoString(nextTrigger);	
	String triggerDay		= dayStr(day(value));

	fsprintf("[%-15s][%s]\n", fsget(ta_str_period).c_str(), 		s_period.c_str());
	fsprintf("[%-15s][%d]\n", fsget(ta_str_isEnabled).c_str(), 		isEnabled);
	fsprintf("[%-15s][%d]\n", fsget(ta_str_isOneShot).c_str(), 		isOneShot);
	fsprintf("[%-15s][%s]\n", fsget(ta_str_createTime).c_str(), 	s_createTime.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_startTime).c_str(), 		s_startTime.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_value).c_str(), 			s_value.c_str());
	// fsprintf("[%-15s][%s]\n", fsget(ta_str_endValue).c_str(),		s_endValue.c_str());
	// fsprintf("[%-15s][%s]\n", fsget(ta_str_o_endValue).c_str(),		s_o_endValue.c_str());
	fsprintf("[%-15s][%s]\n", fsget(ta_str_nextTrigger).c_str(),	s_nextTrigger.c_str());
	fsprintf("[%-15s][%s - %d]\n", fsget(ta_str_triggerDay).c_str(), triggerDay.c_str(), day(nextTrigger));

}

void alarm_daily::lapse_toString		(String & ret) {ret = String("-1");}
void alarm_daily::repeat_toString 		(String & ret) {ret = String("-1");}
void alarm_daily::start_toString		(String & ret) {ret = String(startTime);}
void alarm_daily::isEnabled_toString 	(String & ret) {ret = String(isEnabled);}
void alarm_daily::period_toString 		(String & ret) {ret = timerAlarmPtr->period_to_string(period);}