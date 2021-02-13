#include "timerAlarm.h"

#include <adri_tools_v2.h>

#define DEBUG

timerAlarm * timerAlarmPtr;

alarm_base * alarm_base_array[3];

fs(ta_str_period,		"period");
fs(ta_str_createTime,	"createTime");
fs(ta_str_startTime,	"startTime");
fs(ta_str_isEnabled,	"isEnabled");
fs(ta_str_isOneShot,	"isOneShot");
fs(ta_str_value,		"value");
fs(ta_str_endValue,     "endValue");
fs(ta_str_o_endValue,	"o_endValue");
fs(ta_str_nextTrigger,	"nextTrigger");
fs(ta_str_triggerDay,	"triggerDay");


#include "daily.h"
#include "dailyend.h"
#include "weekly.h"
#include "weeklyend.h"

alarm_base::alarm_base(){

}


template <typename T>
void registerAlarmArray(int pos) {
    alarm_base_array[pos] = new T(pos);
}
void registerAlarm_weeklyEnd 	(int pos) {registerAlarmArray<alarm_weeklyEnd> 	(pos);}
void registerAlarm_weekly 		(int pos) {registerAlarmArray<alarm_weekly> 	(pos);}
void registerAlarm_daily 		(int pos) {registerAlarmArray<alarm_daily> 		(pos);}
void registerAlarm_dailyEnd		(int pos) {registerAlarmArray<alarm_dailyEnd>	(pos);}



timerAlarm::timerAlarm(){
	timerAlarmPtr = this;
}

String timerAlarm::timertoString(time_t t) {
    char tmpStr[100];
    sprintf(tmpStr, "%02d:%02d:%02d", hour(t), minute(t), second(t));
    return String(tmpStr);
}
String timerAlarm::period_to_string(dt_alarmPeriod mod) {
	String ret;
	switch (mod) {
		case dtNotAllocated: 		ret = "dtNotAllocated"; 		break;
		case dtTimer: 				ret = "dtTimer"; 				break;
		case dtExplicitAlarm: 		ret = "dtExplicitAlarm"; 		break;
		case dtDailyAlarm: 			ret = "dtDailyAlarm"; 			break;
		case dtDailyRepeatAlarm:	ret = "dtDailyRepeat"; 			break;
		case dtWeeklyAlarm: 		ret = "dtWeeklyAlarm"; 			break;
		case dtWeeklyEndAlarm: 		ret = "dtWeeklyEndAlarm";		break;
		case dtWeeklyRepeatAlarm:	ret = "dtWeeklyRepeat";			break;
		case dtDailyEndAlarm:		ret = "dtDailyEndAlarm";		break;
		default:break;
	}
	return ret;
}
void timerAlarm::alarm_print(){

}