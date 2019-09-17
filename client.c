 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>

#define SERV_PORT     9898
#define SERC_IP       "127.0.0.1"  //本机
#define BUFSIZE       1024

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


void my_err(const char *string, int line){
    fprintf(stderr, "line:%d",line);
    perror(string);
    exit(1);
}




int menu_login();
void login(int fd);
void registe();
void quit();
void user_menu(int fd); 
void *recv_pack();


int cfd;  //全局变量
char myname[BUFSIZE];



int main(){

    struct sockaddr_in serv_addr;

    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr)); //0初始化
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    int ret = connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret){//出错
        printf("connect failed\n");
        exit(0);
    }

    //登录菜单
    while(1){
    int choice = menu_login();
        if(choice == 0){ //退出
            close(cfd);
            exit(1);
        }
        if(choice){
            switch(choice){
                case 1://登录
                {
                    login(cfd);
                    break;
                }
                case 2: //注册
                {
                    registe();
                    break;
                }
            }
        }
    }
    //close(cfd);
    return 0;
}

int menu_login(){
    printf("\t\t\t\t+--------------------------+\n");
    printf("\t\t\t\t|                          |\n");
    printf("\t\t\t\t|          1* login        |\n");
    printf("\t\t\t\t|          2* register     |\n");
    printf("\t\t\t\t|          0* quit         |\n");
    printf("\t\t\t\t|                          |\n");
    printf("\t\t\t\t+--------------------------+\n");
    int choice;
    scanf("%d", &choice);
    return choice;
}

void login(int cfd){ //返回1成功， 返回-1失败
    printf("Login..\n");

    pthread_t recv_pthread;  //---------------收包子线程

    PACK package;
    PACK rec_package;
    bzero(&package,sizeof(package));  //初始化 
    package.type = 1;   //Login = 1
    package.send_fd = cfd;  // 标记send_fd.

    char buf[BUFSIZE];
    printf("$$-username:");
    scanf("%s", buf);
    while(getchar() != '\n')    
        continue;
    strcpy(package.send_name, buf);
                   printf("\t:%s",package.send_name);

    strcpy(myname, buf);    //保存自己的登录名----------------
    
    printf("$$-password:");
    scanf("%s", buf);
    while(getchar() != '\n')
        continue;
    strcpy(package.password, buf);
    printf("\t password:%s",package.password);
    

                  
    printf("$$$$$$$$发包前...\n");
    //发包
    if(send(cfd, &package, sizeof(package), 0) < 0){
        my_err("send()", __LINE__);
    }
    printf("$$$$$$$$发包后...\n");

    /*
    if(recv(fd, &package, sizeof(package, 0)) < 0){
        my_err("recv()", __LINE__)
    } */
    printf("收包前...\n");
    while(1){ //直到接收到包才退出
        bzero(&rec_package, sizeof(rec_package));  //清零
        int ret;
        if((ret = (recv(cfd, &rec_package, sizeof(rec_package), 0))) > 0)
            break;
    }
    printf("收包后..\n");

    //判断包类型，看是否登陆成功
    //server返回200表示登录成功
    if(package.type == 200){

        pthread_create(&recv_pthread, NULL, recv_pack, NULL);  //子线程只用来收包

        printf("Congratulations!\n");
        printf("Log in successfully! \n");

        //登陆成功之后，当前用户在线
        //查看链表中有没有消息， 有的话就直接打印出来

        //主菜单（用户菜单）--------
        //
        //
        //
        user_menu(cfd);

    }
    //server返回401表示登录失败
    else if(package.type == 401){
        menu_login();
        printf("Login failed..\n");
        printf("Press enter to continue...");
        getchar();
    }

}

void registe(){
    printf("Register..\n");

}


void *recv_pack(){
    printf("recv_pack......\n");

    PACK package;
    while(1){
        bzero(&package, sizeof(package)); //清零初始化

        int len = recv(cfd, &package, sizeof(PACK), 0);
        if(len < 0)
            perror("client: recv_pack()");

        printf("\t收完包....\n");

        //解析包
        switch(package.type){
        case 6:
            printf("\t______收到私聊消息______\n");
            printf(" $- %s:\n\t %s", package.send_name, package.mes);
            //deal_onechat();
            break;
        case 7:
            printf("\t______收到群聊消息_______\n");
            printf(" $- %s:\n\t %s", package.send_name, package.mes);
            //deal_grpchat();
            break;
        case 8:
            printf("\t______有文件要接收_______\n");
            printf(" $- %s send the file: %s", package.send_name,
                                            package.file_name);
            break;
        

        }
    }

} 

void user_menu(int cfd){
    int choice;

    while(1){
        system("clear"); //清屏
        printf("\n\n\n\n");
        printf("+--------------------------------------+\n");
        printf("|                                      |\n");       
        printf("|            1 * 所有好友              |\n");
        printf("|            2 * 在线好友              |\n");
        printf("|            3 * 加好友                |\n");
        printf("|            4 * 删好友                |\n");
        printf("|            5 * 私聊                  |\n");
        printf("|                                      |\n");
        printf("|            6 * 所有群                |\n");
        printf("|            7 * 创建群                |\n");
        printf("|            8 * 退出群                |\n");
        printf("|            9 * 解散群                |\n");
        printf("|           10 * 查看群成员            |\n");
        printf("|           11 * 群聊                  |\n");
        printf("|                                      |\n");
        printf("|           12 * 聊天记录              |\n");
        printf("|                                      |\n");
        printf("|           13 * 发送文件              |\n");
        printf("|           14 * 接收文件              |\n");
        printf("|                                      |\n");
        printf("|            0 * 退出                  |\n");
        printf("|                                      |\n");
        printf("+--------------------------------------+\n");

        puts("$--");
        scanf("%d",&choice);

        switch(choice){
        case 1:  //查看所有好友
            all_friend();
            break;
        case 2:  //在线好友
            online_friend();
            break;
        case 3:  //加好友
            add_friend();
            break;
        case 4:  //删好友
            delete_friend();
            break;
        case 5:  //私聊
            chat_one();
            break;


        case 6:  //查看所有群
            all_group();
            break;
        case 7:  //创建群
            create_group();
            break;
        case 8:  //退出群
            quit_group();
            break;
        case 9:  //解散群
            break_group();
            break;
        case 10: //查看群成员
            view_group_member();
            break;
        case 11: //群聊
            chat_group();
            beak;
        case 12:  //聊天记录

        case 13:  //发送文件
            send_file();
            break;
        case 14:
            recv_file();
            break;
        


        }
    }
}

void add_group(){
    PACK package;
    package.type = 11;
    printf("要添加的群名：");
    scanf("%s",package.recv_name);
    strcpy(package.send_name, myname);
    send(cfd, &package, sizeof(package), 0);
}

void all_group(){
    PACK package;
    package.type = 14;
    strcpy(package.send_name, myname);
    send(cfd, &package, siezof(package), 0);
}


void add_friend(){
    PACK package;
    package.type = 5; 
    printf("要添加的好友名称：");
    scanf("%s",package.recv_name);
    strcpy(package.send_name, myname);
    send(cfd, &package, sizeof(PACK), 0);
}

void delete_friend(){
    PACK package;
    package.type = 6;
    printf("要删除的好友名称：");
    scanf("%s",package.recv_name);
    strcpy(package.send_name, myname);
    send(cfd, &package, sizeof(PACK), 0);
}


void chat_one(){
    PACK package;
    package.type = 7;
    printf("想要私聊的好友名称：");
    scanf("%s",package.recv_name);
    strcpy(package.send_name, myname);

    //char falg[10];
    while(1){
        printf("消息内容：\n");
        scanf("%s",package.mes);
        if(strcmp(package.mes, "end") == 0)
            break;
        send(cfd, &package, sizeof(PACK), 0);
    }
}

void all_friend(){
    PACK package;
    package.type = 3;
    strcpy(package.send_name, myname);
    send(cfd, &package, sizeof(PACK), 0);
}

void online_friend(){
    PACK package;
    package.type = 4;
    strcpy(package.send_name, myname);
    send(cfd, &package, sizeof(PACK), 0);
} 

void create_group(){
    PACK package;
    package.type = 8;
    strcpy(package.send_name, myname);
    send(cfd, &package, sizeof(PACK), 0);
}

void chat_group(){
    PACK package;
    package.type = 13;
    printf("想要发消息的群名称：");
    scanf("%s",package.recv_name);
    strcpy(package.send_name, myname);
    while(1){
        printf("消息内容：\n");
        scanf("%s",package.mes);
        if(strcmp(package.mes, "end") == 0)
            break;
        send(cfd, &package, sizeof(PACK), 0);
    }
}

