/**
 * c语言udp socket编程----服务端
 * 主要函数及流程
 * 申请socket：    int socket(int af,int type,int protocol);
 *                  af:      address family，如AF_INET
 *                  type:    连接类型，通常是SOCK_STREAM或SOCK_DGRAM
 *                  protocol:协议类型，通常是IPPROTO_TCP或IPPROTO_UDP
 *                  返回值：  socket的编号，为-1表示失败
 *
 *                int bind(int socket,sockaddr * address,uint addrlen);
 *                  socket:  之前创建的socket
 *                  sockaddr:目标地址
 *                  addrlen: 上述结构体的长度
 *                  返回值：   为-1表示失败
 *
 *             int recvfrom(int socket,char * buf,uint buflen,int flag,sockaddr * fromaddr,int * addrlen);【阻塞】【仅UDP】
 *                 接收消息，可以获取发送方的地址
 *                 fromaddr:发送方地址(输出参数)
 *                 addrlen:发送方地址结构体的长度（输入输出参数）
 *                 返回值：>0表示收到的字节数，=0表示连接被关闭，-1表示出错
 **/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>


#define DEFAULT_PORT 8000
#define MAXLINE 4096


int main(int argc, const char * argv[]) {
    
    int    socket_fd;
    struct sockaddr_in     servaddr;
    char    buff[4096];
    int     n, len;
    //初始化Socket
    if( (socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //初始化
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT
    
    if (bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("绑定失败\n");
        return 0;
    }
    len = sizeof(servaddr);
    printf("等待数据：\n");
    while (1) {
        //接受客户端传过来的数据
        n = recvfrom(socket_fd, buff, MAXLINE, 0, (struct sockaddr*)&servaddr, &len);
        if (n < 0) {
            printf("接受失败！\n");
            break;
        } else {
            printf("客户端信息:%s\n", buff);
        }
    
    }
    
    close(socket_fd);
    exit(0);
}
