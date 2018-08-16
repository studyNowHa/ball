/*
 * UartContext.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: guoxs
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <termio.h>
#include <sys/ioctl.h>

#include "uart/UartContext.h"
#include "utils/Log.h"
#include "tcp_transmittion/cmd_queue.h"
#include "time.h"


#define UART_DATA_BUF_LEN			16384	// 16KB

serial_cmd_queue  cmd_que;
static pthread_t cmd_serial_threadID;
extern void cmd_serial_parser();

extern int parseProtocol(const BYTE *pData, UINT len);
bool flag_sequent = false;

UartContext::UartContext() :
	mIsOpen(false),
	mUartID(0),
	mDataBufPtr(NULL),
	mDataBufLen(0) {

}

UartContext::~UartContext() {
	delete[] mDataBufPtr;
	closeUart();
}

bool UartContext::openUart(const char *pFileName, UINT baudRate) {
	LOGD("openUart pFileName = %s, baudRate = %d\n", pFileName, baudRate);
	mUartID = open(pFileName, O_RDWR|O_NOCTTY);

	if (mUartID <= 0) {
		mIsOpen = false;
	} else {
		struct termios oldtio = { 0 };
		struct termios newtio = { 0 };
		tcgetattr(mUartID, &oldtio);

		newtio.c_cflag = baudRate|CS8|CLOCAL|CREAD;
		newtio.c_iflag = 0;	// IGNPAR | ICRNL
		newtio.c_oflag = 0;
		newtio.c_lflag = 0;	// ICANON
		newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
		newtio.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
		tcflush(mUartID, TCIOFLUSH);
		tcsetattr(mUartID, TCSANOW, &newtio);

		// 设置为非阻塞
		//fcntl(mUartID, F_SETFL, O_NONBLOCK);

		mIsOpen = run("uart");
		if (!mIsOpen) {
			close(mUartID);
			mUartID = 0;
		}

		LOGD("openUart mIsOpen = %d\n", mIsOpen);
	}

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int ret = pthread_create(&cmd_serial_threadID, &attr, cmd_serial_parser, NULL);
	pthread_attr_destroy(&attr);
	if (ret||!cmd_serial_threadID)
	{
		return;
	}
	pthread_detach(cmd_serial_threadID);

	return mIsOpen;
}

void UartContext::closeUart() {
	LOGD("closeUart mIsOpen: %d...\n", mIsOpen);
	if (mIsOpen) {
		requestExit();

		close(mUartID);
		mUartID = 0;
		mIsOpen = false;
	}
}

bool UartContext::send(const BYTE *pData, UINT len) {
	if (!mIsOpen) {
		return false;
	}

	if (write(mUartID, pData, len) != len) {	// fail
		LOGD("send Fail\n");
		return false;
	}

	// success
//	LOGD("send Success\n");

	return true;
}

UartContext* UartContext::getInstance() {
	static UartContext sUC;
	return &sUC;
}

bool UartContext::readyToRun() {
	if (mDataBufPtr == NULL) {
		mDataBufPtr = new BYTE[UART_DATA_BUF_LEN];
	}

	if (mDataBufPtr == NULL) {
		closeUart();
	}

	return (mDataBufPtr != NULL);
}

bool UartContext::threadLoop() {
	if (mIsOpen) {
		// 可能上一次解析后有残留数据，需要拼接起来
		int readNum = read(mUartID, mDataBufPtr, UART_DATA_BUF_LEN);

		if (readNum >= 5) {
			LOGD("%%% UART rcv stuff ....");
			struct timespec time_now = {0, 0};
			cmd_type cur_cmd;
			clock_gettime(CLOCK_REALTIME, &time_now);
			cur_cmd.cmd_arrived_time = time_now.tv_sec*1000+time_now.tv_nsec/1000000; // unit:ms
			memcpy(cur_cmd.cmd_data, mDataBufPtr, 5);
			if(cmd_que.size() == 10)
			{
				cmd_que.clear();
			}
			cmd_que.push(cur_cmd);
			// 解析协议
			//parseProtocol(mDataBufPtr, readNum);
		} else {
			Thread::sleep(50);
		}

		return true;
	}

	return false;
}
