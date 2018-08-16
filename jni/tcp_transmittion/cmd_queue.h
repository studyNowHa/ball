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
#include "utils/Log.h"


typedef struct
{
    long cmd_arrived_time;
    unsigned char cmd_data[5];
}cmd_type;


class serial_cmd_queue
{
private:
    Mutex mut;
    std::queue<cmd_type> cmd_queue;
    std::queue<cmd_type> empty_queue;
public:
    serial_cmd_queue();
    ~serial_cmd_queue();
    void push(const cmd_type &new_value);
    bool try_pop(cmd_type& value);
    bool empty();
    int  size();
    void clear();
};



