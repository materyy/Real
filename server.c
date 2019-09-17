/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
    >
 ************************************************************************/
#include<sys/epoll.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<errno.h>
#include<ctype.h>
#include<strings.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<mysql/mysql.h>

#define PORT    9898
#define IP      "127.0.0.1"
#define BUFSIZE 1024


typedef struct friend{
    int sum;
    int online[100];
    char name[100][50];
    int frdid[100];
}Friend;

typedef struct onechat{
    int type;
    int send_fd;
    int recv_fd;
    char send_name[500];
    char recv_name[500];
    char mes[1000];
    struct onechat *next;
}Onechat;

typedef struct groupchat{
    int root[100];//--------------------------------??
    char send_name[500];
    char group_name[500];
    char mes[1000];
    struct groupchat *next;
}Grpchat;

typedef struct file{
    int send_fd;
    int recv_fd;
    char send_name[500];
    char recv_name[500];
    char mes1[1000];
    char mes2[1000];
    struct file *next;
}FFILE;

//包
typedef struct package{
    int type;           //消息类型
    int send_fd;        //发送方fd
    int recv_fd;        //接受方fd
    int file_len;       //文件大小
    char send_name[200];    //发送用户名
    char recv_name[200];    //接受用户名
    char password[200];     //用户密码
    char file_name[200];    //文件名
    char mes[BUFSIZE*2];    //消息内容
}PACK;

MYSQL           mysql;
MYSQL_RES       *result = NULL; //列
MYSQL_ROW       row;        //行
MYSQL_FIELD     *field;

FFILE * head_file = NULL; //文件链表
Onechat * head_onechat = NULL; //私聊链表
Grpchat * head_groupchat = NULL; //群聊链表

int server_close;


void my_err(char *string, int line){
    printf("line: %d\n", line);
    perror(string);
    exit(1);
}

//连接数据库
void connect_mysql(MYSQL *mysql){

    if(NULL == mysql_init(mysql)){  //句柄
        my_err("mysql_init()",__LINE__);
    }

    if(mysql_library_init(0, NULL, NULL)){  //初始化
        my_err("mysql_library_init()",__LINE__);
    }

    if(NULL == mysql_real_connect(mysql,  //连接
                                  "localhost",
                                  "root",
                                  "123456",
                                  "our_chat",
                                  0, NULL, 0)){
        my_err("mysql_real_connect()",__LINE__);
    }

    printf("\t Mysql!!OK\n"); //连接成功

    char command[100];
    sprintf(command, "update user set sockfd = NULL");
    if(mysql_real_query(mysql, command, strlen(command))){
        perror("init_mysql_user_sockfd");
    }
    sprintf(command, "update user set status = 0");
    if(mysql_real_query(mysql, command, strlen(command))){
        perror("init_mysql_user_status");
    }
    printf("user initialization was successful!!\n");//用户初始化成功
}

//关闭数据库
int close_mysql(MYSQL mysql){
    printf("here, in the clise_mysql.\n");
    mysql_close(&mysql);
    mysql_library_end();   // 因为调用了mysql_library_init() 
    printf("Fine.Over\n");
    return 0;
}


void login(int confd, PACK * package);
void registe(int confd, PACK * package);
void all_friend(int confd, PACK *package);
void online_friend(int confd, PACK *package);
void add_friend(int confd, PACK * package);
void delete_friend(int confd, PACK * package);
void chat_one(int confd, PACK * package);

void deal(int confd, PACK *package){
    printf("Deal...\n");
    printf("该数据包的类型为：%d",package->type);
    printf(" %d %d \n", package->send_fd, package->recv_fd);
    printf(" %s %s ", package->password, package->send_name);

    int type = package->type;
    switch(type)
    {
    case 1:
        login(confd, package); //登录
        printf("\t\tlogin函数over..\n");
        break;
    case 2:
        registe(confd, package);//注册 
        break;
    case 3:
        all_friend(confd, package); //所有好友
        break;
    case 4:
        online_friend(confd, package); //在线好友
        break;
    case 5:
        add_friend(confd, package); //加好友
        break;
    case 6:
        delete_friend(confd, package); //删除好友
        break;
    case 7:
        chat_one(confd, package);  //私聊
        break;
    case 8:
        create_group(confd, package);  //建群
        break;
    case 9:
        quit_group(confd, package);  //退出群
        break;
    case 10:
        break_group(confd, package);  //解散群
        break;
    case 11:
        add_group(confd, package);   //添加群
        break;
    case 12:
        view_group_member(confd, package);  //查看群成员
        break;
    case 13:
        chat_group(confd, package);  //群聊
        break;
    case 14:
        all_group(confd, package);  //查看所有群
        break;
    }
}

void chat_one(int confd, PACK * package){
    printf(" 私聊......=======\n\n\n\n");


}


void add_friend(int confd, PACK * package){
    printf("加好友...\n\n\n");
}

void delete_friend(int confd, PACK * package) {
    printf(" 删除好友....\n\n\n");
}


void registe(int confd, PACK * package){
    printf("register..\n");
}

void all_friend(int confd, PACK* package){
    printf("view all friend...\n");
}

void online_friend(int confd, PACK *package){
    printf("view online friend...\n");
}

void login(int confd, PACK *package){
    printf("\t\t\t In the login()....\n");
    PACK here_package;

    printf(" %d %d \n", package->send_fd, package->recv_fd);
    printf(" %s %s ", package->password, package->send_name);
    
    //bzero(&here_package, sizeof(here_package)); //清零初始化
    
    char command[200];
    //int flag = 0;
    
    printf("sprintf 之前...\n");

    strcpy(command, "select userid,password from user where username='");
    strcat(command, "matery'");

    /*sprintf(command, "select userid,password from user where username ='%s'",
           package->send_name); //根据登录名在数据库中找出对应的userid,password*/
    

    printf("command:%s",command);
    printf("到这了吗？？\n");
    //printf("\n\tpackage.send_name:%s", package->send_name);
    mysql_real_query(&mysql, command, sizeof(command));
   printf("\t\t...really??\n");

    /*if(!ret){
        printf("find it in mysql!!\n");
        result = mysql_store_result(&mysql);  //查询结果
        if(result == NULL){
            my_err("login() mysql_store_result",__LINE__);
        }
    }*/


    //result = mysql_store_result(&mysql);  //查询结果

    printf("查询结果！！\n");

    //row = mysql_fetch_row(result);
    printf("\t\tmysql row..\n");
    //printf("%s\n", row[0]);
    //printf("%d\n", row[0]);
    //int uid = int(row[0]);  //保存了用户id
  //  printf("     uid=%d",uid);

    printf("package->password = %s\n", package->password);
    result = mysql_store_result(&mysql);//查询结果
    if(result) {
        printf("%%%%%%%%%%%%%%\n");
        while(row = mysql_fetch_row(result)) {
            printf("************\n");
            if(strcmp(row[3], package->password) == 0) {
                printf("111111111111111\n");
                printf("登陆成功\n");
                printf("row[0] = %d\n", row[0]);
            }
        }
    }
    /*if(strcmp(package->password, row[3]) == 0){
    //密码正确登陆成功！！
        printf("%s Login in !!   登陆成功\n",package->send_name);
        //flag = 1;
        sprintf(command, "update user set status=1,sockfd=%d where userame ='%s'",
                    package->send_fd, package->send_name);//修改在线状态
        if(mysql_real_query(&mysql, command, sizeof(command))){
            perror("login() mysql_real_query()");
        }*/

        mysql_free_result(result);  //释放结果集
        here_package.type = 200; //表示登陆成功

//在这里登陆成功之后  首先要查看 目前的账户有那些未接受的消息
//      直接打印出来

        printf("开始发包..\n");
        int len = sizeof(here_package);
        int size;
        if((size = send(package->send_fd, &here_package, sizeof(here_package), 0)) != len)
        {
            //如果send的包大小与package大小不同，则打印出实际发送的包的大小
            printf("send:%d\n",size);  //查看实际发送包的大小
        }
        
        printf("发包成功..\n");

    }

//}

int main(){

    connect_mysql(&mysql);

    /*
    //创建链表
    if(head_onechat == NULL){

    }*/

    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, serv_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //包含了所有的IP
    serv_addr.sin_port = htons(PORT);

    int option_value = 1;
    int length = sizeof(option_value);
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &option_value, length);//让端口能够快速多次使用

    bind(lfd, (struct sockaddr *)&serv_addr, serv_len);
    listen(lfd, 128);
    printf("Now accepting...\n");

    struct sockaddr_in client_addr;
    socklen_t clie_len = sizeof(client_addr); 

    int epfd = epoll_create(1000);
    struct epoll_event ev;
    struct epoll_event events[1000];

    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev); //注册事件

    printf("Now waiting for connection...\n");

    while(1){
        int num = epoll_wait(epfd, events, 1000, -1); //一直等待无时间限制
        for(int i=0; i<num; i++){
            //判断两种不同的连接情况
            if(events[i].data.fd == lfd){
                int cfd = accept(lfd, (struct sockaddr *)&client_addr, &clie_len);
                printf("connect success!!\n");

                ev.events = EPOLLIN;  //可写
                ev.data.fd = cfd;

                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);

                char ip[50];
                printf("client IP:%s \t  PORT:%d is connected\n",
                       inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
                       ntohs(client_addr.sin_port));
            }
            else if(events[i].events & EPOLLIN){
                //可读

                PACK package;
                int fd = events[i].data.fd;
                int len = recv(fd, &package, sizeof(PACK), 0);
                //printf("%d %d %s %s \n", package.type, package.send_fd,
              //                          package.password, package.send_name);

                if(len == 0){
                    //处理fd
                    char command[100];
                    sprintf(command, "update 'user' set status = 0 where username = '%s'",
                            package.send_name);
                    mysql_query(&mysql, command);
                    printf("'%s' will offline.\n",package.send_name);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
                    close(fd);
                }

                printf("here..\n");

                deal(package.send_fd,&package);
            }
        }
    }
   return 0;
}

