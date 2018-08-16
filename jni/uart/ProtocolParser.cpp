/*
 * ProtocolParser.cpp
 *
 *  Created on: Sep 7, 2017
 *      Author: guoxs
 */
#include <vector>
#include "CommDef.h"
#include "uart/ProtocolParser.h"
#include "utils/Log.h"
#include "time.h"
#include "system/Thread.h"
#include "tcp_transmittion/cmd_queue.h"

#define CMD_HEADER_POS 0X00
#define CMD_LEN_POS 0X01
#define CMD_INDEX_POS 0X02
#define CMD_PARA_START_POS  0X03
#define CMD_HEADER	0xAA
#define CMD_END  	0xBB

extern serial_cmd_queue  cmd_que;

static std::vector<OnProtocolDataUpdateFun> sProtocolDataUpdateListenerList;

extern bool flag_sequent;

void registerProtocolDataUpdateListener(OnProtocolDataUpdateFun pListener) {
	LOGD("registerProtocolDataUpdateListener\n");
	if (pListener != NULL) {
		sProtocolDataUpdateListenerList.push_back(pListener);
	}
}

void unregisterProtocolDataUpdateListener(OnProtocolDataUpdateFun pListener) {
	LOGD("unregisterProtocolDataUpdateListener\n");
	if (pListener != NULL) {
		std::vector<OnProtocolDataUpdateFun>::iterator iter = sProtocolDataUpdateListenerList.begin();
		for (; iter != sProtocolDataUpdateListenerList.end(); iter++) {
			if ((*iter) == pListener) {
				sProtocolDataUpdateListenerList.erase(iter);
				return;
			}
		}
	}
}


static void notifyProtocolDataUpdate(const SProtocolData &data) {
	std::vector<OnProtocolDataUpdateFun>::const_iterator iter = sProtocolDataUpdateListenerList.begin();
	for (; iter != sProtocolDataUpdateListenerList.end(); iter++) {
		(*iter)(data);
	}
}

static SProtocolData sProtocolData = { 0 };

SProtocolData& getProtocolData() {
	return sProtocolData;
}


/**
 * 获取校验码
 */
BYTE getCheckSum(const BYTE *pData, int len) {
	int sum = 0;
	for (int i = 0; i < len; ++i) {
		sum += pData[i];
	}

	return (BYTE) (~sum + 1);
}



/**
 * 解析每一帧数据
 */
static void procParse(const BYTE *pData, UINT len) {
	switch (MAKEWORD(pData[3], pData[2])) {
	case CMDID_POWER:
		sProtocolData.cmd = pData[5];
		break;
	}

	// 通知协议数据更新
	notifyProtocolDataUpdate(sProtocolData);
}


/**
 * 功能：解析协议
 * 参数：pData 协议数据，len 数据长度
 * 返回值：实际解析协议的长度
 */
int parseProtocol(const BYTE *pData, UINT len) {
	BYTE data_len;
	BYTE index;
	BYTE para_len;
	BYTE check_rcv;
	BYTE check_cal;
	LOGD("<< uart recv >> : ");
	LOGD("len = %d", len);
	for(int i=0; i<len; i++)
	{
		LOGD(" - %x - ", pData[i]);
	}
	BYTE header = pData[CMD_HEADER_POS];
	BYTE end = pData[len-1];
	if(header!=0xAA || end!=0xBB)
	{
		LOGD("*** <uart> cmd header or end error");
		goto error_out;
	}
	data_len = pData[CMD_LEN_POS];
	index = pData[CMD_INDEX_POS];
    para_len = data_len - 2;
	check_rcv = pData[len-2];
	check_cal = data_len^index;
	for(int i=0; i<para_len; i++)
	{
		check_cal = check_cal^pData[CMD_PARA_START_POS+i];
	}
	if(check_cal != check_rcv)
	{
		LOGD("*** <uart> cmd check error");
		goto error_out;
	}
	sProtocolData.cmd = index;
	LOGD("UartParser recev cmd<%x> ", index);
	notifyProtocolDataUpdate(sProtocolData);
	error_out:
	return;
}

void cmd_serial_parser()
{
	LOGD("@@@@@@@@@@");
	LOGD("@@@@@@@@@@");
	LOGD("@@@@@@@@@@");
	LOGD("cmd_serial_parser thread running now ...");
	LOGD("@@@@@@@@@@");
	LOGD("@@@@@@@@@@");
	LOGD("@@@@@@@@@@");

	cmd_type  cmd;
	long last_cmd_time  = 0;
	while(true)
	{
		if(cmd_que.try_pop(cmd))
		{
			LOGD("cur_cmd arrived time : %d ms", cmd.cmd_arrived_time);
			if(cmd.cmd_arrived_time - last_cmd_time < 400)
			{
				last_cmd_time = cmd.cmd_arrived_time;
				LOGD("@@@@@@@@@@");
				LOGD("cmd sent too frequent!!!");
				LOGD("@@@@@@@@@@");
				continue;
			}
			last_cmd_time = cmd.cmd_arrived_time;
			parseProtocol(cmd.cmd_data, 5);
		}

		Thread::sleep(100);
	}
}
