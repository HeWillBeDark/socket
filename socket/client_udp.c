/**
 * c语言udp socket编程 ------ 客户端
 * 主要函数及流程
 * 申请socket：    int socket(int af,int type,int protocol);
 *                  af:      address family，如AF_INET
 *                  type:    连接类型，通常是SOCK_STREAM或SOCK_DGRAM
 *                  protocol:协议类型，通常是IPPROTO_TCP或IPPROTO_UDP
 *                  返回值：  socket的编号，为-1表示失败
 *
 *              int sendto(int socket,char * buf,uint buflen,int flag,sockaddr * address,uint addrlen);【仅UDP】
 *                  socket:  之前创建的socket
 *                  buf:     要发送的缓冲区
 *                  buflen:  要发送的长度
 *                  flag:    一般为0
 *                  sockaddr:目标地址
 *                  addrlen: 上述结构体的长度
 *                  返回值：   发送出去的长度
 *
 **/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/socket.h>


#define DEFAULT_PORT 8000
#define MAXLINE 4096


int main(int argc, const char * argv[]) {
    
    long send_len=0;
    char send_buf[4096];
    int socket_fd;
    struct sockaddr_in     servaddr;
    
    //初始化Socket
    if( (socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //初始化
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT
    
    //发送,接收数据
    printf("请输入发送信息:");
    scanf("%s", send_buf);
    send_len = sendto(socket_fd, send_buf, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (send_len < 0) {
        printf("发送失败\n");
    }

    close(socket_fd);
    return 0;
}

