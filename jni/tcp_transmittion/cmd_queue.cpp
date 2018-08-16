#include <pthread.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <system/Mutex.h>
#include "utils/TimeHelper.h"
#include <sys/reboot.h>
#include <utility>
#include "utils/BrightnessHelper.h"
#include "utils/Log.h"
#include "time.h"
#include "cmd_queue.h"

void serial_cmd_queue::push(const cmd_type &new_value)
{
	mut.lock();
    cmd_queue.push(new_value);
    mut.unlock();
}

serial_cmd_queue::serial_cmd_queue()
{

}

serial_cmd_queue::~serial_cmd_queue()
{

}

bool serial_cmd_queue::try_pop(cmd_type& value)
{
	mut.lock();
    if(cmd_queue.empty())
    {
    	mut.unlock();
    	return false;
    }
    value = cmd_queue.front();
    cmd_queue.pop();
    mut.unlock();
    return true;
}


void serial_cmd_queue::clear()
{
	mut.lock();
	cmd_queue = empty_queue;
	mut.unlock();
}

bool serial_cmd_queue::empty()
{
	mut.lock();
	bool ret = cmd_queue.empty();
	mut.unlock();
    return ret;
}

int serial_cmd_queue::size()
{
	mut.lock();
	int ret = cmd_queue.size();
	mut.unlock();
    return ret;
}


