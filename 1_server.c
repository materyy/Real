#include <stdio.h>

//#include <mysql/mysql.h>
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/epoll.h>


#define LOGIN                    1          //登录
#define REGISTER                 2          //注册
#define FRIEND_SEE               3          //查看好友列表
#define FRIEND_ADD               4          //添加好友
#define FRIEND_DEL               5          //删除好友
#define GROUP_SEE                6          //查看群组列表
#define GROUP_CREATE             7          //创建群
#define GROUP_JOIN               8          //加入群
#define GROUP_QIUT               9          //退出群
#define GROUP_DEL                10         //删除群
#define CHAT_ONE                 11         //私聊
#define CHAT_MANY                12         //群聊
#define FILE_SEND_BEGIN          13         //开始发送文件
#define FILE_SEND_BEGIN_RP       14
#define FILE_SEND_STOP_RP        15
#define FILE_RECV_RE             16
#define FILE_SEND                17
#define FILE_RECV_BEGIN          18 
#define FILE_RECV_BEGIN_RP       19
#define FILE_RECV_STOP_RP        20
#define FILE_RECV                21
#define FILE_FINI_RP             22
#define MES_RECORD               23         //消息记录
#define EXIT                     -1         //退出
 
 
 
 
#define FILE_STATU_RECV_ING       1     //正在接收文件
#define FILE_STATU_RECV_STOP      2     //停止接收文件
#define FILE_STATU_SEND_ING       3     //正在发送文件
#define FILE_STATU_SEND_FINI      4     //停止发送文件
 
 
#define SIZE_PASS_NAME  30          
#define MAX_CHAR        1024
#define EPOLL_MAX       200000
#define LISTENMAX       1000
#define USER_MAX        100    //the user on line
#define GROUP_MAX       50
#define FILE_MAX        50
#define NUM_MAX_DIGIT   10
 
#define OFFLINE    0        //离线状态
#define ONLINE     1        //在线状态
#define BUSY       2        //忙碌状态


//用户信息结构体------------------------------
typedef struct infor_user 
{
    char username[MAX_CHAR];
    unsigned int password[4];
    int  status;        //don't foget to change is to 0 when the server begin
    int  socket_id;
    char friends[USER_MAX][MAX_CHAR];//begin from 1
    int  friends_num;
    char group[GROUP_MAX][MAX_CHAR];  //begin from 1
    char group_num;
}USER_INFO; 
 
USER_INFO  m_user_info  [USER_MAX];  //用户列表？？
int        m_user_num;               //用户数目



//群聊信息结构体---------------------------
typedef struct infor_group
{
    char  group_name[MAX_CHAR];
    int   member_num;
    char  member_name[USER_MAX][MAX_CHAR];  //begin from 1
}GROUP_INFO;
 
GROUP_INFO   m_group_info  [GROUP_MAX]; //begin from 1群组列表？
int          m_group_num;               //群组个数


//文件信息结构体-------------------------------------
typedef struct file_infor
{
    char  file_name[MAX_CHAR];
    char  file_send_name[MAX_CHAR];
    char  file_recv_name[MAX_CHAR];
    int   file_size;
    int   file_size_now;
    int   flag ;
}FILE_INFO;

FILE_INFO   m_file_info  [FILE_MAX]; //begin from 1   //文件数组
int          m_file_num;                              //文件个数


//传送数据包结构体
typedef struct datas{
    char     send_name[MAX_CHAR];
    char     recv_name[MAX_CHAR];
    int      send_fd;
    int      recv_fd;
    time_t   time;
    char     mes[MAX_CHAR*2];
}DATA;
 
typedef struct package{
    int   type;
    DATA  data;
}PACK;


PACK m_pack_send   [MAX_CHAR*2];    // send
int  m_send_num;


//*****************************************************************
// 主线程结构体？？？
typedef struct pthread_parameter
{
    int a;
    char str1[SIZE_PASS_NAME];
    char str2[SIZE_PASS_NAME];
}PTHREAD_PAR;







//**************************
//全局变量:

int listenfd, epollfd; 

char * IP = "127.0.0.1";
short PORT = 6789;


int log_file_fd;
int user_infor_fd;
int group_infor_fd;

pthread_mutex_t mutex;
pthread_mutex_t mutex_recv_file;
pthread_mutex_t mutex_check_file;

/*
MYSQL           mysql;
MYSQL_RES       *res = NULL;
MYSQL_ROW       row;
char            query_str[MAX_CHAR*4];
int             rc, fields;
int             rows;
*/


//**************************


void my_err(const char * string, int line)
{
    fprintf(stderr, "line:%d ",line);
    perror(string);
    exit(1);
}


void signal_close(int i)
{
    puts("signal_close");
//在关闭服务器之前 关闭服务器的socket

    //#ifdef LOG
    //      close(log_file_fd);
    //#else
    //#endif

    write_infor();
//    mysql_free_result(res);
//    mysql_close(&mysql);
    printf("server is closed\n");
    exit(1);
}


void send_pack(PACK *pack_send_pack_t)
{
    puts("send_pack");
    pthread_mutex_lock(&mutex);
    memcpy(&(m_pack_send[m_send_num++]), pack_send_pack_t, sizeof(PACK));
    pthread_mutex_unlock(&mutex);
}

int read_infor()
{
    puts("read_infor");
    USER_INFO read_file_t;

    //user_infor_fd 是全局变量
    if((user_infor_fd = open("user_infor.txt", O_RDONLY | O_CREAT , S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open", __LINE__);
        return -1;
    }
    int length;
    while((length = read(user_infor_fd, &read_file_t, sizeof(USER_INFO))) > 0)
    {
        read_file_t.status = 0;
        m_user_info[++m_user_num] = read_file_t;
    }
    close(user_infor_fd);
    printf("have read the user_infor.txt\n");




    GROUP_INFO read_file_group;

    //group_infor_fd 是全局变量
    if((group_infor_fd = open("group_infor.txt",O_RDONLY | O_CREAT , S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open", __LINE__);
        return -1;
    }
    //前面已经声明过length变量
    while((length = read(group_infor_fd, &read_file_group, sizeof(GROUP_INFO))) > 0)
    {
        m_group_info[++m_group_num] = read_file_group;
    }
    close(group_infor_fd);
    printf("have read the group_infor.txt\n");
}


int write_infor()
{
    printf("write_infor\n");
}


void print_infor_group()
{
    printf("print infor group\n");

}


void print_infor_user()
{
    printf("print infor user\n");
}

void print_infor_file()
{
    printf("print_infor_file\n");
}


// *void set_user_online(char username_t[])
// {
//
//
//
// }

/*
int connect_mysql_init()
{
    if(NULL == mysql_init(&mysql))
    {
        printf("mysql_init(): %s\n", mysql_error(&mysql));
        return -1;
    }

    if(NULL == mysql_real_connect(&mysql,
                                  "localhost",
                                  "root",
                              //  "__my  password__",
                                  "here_we_are",
                                  0,
                                  NULL,
                                  0))
    {
        printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
        return -1;
    }

    printf("connect mysql successful!!\n");

    mysql_free_result(res);
}
*/




int find_userinfor(char username_t[])
{
    printf("find_userinfor\n");
    int i;
    if(m_user_num == 0)
        return 0;
    for(i=1;i<m_user_num;i++)
    {
        if(strcmp(m_user_info[i].username,username_t) == 0)
            return i;
    }
    if(i == m_user_num+1)
        return 0;
}



void login(PACK *recv_pack)
{
    printf("login\n");
    /*
    int id = 0;
    char login_flag[10];

    if((id = find_userinfor(recv_pack->data.send_name)) == 0)
    {
        login_flag[0] = '2';
    }
    else if(m_user_info[id].status == 1)
    {
        login_flag[0] = '3';
    }
    else if(   )
    {

    }
    else login_flag[0] ='0';

    login_flag[1]='\0';


    .......
*/

}




//**************************
int judge_username_same(char username_t[])
{
    printf("judge_username_same\n");
    int i;
    if(m_user_num == 0)
        return 1;
    for(i=1;i<=m_user_num;i++)
    {
        if(strcmp(m_user_info[i].username, username_t) == 0)
            return 0;
    }
    if(i == m_user_num+1)
        return 1;
}



void registe(PACK *recv_pack)
{
    printf("rdgiste\n");
    char registe_flag[10];


    if(judge_username_same(recv_pack->data.send_name))
    {
        registe_flag[0] = '1';
        //there are not same user names
        //so, we need add user infor
        m_user_num++;
        strcpy(m_user_info[m_user_num].username, recv_pack->data.send_name);
        strcpy(m_user_info[m_user_num].password, recv_pack->data.mes);
        printf("\n\t\t registe success \n");
        m_user_info[m_user_num].status = 0;
    }
    else
        registe_flag[0] = '0';
    registe_flag[1]= '\0';

    strcpy(recv_pack->data.recv_name, recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name, "server");
    strcpy(recv_pack->data.mes, registe_flag);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;
    send_pack(recv_pack);
    free(recv_pack);

}


void send_status(PACK *recv_pack)
{
    printf("send_status\n");

}

void friend_add(PACK *recv_pack)
{
/*
 *recv_pack->data.mes 中存放信息
 也就是 要添加的好友信息

 但要添加好友，必须 互相在对方的好友列表都添加对方 才算添加成功！！
*/
    printf("friend_add\n");
    int id_own, id_friend;
    id_own = find_userinfor(recv_pack->data.send_name);

    strcpy(m_user_info[id_own].friends[++(m_user_info[id_own].friends_num)], recv_pack->data.mes);

    id_friend = find_userinfor(recv_pack->data.mes);
    strcpy(m_user_info[id_friend].friends[++(m_user_info[id_friend].friends_num)],recv_pack->data.send_name);
    
    //在这里可以打印一下  新添加的这位好友 看看是否添加成功!!

    free(recv_pack);
}



void del_friend_infor(int id, char friend_name[])
{
    printf("del_friend_infor\n");
    //仅仅删除好友信息

    int id_l, i;

/*  for  循环查找这位好友        找到之后 
        id_l = i;
        break;
*/

//从这个要删除的好友开始，好友列表之后的每一位都向前挪一位
/*
    for(i = id_l; i<m_user_info[id].friends_num; i++)
    {
        strcpy(m_user_info[id].friends[i],m_user_info[id].friends[i+1]);
    }
    m_user_info[id].friends_num--;
*/
}


void friend_del(PACK *recv_pack)
{
    printf("friend_del\n");
    //删除好友也是相互的！！！！！
    int id_own,id_own_l;
    int id_friend,id_friend_l;
    id_own = find_userinfor(recv_pack->data.send_name);

    del_friend_infor(id_own, recv_pack->data.mes);

    id_friend = find_userinfor(recv_pack->data.mes);

    del_friend_infor(id_friend,recv_pack->data.send_name);

    free(recv_pack);
}

void send_mes_to_one(PACK *recv_pack)
{
    printf("send_mes_to_one\n");
}




void send_pack_memcpy_server(int type, char *send_name, char *recv_name, int sockfdl, char *mes)
{
    puts("send_pack_memcpy_server");
    PACK pack_send_pack;
    time_t times;
    pack_send_pack.type = type;

    strcpy(pack_send_pack.data.send_name,send_name);
    strcpy(pack_send_pack.data.recv_name,recv_name);
    memcpy(pack_send_pack.data.mes, mes, MAX_CHAR*2);
    time(&times);
    pack_send_pack.data.time = times;
    printf("sockfdl:%d\n",sockfdl);
    if(send(sockfdl, &pack_send_pack, sizeof(PACK), 0) < 0)
    {
        my_err("send", __LINE__);
    }
}


//************************************************
void *serv_send_thread(void *arg)
{
    puts("serv_send_thread");
}


void init_server_pthread()
{
    puts("init_server_pthread");
    printf("init server pthread !!!\n");
    pthread_t pid_send;
    pthread_create(&pid_send, NULL, serv_send_thread, NULL);
}


void * deal(void *recv_pack_t)
{
    puts("deal");
    int i;
    PACK * recv_pack = (PACK*)recv_pack_t;
    printf("\n\n deal function = %d\n", recv_pack->type);

    switch(recv_pack->type)
    {
        case LOGIN:
            login(recv_pack);
            break;
        case REGISTER:
            registe(recv_pack);
            break;
        case FRIEND_SEE:
            //send_statu(recv_pack);
            break;
        case FRIEND_ADD:
            //friend_add(recv_pack);
            break;
        case FRIEND_DEL:
            friend_del(recv_pack);
            break;
        case GROUP_CREATE:
            //group_create(recv_pack);
            break;
        case GROUP_JOIN:
            //group_join(recv_pack);
            break;
        case GROUP_QIUT:
            //group_qiut(recv_pack);
            break;
        case GROUP_DEL:
            //group_del(recv_pack);
            break;
        case CHAT_ONE:
            send_mes_to_one(recv_pack);
            break;
        case CHAT_MANY:
           // send_mes_to_group(recv_pack);
            break;
        case EXIT:
            break;


    }
}

//*****************main()**********************
int main()
{
    puts("main!!!!!!");
    int n;

    pthread_t pid;
    PACK recv_t;
    PACK *recv_pack;
    int err, socketfd, fd_num;
    struct sockaddr_in fromaddr;
    socklen_t len = sizeof(fromaddr);
    struct epoll_event ev, events[LISTENMAX];



    read_infor();
    

    //********************数据库********************
    //conect_mysql_init();

    signal(SIGINT, signal_close);  
    pthread_mutex_init(&mutex, NULL);

    printf("the server starts..\n");
    init_server_pthread();
    epollfd = epoll_create(EPOLL_MAX);   //epoll句柄

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
    {
        //my_err("create socket failed", __LINE__);
        perror("create socket failed\n");
        printf("server startup failed\n");
        exit(-1);
    }

    err = 5;

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &err, sizeof(int));
    //保证在该套接字在正常关闭或异常退出之后，很短时间内其他程序就而已绑定该端口

    ev.data.fd = listenfd; //要处理的event的fd
    ev.events = EPOLLIN;  //该event可以读
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);   //用来注册事件


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);  //点分十进制--> 网络字节序(大端)
    if(bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind error");
        printf("server startup failed\n");
        exit(-1);
    }
    printf("bind success\n");


    if(listen(listenfd, 10) == -1)     //同时连接服务器的线程最多10个
    {
        perror("listen error");
        printf("server startup failed\n");
        exit(-1);
    }
    printf("listen success\n");
    printf("server init success\n");
    printf("server start service\n");

    while(1)
    {
        puts("before epoll_wait()");
        fd_num = epoll_wait(epollfd,events, EPOLL_MAX, 1000);

        puts("After epoll_wait()");
        printf("fd_num = %d\n",fd_num);

        for(int i=0;i<fd_num;i++)
        {
            if(events[i].data.fd == listenfd)
            {
                socketfd = accept(listenfd, (struct sockaddr*)&fromaddr, &len);
                
                printf("socketfd = %d\n",socketfd);
                printf("%d connected!!\n",socketfd);
                ev.data.fd = socketfd;
                ev.events = EPOLLIN;   

                puts("epoll_ctl before!!!");
                epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
                puts("!!!!!here epoll_ctl()");
            }

            else if(events[i].events & EPOLLIN)  //被监听的事件可读 
            {
                n = recv(events[i].data.fd,&recv_t, sizeof(PACK), 0);
                //读取数据，n值为 实际读取到的字节长度
                
                recv_t.data.send_fd = events[i].data.fd;
                //出错处理
                if(n<0)
                {
                    close(events[i].data.fd);
                    perror("recv");
                    continue;
                }

                recv_pack = (PACK*)malloc(sizeof(PACK));
                memcpy(recv_pack, &recv_t, sizeof(PACK));
                pthread_create(&pid, NULL, deal, (void *)recv_pack);
            }
        }
    }

    return 0;
}
