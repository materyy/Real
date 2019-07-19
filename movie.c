#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{ 
    char name[40];   //名称
    char actor[40];    //主演
    char num[40];  //编号
    int age;   //出产时长
    float score;  //评分情况
}movie;

typedef struct node
{
    movie data;
    struct node *left;
    struct node *right;
}Node;


void init();
Node *createlist();
void initallist(Node *head);
int insertmovie(Node *head);
void insertlist(Node *head,movie data);
int deletemovie(Node *head);
int deletelist_name(Node *head,char *name);
int deletelist_actor(Node *head,char *actor);
int deletelist_num(Node *head,char *num);
void searchmovie(Node *head);
Node *searchlist_name(Node *head,char *name);
Node *searchlist_actor(Node *head,char *actor);
Node *searchlist_num(Node *head,char *num);
void searchlist_age(Node *head,int age);
Node *searchlist_score(Node *head,float score);
int lenlist(Node *head);
void sortlist_name(Node *head,int len);
void printlist(Node *head);
int savetofile(Node *head);
int modifymovie(Node *head);
int modifylist(Node *head,char *name);
int third_chance();
void sortlist_age(Node *head,int len);
void sortlist_num(Node *head,int len);
void sortlist_score(Node *head,int len);
void sortlist_actor(Node *head,int len);
void printmoviehead(void);

//主函数
int main()
{
    third_chance();
    //init();      //第一次运行需要执行，创建一个文件，并初始化一部分数据
    Node *head=createlist();
    initallist(head);
    while(1)
    {
        int len=lenlist(head);
        system("clear");
        printf("\n\n\n\n\t\t**************************电影信息管理系统***************************\n");
        printf("\t\t                            目前所有影片\n\n");
        printlist(head);
        printf("\t\t    1**添加  2**删除  3**查找  4**修改  5**排序  6**保存  7**退出\n");
        printf("\t\t*********************************************************************\n");
        int select=0;
        puts("请做出选择：");
        printf("-------");
        scanf("%d",&select);
        if(select>=1&&select<=7)
        {
            switch(select)
            {
            case 1:   //添加
                insertmovie(head);
                system("clear");
                printmoviehead();
                printlist(head);
                break;
            case 2:   //删除
                deletemovie(head);
                printmoviehead();
                printlist(head);
                break;
            case 3:    //查找
                searchmovie(head);
                getchar();
                puts("\n\n\t\t       ---------------*按回车键返回主页面*--------------");
                getchar();
                break;
            case 4:    //修改
                modifymovie(head);
                system("clear");
                printmoviehead();
                printlist(head);
                break;
            case 5:     //排序
                printf("\t\t请选择想要排序的方式：\n");
                printf("\t\t 1--按照影片名称排序\t\t   4--按照评分情况排序\n");
                printf("\t\t 2--按照已有序号排序\t\t   5--按照主演姓名排序\n");
                printf("\t\t 3--按照上映日期排序\t\t  \n");
                int mychoice;
                printf("请做出选择：");
                scanf("%d",&mychoice);
                if(mychoice==1)  //根据-电影名称-排序
                {
                    sortlist_name(head,len);
                    printmoviehead();
                    printlist(head);
                    break;
                }
                else if(mychoice==2)  //根据-序号-排序
                {
                    sortlist_num(head,len);
                    printmoviehead();
                    printlist(head);
                    break;
                }
                else if(mychoice==3)  //根据-上映日期-排序
                {
                    sortlist_age(head,len);
                    printmoviehead();
                    printlist(head);
                    break;
                }
                else if(mychoice==4)   //根据-评分情况-排序
                {
                    sortlist_score(head,len);
                    printmoviehead();
                    printlist(head);
                    break;
                }
                else if(mychoice==5)   //根据-主演姓名-排序
                {
                    sortlist_actor(head,len);
                    printmoviehead();
                    printlist(head);
                    break;
                }
                else         
                {
                    system("clear");
                    printf("\n\n\n\n\n\t\t\t\t------@@输入错误@@------");
                    printf("\n\t\t\t\t ------请重新选择------");
                    printf("\n\n\t\t\t\t       ---返回---\n");
                    getchar();
                    getchar();
                    break;
                }


            
            case 6:     //保存
                savetofile(head);
                getchar();
                system("clear");

                puts("\n\n\n\n\n\n\n\t\t\t\t\t\tsuccess!");
                puts("\t\t\t\t\tpress any keys to quit!");
                getchar();
                printmoviehead();
                printlist(head);
                break;
            case 7:     //退出
                system("clear");
                getchar();
                puts("\n\n\n\n\t\t---------------------------====即将退出!====---------------------------");
                getchar();
                system("clear");
                puts("\n\n\n\t\tEMMMMMM...");
                printf("\n\n\t\t==想走？！保存了没？？");
                printf("\n\t\t``````````````````````\n");
                puts("\t\t==1>存了存了，烦死了\n\t\t==2>还没呢，急啥呀\n\t\t==3>不存了\n");
                int sav;
                printf("\t\t&选一个--");
                scanf("%d",&sav);
                if(sav==1)
                {
                    getchar();
                    printf("\t\t|||可把你给能坏了，不就应该存起来吗？！！");
                    getchar();
                    puts("\t\t|||退出了，886...");
                    puts("\n\n\n\n");
                    exit(1);
                    break;
                }
                else if(sav==2)
                {
                    getchar();
                    system("clear");
                    puts("\n\n\n\n\n\t\t|||!!");
                    puts("\t\t|||清醒一点！！！！！");
                    puts("\t\t|||请注意给出的 **6 选项！");
                    getchar();
                    break;
                }
                else if(sav==3)
                {
                    getchar();
                    system("clear");
                    puts("\n\n\n\n\n\t\t|||!!!!!");
                    puts("\t\t|||好嘛");
                    puts("\t\t|||不存就不存..");
                    puts("\t\t再见，886...");
                    getchar();
                    exit(1);
                    break;
                }
                getchar();
            }
        }
        else
        {
            puts("Error!");
            break;
        }
    }

    return 0;
}


//写入文件  初始化影片信息
void init(void)
{
    FILE *fp=fopen("themovie.txt","w");
    movie movies[40]={{"blue_moon","AndreHolland","00001",1,9.0},{"Infernal_Affairs","Liudehua","00002",4,9.1},{"Scen_of_a_Woman","AlPacino","00003",3,8.9},{"Gone_with_the_Wind","  ","00004",6,8.7},{"Ghost","   ","00005",6,8.9},{"Kramer_vs.kramer","    ","00006",8,7.9}};   
    fwrite(movies,sizeof(movie),6,fp);
    fclose(fp);
}

//打印表格头
void printmoviehead(void)
{
                printf("\t\t****************************电影信息管理系统************************\n");
                printf("\t\t                              目前所有影片\n");
}

//创建一个带头结点的双向链表
Node *createlist()
{
    Node *head=(Node *)malloc(sizeof(Node));
    if(head!=NULL)
    {
        head->right=NULL;
        head->left=NULL;
        return head;
    }
    else
    {
        puts("创建链表失败！Error!");
        return NULL;
    }
}


//初始化链表
void initallist(Node *head)
{
    FILE *fp=fopen("themovie.txt","rw");
    movie newmovie;
    while(fread(&newmovie,sizeof(movie),1,fp)!=0)
    {
        insertlist(head,newmovie);
    }
    fclose(fp);
}

//插入功能调度
int insertmovie(Node *head)
{
    movie newmovie;
    printf("\n\t\t$~Please input the movie's message:\n");
    printf("\t\t$~name:");
    scanf("%s",newmovie.name);
    printf("\t\t$~actor:");
    scanf("%s",newmovie.actor);
    printf("\t\t$~num:");
    scanf("%s",newmovie.num);
    printf("\t\t$~age:");
    scanf("%d",&newmovie.age);
    printf("\t\t$~score:");
    scanf("%f",&newmovie.score);

    Node *newnode=(Node *)malloc(sizeof(Node));
    if(newnode!=NULL)
    {
        newnode->data=newmovie;
        newnode->left=head;
        newnode->right=head->right;
        head->right=newnode;
        if(newnode->right!=NULL)
            newnode->right->left=newnode;
        else
            head->left=newnode;
        return 1;
    }
    else 
        return -1;
}

//插入功能实现函数
void insertlist(Node *head,movie data)
{
    Node *newnode=(Node *)malloc(sizeof(Node));
    if(newnode!=NULL)
    {
        newnode->data=data;
        newnode->right=head->right;
        newnode->left=head;
        head->right=newnode;
        if(newnode->right==NULL)
            head->left=newnode;
        else
            newnode->right->left=newnode;
    }
}

//删除功能调度函数
int deletemovie(Node *head)
{
    printf("\t\tHow do you want to delete?\n\t\t");
    puts("1-by name\t\t2->by num");
    int choice;
    printf("\n\t\t$~");
    scanf("%d",&choice);
    if(choice<4&&choice>0)
    {
        if(choice==1)
        {
            printf("\t\t#Please input the movie's name:");
            char name[40];
            printf("\n\n\t\t$~");
            scanf("%s",name);
            int d=deletelist_name(head,name);
            if(d==1)
            {
                system("clear");
                puts("\n\n\n\n\n\n\n\n\t\t__________________________________________________________________");
                puts("\n\n\t\t\t\t\t=====success=====");
                puts("\n\t\t__________________________________________________________________");
            }
            else
            {
                system("clear");
                puts("\n\n\n\n\n\n\n\n\t\t__________________________________________________________________");
                puts("\n\n\t\t\t\t\t======fault======");
                puts("\t\t\t\t     --Can't find the movie--");
                puts("\n\t\t__________________________________________________________________");
            }
            getchar();
            getchar();
        }
        else if(choice==2)
        {
            puts("\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
            printf("\t\t#Please enter the name of the actor in the movie:");
            char num[40];
            printf("\n\n\t\t$~");
            scanf("%s",num);
            int d=deletelist_num(head,num);
            if(d==1)
            {
                system("clear");
                puts("\n\n\n\n\n\n\n\n\t\t__________________________________________________________________");
                puts("\n\n\t\t\t\t\t=====success=====");
                puts("\n\t\t__________________________________________________________________");
            }
            else
            {
                system("clear");
                puts("\n\n\n\n\n\n\n\n\t\t__________________________________________________________________");
                puts("\n\n\t\t\t\t\t======fault======");
                puts("\t\t\t\t     --Can't find the movie--");
                puts("\n\t\t__________________________________________________________________");
            }
            getchar();
            getchar();
        }
    }
    else 
    {
        puts("\t\t---------------&&Error&&---------------\n");
        puts("\t\t        =========break=========");

    }
}

//按照   |影片名称|  删除实现函数
int deletelist_name(Node *head,char *name)
{
    Node *Searchmovie=searchlist_name(head,name);
    if(Searchmovie!=NULL)
    {
        Searchmovie->left->right=Searchmovie->right;
        Searchmovie->right->left-Searchmovie->left;
        free(Searchmovie);
        return 1;
    }
    else 
        return 0;  
}


//按照    |影片序号|  删除实现函数
int deletelist_num(Node *head,char *num)
{
    Node *Searchmovie=searchlist_num(head,num);
    if(Searchmovie!=NULL)
    {
        Searchmovie->left->right=Searchmovie->right;
        Searchmovie->right->left-Searchmovie->left;
        free(Searchmovie);
        return 1;
    }
    else 
        return 0;  
}


//查找功能调度
void searchmovie(Node *head)
{
    printf("\t\t*********************************************************************\n\n");
    puts("\t\tHow do you want to search?\n");
    puts("\t\t1->by name\t\t\t4->by age");
    puts("\t\t2->by actor\t\t\t5->by score");
    puts("\t\t3->by num");
    printf("\t\t---");
    int chazhao;
    scanf("%d",&chazhao);
    if(chazhao==1)
    {
        puts("\t\tplease input the movie's name:");
        char name[40];
        printf("\t\t$~");
        scanf("%s",name);
        Node *result=searchlist_name(head,name);
        if(result!=NULL)
        {
            system("clear");
            printf("\n\n\t\tNAME\t\t\tACTOR\t\tNUM\tAGE\tSCORE\n\n");
            printf("\t\t-----------------------------------------------------------------\n");
            printf("\t\t%-24s%-16s%-8s%-8d%-8.2f\n",result->data.name,result->data.actor,result->data.num,result->data.age,result->data.score);
        }
        else
        {
            puts("\t\t==\n\t\t==\n\t\t==\n\t\t==");
            puts("\t\t==Can't find the movie!");   
        }
    }
    else if(chazhao==2)
    {
        puts("\t\tplease input the movie's actor:");
        char actor[40];
        printf("\t\t$~");
        scanf("%s",actor);
        searchlist_actor(head,actor);
    }
    else if(chazhao==3)
    {
        puts("\t\tplease input the movie's num:");    
        char num[40];
        printf("\t\t$~");
        scanf("%s",num);
        Node *result=searchlist_num(head,num);
        if(result!=NULL)
        {
            system("clear");
            printf("\n\n\t\tNAME\t\t\tACTOR\t\tNUM\tAGE\tSCORE\n\n");
            printf("\t\t-----------------------------------------------------------------\n");
            printf("\t\t%-24s%-16s%-8s%-8d%-8.2f\n",result->data.name,result->data.actor,result->data.num,result->data.age,result->data.score);
        }
        else
        {
            puts("\t\t==\n\t\t==\n\t\t==\n\t\t==");
            puts("\t\t==Can;t find the movie!");
        }
    }

    else if(chazhao==4)
    {
        puts("\t\tPlease input the movie's age:");
        int age;
        printf("\t\t$~");
        scanf("%d",&age);
        searchlist_age(head,age);
    }
    else if(chazhao==5)
    {
        puts("\t\tPlease input the movie's score:");
        float score;
        printf("\t\t$~");
        scanf("%f",&score);
        searchlist_score(head,score);
    }
    else 
        puts("\n\n\t========Error========");
}


//按照  电影名称  查找功能实现函数   ---1
Node *searchlist_name(Node *head,char *name)
{
    Node *themovie=head;
    do
    {
        themovie=themovie->right;
        if(strcmp(themovie->data.name,name)==0)
            return themovie;
    }
    while(themovie->right!=NULL);
    return NULL;
}

//按照  主演姓名  查找功能实现函数    ---2
Node *searchlist_actor(Node *head,char *actor)
{
    int t=0;
    Node *themovie=head;
    system("clear");
    printf("\n\n\t\tNAME\t\t\tACTOR\t\tNUM\tAGE\tSCORE\n\n");
    printf("\t\t-----------------------------------------------------------------\n");
    while(themovie->right!=NULL)
    {
        themovie=themovie->right;
        if(strcmp(themovie->data.actor,actor)==0)
        {
            printf("\t\t%-24s%-16s%-8s%-8d%-8.2f\n",themovie->data.name,themovie->data.actor,themovie->data.num,themovie->data.age,themovie->data.score);
            t++;
        }
    }
    if(t==0)
    {
        puts("\t\t==\n\t\t==\n\t\t==\n\t\t==");
        puts("\t\t==Can't find the mvoie!");
    }
}

//按照  影片序号  查找功能实现函数    ---3
Node *searchlist_num(Node *head,char *num)
{
    Node *themovie=head;
    do
    {
        themovie=themovie->right;
        if(strcmp(themovie->data.num,num)==0)
            return themovie;
    }
    while(themovie->right!=NULL);
    return NULL;
}

//按照  上映时间  查找功能实现函数   ---4
void searchlist_age(Node *head,int age)
{
    int t=0;
    Node *themovie=head;
    system("clear");
    printf("\n\n\t\tNAME\t\t\tACTOR\t\tNUM\tAGE\tSCORE\n\n");
    printf("\t\t-----------------------------------------------------------------\n");
    while(themovie->right!=NULL)
    {
        themovie=themovie->right;
        if(themovie->data.age==age)
        {
            printf("\t\t%-24s%-16s%-8s%-8d%-8.2f\n",themovie->data.name,themovie->data.actor,themovie->data.num,themovie->data.age,themovie->data.score);
            t++;
        }
    }
    if(t==0)
    {
        puts("\t\t==\n\t\t==\n\t\t==\n\t\t==");
        puts("\t\t==Can't find the mvoie!");
    }
}
//按照  评分情况  查找功能实现函数    ---5
Node *searchlist_score(Node *head,float score)
{
    int t=0;
    Node *themovie=head;
    system("clear");
    printf("\n\n\t\tNAME\t\t\tACTOR\t\tNUM\tAGE\tSCORE\n\n");
    printf("\t\t-----------------------------------------------------------------\n");
    while(themovie->right!=NULL)
    {
        themovie=themovie->right;
        if(themovie->data.score==score)
        {
            printf("\t\t%-24s%-16s%-8s%-8d%-8.2f\n",themovie->data.name,themovie->data.actor,themovie->data.num,themovie->data.age,themovie->data.score);
            t++;
        }
    }
    if(t==0)
    {
        puts("\t\t==\n\t\t==\n\t\t==\n\t\t==");
        puts("\t\t==Can't find the mvoie!");
    }
}
//求双向链表长度
int lenlist(Node *head)
{
    Node *p=head;
    int len=0;
    while(p->right!=NULL)
    {
        len++;
        p=p->right;
    }
    return len;
}

//根据影片**名称字符串**排序
void sortlist_name(Node *head,int len)
{
    Node *p,*q,*max;
    movie t;
    p=head->right;
    int i,j;
    for(i=0;i<len-1;i++)
    {
        if(p==NULL)
            break;
        max=p;
        q=p;
        for(j=0;j<len;j++)
        {
            if(q==NULL)
                break;
            if(strcmp(max->data.name,q->data.name)<0)
                max=q;
            q=q->right;
        }
        if(max!=p)
        {
            t=max->data;
            max->data=p->data;
            p->data=t;
        }
        p=p->right;
    }
}


//根据影片 序号  排序
void sortlist_num(Node *head,int len)
{
    Node *p,*q,*max;
    movie t;
    p=head->right;
    int i,j;
    for(i=0;i<len-1;i++)
    {
        if(p==NULL)
            break;
        max=p;
        q=p;
        for(j=0;j<len;j++)
        {
            if(q==NULL)
                break;
            if(strcmp(max->data.num,q->data.num)<0)
                max=q;
            q=q->right;
        }
        if(max!=p)
        {
            t=max->data;
            max->data=p->data;
            p->data=t;
        }
        p=p->right;
    }
}
 

//根据影片   主演姓名  排序
void sortlist_actor(Node *head,int len)
{
    Node *p,*q,*max;
    movie t;
    p=head->right;
    int i,j;
    for(i=0;i<len-1;i++)
    {
        if(p==NULL)
            break;
        max=p;
        q=p;
        for(j=0;j<len;j++)
        {
            if(q==NULL)
                break;
            if(strcmp(max->data.actor,q->data.actor)<0)
                max=q;
            q=q->right;
        }
        if(max!=p)
        {
            t=max->data;
            max->data=p->data;
            p->data=t;
        }
        p=p->right;
    }
}

//根据影片**出产时长**排序
void sortlist_age(Node *head,int len)
{
    Node *p,*q,*max;
    movie t;
    p=head->right;
    int i,j;
    for(i=0;i<len-1;i++)
    {
        if(p==NULL)
            break;
        max=p;
        q=p;
        for(j=0;j<len;j++)
        {
            if(q==NULL)
                break;
            if((max->data.age)<(q->data.age))
                max=q;
            q=q->right;
        }
        if(max!=p)
        {
            t=max->data;
            max->data=p->data;
            p->data=t;
        }
        p=p->right;
    }
}


//根据影片**评分情况**排序
void sortlist_score(Node *head,int len)
{
    int i,j;
    Node *p,*q,*max;
    p=head->right;
    movie t;
    for(i=0;i<len-1;i++)
    {
        if(p==NULL)
            break;
        max=p;
        q=p;
        for(j=0;j<len;j++)
        {
            if(q==NULL)
                break;
            if((max->data.score)<(q->data.score))
                max=q;
            q=q->right;
        }
        if(max!=p)
        {
            t=max->data;
            max->data=p->data;
            p->data=t;
        }
        p=p->right;
    }
}

//打印双链表内容
void printlist(Node *head)
{
    Node *p=head->right;
    printf("\t\tNAME\t\t\tACTOR\t\t\tNUM\tAGE\tSCORE\n");
    printf("\t\t---------------------------------------------------------------------\n");
    while(p!=NULL)
    {
        printf("\t\t%-24s%-24s%-8s%-8d%-8.2f\n",p->data.name,p->data.actor,p->data.num,p->data.age,p->data.score);
        p=p->right;
    }
    printf("\t\t*********************************************************************\n");
}


//将影片信息写回文件themovie.txt     
int savetofile(Node *head)
{
    FILE *fp=fopen("themovie.txt","w");
    Node *p=head->right;
    while(p!=NULL)
    {
        fwrite(&p->data,sizeof(movie),1,fp);
        p=p->right;
    }
    fclose(fp);
    return 1;
}

//修改功能调度
int modifymovie(Node *head)
{
    printf("\t\tplease input the movie's name what you want to exchange:");
    printf("\n\t\t$~");
    char name[40];
    scanf("%s",name);
    int themovie=modifylist(head,name);
    if(themovie)
        puts("success!");
    else 
        puts("fault!");
    return themovie;
}

//修改功能能实现函数
int modifylist(Node *head,char *name)
{
    Node *result=searchlist_name(head,name);
    if(result!=NULL)
    {
        deletelist_name(head,name);

        if(insertmovie(head)==1)
        {
            puts("success!");
            getchar();
            return 1;
        }
        else 
        {
            puts("fault!");
            return 0;    
        }
    }
    else
        return 0;
}

//三次输入密码的机会

int third_chance(void)
{
    int i;
    char ch[40];
    puts("请输入密码：");
    scanf("%s",ch);
    for(i=0;i<3;i++)
    {
        if(strcmp(ch,"666666")==0)
        {
            system("clear");
            puts("\n\n\n\n\t\t _______________________________________________________________");
            puts("\t\t|                                                               |");
            puts("\t\t|                                                               |");
            puts("\t\t|                                                               |");
            puts("\t\t|===========================$登陆成功$==========================|");
            puts("\t\t|                                                               |");
            puts("\t\t|                                                               |");
            puts("\t\t|_______________________________________________________________|");
            getchar();
            getchar();
            break;
        }
        else
        {
            if(i==2)
            {
                puts("输入密码有误，请核对后重新输入！");
                break;
            }
            printf("密码有误，还有%d次机会输入：\n",2-i);
            if(i==1)
                printf("('6'*6):  ");
            scanf("%s",ch);
        }
    }
    return 0;
}
