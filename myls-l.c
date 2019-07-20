#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <grp.h>
#include <string.h>
#include <pwd.h>

static bool flags[4];
int myls(char *s,bool flags[]);
void lprint(char *fname);

int main(int argc,char *argv[])
{
    char c;
    int i;
    while((c = getopt(argc,argv,"lidarFR"))!= -1) //获取命令行输入的选项参数
    {
        switch(c)
        {
        case 'l': flags[0]=1; break;
        case 'a': flags[1]=1; break;
        case 'R': flags[2]=1; break;
        
        deault:  return -1;
        }
    }
    if(argc==optind) //命令行没有输入目录参数，设定函数目录为当前目录
    {
        myls(".",flags);
    }
    else
    {
        for(i = optind; i<argc; i++) //依次对输入的所有目录参数进行myls函数操作
            myls(argv[i],flags);
    }
    return 0;
}

//myls函数，调用lprint函数实现-l格式化输出
int myls(char *s,bool flags[])
{
    struct stat buf,statbuf,tmps;
    struct stat sort[256];
    char *t;
    char fname[256],tmpm[256],sfname[256];
    char sname[256][256];
    DIR *dir;
    struct dirent *pd;
    int num,i,j;

    if(S_ISDIR(buf.st_mode))
    {}
    else
    {
        if(lstat(s,&buf)<0) return -1;//读取文件状态信息
        if(flags[0]) lprint(s); // *-l*
        printf("%-s",s);
        if (flags[0])
        {
            /* //printf("\n"); //*-l* */
        }
        else
        {
            printf("\t");
        }
    }
    printf("\n");
}

//lprint 函数，控制 -l 格式输出
void lprint(char *fname)
{
    struct stat buf;
    int n;
    char link[256];
    struct passwd *pw;
    struct group *gr;
    struct tm *t;
    lstat(fname,&buf);
    switch(buf.st_mode & S_IFMT) //获取并转换之后打印文件类型
    {
    case S_IFREG:
        printf("-");
        break;
    case S_IFDIR:
        printf("d");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFBLK:
        printf("b");
        break;
    case S_IFIFO:
        printf("p");
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFSOCK:
        printf("s");
        break;
    }
    for(n=8;n>=0;n--)  //获取并转换之后打印文件的读写属性
    {
        if(buf.st_mode&(1 << n))
        {
            switch(n%3)
            {
            case 2:
                printf("r");
                break;
            case 1:
                printf("w");
                break;
            case 0:
                printf("x");
                break;
            default:
                break;
            }
        }
        else
            printf("-");
    }
    printf("%3d",buf.st_nlink); //硬链接数

    pw = getpwuid(buf.st_uid); //所属用户名
    printf(" %-6s",pw->pw_name);

    gr = getgrgid(buf.st_gid); //所属用户组名
    printf(" %-6s",gr->gr_name);

    printf(" %5ld",buf.st_size); //字节数

    t=localtime(&buf.st_mtime); //最后修改时间
    printf(" %d-%d-%d %02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min);

    if(S_ISLNK(buf.st_mode)) //判断是否为链接文件
    {
        printf("->");
        readlink(fname,link,100);
        printf("%s",link);
    }


    printf("\t");

}
