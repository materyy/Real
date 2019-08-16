//#include <mysql.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <termios.h> 



#define LOGIN                    1      //登录
#define REGISTER                 2      //注册
#define FRIEND_SEE               3      //查看好友列表
#define FRIEND_ADD               4      //添加好友
#define FRIEND_DEL               5      //删除好友 
#define GROUP_SEE                6      //查看群组信息
#define GROUP_CREATE             7      //创建群
#define GROUP_JOIN               8      //加入群
#define GROUP_QIUT               9      //退出群
#define GROUP_DEL                1      //删除群
#define CHAT_ONE                 11     //私聊
#define CHAT_MANY                12     //群聊
#define FILE_SEND_BEGIN          13     //发送文件开始
#define FILE_SEND_BEGIN_RP       14     
#define FILE_SEND_STOP_RP        15
#define FILE_RECV_RE             16
#define FILE_SEND                17     //发送
#define FILE_RECV_BEGIN          18     //开始接收
#define FILE_RECV_BEGIN_RP       19
#define FILE_RECV_STOP_RP        20
#define FILE_RECV                21     //接收
#define FILE_FINI_RP             22     
#define MES_RECORD               23     //发送记录
#define EXIT                     -1     //退出；异常


#define SIZE_PASS_NAME   30 
#define MAX_PACK_CONTIAN 100
#define MAX_CHAR         1024
#define NUM_MAX_DIGIT    10

#define OFFLINE    0        //离线 状态
#define ONLINE     1        //在线 状态   
#define BUSY       2        //忙碌 状态

//*********************************************************

//朋友信息
typedef struct  friend_info
{
    int statu;                     
    int mes_num;                  
    char name[MAX_CHAR];               
}FRIEND_INFO; 

//自己的信息
typedef struct user_infor
{
    char            username    [MAX_CHAR];
    FRIEND_INFO     friends     [MAX_CHAR];
    int             friends_num;
    char            group       [MAX_CHAR][MAX_CHAR];
    int             group_num;
}USER_INFOR;

USER_INFOR m_my_infor;


//包结构体信息
typedef struct datas
{
    char    send_name[MAX_CHAR];
    char    recv_name[MAX_CHAR];
    int     send_fd;
    int     recv_fd;
    time_t  time;
    char    mes[MAX_CHAR*2];
}DATA;
 
typedef struct package{
    int type;
    DATA  data;
}PACK;


//***********
typedef struct print_mes
{
    char name[MAX_CHAR];
    char time[MAX_CHAR];
    char mes [MAX_CHAR];

}PRINT_MES;



//********************
PACK m_pack_send    [MAX_CHAR];
int m_send_num;



PACK m_pack_recv_friend_see   [MAX_PACK_CONTIAN];
PACK m_pack_recv_chat         [MAX_PACK_CONTIAN];
PACK m_pack_recv_send_file    [MAX_PACK_CONTIAN];
PACK m_pack_recv_file_mes     [MAX_PACK_CONTIAN];
PACK m_pack_recv_file         [MAX_PACK_CONTIAN];
 
int m_recv_num_friend_see;
int m_recv_num_chat;
int m_recv_num_send_file;
int m_recv_num_file_mes;
int m_recv_num_file;
int m_print_mes_flag;
//****************************************


//*****************全局变量*******************

int sockfd;
char *IP = "127.0.0.1";
short PORT = 6789;
pthread_mutex_t mutex_local_user;

pthread_mutex_t mutex_recv_file;

//********************************************


int print_menu()
{
    int choice;
    do
    {
        //get_status_mes(); //将mes部分清零，传递包


        printf("\n\t\t*******************************************\n");
        printf("\t\t*              1.show   friends             *\n");
        printf("\t\t*              2.add    friends             *\n");
        printf("\t\t*              3.delete friends             *\n");
        printf("\t\t*              4.show   group               *\n");
        printf("\t\t*              5.create group               *\n");
        printf("\t\t*              6.join   group               *\n");
        printf("\t\t*              7.quit   group               *\n");
        printf("\t\t*              8.delete group               *\n");
        printf("\t\t*              9.chat with one              *\n");
        printf("\t\t*              10.chat with many            *\n");
        printf("\t\t*              11.send  file                *\n");
        printf("\t\t*              12.file message box %d       *\n",m_recv_num_file_mes);
        printf("\t\t*              13.mes recording             *\n");
        printf("\t\t*              0.exit                       *\n");
        printf("\t\t*********************************************\n");
        printf("\t\t----$ choice：");

        scanf("%d", choice);
        switch(choice)
        {
            case 1:
              //  friends_see();
                break;
            case 2:
              //  add_friend();
                break;
            case 3:
              //  del_friend();
                break;
            case 4:
              //  group_see();
                break;
            case 5:
             //   group_create();
                break;
            case 6:
             //   group_join();
                break;
            case 7:
             //   group_qiut();
                break;
            case 8:
             //   group_del();
                break;
            case 9:
            //    send_mes_to_one();
                break;
            case 10:
             //   send_mes_to_group();
                break;
            case 11:
            //    send_file();
                break;
            case 12:
             //   file_mes_box();
                break;
            case 13:
             //   mes_record();
                break;
            default:
                break;
        }
    }while(choice != 0);
    return 0;
}


//用户摁下 ctrl+C 就执行该函数
void sig_close(int i)
{
    puts("sig_close()");
    //关闭套接字
    close(sockfd);
    exit(0);
    //退出——————
}


void registe()
{
    puts("registe()");
}







int login()
{
    puts("login()");

    int flag = 0;

}



//int send_login(char username_t[], char password_t[])
//{
//
//}



//登录 menu
int login_menu()
{
    puts("login_menu()");
    int choice;
    do
    {
        puts(" ");
        printf("\t\t+-------------------------------------------+\n");
        printf("\t\t|                1--login in                |\n");
        printf("\t\t|                2--register                |\n");
        printf("\t\t|                0----exit                  |\n");
        printf("\t\t+-------------------------------------------+\n");
        printf("\t\t $---choice: ");
        scanf("%d",choice);
        switch(choice)
        {
            case 1:
                if(login() == 1)
                    return 1;
                break;
            case 2:
                registe();
                break;
            default:
                break;
        }
    }while(choice != 0);
    return 0;
}


//自定义错误处理函数
void my_err(const char *string, int line)
{
    fprintf(stderr, "line: %d ", line);
    perror(string);
    exit(1);
}


//启动 client 并且 connect to server ！
void init()
{
    puts("init()");
    struct sockaddr_in addr;
    printf("The client starts..\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("Unable to connect to server..");
        printf("Client startup failed..\n");
        exit(-1);
    }
    printf("Client startup success\n");
}



void *deal_status(void *argc)
{
    puts("deal_status()");

    int i;
    printf("function deal_status() :\n");

    while(1)
    {
        pthread_mutex_lock(&mutex_local_user);
        for(i=1;i<=m_recv_num_friend_see;i++)
        {
            change_status(m_pack_recv_friend_see[i]);
        }
        m_recv_num_friend_see = 0;
        pthread_mutex_unlock(&mutex_local_user);
        usleep(1);
    }
}


void init_clien_pthread()
{
    puts("init_clien_pthread()");

    pthread_t pid_deal_status, pid_recv, pid_recv_file;

    pthread_create(&(pid_deal_status), NULL, (void *)deal_status, NULL);

    //pthread_create(&pid_recv, NULL, clien_recv_thread, NULL);
}


void change_status(PACK pack_deal_status_t)
{
    puts("change_status()");

}




int main(int argc, char const *atgv[])
{
    //int flag = 0;
    puts("main()");
    signal(SIGINT, sig_close);  //程序中断 ctrl+c 时，执行sig_close()函数

    init(); //启动&连接server

    if(login_menu() == 0)
        return 0;

    //*****************************************
    init_clien_pthread();  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    


    print_menu();

    return 0;
}
