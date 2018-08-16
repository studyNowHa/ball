#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <dirent.h>
#include <sys/epoll.h>
#include <vector>
#include <sys/vfs.h>
#include "storage/StoragePreferences.h"
#include "system/Thread.h"
#include "utils/TimeHelper.h"
#include "utils/Log.h"
#include "data.h"
#include "ISocketListener.h"
#include "net/NetManager.h"
#include "storage/StoragePreferences.h"

#define    MAXLINE        1024

static FILE *file_send;
static FILE *file_rec;
static std::string file_rec_path;
static std::string fileName_rec;
static int fileSize_rec;
static int recvSize;

static int fileNumber=0;
static int fileNumber2=0;

static int chooseDirFlag=0;
static bool isPacketHead;
static 	pthread_t thread_heartID = 0;
static 	pthread_t thread_wifiID = 0;

volatile bool tcp_running = true;
volatile bool tcp_heart_running  = true;
volatile bool data_trans_running = true;
volatile bool tcpHeartBeat_exit_flag = false;
volatile bool tcpStart_exit_flag = false;
extern volatile bool wifi_link_ok;
extern std::string global_ip_address;
extern std::vector<std::string> material_string;

ServerSocket new_sock;

// listener
extern iiSocketListener socket_listener;

extern flash_wifi_init();

void Shutdown(int tmp_fd, int mode)
{
		switch(mode) {
			case 0:
				shutdown(tmp_fd, SHUT_RD);			
				break;
			case 1:
				shutdown(tmp_fd, SHUT_WR);			
				break;
			case 2:
				shutdown(tmp_fd, SHUT_RDWR);			
				break;
			default:
				shutdown(tmp_fd, SHUT_RDWR);		
				break;
		}		
}



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

int get_sd_free_capicity()
{
	struct statfs diskInfo;
	statfs("/mnt/extsd", &diskInfo);
	unsigned long long totalBlocks = diskInfo.f_bsize;
	unsigned long long totalSize = totalBlocks * diskInfo.f_blocks;
	size_t mbTotalsize = totalSize>>20;
	unsigned long long freeDisk = diskInfo.f_bfree*totalBlocks;
	int mbFreedisk = freeDisk>>10;
	return mbFreedisk;
}


void tcpHeartBeat()
{
    LOGD("tcp heart beat running now....\r\n");
    bool bconnect_with_PC = true; 

    ServerSocket heartbeart_server (4321);
    ServerSocket heartbeart_sock;
    heartbeart_server.accept(heartbeart_sock);
    LOGD("accept heart beat socket now....\r\n");
    std::string heart_beat_string = "heart beating!";
    std::string rcv_beat_string;
    int len = 0;
    int rcv_counter = 0;
    char BUF[MAXLINE];
    tcp_heart_running = true;
    while(tcp_heart_running)
    {
        rcv_counter = 0;
        len = heartbeart_sock.sendData(heart_beat_string.c_str(), heart_beat_string.length());
        if(len <=0)
        {
            LOGD("heart beating send error... ");
            bconnect_with_PC = false;
            break;
        }
        LOGD("sending beat ...");
        
        // recv PC response
        heartbeart_sock.set_non_blocking(true);
        while(true)
        {
            rcv_counter++;
            len = heartbeart_sock.recvBuf(BUF); //block here fro hertbeat response
            if(len > 0)
            {
                rcv_beat_string = BUF;
                if(rcv_beat_string.substr(0,11) == "rcv beating")
                {
                     LOGD("rcv beating ... ");
                     break;  
                }
            }
            else if(len < 0)
            {
            	LOGD("*** heartbeart_sock rcv error ! ");
                bconnect_with_PC = false;
            	break;
            }
            if(rcv_counter > 4)
            {
                bconnect_with_PC = false;
                break;
            }
            sleep(1);
        }

        if(bconnect_with_PC == false)
        {
            LOGD("lose contact with PC ...");
            new_sock.set_non_blocking(true);
            data_trans_running = false; 
            break;
        }
        sleep(2);
    }
    Shutdown(heartbeart_sock.get_sock_fd(), 2);
    heartbeart_sock.closeSocket();
    Shutdown(heartbeart_server.get_sock_fd(), 2);
    heartbeart_server.closeSocket();
    LOGD("tcp heart beat thread exit now...");
    tcpHeartBeat_exit_flag = true;
}

void start_heartbeat_thread()
{
     // start tcp heart beat server first
    pthread_attr_t attr;
	pthread_attr_init(&attr);
	int ret = pthread_create(&thread_heartID, &attr, tcpHeartBeat, NULL);
	pthread_attr_destroy(&attr);
    pthread_detach(thread_heartID);
}


void split_string(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
 
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}


void compare_process(std::vector<std::string>& material_string, std::vector<std::string>& tmp_list_tmp,
                                     std::vector<std::string>& tmp_list_final)
{
    for(int i=0; i<tmp_list_tmp.size(); i++)
    {
        char flag = 0;
        for(int j=0; j<material_string.size(); j++)
        {
            if(material_string.at(j) == tmp_list_tmp.at(i))
            {
                flag = 1;
            }
        }
        if(flag == 0)
        {
            tmp_list_final.push_back(tmp_list_tmp.at(i));
        }
    }
    for(int i=0; i<material_string.size(); i++)
    {
        char flag = 0;
        for(int j=0; j<tmp_list_tmp.size(); j++)
        {
            if(material_string.at(i) == tmp_list_tmp.at(j))
            {
                flag = 1;
            }
        }
        if(flag == 0)
        {
            std::string rm_file = std::string("/mnt/extsd/materials/") + material_string.at(i);
            LOGD("rm file : %s", rm_file.data());
            remove(rm_file.data());
            system("sync");
        }
    }
}


                

void tcpStart()
{
    std::vector<std::string> fileNames;
    std::vector<std::string> fileNames2;
    char BUF[MAXLINE];
    int receivedSize;
    int nfds = 0;
    data_trans_running = true;
    LOGD("tcp file data trans thread running now....");
    // Create the socket
    ServerSocket server ( 1234 );
    isPacketHead = true;
    std::string data;
    server.accept ( new_sock );
    LOGD("accept file data trans socket now....fd:%d\r\n", new_sock.get_sock_fd());
    new_sock.set_non_blocking(false);

    int keepAlive = 1; 	 // 开启keepalive属性. 缺省值: 0(关闭)
    int keepIdle = 2;	 // 如果在 秒内没有任何数据交互,则进行探测. 缺省值:7200(s)
    int keepInterval = 2;	// 探测时发探测包的时间间隔为 秒. 缺省值:75(s)
    int keepCount = 3;	 // 探测重试的次数. 全部超时则认定连接失效..缺省值:9(次)
    setsockopt(new_sock.get_sock_fd(), SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));
    setsockopt(new_sock.get_sock_fd(), SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));
    setsockopt(new_sock.get_sock_fd(), SOL_TCP, TCP_KEEPINTVL, (void*)&keepInterval, sizeof(keepInterval));
    setsockopt(new_sock.get_sock_fd(), SOL_TCP, TCP_KEEPCNT, (void*)&keepCount, sizeof(keepCount));

    while ( data_trans_running )
    {          
        receivedSize = new_sock.RecvBuf(BUF);
        if(receivedSize > 0)
        {
            data = BUF;
            //LOGD("tcp debug rcv info: %s", data.c_str());
            if(data.substr(0,18) == "download materials")
            {
                LOGD("rcv cmd in trans from PC : %s ", data.c_str());
                if(!Data::scanDirslot("materials"))
                {
                    std::string s("file not exists");
                    new_sock << s;
                }
                else
                {
                    std::string s2("clean dir");
                    new_sock << s2;
                }
                continue;
            }

            if(data.substr(0,13) == "download file")
            {
                LOGD("rcv cmd in trans from PC : %s ", data.c_str());
                if(!Data::scanDirslot("file"))
                {
                    std::string str("file not exists");
                    new_sock << str;
                }
                else
                {
                    std::string str2("clean file dir");
                    new_sock << str2;
                }
                continue;
            }
            if(data.substr(0,12) == "check names:")
            {
                //new_sock.set_non_blocking(false);
                file_rec_path = Data::currentFilePath("materials");
                LOGD("rcv cmd in trans from PC : %s ", data.c_str());
                std::vector<std::string> tmp_list_final;
                std::vector<std::string> tmp_list_tmp;
                int index = data.find(":");
                std::string string_names =  data.substr(index+1);
                split_string(string_names, tmp_list_tmp, "$");
                for(int i=0; i<tmp_list_tmp.size(); i++)
                {
                    LOGD("tmp_list: %s", tmp_list_tmp.at(i).data());
                }
                compare_process(material_string, tmp_list_tmp, tmp_list_final);
                std::string updated_names;
                for(int i=0; i<tmp_list_final.size(); i++)
                {
                        updated_names += tmp_list_final.at(i);
                        updated_names += "$";
                }
                LOGD("updated_names: %s", updated_names.data());
                std::string strr("updated names:");
                strr += updated_names;
                new_sock << strr;
                continue;
            }
            if(data.substr(0,14) == "clean file dir")
            {
                LOGD("rcv cmd in trans from PC : %s ", data.c_str());
                file_rec_path = Data::currentFilePath("file");
                std::string strr2("receive clean file dir");
                new_sock << strr2;
                continue;
            }
            if(data.substr(0,17) == "receive clean dir")
            {
                LOGD("rcv cmd in trans from PC : %s ", data.c_str());
                chooseDirFlag=1;
                fileNames = Data::getFileNames(Data::currentFilePath("materials"));
                if(!fileNames.size())
                {
                    std::string strr3("no download file");
                    new_sock << strr3;
                }
                else
                {
                    fileNumber  = fileNames.size();
                    std::string ssss;
                    switch(Data::getIntLength(fileNumber))
                    {
                            case 1:
                                ssss = "downloadFileNumber000" + Data::intToString(fileNumber);
                            break;
                            case 2:
                                ssss = "downloadFileNumber00" + Data::intToString(fileNumber);
                            break;
                            case 3:
                                ssss = "downloadFileNumber0" + Data::intToString(fileNumber);
                            break;
                            default:
                            break;
                    }
                    new_sock << ssss;
                    std::string filePath = Data::currentFilePath("materials");
                    filePath = filePath.append("/");
                    filePath = filePath.append(fileNames.at(0));
                    LOGD("send file path:  %s", filePath.c_str());
                    new_sock.sendFile(filePath, fileNames.at(0)/*,file_send*/);
                    if ((file_send = fopen(filePath.c_str(), "rb")) == NULL)
                    {
                        LOGD("*** Open file failed!");                                    
                    }
                    else
                    {
                        LOGD("Open file success!");
                    }
                }
                continue;
            }
            if(data.substr(0,22) == "receive clean file dir")
            {
                LOGD("rcv cmd in trans from PC : %s ", data.c_str());
                chooseDirFlag=2;
                //send file
                fileNames2 = Data::getFileNames(Data::currentFilePath("file"));
                if(!fileNames2.size())
                {
                    std::string strr3("no download file");
                    new_sock << strr3;
                }
                else
                {
                    fileNumber2  = fileNames2.size();
                    std::string filePath2_f= Data::currentFilePath("file");
                    filePath2_f = filePath2_f.append("/");
                    filePath2_f = filePath2_f.append(fileNames2.at(0));

                    new_sock.sendFile(filePath2_f, fileNames2.at(0)/*,file_send*/);
                    if ((file_send = fopen(filePath2_f.c_str(), "rb")) == NULL)
                    {
                        LOGD("Open file failed!");
                        break;                                    
                    }
                    else
                    {
                        LOGD("Open file success!");
                    }
                }
                continue;
            }
            if(data.substr(0,5) == "aa bb") // command parse from pc
            {
                LOGD("receive command from pc : %s ", data.c_str());
                if(data.substr(0,18) == "aa bb power on_off")
                {
                    socket_listener.notify("power on_off");
                }
                if(data.substr(0,10) == "aa bb play")
                {
                    socket_listener.notify("play");
                }
                else if(data.substr(0,11) == "aa bb pause")
                {
                    socket_listener.notify("pause");
                }
                else if(data.substr(0,10) == "aa bb stop")
                {
                    socket_listener.notify("stop");
                }
                else if(data.substr(0,19) == "aa bb previous play")
                {
                    socket_listener.notify("previous play");
                }
                else if(data.substr(0,15) == "aa bb next play")
                {
                    socket_listener.notify("next play");
                }
                else if(data.substr(0,13) == "aa bb forward")
                {
                    socket_listener.notify("forward");
                }
                else if(data.substr(0,14) == "aa bb backward")
                {
                    socket_listener.notify("backward");
                }
                else if(data.substr(0,13) == "aa bb lumi up")
                {
                    socket_listener.notify("lumi up");
                }
                else if(data.substr(0,15) == "aa bb lumi down")
                {
                    socket_listener.notify("lumi down");
                }
                else if(data.substr(0,14) == "aa bb sound up")
                {
                    socket_listener.notify("sound up");
                }
                else if(data.substr(0,16) == "aa bb sound down")
                {
                    socket_listener.notify("sound down");
                }
                else if(data.substr(0,18) == "aa bb clock on_off")
                {
                    socket_listener.notify("clock on_off");
                }
                else if(data.substr(0,17) == "aa bb mode change")
                {
                    socket_listener.notify("mode change");
                }
                else if(data.substr(0,15) == "aa bb sys reset")
                {
                    socket_listener.notify("sys reset");
                }
                else if(data.substr(0,22) == "aa bb restart playloop")
                {
                    socket_listener.notify("restart playloop");
                }
                else if(data.substr(0,25) == "aa bb upload stop playing") //before upload materials from pc
                {
                    socket_listener.notify("upload stop playing");
                }
                else if(data.substr(0,15) == "aa bb rtc time:")
                {
                    socket_listener.notify(data);
                }
                else if(data.substr(0,22) == "aa bb cancel uploading")
                {
                   LOGD("cancel uploading now");
		           fclose(file_rec);
                   isPacketHead=true;
                   system("rm -rf /mnt/extsd/materials");
                }
                else if(data.substr(0,20) == "aa bb total file kb:")
				{
                	int pos = data.find(":");
                	std::string total_kb = data.substr(pos+1);
                	int total_size_rcv = atoi(total_kb.c_str());
                	int current_free_size = get_sd_free_capicity();
                	LOGD("total_size rcv : %s KB", total_kb.c_str());
                	LOGD("current SD memory : %d KB", current_free_size);
                	if(total_size_rcv > current_free_size) // SD card cap not enough
                	{
                		LOGD("**** SD card capacity not enough !!");
                		std::string str("SD capacity not enough");
                		new_sock << str;
                	}
                    else
                    {
                        LOGD("SD card capacity enough");
                		std::string str("SD capicity ok");
                		new_sock << str;
                    }
				}
                else if(data.substr(0,28) == "aa bb wifi mode change to AP")
                {
                    LOGD("wifi mode will change to AP!!!");
                    std::string tmp_str;
                    if(StoragePreferences::getString("wifi_mode_flash",tmp_str)=="LINK")
                    {
                         NETMANAGER->getWifiManager()->disconnect();                       
                    }
                    put_string_wrapper("wifi_mode_flash", "AP");
                    NETMANAGER->getSoftApManager()->setEnable(false);
			        NETMANAGER->getWifiManager()->enableWifi(false);
                    //then restart machine
                    //socket_listener.notify("re start system");
                    pthread_attr_t attr;
                    pthread_attr_init(&attr);
                    int ret = pthread_create(&thread_wifiID, &attr, flash_wifi_init, NULL);
                    pthread_attr_destroy(&attr);
                    if (ret||!thread_wifiID)
                    {
                        return;
                    }
                    pthread_detach(thread_wifiID);        
                }
                else if(data.substr(0,30) == "aa bb wifi mode change to LINK")
                {
                    LOGD("wifi mode will change to LINK!!!");
                    std::string tmp_str;
                    put_string_wrapper("wifi_mode_flash", "LINK");
                    NETMANAGER->getSoftApManager()->setEnable(false);
			        NETMANAGER->getWifiManager()->enableWifi(false);
                    //then restart machine
                    //socket_listener.notify("re start system");     
                    pthread_attr_t attr;
                    pthread_attr_init(&attr);
                    int ret = pthread_create(&thread_wifiID, &attr, flash_wifi_init, NULL);
                    pthread_attr_destroy(&attr);
                    if (ret||!thread_wifiID)
                    {
                        return;
                    }
                    pthread_detach(thread_wifiID);                     
                }
                continue;
            }
            if(data.substr(0,13) == "FileHead recv")
            {
                LOGD("FileHead receive\n");
                //new_sock.set_non_blocking(false);
                int len=0;
                char buf[4096];
                bzero(buf, 4096);
                while ((len = fread(buf, sizeof(char), 4096, file_send)) >0 )
                {
                    len = new_sock.sendData(buf, len);
                    bzero(buf, 4096);
                }
                //new_sock.set_non_blocking(true);
                continue;
            }
             if(data.substr(0,15) == "file send error")
            {
                LOGD("file send error receive\n");
                fclose(file_rec);
                isPacketHead=true;
                system("rm -rf /mnt/extsd/materials");
			    system("rm -rf /mnt/extsd/file");
			    system("sync");
                continue;
            }
            if(data.substr(0,15) == "file write done")
            {
                LOGD("file write done\r\n");
                system("sync");
                fclose(file_send);
                if(chooseDirFlag==1)
                {
                    fileNumber = fileNumber-1;
                    if(fileNumber)
                    {
                        std::string filePath2 = Data::currentFilePath("materials");
                        filePath2 = filePath2.append("/");
                        filePath2 = filePath2.append(fileNames.at(fileNames.size()-fileNumber));
                        new_sock.sendFile(filePath2, fileNames.at(fileNames.size()-fileNumber)/*,file_send*/);
                        if ((file_send = fopen(filePath2.c_str(), "rb")) == NULL)
                        {
                            LOGD("*** Open materials file failed!\r\n");
                            break;
                        }
                    }
                    else 
                    {
                        std::string str3("trans mf");
                        new_sock << str3;
                    }

                }
                else if(chooseDirFlag==2)
                {
                    fileNumber2 = fileNumber2-1;
                    if(fileNumber2)
                    {
                        std::string filePath2_f = Data::currentFilePath("file");
                        filePath2_f = filePath2_f.append("/");
                        filePath2_f = filePath2_f.append(fileNames2.at(fileNames2.size()-fileNumber2));
                        new_sock.sendFile(filePath2_f, fileNames2.at(fileNames2.size()-fileNumber2)/*,file_send*/);
                        if ((file_send = fopen(filePath2_f.c_str(), "rb")) == NULL)
                        {
                            LOGD("*** Open file failed!");
                            break;
                        }
                    }
                    else 
                    {
                        std::string str4("trans f");
                        new_sock << str4;
                    }
                }else
                {
                }
                continue;
            }
           
            if(isPacketHead)
            {
                isPacketHead=false;

                recvSize=0;
                //分割
                std::vector<std::string> vec;
                Data::splitStr(data, vec, "##");
                fileName_rec = vec[0];
                std::string sss = vec[1];
                fileSize_rec = atoi(sss.c_str());

                std::string tmpPath;
                if(fileName_rec == "update.img")
                {
                    file_rec_path = Data::currentFilePath("");
                    tmpPath = file_rec_path  +  fileName_rec;
                }
                else
                {
                    tmpPath = file_rec_path  + "/" +  fileName_rec;
                }

                LOGD("file_rec_path: %s", tmpPath.c_str());
                LOGD("file_rec_size: %d bytes", fileSize_rec);
                if ((file_rec = fopen(tmpPath.c_str(), "wb+")) == NULL)
                {
                    LOGD("*** Open file failed\n");
                    break;
                }

                std::string str5("FileHead recv");
                new_sock << str5;

            }
            else
            {
                int len = fwrite(BUF, sizeof(char), receivedSize, file_rec);
                recvSize += len;
                if(recvSize==fileSize_rec)  //receive complete
                {
                    fclose(file_rec);
                    isPacketHead=true;
                    LOGD("file rec done ");
                    std::string str6("file write done");
                    new_sock << str6;
                    //new_sock.set_non_blocking(true);
                }
            }
        }
        else if(receivedSize < 0)
        {
            LOGD("****new socket rcv error");
            break;
        }
        else if(receivedSize == 0)
        {
            Thread::sleep(200);
        }   
    }
    Shutdown(new_sock.get_sock_fd(), 2);
    new_sock.closeSocket();
    Shutdown(server.get_sock_fd(), 2);
    server.closeSocket();
    LOGD("tcp file data trans thread exit now....");    
    tcpStart_exit_flag = true;
}

int udpStart()
{
    setvbuf(stdout, NULL, _IONBF, 0);   
    fflush(stdout);   
  
    // 绑定地址
    struct sockaddr_in addrto;  
    bzero(&addrto, sizeof(struct sockaddr_in));  
    addrto.sin_family = AF_INET;  
    addrto.sin_addr.s_addr = htonl(INADDR_ANY);  
    addrto.sin_port = htons(6000);  
      
    // 广播地址
    struct sockaddr_in from;  
    bzero(&from, sizeof(struct sockaddr_in));  
    from.sin_family = AF_INET;  
    from.sin_addr.s_addr = htonl(INADDR_ANY);  
    from.sin_port = htons(6000);  
      
    int sock = -1;  
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
    {     
        LOGD("socket error");
        return false;  
    }     
  
    const int opt = 1;  
    //设置该套接字为广播类型，
    int nb = 0;  
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
    if(nb == -1)  
    {  
    	LOGD("set socket error...");
        return false;  
    }  
    int on = 1;
    if ( setsockopt ( sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;
  
    if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1)   
    {     
    	LOGD("bind error...");
        return false;  
    }  
    
    int opts = fcntl ( sock, F_GETFL );
    opts = ( opts | O_NONBLOCK );
    fcntl ( sock, F_SETFL,opts );
    
    int len = sizeof(sockaddr_in);  
    char rmsg[100] = {0};  
    int nlen=sizeof(addrto);	
  
    while(1)  
    {  
        //从广播地址接受消息
    	LOGD("wating for receive udp message\n");
        int ret=recvfrom(sock, rmsg, 100, 0, (struct sockaddr*)&from, (socklen_t*)&len);  
        if(ret<=0)  
        {  
        	LOGD("read error(maybe)....");
            LOGD("error msg: %s", strerror(errno));
        }  
        else  
        {         
            printf("%s\t", rmsg);   
	        if(strcmp(rmsg,"request")==0) 
            {
		        std::string msg="!!!" + global_ip_address;
		        int ret=sendto(sock, msg.c_str(), strlen(msg.c_str()), 0, (sockaddr*)&from, nlen);
                if(ret<0)
                {
                	LOGD("send error....");
                }
                else
                {		
                    printf("return ok ");	
                }
	        }
            else if(strcmp(rmsg,"ack")==0)
            {
                printf("receive ack message from pc!");
                Shutdown(sock, 2);
                close(sock);
                break;
	        }
            for(int i=0;i<100;i++)
            {
                rmsg[i]=0;
            } 
        }  
        Thread::sleep(500);
    }  
    LOGD("jump out udp");
}


void* tcp_trans_loop(void* arg)
{
    // init work
    fileNumber=0;
    fileNumber2=0;
    chooseDirFlag=0;
    isPacketHead=true;
    while(wifi_link_ok == false) // wait wifo init success
    {
        sleep(1);
    }
    LOGD(":::::::::::::");
    LOGD(":::::::::::::");
    LOGD(":::::::::::::");
    LOGD(":::::::::::::");
    LOGD("tcp loop thread start now ...");
    LOGD(":::::::::::::");
    LOGD(":::::::::::::");
    LOGD(":::::::::::::");
    LOGD(":::::::::::::");
    tcp_running = true;
    data_trans_running = true;
    while(tcp_running)
    {     
   	    udpStart();
        start_heartbeat_thread(); 
   	    tcpStart();
   	    while(tcpStart_exit_flag==false || tcpHeartBeat_exit_flag==false) // when out of tcpStart make this two threads all exit
		{
			Thread::sleep(100);
		}
		tcpStart_exit_flag = false;
		tcpHeartBeat_exit_flag = false;
        Thread::sleep(1000);
    }
    LOGD("tcp loop end now ...");
    
}
