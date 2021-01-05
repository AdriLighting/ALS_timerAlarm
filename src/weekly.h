alarm_weekly::alarm_weekly(int _pos) : alarm_base() {
	fsprintf("\n[alarm_weekly::alarm_weekly]\n");
	objPos = _pos;
}
alarm_weekly::~alarm_weekly(){
	fsprintf("\n[alarm_weekly::destructor]\n");
	delete timerUpdated;	
}
void alarm_weekly::setup(
		String _name, 
		time_t _value, 
		OnTick_t _onTickHandler, 
		timeDayOfWeek_t _dayOfWeek, 
		OnTick_t _endTickHandler
	) {
	fsprintf("\n[alarm_weekly::setup]\n");
	createTime     	= now();
	isEnabled 		= true;
	name 			= _name;
	onTickHandler 	= _onTickHandler;
	endTickHandler 	= _endTickHandler;
	period			= dtWeeklyAlarm;
	dayOfWeek		= _dayOfWeek;
	value 			= (_dayOfWeek-1) * SECS_PER_DAY + _value;
	startTime 		= _value;
	nextTrigger 	= _value;
	timerUpdated 	= new adri_timer(1000,"",true);
	// if ((_value + previousSunday(now())) <= now()) {
	// 	calendar_array[pos]->nextTrigger = _value + nextSunday(now());
	// } else {
	// 	calendar_array[pos]->nextTrigger = _value + previousSunday(now());
	// }			
}
void alarm_weekly::edit(time_t _value){
		value			= (dayOfWeek-1) * SECS_PER_DAY + _value;
		startTime 		= _value;
		nextTrigger 	= _value;
		if ((value + previousSunday(now())) <= now()) {
			nextTrigger = value + nextSunday(now());
		}
}
int alarm_weekly::updateNextTrigger(){
	time_t time = now();
	if (dtIsAlarm(period) && nextTrigger <= time) {
		if ((value + previousSunday(now())) <= time) {
			#ifdef DEBUG
				Serial.printf("\n\t[1 alarm_weekly] [%s <= %s] %s -> upd: time has passed then set for the next week.\n", 
					timerAlarmPtr->timertoString(value + previousSunday(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + nextSunday(time)).c_str());						
			#endif
			nextTrigger = value + nextSunday(time);
			return 1;
		} else {
			#ifdef DEBUG
				Serial.printf("\n\t[0 alarm_weekly] [%s >= %s] %s -> upd: set the date to today and add the time given in value\n", 
					timerAlarmPtr->timertoString(value + previousSunday(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + previousSunday(time)).c_str());
			#endif
			nextTrigger = value + previousSunday(time);
			return 0;
		}
	}

	return -1;
}
void alarm_weekly::loop(){
	if (!isEnabled) return;	

	if (timerUpdated->loop_stop()) {
		isUpdated = true;
		fsprintf("\n[alarm_weekly::loop] [isUpdated: %d]\n", isUpdated);
	}

	if ( (now() >= nextTrigger)) {
		int upd = updateNextTrigger();
		if (!isUpdated) return;
		if ((upd == 1 ) && (onTickHandler != NULL)) {
			(*onTickHandler)(0);
			(*endTickHandler)(1);						
		}	
	}	
}
void alarm_weekly::print(){
	if (!isUpdated) return;
	fsprintf("\n[alarm_weekly::print]\n");
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

void alarm_weekly::lapse_toString		(String & ret) {ret = String("-1");}
void alarm_weekly::repeat_toString 		(String & ret) {ret = String("-1");}
void alarm_weekly::start_toString		(String & ret) {ret = String(startTime);}
void alarm_weekly::isEnabled_toString 	(String & ret) {ret = String(isEnabled);}
void alarm_weekly::period_toString 		(String & ret) {ret = timerAlarmPtr->period_to_string(period);}