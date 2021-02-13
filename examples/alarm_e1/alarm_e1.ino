#include <adri_soft_logger.h>
#include <adri_tools_v2.h>
#include <ALS_timerAlarm.h>
#include <adri_wifiConnect.h>
#include <adri_timeNtp.h>
#include <adri_timer.h> 
#include "secret.h"

wifiConnect 		* myWifi;	// PTR pour unr instance statique "wifiConnect"
wifi_credential_ap	* myWifiAp;	// PTR pour unr instance statique "wifi_credential_ap"

const char 			* myWifiHostname = "MY_WIFI"; 	// AP AND DNS HOSTNAME 
													
										// 	AWC_LOOP; 		WIFI CONNECT STARTUP WITH STATIC 
										// 	AWC_SETUP; 		WIFI CONNECT STARTUP WITH STATIC 
WIFICONNECT_MOD		myWifiConnectMod 	= 	AWC_SETUP;	

										// 	AWCS_MULTI;		STA CONNECT WITH MULTIPLE SSID
										// 	AWCS_NORMAL;	STA CONNECT WITH THE SELECTED SSID
WIFICONNECTSSID_MOD myWifiSSIDMod 		= 	AWCS_NORMAL;	

boolean 			myWifiOTA 			= false; 	// ENABLED OTA

int 				myWifiConnectDone	= 0;		// WIFI CONNECT LOOP POSTION 

adri_timeNtp 		* _ntpTime;

timerAlarm 			* _timerAlarm;

adri_timer 			* _timer_disp;

adriToolsv2_serialRead    * _serial;




void setup()
{
	Serial.begin(115200);
	delay(1000);
	fsprintf("\n");

	LittleFS.begin();

	new adri_toolsV2(500);
	new adriToolsLogger(500);

    _serial = new adriToolsv2_serialRead();
    _serial->cmd_array(1, 2);
    _serial->cmd_item_add(1, "menu",                    "a", "", _serial_menu);
    _serial->cmd_item_add(1, "_serial_ESPreset",      	"z", "", _serial_ESPreset);
    _serial->cmd_array(2, 1);
    _serial->cmd_item_add(2, "activateEffect",    		"p", "", _serial_alarmPrint);
	// _serial->menu();	

	_timer_disp = new adri_timer(1000,"",true);

	myWifi 		= new wifiConnect();
	myWifiAp 	= new wifi_credential_ap("");

	myWifiAp->hostname_set(adri_toolsv2Ptr_get()->ch_toString(myWifiHostname));
	wifi_credential_ap_register(myWifiAp);

	wifi_credential_sta_fromSPIFF();
	wifi_credential_set(
		0, 						
		SECRET_SSID, 		
		SECRET_PASS, 			
		"",						
		"",						
		""						
	);	
	wifi_credential_sta_toSpiff();	
	// wifi_credential_sta_print();

	myWifi->load_fromSpiif 				();
	myWifi->credential_sta_pos_set 		(0);

	// configuration du lancement de la coonection
	myWifi->connect_set 				(myWifiConnectMod);
	if (myWifiConnectMod == AWC_LOOP) myWifiSSIDMod = AWCS_NORMAL; 	// wifi connect loop ne supporte pas encor de multiple ssid
	myWifi->connectSSID_set 			(myWifiSSIDMod);


	myWifi->station_set 				(WIFI_STA);
	myWifi->hostName_set 				(myWifiHostname); 			// initialisation dns si ota desactiver
	myWifi->setup_id					();							// initialize les id STA

	//
	myWifiAp->psk_set 					("mywifiappsk");						// pswd AP
	myWifiAp->ip_set 					(myWifi->_credential_sta->ip_get());	// ip 	AP
	myWifiAp->print 					();	



	if (myWifiConnectMod == AWC_SETUP) {
		myWifi->setup 						();
		if(!myWifiOTA) 	myWifi->MDSN_begin	();
		wifi_connect_statu 					();
		fsprintf("\n");
		ADRI_LOG(-1, 2, 2, "[myWifiConnectDone] : %s", adri_toolsv2Ptr_get()->on_time().c_str());
		myWifiConnectDone = 1;		
	}


	_ntpTime = new adri_timeNtp();
	_ntpTime->setup(true);

	_timerAlarm = new timerAlarm();

    char* cDay = dayStr(weekday());
    // Serial.printf("%s\n", cDay);
    // Serial.printf("%d\n", day(now()));
    // int search_day = calendar_date_get_dow("fr", "Vendredi"); 
    int search_day = calendar_date_get_dow("en", String(cDay)); 
    timeDayOfWeek_t thisDay = dowSunday;
    if (search_day >= 0) {
       ADRI_LOG(-1, 2, 2, "\ncalendar_date_get_dow: %s - %s - %s\n", calendar_dates[search_day].day_en, calendar_dates[search_day].day_fr, dayStr(search_day));
        thisDay = calendar_dates[search_day].dow;
    }
    time_t t1 		= AlarmHMS(hour(),minute(),second()) + AlarmHMS(0,0,30);
    time_t t2 		= AlarmHMS(hour(),minute(),second()) + AlarmHMS(0,1,30);
    time_t lapse 	= AlarmHMS(0,0,20);


 //    registerAlarm_weeklyEnd (0);
	// alarm_base_array[0]->setup(
	// 		"t1", 
	// 		t1, 
	// 		lapse, 
	// 		mF_1, 
	// 		thisDay, 
	// 		mF_2
	// 	);

	
    registerAlarm_daily (0);
	alarm_base_array[0]->setup(
			"t1", 
			t1, 
			mF_1, 
			mF_2
		);


	// // alarm_base_array[1]->desactivate();
	// alarm_base_array[1] = nullptr;

	// ADRI_LOGV(-1, 2, 2 alarm_base_array[1], "", "");
	// if(alarm_base_array[1]!=nullptr)alarm_base_array[1]->print();
	



	// alarm_base_array[2]->edit(
	// 		t1
	// 	);


}

void loop()
{
	myWifi->MDSN_loop();
	_ntpTime->loop();
	_serial->loop();
	alarm_base_array[0]->loop();
	// alarm_base_array[0]->loop();
	// alarm_base_array[1]->loop();
	// if(_timer_disp->loop()){alarm_base_array[0]->print();}
}

String _serial_menu(String cmd, String value) {
    adriToolsv2_serialReadPtr_get()->menu();
    return "";
}

String _serial_ESPreset(String cmd, String value){
    ESP.restart();
    return "";
}	

String _serial_alarmPrint(String cmd, String value){
    alarm_base_array[value.toInt()]->print();
    return "";
}		

void mF_1(int p){
	ADRI_LOG(-1, 2, 2, "[mF_1] : %s", _timerAlarm->timertoString(now()).c_str());
	alarm_base_array[p]->print();

}
void mF_2(int p){
	ADRI_LOG(-1, 2, 2, "[mF_2] : %s", _timerAlarm->timertoString(now()).c_str());
	alarm_base_array[p]->print();
	
}
