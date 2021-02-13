#ifndef TIMERALARM_H
	#define TIMERALARM_H 

	#include <Arduino.h>
	#include <TimeLib.h>
	#include <calendar_date.h>
	#include <adri_timer.h>

	#define AlarmHMS(_hr_, _min_, _sec_) (_hr_ * SECS_PER_HOUR + _min_ * SECS_PER_MIN + _sec_)

	#define dtIsAlarm(_type_)  (_type_ >= dtExplicitAlarm && _type_ < dtLastAlarmType)

	typedef enum {
		dtMillisecond,
		dtSecond,
		dtMinute,
		dtHour,
		dtDay
	} dtUnits_t;


	typedef enum {
		dtNotAllocated,
		dtTimer,
		dtExplicitAlarm,
		dtDailyAlarm,
		dtDailyEndAlarm,
  		dtDailyRepeatAlarm,
		dtWeeklyAlarm,
		dtWeeklyEndAlarm,
		dtWeeklyRepeatAlarm,
		dtLastAlarmType
	} dt_alarmPeriod ;


	typedef void (*OnTick_t)(int cPos);  

	class alarm_base
	{
	public:
		alarm_base();
		virtual void 	loop();
		virtual void 	print();
		virtual void 	desactivate();

		virtual void  	setup(
							String _name, 
							time_t _value, 
							OnTick_t _onTickHandler, 
							OnTick_t _endTickHandler
						){}	

		virtual void  	setup(
							String _name, 
							time_t _value, 
							time_t _endValue, 
							OnTick_t _onTickHandler, 
							timeDayOfWeek_t _dayOfWeek, 
							OnTick_t _endTickHandler
						){}					
		virtual void  	setup(
							String _name, 
							time_t _value, 
							OnTick_t _onTickHandler, 
							timeDayOfWeek_t _dayOfWeek, 
							OnTick_t _endTickHandler
						){}
		virtual void  	setup(
							String _name, 
							time_t _value, 
							time_t _endValue, 
							OnTick_t _onTickHandler, 
							OnTick_t _endTickHandler
						){}		


		virtual int		updateNextTrigger();
		virtual	void 	lapse_toString		(String & ret);
		virtual	void 	repeat_toString		(String & ret);
		virtual	void 	start_toString 		(String & ret);
		virtual	void 	isEnabled_toString 	(String & ret);					
		virtual	void 	period_toString		(String & ret);			

	};


	class alarm_daily : public alarm_base {
		public:
			int 				objPos;
			int 				pos;
			String 				name;

			time_t 				createTime 	= 0;
			time_t 				startTime 	= 0;

			time_t 				value 		= 0;
			time_t 				nextTrigger	= 0;
			
			OnTick_t 			onTickHandler;
			OnTick_t 			endTickHandler;

			dt_alarmPeriod 		period 		= dtDailyEndAlarm;

			boolean 			isEnabled;  
			boolean 			isOneShot =true;  
			boolean 			isUpdated =false;  
			adri_timer 			* timerUpdated;

			alarm_daily(int _pos);
			~alarm_daily();
			void 	setup(
							String _name, 
							time_t _value, 
							OnTick_t _onTickHandler, 
							OnTick_t _endTickHandler
						);
			int 	updateNextTrigger();
			void 	loop 			();
			void 	print 			();	
			void 	desactivate 	(){}			
			void 	edit 			(time_t _value);	
			void 	lapse_toString		(String & ret);
			void 	repeat_toString		(String & ret);
			void 	start_toString 		(String & ret);
			void 	isEnabled_toString 	(String & ret);					
			void 	period_toString		(String & ret);					
	};

	class alarm_dailyEnd : public alarm_base {
		public:
			int 				objPos;
			int 				pos;
			String 				name;

			time_t 				createTime 	= 0;
			time_t 				startTime 	= 0;

			time_t 				value 		= 0;
			time_t 				endValue 	= 0;
			time_t 				o_endValue 	= 0;
			time_t 				nextTrigger	= 0;
			
			OnTick_t 			onTickHandler;
			OnTick_t 			endTickHandler;

			dt_alarmPeriod 		period 		= dtWeeklyEndAlarm;

			boolean 			isEnabled;  
			boolean 			isOneShot =true;  
			boolean 			isUpdated =false;  
			adri_timer 			* timerUpdated;

			alarm_dailyEnd(int _pos);
			~alarm_dailyEnd();
			void 	setup(
							String _name, 
							time_t _value, 
							time_t _endValue, 
							OnTick_t _onTickHandler, 
							OnTick_t _endTickHandler
						);			
			int 	updateNextTrigger();
			void 	loop();
			void 	print();
			void 	desactivate(){delete this;}			
			void 	edit(time_t _value, time_t _endValue);			
			void 	lapse_toString		(String & ret);
			void 	repeat_toString		(String & ret);
			void 	start_toString 		(String & ret);
			void 	isEnabled_toString 	(String & ret);					
			void 	period_toString		(String & ret);	
	};	

	class alarm_weekly : public alarm_base {
		public:
			int 				objPos;
			int 				pos;
			String 				name;

			time_t 				createTime 	= 0;
			time_t 				startTime 	= 0;

			time_t 				value 		= 0;
			time_t 				nextTrigger	= 0;
			
			OnTick_t 			onTickHandler;
			OnTick_t 			endTickHandler;

			dt_alarmPeriod 		period 		= dtWeeklyAlarm;
			timeDayOfWeek_t 	dayOfWeek;

			boolean 			isEnabled;  
			boolean 			isOneShot =true;  
			boolean 			isUpdated =false;  
			adri_timer 			* timerUpdated;

			alarm_weekly(int _pos);
			~alarm_weekly();
			void 	setup(
							String _name, 
							time_t _value, 
							OnTick_t _onTickHandler, 
							timeDayOfWeek_t _dayOfWeek, 
							OnTick_t _endTickHandler
						);
			int 	updateNextTrigger();
			void 	loop();
			void 	print();			
			void 	desactivate(){delete this;}			
			void 	edit(time_t _value);	
			void 	lapse_toString		(String & ret);
			void 	repeat_toString		(String & ret);
			void 	start_toString 		(String & ret);
			void 	isEnabled_toString 	(String & ret);					
			void 	period_toString		(String & ret);						
	};

	class alarm_weeklyEnd : public alarm_base {
		public:
			int 				objPos;
			int 				pos;
			String 				name;

			time_t 				createTime 	= 0;
			time_t 				startTime 	= 0;

			time_t 				value 		= 0;
			time_t 				endValue 	= 0;
			time_t 				o_endValue 	= 0;
			time_t 				nextTrigger	= 0;
			
			OnTick_t 			onTickHandler;
			OnTick_t 			endTickHandler;

			dt_alarmPeriod 		period 		= dtWeeklyEndAlarm;
			timeDayOfWeek_t 	dayOfWeek;

			boolean 			isEnabled;  
			boolean 			isOneShot =true;  
			boolean 			isUpdated =false;  
			adri_timer 			* timerUpdated;

			alarm_weeklyEnd(int _pos);
			~alarm_weeklyEnd();
			void 	setup(
							String _name, 
							time_t _value, 
							time_t _endValue, 
							OnTick_t _onTickHandler, 
							timeDayOfWeek_t _dayOfWeek, 
							OnTick_t _endTickHandler
						);			
			int 	updateNextTrigger();
			void 	loop();
			void 	print();
			void 	desactivate(){delete this;}			
			void 	edit(time_t _value, time_t _endValue);			
			void 	lapse_toString		(String & ret);
			void 	repeat_toString		(String & ret);
			void 	start_toString 		(String & ret);
			void 	isEnabled_toString 	(String & ret);					
			void 	period_toString		(String & ret);	
	};	


	class timerAlarm
	{
	public:
		timerAlarm();
		~timerAlarm(){};
		String timertoString 		(time_t t) ;
		String period_to_string 	(dt_alarmPeriod mod) ;
		void alarm_print 			();
	};


	 void registerAlarm_weeklyEnd 	(int pos);
	 void registerAlarm_weekly 		(int pos);
	 void registerAlarm_daily 		(int pos);
	 void registerAlarm_dailyEnd	(int pos);


	 extern alarm_base * alarm_base_array[];
#endif