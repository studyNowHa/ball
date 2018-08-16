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

#include "system/Thread.h"
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "utils/TimeHelper.h"
#include <sys/reboot.h>
#include "utils/BrightnessHelper.h"
#include "utils/Log.h"
#include "tcp_transmittion/ISocketListener.h"
#include "net/NetManager.h"
#include "storage/StoragePreferences.h"

iiSocketListener socket_listener;

/* commands from uart*/
#define  POWER_ON_OFF    	   0x00
#define  PLAY      			   0x01
#define  PAUSE       	       0x02
#define  STOP   	   		   0x03
#define  PREVIOUS_PLAY         0x04
#define  NEXT_PLAY        	   0x05
#define  FORWARD        	   0x06
#define  BACKWARD        	   0x07
#define  LUMI_UP        	   0x08
#define  LUMI_DOWN        	   0x09
#define  SOUND_UP        	   0x0A
#define  SOUND_DOWN        	   0x0B
#define  CLOCK_ON_OFF      	   0x0C
#define  PLAY_MODE_CHANGE      0x0D
#define  WIFI_MODE_CHANGE      0x0E
#define  SYS_RESET      	   0x0F
/* commands */


/* play indicator info api */
#if 1
#define PLAY_STOP_INDICATER  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("stop.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define SOUND_UP_INDICATER(value)  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("soundup.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mLumiValuePtr->setText(value);\
								mLumiValuePtr->setVisible(true);\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
								mLumiValuePtr->setVisible(false);\
							}while(0)
#define SOUND_DOWN_INDICATER(value)  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("sound_down.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mLumiValuePtr->setText(value);\
								mLumiValuePtr->setVisible(true);\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
								mLumiValuePtr->setVisible(false);\
							}while(0)
#define SINGLE_LOOP_INDICATER  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("singleloop.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define RANDOM_INDICATER  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("random.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define PREVIOUS_INDICATER  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("previous.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define PAUSE_INDICATER  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("pause.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define NEXT_INDICATER  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("next.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define LUMI_INDICATER(value)  \
							do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("lumi.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mLumiValuePtr->setText(value);\
								mLumiValuePtr->setVisible(true);\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
								mLumiValuePtr->setVisible(false);\
							}while(0)
#define LOOP_INDICATER  \
						do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("loop.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define FORWARD_INDICATER  \
						do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("forward.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#define BACKWARD_INDICATER  \
						do{\
								std::string tmp_str = std::string("/mnt/extsd/indicator/") + std::string("back.png");\
								mVideoIndicatorViewPtr->setBackgroundPic(tmp_str.data());\
								mVideoIndicatorViewPtr->setVisible(true);\
								Thread::sleep(700);\
								mVideoIndicatorViewPtr->setVisible(false);\
							}while(0)
#endif


#define NO_CONFIG_FILE  -2
#define CONFIG_EXIST     0

typedef enum {
	SINGLE_LOOP_PLAY,
	LOOP_PLAY,
	RANDOM_PLAY
}PLAY_MODE;


/* material info varialbes*/
static std::map<int, std::string> order_count;
static std::map<int, std::string> order_name;
static std::vector<int> material_number;
std::vector<std::string> material_string;
static std::map<std::string, int>  material_map;
/* material info varialbes*/


/* signal variables */
static volatile int current_material_index = 0; 
static volatile int pic_current_index_tmp = 0;
static volatile bool is_video_playing = false; 
static volatile bool is_video_stop_status = false;

static volatile bool pic_holding_on = false;
static volatile bool pic_stop = false;
static volatile bool pic_forward = false;
static volatile bool pic_backward = false;
static volatile bool video_running = true;
static volatile bool clock_show = false;
static volatile int  current_play_mode = SINGLE_LOOP_PLAY;
static int pic_loop_count = 0;
/* signal variables */

static 	pthread_t thread_playloopID = 0;


extern std::string global_ip_address;
extern volatile bool tcp_running;
extern volatile bool tcp_heart_running;
extern volatile bool data_trans_running;

extern flash_wifi_init();

static volatile int timesSC = 0;
static volatile int timesMC = 0;
static volatile int timesHC = 0;

static volatile float cur_volum = 0.5;

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

static void update_time()
{
	char timeStr[20];
	//static bool bflash = false;
	struct tm *t = TimeHelper::getDateTime();

	sprintf(timeStr, "%d/%02d/%02d", 1900 + t->tm_year, t->tm_mon + 1, t->tm_mday);
	mTextDatePtr->setText(timeStr);

	static const char *day[] = { "日", "一", "二", "三", "四", "五", "六" };
	sprintf(timeStr, "星期%s", day[t->tm_wday]);
	mTextWeekPtr->setText(timeStr);

	timesSC = t->tm_sec;
	timesMC = t->tm_min;
	timesHC = t->tm_hour;

	mPointerSecondPtr->setTargetAngle(timesSC*6);
	mPointerMinutePtr->setTargetAngle(timesMC*6);
	mPointerHourPtr->setTargetAngle(timesHC*5*6+timesMC*1/2);
}


#if 0
void play_indacator_init()
{
	mStopPicPtr->setVisible(false);
	mSoundUpPicPtr->setVisible(false);
	mSounddownPicPtr->setVisible(false);
	mSingleloopPicPtr->setVisible(false);
	mRandomPicPtr->setVisible(false);
	mPreviousPicPtr->setVisible(false);
	mPausePicPtr->setVisible(false);
	mNextPicPtr->setVisible(false);
	mLumiPicPtr->setVisible(false);
    mLoopPicPtr->setVisible(false);
	mForwardPicPtr->setVisible(false);f
	mBackwardPicPtr->setVisible(false);
	mLumiValuePtr->setVisible(false);
}
#endif

void show_datetime_info()
{
	sleep(1);
	clock_show = true;
	mPicTextviewPtr->setVisible(false);
	mVideoview1Ptr->setVisible(false);
	
	mPointerSecondPtr->setVisible(true);
	mPointerMinutePtr->setVisible(true);
	mPointerHourPtr->setVisible(true);

	mTextDatePtr->setVisible(true);
	mTextWeekPtr->setVisible(true);
	mDigitalclockPtr->setVisible(true);
	sleep(1);
}

void hide_datetime_info()
{	
	sleep(1);
	mTextDatePtr->setVisible(false);
	mTextWeekPtr->setVisible(false);
	mDigitalclockPtr->setVisible(false);

	mPointerSecondPtr->setVisible(false);
	mPointerMinutePtr->setVisible(false);
	mPointerHourPtr->setVisible(false);

	clock_show = false;
	sleep(1);
}


void reload_play_times()
{
    for(int i=0; i<material_string.size(); i++)
    {
        material_map[material_string[i]] = material_number[i];
    }
}

int get_material_info(std::string material_path)
{
	order_count.clear();
	order_name.clear();
	material_number.clear();
	material_string.clear();
	material_map.clear();

    std::ifstream fin;
    fin.open(material_path.c_str(), ios::in);
    if(!fin.is_open())
    {
        std::cerr << "file not exist" << std::endl;
        return NO_CONFIG_FILE;
    }
    std::string  tmp_str;
    getline(fin, tmp_str); // regardless of [pictureTime]
    while (true)
    {
        getline(fin, tmp_str);
		LOGD("tmp_str %s",  tmp_str.c_str());
        if(tmp_str.compare(0, 11, "[playOrder]") == 0)
        {
        	LOGD("break\r\n");
            break;
        }
        int pos = 0;
        pos = tmp_str.find("=");
        if(pos!=-1)
        {
			int order = atoi(tmp_str.substr(0,pos).c_str());
			order_count[order] = tmp_str.substr(pos+1);	
			//LOGD("order %d  + count %s", order, tmp_str.substr(pos+1).c_str());			
        }
    }
	//LOGD("order_count[0] = %s", order_count[0].c_str());

    while(getline(fin, tmp_str))
    {
        int pos1 = 0;
        int pos2 = 0;
        pos1 = tmp_str.find("=");
        pos2 = tmp_str.find(".");
        if( (pos1!=-1) && (pos2!=-1) && (tmp_str!="") )
        {
			int order =  atoi(tmp_str.substr(0,pos1).c_str());
            tmp_str = tmp_str.substr(pos1+1);
            int pos = tmp_str.find_last_of("/");
            tmp_str = tmp_str.substr(pos+1);
            tmp_str = tmp_str.substr(0, tmp_str.length()-1);
            order_name[order] = tmp_str;
        }
    }

	for(int i=0; i<order_count.size(); i++)
	{
		material_number.push_back(atoi(order_count[i].c_str())) ;
		material_string.push_back(order_name[i].c_str());
	}

    for(int i=0; i<material_string.size(); i++)
    {
        material_map[material_string[i]] = material_number[i];
    }

    LOGD("materials info as follows: \r\n");
    for(int i=0; i<material_string.size(); i++)
    {
    	LOGD("%s --- %d", material_string[i].c_str(), material_map[material_string[i]]);
    }

    fin.close();
    return CONFIG_EXIST;
}

std::string get_file_postfix(std::string file_name)
{
    if(file_name == "")
    {
        return "";
    }
    int pos = file_name.find(".");
    if(pos == -1)
    {
        return "";
    }
    else
    {
        file_name = file_name.substr(pos+1);
    }
    return file_name;

}

void my_delay(int seconds)
{
	struct timeval temp;
	temp.tv_sec = seconds;
	temp.tv_usec = 0;
	select(0, NULL, NULL, NULL, &temp);
	return;
}

void play_loop()
{
	LOGD("entering play_loop thread now");
	current_material_index = 0; 
	pic_current_index_tmp = 0;
    is_video_playing = false; 
    is_video_stop_status = false;
    pic_holding_on = false;
    pic_stop = false;
    pic_forward = false;
    pic_backward = false;
    video_running = true;
    clock_show = false;
	pic_loop_count = 0;
    //current_play_mode = SINGLE_LOOP_PLAY;
	sleep(3);
	while(video_running) 
	{
		Thread::sleep(200);
		std::string current_material_name = material_string[current_material_index];
		std::string material_type =  get_file_postfix(current_material_name);
		std::string final_material_string = std::string("/mnt/extsd/materials/") + std::string(current_material_name);
		LOGD(" index %d >> load %s", current_material_index, final_material_string.c_str());
		if(material_type.compare(0, 3, "mp4") == 0) 
		{
			if(is_video_playing == false) 
			{
				mPicTextviewPtr->setVisible(false);
				mVideoview1Ptr->setVisible(true);
				is_video_playing = true;
				mVideoview1Ptr->play(final_material_string.c_str());
			}
		}
		else 
		{
			pic_current_index_tmp = current_material_index;
			mPicTextviewPtr->setVisible(true);
			mVideoview1Ptr->setVisible(false);
			mPicTextviewPtr->setBackgroundPic(final_material_string.c_str());
		    pic_loop_count=0;
			int pic_total_time = material_map[current_material_name];
			LOGD("current pic total time = %d", pic_total_time);
			while(pic_loop_count < pic_total_time)
			{
				pic_loop_count++;
				if(pic_stop == true) 
				{
					pic_stop = false;
					break;
				}
				while(pic_holding_on) 
				{
					Thread::sleep(100);
				}
				if(pic_forward == true) 
				{
					pic_forward = false;
					if(pic_loop_count+2 < pic_total_time)
					{
						pic_loop_count+=2;
					}
					mPlaySeekbarPtr->setVisible(true);
					mPlaySeekbarPtr->setMax(pic_total_time);
					mPlaySeekbarPtr->setProgress(pic_loop_count);
					Thread::sleep(500);
					mPlaySeekbarPtr->setVisible(false);
				}
				if(pic_backward == true) 
				{
					pic_backward = false;
					pic_loop_count-=2;
					if(pic_loop_count < 0) 
					{
						pic_loop_count = 0;
					}
					mPlaySeekbarPtr->setVisible(true);
					mPlaySeekbarPtr->setMax(pic_total_time);
					mPlaySeekbarPtr->setProgress(pic_loop_count);
					Thread::sleep(500);
					mPlaySeekbarPtr->setVisible(false);
				}
				my_delay(1);
			}
			LOGD("current_material_index=%d  -- pic_current_index_tmp=%d", current_material_index, pic_current_index_tmp);
		    if(current_material_index == pic_current_index_tmp)
			{
				if(current_play_mode == LOOP_PLAY)
				{
					current_material_index++;
					if(current_material_index == material_string.size())
					{
							reload_play_times();
							current_material_index = 0;
					}
				}
				else if(current_play_mode == RANDOM_PLAY)
				{
					reload_play_times();
					current_material_index = rand()%material_string.size();
				}
			}
		
		}
	}

	LOGD("leaving play_loop thread now");
}

/**
 * 注册定时器
 * 在此数组中添加即可
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = 
{
	{0,  1000},
	//{1,  200},
};

void  start_loop_thread()
{
	int rc = 0;
	LOGD("get information of all materials");
	// get information of all materials
	rc = get_material_info("/mnt/extsd/materials/playorder.ini");
	if(rc == NO_CONFIG_FILE)  // show info of date&time
	{
		show_datetime_info();
	}
	else if(rc == CONFIG_EXIST)
	{
		hide_datetime_info();
		// start play materials thread
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		int ret = pthread_create(&thread_playloopID, &attr, play_loop, NULL);
		pthread_attr_destroy(&attr);
		if (ret || !thread_playloopID)
		{
			return;
		}
		pthread_detach(thread_playloopID);
	}
}


static void onUI_init()
{
	int rc;
    current_material_index = 0; 
    is_video_playing = false; 
    is_video_stop_status = false;
    pic_holding_on = false;
    pic_stop = false;
    pic_forward = false;
    pic_backward = false;
    video_running = true;
    clock_show = false;
    current_play_mode = SINGLE_LOOP_PLAY; //default play mode

    //update time
    update_time();

    //Tips :添加 UI初始化的显示代码到这里,如:mText1->setText("123");
	mVideoIndicatorViewPtr->setVisible(false);
	mPicTextviewPtr->setVisible(false);
	mVideoview1Ptr->setVisible(false);
	mVideoview1Ptr->setVolume(cur_volum);
	mPlaySeekbarPtr->setVisible(false);
	mPlaySeekbarPtr->setMax(100);
	mStatusTextviewPtr->setVisible(false);
	mDigitalclockPtr->setVisible(false);
	mTextWeekPtr->setVisible(false);
	mTextDatePtr->setVisible(false);
    //hide all indicator 
	//play_indacator_init();
	//hide rtc show first
	hide_datetime_info();
	// start rtc time loop or materials loop depend on the existence of materials folder
	start_loop_thread();

}

static void onUI_quit() 
{
	video_running = false;
	tcp_running = false;
	pic_stop = true;
	Thread::sleep(5000);
	LOGD("UI exit now ..");
}


static void show_current_status(std::string msg, int timer)
{
	mStatusTextviewPtr->setVisible(true);
	mStatusTextviewPtr->setText(msg);
	Thread::sleep(timer);
	mStatusTextviewPtr->setVisible(false);
}


static void onProtocolDataUpdate(const SProtocolData &data) 
{
    // 串口数据回调接口
	switch(data.cmd) 
	{
		case POWER_ON_OFF: 
		{
			// shut down
			mPicTextviewPtr->setVisible(false);
			mVideoview1Ptr->stop();
			mVideoview1Ptr->setVisible(false);
			mPlaySeekbarPtr->setVisible(false);
			mStatusTextviewPtr->setVisible(false);
			mTextDatePtr->setVisible(false);
			mTextWeekPtr->setVisible(false);
			mDigitalclockPtr->setVisible(false);
			hide_datetime_info();
			//reset sound lumi here
			BRIGHTNESSHELPER->setBrightness(70);

			video_running = false;
			pic_holding_on = false;
			pic_stop = true;
			// trans.cpp socket thread stop 
			tcp_heart_running = false;
			data_trans_running = false;
			tcp_running = false;
			sleep(7);
            //disable wifi func
			NETMANAGER->getSoftApManager()->setEnable(false);
			NETMANAGER->getWifiManager()->enableWifi(false);
			LOGD("hardware shut down now ... ");
			show_current_status("关机中", 3000);
		}
		break;

		case PLAY:
		{ 
			if(clock_show == true)
			{
				break;
			}
			if(is_video_playing == true) 
			{
				if(!mVideoview1Ptr->isPlaying())
				{
					if(is_video_stop_status == true)
					{
						is_video_stop_status = false;
						std::string current_material_name = material_string[current_material_index];
						std::string final_material_string = std::string("/mnt/extsd/materials/") 
															+ std::string(current_material_name);
						mVideoview1Ptr->play(final_material_string.c_str());
					}
					else
					{
						mVideoview1Ptr->resume();	
					}
					
				}
			}
			else 
			{
				if(pic_holding_on == true)
				{
					mPicTextviewPtr->setVisible(true);
					pic_holding_on = false;
				}
			}
			show_current_status("开始播放", 700);
		}
		break;

		case PAUSE:
		{
			if(clock_show == true)
			{
				break;
			}
			if(is_video_playing == true) 
			{
				if(mVideoview1Ptr->isPlaying())
				{
					mVideoview1Ptr->pause();			
				}
			}
			else 
			{
				if(pic_holding_on == false)
				{
					pic_holding_on = true;
				}
			}
			PAUSE_INDICATER;
			show_current_status("暂停播放", 700);
		}
		break;

		case STOP:
		{
			if(clock_show == true)
			{
				break;
			}
			if(is_video_playing == true) 
			{
				mVideoview1Ptr->stop();	
				is_video_stop_status = true;
			}
			else 
			{
				if(pic_holding_on == false)
				{
					pic_holding_on = true;					
				}
				mPicTextviewPtr->setVisible(false);
				pic_loop_count = 0;
			}
			PLAY_STOP_INDICATER;
			show_current_status("停止播放", 700);
			
			
		}
		break;

		case NEXT_PLAY: 
		{
			if(clock_show == true)
			{
				break;
			}
			reload_play_times();
			current_material_index++;
			if(current_material_index == material_string.size())
			{
				current_material_index = 0;
			}
			if(is_video_playing == true) 
			{	
				mVideoview1Ptr->stop();
				is_video_playing = false;
			}
			else 
			{
				if(pic_holding_on == true) 
				{
					pic_holding_on = false;
				}
				pic_stop = true;
				mPicTextviewPtr->setVisible(true);	
			}
			NEXT_INDICATER;
			show_current_status("下一素材", 700);
		}
		break;

		case PREVIOUS_PLAY: 
		{
			if(clock_show == true)
			{
				break;
			}
			reload_play_times();
			current_material_index--;
			if(current_material_index == -1) 
			{
				current_material_index = material_string.size()-1;
			}
			if(is_video_playing == true) 
			{
				mVideoview1Ptr->stop();
				is_video_playing = false;
			}
			else 
			{
				if(pic_holding_on == true) 
				{
					pic_holding_on = false;
				}
				pic_stop = true;
				mPicTextviewPtr->setVisible(true);	
			}
			PREVIOUS_INDICATER;
			show_current_status("上一素材", 700);
		}
		break;

		case  FORWARD: 
		{
			if(clock_show == true)
			{
				break;
			}
			if(is_video_playing == true) 
			{
				if(is_video_stop_status == false)
				{
					mVideoview1Ptr->resume();
					mPlaySeekbarPtr->setMax(mVideoview1Ptr->getDuration());				
					int current_pos_video = mVideoview1Ptr->getCurrentPosition();
					int move_dest = current_pos_video + 5000; // add 2s
					if(move_dest < mVideoview1Ptr->getDuration())
					{
						mPlaySeekbarPtr->setVisible(true);
						Thread::sleep(1000);
						mVideoview1Ptr->seekTo(move_dest);
						mPlaySeekbarPtr->setProgress(move_dest);
						mPlaySeekbarPtr->setVisible(false);
					}
				}
				
			}
			else 
			{
				if(pic_holding_on == true) 
				{
					pic_holding_on = false;
				}
				pic_forward = true;
			}
			FORWARD_INDICATER;
			show_current_status("快进", 700);
		}
		break;

		case  BACKWARD: 
		{
			if(clock_show == true)
			{
				break;
			}
			if(is_video_playing == true) 
			{
				if(is_video_stop_status == false)
				{
					mVideoview1Ptr->resume();
					mPlaySeekbarPtr->setMax(mVideoview1Ptr->getDuration());				
					int current_pos_video = mVideoview1Ptr->getCurrentPosition();
					int move_dest = current_pos_video -5000; // 回退
					if(move_dest > 0)
					{
						mPlaySeekbarPtr->setVisible(true);
						mVideoview1Ptr->seekTo(move_dest);
						mPlaySeekbarPtr->setProgress(move_dest);
						mPlaySeekbarPtr->setVisible(false);
					}
				}
			}
			else 
			{
				if(pic_holding_on == true) 
				{
					pic_holding_on = false;
				}
				pic_backward = true;
			}
			BACKWARD_INDICATER;
			show_current_status("回退", 700);
		}
		break;

		case LUMI_UP:
		{
			if(BRIGHTNESSHELPER->getBrightness()+10 > 100)
			{
				LUMI_INDICATER(100);
				show_current_status("已达最大亮度", 700);
				break;
			}
			BRIGHTNESSHELPER->setBrightness(BRIGHTNESSHELPER->getBrightness()+10);
			LUMI_INDICATER(BRIGHTNESSHELPER->getBrightness());
			show_current_status("亮度 + ", 700);
		}
		break;

		case LUMI_DOWN:
		{
			if(BRIGHTNESSHELPER->getBrightness()-10 < 0)
			{
				LUMI_INDICATER(0);
				show_current_status("已达最小亮度", 700);
				break;
			}
			BRIGHTNESSHELPER->setBrightness(BRIGHTNESSHELPER->getBrightness()-10);
			LUMI_INDICATER(BRIGHTNESSHELPER->getBrightness());
			show_current_status("亮度 - ", 700);
		}
		break;

		case SOUND_UP:
		{		
			if(cur_volum+0.1 > 1.0)
			{
				show_current_status("声音已达最大", 700);
			}
			else
			{
				cur_volum += 0.1;
				mVideoview1Ptr->setVolume(cur_volum);
			}
			show_current_status("声音 + ", 700);
			SOUND_UP_INDICATER((int)(cur_volum*100));
			
		}
		break;

		case SOUND_DOWN:
		{		
			if(cur_volum-0.1 < 0)
			{
				show_current_status("声音已达最小", 700);
			}
			else
			{
				cur_volum -= 0.1;
				mVideoview1Ptr->setVolume(cur_volum);
			}
			show_current_status("声音 - ", 700);
			SOUND_UP_INDICATER((int)(cur_volum*100));
		}
		break;

		case CLOCK_ON_OFF:
		{	
			if(clock_show == true) // already show then turn off date show
			{
				hide_datetime_info();
				// resume what is playing now 
				if(is_video_playing == true) 
				{
					mVideoview1Ptr->setVisible(true);
					if(!mVideoview1Ptr->isPlaying())
					{
						if(is_video_stop_status == true)
						{
							is_video_stop_status = false;
							std::string current_material_name = material_string[current_material_index];
							std::string final_material_string = std::string("/mnt/extsd/materials/") 
																+ std::string(current_material_name);
							mVideoview1Ptr->play(final_material_string.c_str());
						}
						else
						{
							mVideoview1Ptr->resume();	
						}
						
					}
				}
				else 
				{
					if(pic_holding_on == true)
					{						
						pic_holding_on = false;
					}
					mPicTextviewPtr->setVisible(true);
				}
			}	
			else
			{
				// pause what is playing now first
				if(is_video_playing == true) 
				{
					if(mVideoview1Ptr->isPlaying())
					{
						mVideoview1Ptr->pause();			
					}
				}
				else 
				{
					if(pic_holding_on == false)
					{
						pic_holding_on = true;
					}
				}
				show_datetime_info();
			}
			
		}
		break;

		case PLAY_MODE_CHANGE:
		{
			switch(current_play_mode)
			{
				case SINGLE_LOOP_PLAY:
				current_play_mode = LOOP_PLAY;
				LOOP_INDICATER;
				show_current_status("进入连续循环播放模式", 700);
				break;
				case LOOP_PLAY:
				current_play_mode = RANDOM_PLAY;
				RANDOM_INDICATER;
				show_current_status("进入随机循环播放模式", 700);
				break;
				case RANDOM_PLAY:
				current_play_mode = SINGLE_LOOP_PLAY;
				SINGLE_LOOP_INDICATER;
				show_current_status("进入单曲循环播放模式", 700);
				break;
				default:break;
			}

		}
		break;

		case SYS_RESET:
		{
			// reset ui first
			mPicTextviewPtr->setVisible(false);
			mVideoview1Ptr->stop();
			mVideoview1Ptr->setVisible(false);
			mPlaySeekbarPtr->setVisible(false);
			mStatusTextviewPtr->setVisible(false);
			mTextDatePtr->setVisible(false);
			mTextWeekPtr->setVisible(false);
			mDigitalclockPtr->setVisible(false);
			//reset sound lumi here
			BRIGHTNESSHELPER->setBrightness(70);

			video_running = false;
			pic_holding_on = false;
			// trans.cpp socket thread stop 
			tcp_heart_running = false;
			data_trans_running = false;
			tcp_running = false;
			pic_stop = true;
			sleep(2);

			int rc = 0;
			rc = system("rm -rf /mnt/extsd/materials");
			rc = system("rm -rf /mnt/extsd/file");
			system("sync");
			put_string_wrapper("wifi_mode_flash", "AP");
			put_string_wrapper("ap_ssid_flash", "Machine");
			put_string_wrapper("ap_psw_flash", "12345678");
			put_string_wrapper("link_ssid_flash", "null");
			put_string_wrapper("link_psw_flash", "null");	

			NETMANAGER->getSoftApManager()->setEnable(false);
			NETMANAGER->getWifiManager()->enableWifi(false);

			LOGD("hardware reset to default mode now ... ");
			show_current_status("恢复出厂设置中..", 5000);
			reboot(RB_AUTOBOOT);
		}
		default: break;
	}
}

static bool onUI_Timer(int id)
{
    //Tips:添加定时器响应的代码到这里,但是需要在本文件的 REGISTER_ACTIVITY_TIMER_TAB 数组中 注册
    //id 是定时器设置时候的标签,这里不要写耗时的操作，否则影响UI刷新,ruturn:[true] 继续运行定时器;[false] 停止运行当前定时器
	if(id == 0) 
	{
		timesSC++;
		mPointerSecondPtr->setTargetAngle(timesSC*6);
		if((timesSC/60)>0){//分钟指针
			timesSC = 0;
			timesMC++;
			mPointerMinutePtr->setTargetAngle(timesMC*6);
			mPointerHourPtr->setTargetAngle(timesHC*5*6+timesMC*1/2);//小时指针
		}
		if((timesMC/60)>0){//小时指针
			timesMC = 0;
			timesHC++;
		}
		if(timesHC>24){
			timesHC = 0;//24小时清零
		}
	}

    return true;
}


static bool onplayActivityTouchEvent(const MotionEvent &ev) 
{
    // 返回false触摸事件将继续传递到控件上，返回true表示该触摸事件在此被拦截了，不再传递到控件上
    return false;
}

static void onVideoViewPlayerMessageListener_Videoview1(ZKVideoView *pVideoView, int msg) 
{
	switch (msg) 
	{
		case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED:
			break;
		case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED:
			if(current_play_mode == SINGLE_LOOP_PLAY)
			{
				goto out;
			}
			material_map[material_string[current_material_index]] = material_map[material_string[current_material_index]]-1;
			if(material_map[material_string[current_material_index]] == 0) //play number equal 0
			{
				reload_play_times();
				if(current_play_mode == RANDOM_PLAY)
				{
					int random_index = rand()%material_string.size();
					current_material_index = random_index;
					goto out;
				}
				current_material_index++;
				if(current_material_index == material_string.size())
				{						
						current_material_index = 0;
				}
			}
			out: 
			is_video_playing = false;
			Thread::sleep(200);
			break;
		case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_ERROR:
			break;
	}
}

static void onProgressChanged_PlaySeekbar(ZKSeekBar *pSeekBar, int progress) 
{
    //LOGD(" ProgressChanged PlaySeekbar %d !!!\n", progress);
}

void cleanDir(const std::string &path)
{
        DIR *pDir = NULL;
        struct dirent *dmsg;
        char szFileName[128];
        char szFolderName[128];

        strcpy(szFolderName, path.c_str());
        strcat(szFolderName, "/%s");
        if ((pDir = opendir(path.c_str())) != NULL)
        {
           // __android_log_print(ANDROID_LOG_INFO, COM_HSDI_LOG_TITLE, "open dir success");
            // 遍历目录并删除文件
            while ((dmsg = readdir(pDir)) != NULL)
            {
               // __android_log_print(ANDROID_LOG_INFO, COM_HSDI_LOG_TITLE, "do while %s-->%s", szFileName);
                if (strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
                {
                    sprintf(szFileName, szFolderName, dmsg->d_name);
                   // __android_log_print(ANDROID_LOG_INFO, COM_HSDI_LOG_TITLE, "do remove %s", szFileName);
                    std::string tmp = szFileName;
                    //如果是文件夹，名称中不包含"."
                    if (tmp.find(".") == -1){
                    	cleanDir(szFileName);
                    }
                    remove(szFileName);
                }
            }
        }

        if (pDir != NULL)
        {
            closedir(pDir);
        }
}



void iiSocketListener::notify(std::string msg)
{
	SProtocolData cmd_data;
	if(msg == "power on_off")
	{
		LOGD("excute <power on_off> cmd");
		cmd_data.cmd = POWER_ON_OFF;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "play")
	{
		LOGD("excute <play> cmd");
		cmd_data.cmd = PLAY;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "stop")
	{
		LOGD("excute <stop> cmd");
		cmd_data.cmd = STOP;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "pause")
	{
		LOGD("excute <pause> cmd");
		cmd_data.cmd = PAUSE;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "previous play")
	{
		LOGD("excute <previous play> cmd");
		cmd_data.cmd = PREVIOUS_PLAY;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "next play")
	{
		LOGD("excute <next play> cmd");
		cmd_data.cmd = NEXT_PLAY;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "next play")
	{
		LOGD("excute <next play> cmd");
		cmd_data.cmd = NEXT_PLAY;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "forward")
	{
		LOGD("excute <forward> cmd");
		cmd_data.cmd = FORWARD;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "backward")
	{
		LOGD("excute <backward> cmd");
		cmd_data.cmd = BACKWARD;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "lumi up")
	{
		LOGD("excute <lumi up> cmd");
		cmd_data.cmd = LUMI_UP;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "lumi down")
	{
		LOGD("excute <lumi down> cmd");
		cmd_data.cmd = LUMI_DOWN;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "sound up")
	{
		LOGD("excute <sound up> cmd");
		cmd_data.cmd = SOUND_UP;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "sound down")
	{
		LOGD("excute <sound down> cmd");
		cmd_data.cmd = SOUND_DOWN;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "clock on_off")
	{
		LOGD("excute <clock on_off> cmd");
		cmd_data.cmd = CLOCK_ON_OFF;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "mode change")
	{
		LOGD("excute <play mode change> cmd");
		cmd_data.cmd = PLAY_MODE_CHANGE;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "re start system")
	{
		// reset ui first
		mPicTextviewPtr->setVisible(false);
		mVideoview1Ptr->stop();
		mVideoview1Ptr->setVisible(false);
		mPlaySeekbarPtr->setVisible(false);
		mStatusTextviewPtr->setVisible(false);
		mTextDatePtr->setVisible(false);
		mTextWeekPtr->setVisible(false);
		mDigitalclockPtr->setVisible(false);
		video_running = false;
		pic_holding_on = false;
		// trans.cpp socket thread stop 
		tcp_heart_running = false;
		data_trans_running = false;
		tcp_running = false;
		pic_stop = true;
		sleep(2);
		NETMANAGER->getSoftApManager()->setEnable(false);
		NETMANAGER->getWifiManager()->enableWifi(false);
		LOGD("hardware reset to default mode now ... ");
		show_current_status("系统重启中..", 5000);
		reboot(RB_AUTOBOOT);
	}
	else if(msg == "sys reset")
	{
		LOGD("excute <sys reset> cmd");
		cmd_data.cmd = SYS_RESET;
		onProtocolDataUpdate(cmd_data);
	}
	else if(msg == "upload stop playing")
	{
		LOGD("reveive upload stop playing from pc");
		show_current_status("准备上传素材...", 4000);
		if(clock_show == false)
		{
			if(is_video_playing == true)
			{
				mVideoview1Ptr->stop();
			}
			else
			{
				pic_holding_on = true;
			}
		}		
	}
	else if(msg == "restart playloop")
	{
		LOGD("reveive restart playloop in playlogic.cc");
		current_material_index = 0; 
		is_video_playing = false; 
		is_video_stop_status = false;
		pic_holding_on = false;
		pic_stop = true; // out of pic loop 
		pic_forward = false;
		pic_backward = false;
		video_running = false;
		clock_show = false;
		show_current_status("重启中..", 5000);
		start_loop_thread();				
	}
	else if(msg.substr(0,15) == "aa bb rtc time:")
	{
		 int pos =  msg.find(":");
         std::string rtc_time = msg.substr(pos+1);
         TimeHelper::setDateTime(rtc_time.c_str());/* date str format should be : 2017-09-13 16:00:00 */
		 show_current_status("已更新系统时间", 5000);
		 update_time();
	}
	else if(msg.substr(0,14) == "show wifi msg:")
	{
		int index = msg.find(":");
		if(index != -1)
		{
			std::string m_msg = msg.substr(index+1);
			show_current_status(m_msg, 5000);
		}
		
	}
	else if(msg.substr(0,18) =="change to link now")
	{
		show_current_status("wifi将切换至连接模式", 5000);
	}
	else if(msg.substr(0,16) =="change to AP now")
	{
		show_current_status("wifi将切换至热点模式", 5000);
	}
	
}

