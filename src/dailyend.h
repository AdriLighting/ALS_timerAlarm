alarm_dailyEnd::alarm_dailyEnd (int _pos) : alarm_base() {
	fsprintf("\n[alarm_dailyEnd::alarm_dailyEnd]\n");
	objPos = _pos;
}
alarm_dailyEnd::~alarm_dailyEnd(){
	fsprintf("\n[alarm_dailyEnd::destructor]\n");
	delete timerUpdated;
}
void alarm_dailyEnd::setup (
	String _name, 
	time_t _value, 
	time_t _endValue, 
	OnTick_t _onTickHandler, 
	OnTick_t _endTickHandler
	) {
	fsprintf("\n[alarm_dailyEnd::setup]\n");
	createTime     = now();
	o_endValue		= _endValue;
	isEnabled 		= true;
	isOneShot 		= true;
	name 			= _name;
	onTickHandler 	= _onTickHandler;
	endTickHandler 	= _endTickHandler;		
	period			= dtDailyEndAlarm;
	startTime 		= _value;
	if ((_value+_endValue) + previousMidnight(now()) <= now()) {
			value 	= _value + nextMidnight(now());
	} else 	value 	= _value;	
	endValue 		= value + _endValue;
	nextTrigger 	= value;
	timerUpdated 	= new adri_timer(1000,"",true);

}
void alarm_dailyEnd::edit(time_t _value, time_t _endValue){
		o_endValue 		= _endValue;
		startTime 		= _value;
		if ((_value+_endValue) + previousMidnight(now()) <= now()) {
				value 	= _value + nextMidnight(now());
		} else 	value 	= _value;	
		endValue 		= value + _endValue;
		nextTrigger 	= value;	
}
int alarm_dailyEnd::updateNextTrigger(){

	time_t time = now();

	if ( nextTrigger <= time) {
		
			if ((value + previousMidnight(now())) <= time) {
				if (isOneShot){
					isOneShot= false;
					return 1;
				}	
				if (endValue + previousMidnight(now()) <= time) {
					#ifdef DEBUG
						Serial.printf("\n\t[2 alarm_dailyEnd] [%s <= %s] %s -> upd: time has passed then set for tomorrow.\n", 
							timerAlarmPtr->timertoString(endValue + previousMidnight(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + nextMidnight(time)).c_str());						
					#endif
					isOneShot= true;	
					nextTrigger = value + nextMidnight(time);
					return 2;
				}
				
			} else {
				#ifdef DEBUG
					Serial.printf("\n\t[0 alarm_dailyEnd] [%s >= %s] %s -> upd: set the date to today and add the time given in value\n", 
						timerAlarmPtr->timertoString(value + previousMidnight(now())).c_str(), timerAlarmPtr->timertoString(time).c_str(), timerAlarmPtr->timertoString(value + previousMidnight(time)).c_str());
				#endif
				isOneShot= true;	
				nextTrigger = value + previousMidnight(time);
				return 0;
			}	
	}

	return -1;
}
void alarm_dailyEnd::loop(){
	if (!isEnabled) return;

	if (timerUpdated->loop_stop()) {
		isUpdated = true;
		fsprintf("\n[alarm_dailyEnd::loop] [isUpdated: %d]\n", isUpdated);
	}

	if ( (now() >= nextTrigger)) {
		int upd = updateNextTrigger();
		if (!isUpdated) return;
		if ((upd == 1 ) && (onTickHandler != NULL)) {
			(*onTickHandler)(objPos);     // call the handler
		}
		if ((upd == 2 ) && (endTickHandler != NULL)) {
			(*endTickHandler)(objPos);
		}	
	}	
}

void alarm_dailyEnd::print(){
	if (!isUpdated) return;
	fsprintf("\n[alarm_dailyEnd::print]\n");
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

void alarm_dailyEnd::lapse_toString			(String & ret) {ret = String(o_endValue);}
void alarm_dailyEnd::repeat_toString 		(String & ret) {ret = String("-1");}
void alarm_dailyEnd::start_toString			(String & ret) {ret = String(startTime);}
void alarm_dailyEnd::isEnabled_toString 	(String & ret) {ret = String(isEnabled);}
void alarm_dailyEnd::period_toString 		(String & ret) {ret = timerAlarmPtr->period_to_string(period);}