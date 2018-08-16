#pragma once
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[ID值]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[ID值]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[ID值]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXX->setText("****") 在控件TextXXX上显示文字****
*mButton1->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBar->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度
*
* 在Eclipse编辑器中  使用 “alt + /”  快捷键可以打开智能提示
*/

#include "utils/TimeHelper.h"
#include "net/NetManager.h"
#include "manager/ConfigManager.h"
#include "storage/StoragePreferences.h"
#include "tcp_transmittion/ISocketListener.h"
#include <iostream>
#include <stdio.h>


// listener
extern iiSocketListener socket_listener;
extern volatile bool power_on; 
extern int get_sd_free_capicity();
static 	pthread_t thread_tcploopID = 0;
volatile bool wifi_link_ok = false;

// file data transmtion thread loop
extern void* tcp_trans_loop(void* arg);

class wifi_info
{
public:
	std::string wifi_ssid;
	std::string wifi_psw;
	std::string wifi_mode;
	wifi_info()
	{
		wifi_ssid = "";
		wifi_psw = "";
		wifi_mode = "";
	}
};


static 	pthread_t thread_wifiID = 0;
static  wifi_info  m_wifi_info;
std::string global_ip_address = "0.0.0.0";

class MyWifiListener : public WifiManager::IWifiListener
{
public:
	virtual void handleWifiEnable(E_WIFI_ENABLE event, int args);
	virtual void handleWifiConnect(E_WIFI_CONNECT event, int args);
	virtual void handleWifiScanResult(std::vector<WifiInfo> *wifiInfos);
	virtual void handleWifiErrorCode(E_WIFI_ERROR_CODE code);
	virtual void notifySupplicantStateChange(int networkid, const char* ssid,
			const char* bssid, E_SUPPLICATION_STATE newState);
};

static MyWifiListener sWifiListener;


static void updateUI_time()
{

}

/**
 * 注册定时器
 * 在此数组中添加即可
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = 
{

};


static put_string_wrapper(std::string key, std::string val)
{
	int count = 0;
	while(1)
	{
		count++;
		StoragePreferences::putString(key, val);
		std::string str_tmp = StoragePreferences::getString(key, str_tmp);
		if(str_tmp == val)
		{
			break;
		}
		if(count>10)
		{
			LOGD("***** put_string_wrapper write error 《key》：%s,《val》:%s ", key.c_str(), val.c_str());
		}
	}
	
}

int get_wifi_info()
{
	std::ifstream fin;
	fin.open("/mnt/extsd/file/config.ini", ios::in);
	if(!fin.is_open())
	{
		LOGD("*** config.ini file not exist ***");
		fin.close();
		return -1;
	}
	Thread::sleep(200);
	std::string  tmp_str;
	getline(fin, tmp_str); // regardless of [wifi]

	getline(fin, tmp_str); // get wifi mode
	tmp_str = tmp_str.substr(0, tmp_str.length()-1);
	int pos = tmp_str.find("=");
	if(pos != -1)
	{
		m_wifi_info.wifi_mode = tmp_str.substr(pos+1);
		if(m_wifi_info.wifi_mode == "")
		return -1;
	}
	else
	{
		return -1;
	}

	getline(fin, tmp_str); // get wifi name(ssid)
	tmp_str = tmp_str.substr(0, tmp_str.length()-1);
	pos = tmp_str.find("=");
	if(pos != -1)
	{
		m_wifi_info.wifi_ssid = tmp_str.substr(pos+1);
		if(m_wifi_info.wifi_ssid == "")
		return -1;
	}
	else
	{
		return -1;
	}

	getline(fin, tmp_str); // get wifi pwd
	tmp_str = tmp_str.substr(0, tmp_str.length()-1);
	pos = tmp_str.find("=");
	if(pos != -1)
	{
		m_wifi_info.wifi_psw = tmp_str.substr(pos+1);
		if(m_wifi_info.wifi_psw == "")
			return -1;
	}
	else
	{
		return -1;
	}

	fin.close();
	return 0;
}

void show_wifi_info(std::string msg)
{
	std::string wifi_info_total = "";

	wifi_info_total = std::string("Wifi 模式  : ") + m_wifi_info.wifi_mode ;
	LOGD("<<<< %s >>>", wifi_info_total.c_str());

	wifi_info_total = std::string("Wifi 名称  : ") + m_wifi_info.wifi_ssid ;
	LOGD("<<< %s >>>", wifi_info_total.c_str());

	wifi_info_total = std::string("Wifi 密码  : ") + m_wifi_info.wifi_psw ;
	LOGD("<<< %s >>>", wifi_info_total.c_str());

	//std::string send_msg = std::string("show wifi msg:") + msg;
	//socket_listener.notify(send_msg);
}



void flash_info_dump()
{
	std::string curflash_wifi_mode =  StoragePreferences::getString("wifi_mode_flash",curflash_wifi_mode);
	std::string curflash_ap_ssid = StoragePreferences::getString("ap_ssid_flash",curflash_ap_ssid);
	std::string curflash_ap_psw = StoragePreferences::getString("ap_psw_flash",curflash_ap_psw);
	std::string curflash_link_ssid = StoragePreferences::getString("link_ssid_flash",curflash_link_ssid);
	std::string curflash_link_psw = StoragePreferences::getString("link_psw_flash",curflash_link_psw);

	LOGD("*********flash info dump *********");
	LOGD("<<curflash_wifi_mode : %s>>", curflash_wifi_mode.c_str());
	LOGD("-----------------------------------");
	LOGD("curflash_ap_ssid : %s", curflash_ap_ssid.c_str());
	LOGD("curflash_ap_psw : %s", curflash_ap_psw.c_str());
	LOGD("curflash_link_ssid : %s", curflash_link_ssid.c_str());
	LOGD("curflash_link_psw : %s", curflash_link_psw.c_str());
	LOGD("*********flash info dump end*********");
}

void flash_wifi_init()
{
	int ret = 0;
	wifi_link_ok = false;
	LOGD("read flash info and wifi init");
	flash_info_dump();
	std::string wifi_mode_flash;
	wifi_mode_flash = StoragePreferences::getString("wifi_mode_flash",wifi_mode_flash);
	if(wifi_mode_flash!="AP" && wifi_mode_flash!="LINK")
	{
		LOGD("first wifi init on new board");
		put_string_wrapper("wifi_mode_flash", "AP");

		put_string_wrapper("ap_ssid_flash", "Machine");
		put_string_wrapper("ap_psw_flash", "12345678");
		
		put_string_wrapper("link_ssid_flash", "null");
		put_string_wrapper("link_psw_flash", "null");
	}
	ret = get_wifi_info();
	LOGD("get_wifi_info(): ret = %d", ret);
	if(ret == 0)
	{
		if(m_wifi_info.wifi_mode == "mode_ap")
		{
			put_string_wrapper("wifi_mode_flash", "AP");
			put_string_wrapper("ap_ssid_flash", m_wifi_info.wifi_ssid);
			put_string_wrapper("ap_psw_flash", m_wifi_info.wifi_psw);
			LOGD("update wifi <ap> info in flash");
		}
		else if(m_wifi_info.wifi_mode == "mode_link")
		{
			put_string_wrapper("wifi_mode_flash", "LINK");
			put_string_wrapper("link_ssid_flash", m_wifi_info.wifi_ssid);
			put_string_wrapper("link_psw_flash", m_wifi_info.wifi_psw);
			LOGD("update wifi <link> info in flash");
		}
		// then rm wifi config file
		system("rm -r /mnt/extsd/file/config.ini");
		system("sync");
	}
	// then start wifi according to latest wifi info in flash
	sleep(10);
	wifi_mode_flash = StoragePreferences::getString("wifi_mode_flash", wifi_mode_flash);
	if(wifi_mode_flash == "AP")
	{
		//NETMANAGER->getSoftApManager()->setEnable(false);
		LOGD(":::::::::::::wifi will be set into mode <ap> mode now::::::::::::::");
		//  AP Mode
		int count = 0;
		NETMANAGER->getSoftApManager()->setEnable(true);
		while(NETMANAGER->getSoftApManager()->isEnable() == false)
		{
			count++;
			LOGD("ap mode setting ...");
			sleep(1);
			if(count>7)
			{
				break;
			}
		}
		std::string tmp_str;	
		sleep(5);
		NETMANAGER->getSoftApManager()->setSsidAndPwd(StoragePreferences::getString("ap_ssid_flash",tmp_str).c_str(), StoragePreferences::getString("ap_psw_flash",tmp_str).c_str());
		global_ip_address = std::string(NETMANAGER->getSoftApManager()->getIp());
	}
	else if(wifi_mode_flash == "LINK")
	{
linkrestart:
		LOGD("::::::::::::wifi will be set into mode link mode now::::::::::::");
	//  link mode
		NETMANAGER->getWifiManager()->enableWifi(true);
		int count = 0;
		while(NETMANAGER->getWifiManager()->isWifiEnable() == false)
		{
			LOGD("link mode setting ...");
			count++;
			if(count>7)
			{
				break;
			}
			sleep(1);
		}
		sleep(5);
		std::string tmp_str;
		NETMANAGER->getWifiManager()->connect(StoragePreferences::getString("link_ssid_flash",tmp_str), StoragePreferences::getString("link_psw_flash",tmp_str));
		global_ip_address = NETMANAGER->getWifiManager()->getIp();
		if(global_ip_address == "0.0.0.0")
		{
			LOGD("link addr then 0.0.0.0 restart now..");
			NETMANAGER->getWifiManager()->enableWifi(false);
			sleep(4);
			goto linkrestart;
		}
		
	}
	wifi_link_ok = true; // inform tcp transfer thread
	LOGD("$$$$$$$$$$$$$$");
    LOGD("$$$$$$$$$$$$$$");
	LOGD("$$$$$$$$$$$$$$");
	LOGD("$$$$$$$$$$$$$$");
	LOGD("<< global_ip_address >> :: [%s]", global_ip_address.c_str());
	LOGD("$$$$$$$$$$$$$$");
	LOGD("$$$$$$$$$$$$$$");
	LOGD("$$$$$$$$$$$$$$");
	LOGD("$$$$$$$$$$$$$$");
	// then show latest wifi information
	if(wifi_mode_flash == "AP")
	{
		std::string tmp_str;
		LOGD("<<wifi in AP mode>>");
		std::string ssid = StoragePreferences::getString("ap_ssid_flash",tmp_str);
		LOGD("<<ssid>> : %s", ssid.c_str());
		std::string psw = StoragePreferences::getString("ap_psw_flash",tmp_str);
		LOGD("<<psw>> : %s", psw.c_str());
		std::string send_msg = std::string("show wifi msg:wifi<热点> 初始化成功");
		socket_listener.notify(send_msg);
	}
	else if(wifi_mode_flash == "LINK")
	{
		std::string tmp_str;
		LOGD("<<wifi in LINK mode>>");
		std::string ssid = StoragePreferences::getString("link_ssid_flash",tmp_str);
		LOGD("<<ssid>> : %s", ssid.c_str());
		std::string psw = StoragePreferences::getString("link_psw_flash",tmp_str);
		LOGD("<<psw>> : %s", psw.c_str());
		std::string send_msg = std::string("show wifi msg:wifi 连接成功");
		socket_listener.notify(send_msg);
	}
	
}

#if 0   // first version of wifi int without flash saving
void wifi_init()
{
	int ret = 0;
	ret = get_wifi_info();
	LOGD("get_wifi_info(): ret = %d", ret);
	if(ret == -1)
	{
			LOGD("***** can not open wifi info config file so changed into defult AP mode", ret);
			sleep(8); // wait wifi power up init complete
		//  AP Mode
			NETMANAGER->getWifiManager()->enableWifi(false);
			NETMANAGER->getSoftApManager()->setEnable(true);
			while(NETMANAGER->getSoftApManager()->isEnable() == false)
			{
				LOGD("ap mode setting ...");
				sleep(1);
			}
			m_wifi_info.wifi_mode = "mode_ap";
			m_wifi_info.wifi_ssid = "Machine";
			m_wifi_info.wifi_psw =  "12345678";
			NETMANAGER->getSoftApManager()->setSsidAndPwd("Machine", "12345678");
			sleep(1); //wait wifi set complete
			global_ip_address = std::string(NETMANAGER->getSoftApManager()->getIp());
	}
	else
	{
		if(m_wifi_info.wifi_mode == "mode_ap")
		{
			LOGD("wifi will be set into mode ap mode now");
			sleep(8);
			//  AP Mode
			NETMANAGER->getWifiManager()->enableWifi(false);
			NETMANAGER->getSoftApManager()->setEnable(true);
			while(NETMANAGER->getSoftApManager()->isEnable() == false)
			{
				LOGD("ap mode setting ...");
				sleep(1);
			}
			std::string previous_ssid = NETMANAGER->getSoftApManager()->getSsid();
			LOGD("previous_ssid ssid : %s", previous_ssid.c_str());
			NETMANAGER->getSoftApManager()->setSsidAndPwd(m_wifi_info.wifi_ssid.c_str(), m_wifi_info.wifi_psw.c_str());
			sleep(1);
			global_ip_address = std::string(NETMANAGER->getSoftApManager()->getIp());
		}
		else if(m_wifi_info.wifi_mode == "mode_link")
		{
				LOGD("wifi will be set into mode link mode now");
				sleep(8);
			//  link mode
				NETMANAGER->getSoftApManager()->setEnable(false);
				NETMANAGER->getWifiManager()->enableWifi(true);
				while(NETMANAGER->getWifiManager()->isWifiEnable() == false)
				{
					LOGD("link mode setting ...");
					sleep(1);
				}
				NETMANAGER->getWifiManager()->connect(m_wifi_info.wifi_ssid, m_wifi_info.wifi_psw);
				sleep(1);
				global_ip_address = NETMANAGER->getWifiManager()->getIp();
		}
	}

	LOGD("<< global_ip_address >> :: [%s]", global_ip_address.c_str());
	if(m_wifi_info.wifi_mode == "mode_link")
	{
		show_wifi_info("wifi 初始化成功 <连接模式>");
	}
	else if(m_wifi_info.wifi_mode == "mode_ap")
	{
		show_wifi_info("wifi 初始化成功 <热点模式>");
	}
	
}
#endif



static void onUI_init()
{
    //Tips :添加 UI初始化的显示代码到这里,如:mText1->setText("123");
	// init thread and related resources
	// init wifi init thread
	#if 1
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int ret = pthread_create(&thread_wifiID, &attr, flash_wifi_init, NULL);
	pthread_attr_destroy(&attr);
	if (ret||!thread_wifiID)
	{
		return;
	}
	pthread_detach(thread_wifiID);
	#endif

	// get current free storage of sdcard
	int free_sd_cap = get_sd_free_capicity();
	LOGD("<< free_sd_cap ： %d kb>> ", free_sd_cap);

	// init file transmistion thread
    ret = 0;
	pthread_attr_init(&attr);  	   
	ret = pthread_create(&thread_tcploopID, &attr, tcp_trans_loop, NULL);
	pthread_attr_destroy(&attr);
	if (ret || !thread_tcploopID)
	{
		return;
	}
	pthread_detach(thread_tcploopID);

	// start to play video
	EASYUICONTEXT->openActivity("playActivity");
}

static void onUI_quit() 
{
	
}


#define  POWER_ON_OFF    	   0x00
static void onProtocolDataUpdate(const SProtocolData &data) 
{
    // 串口数据回调接口
	switch(data.cmd) 
	{
		case POWER_ON_OFF:
			EASYUICONTEXT->openActivity("playActivity");
		break;

		default: break;

	}

}

static bool onUI_Timer(int id)
{
    //Tips:添加定时器响应的代码到这里,但是需要在本文件的 REGISTER_ACTIVITY_TIMER_TAB 数组中 注册
    //id 是定时器设置时候的标签,这里不要写耗时的操作，否则影响UI刷新,ruturn:[true] 继续运行定时器;[false] 停止运行当前定时器
	if(id == 0) 
	{

		return true;
	}
	if(id == 1)
	{

		return false;
	}

}


static bool onmainActivityTouchEvent(const MotionEvent &ev) 
{
    // 返回false触摸事件将继续传递到控件上，返回true表示该触摸事件在此被拦截了，不再传递到控件上
    return false;
}




void MyWifiListener::handleWifiEnable(E_WIFI_ENABLE event, int args) {
	switch (event) {
	case E_WIFI_ENABLE_ENABLE:
		break;
	case E_WIFI_ENABLE_DISABLE:

		break;
	case E_WIFI_ENABLE_DISABLEING:
	case E_WIFI_ENABLE_ENABLEING:

		break;
	case E_WIFI_ENABLE_UNKNOW:

		break;
	}
}

void MyWifiListener::handleWifiConnect(E_WIFI_CONNECT event, int args) {
	switch (event) {
		case E_WIFI_CONNECT_CONNECTED:
			break;

		case E_WIFI_CONNECT_CONNECTING:
			break;

		case E_WIFI_CONNECT_ERROR:
			break;
		}

}

void MyWifiListener::handleWifiScanResult(std::vector<WifiInfo> *wifiInfos) {

}

void MyWifiListener::handleWifiErrorCode(E_WIFI_ERROR_CODE code) {
	switch (code) {
	case E_WIFI_ERROR_CODE_PASSWORD_INCORRECT:

		break;
	}
}

void MyWifiListener::notifySupplicantStateChange(int networkid, const char *ssid, const char *bssid, E_SUPPLICATION_STATE newState) {
	switch (newState) {
	case DISCONNECTED:
		break;
	case INTERFACE_DISABLED:
		break;
	case INACTIVE:
		break;
	case SCANNING:
		break;
	case AUTHENTICATING:
		break;
	case ASSOCIATING:
		break;
	case ASSOCIATED:
		break;
	case FOUR_WAY_HANDSHAKE:
		break;
	case GROUP_HANDSHAKE:
		break;
	case COMPLETED:
		break;
	case DORMANT:
		break;
	case UNINITIALIZED:
		break;
	case INVALID:
		break;
	}

}
