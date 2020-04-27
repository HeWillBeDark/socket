/**
 * c语言tcp socket编程----服务端
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
 *                int listen(int socket,int maxconn);【仅TCP】【服务器】
 *                  将一个socket设置为监听状态,专门用来监听的socket叫做master socket
 *                  maxconn:最大接收连接数
 *                  返回值：失败返回-1，成功返回0
 *
 *                int accept(int socket,sockaddr * fromaddr,int * addrlen);【阻塞】【仅TCP】【服务器】
 *                  接收一个客户机的连接，返回一个socket，来自客户机的socket叫connected socket
 *                  socket:用来监听的socket（master socket）
 *                  fromaddr:客户机的地址信息
 *                  addrlen:地址结构体的长度（输入输出参数）
 *                  返回值：返回一个新的socket，这个socket专门用来与此客户机通讯（connected socket）
 *
 *                int recv(int socket,char * buf,uint buflen,int flag);【阻塞】
 *                  UDP时：接收任何一个发送到该socket的消息（无法获取发送方地址）
 *                  TCP时：接收一个已连接的socket (connected socket)发送的信息
 *                  socket:UDP时，为之前创建的socket，TCP时，为connected socket(accept接收到的socket)
 *                  buf:接收的缓冲区
 *                  buflen:缓冲区的长度
 *                  flag:一般为0
 *                  返回值：>0表示收到的字节数，=0表示连接被关闭，-1表示出错
 *                  注意：对于TCP，请确保socket是已连接的，因为只有已连接的socket会阻塞此函数
 *                  该函数实际上是从缓冲区取指定长度的数据，如果缓冲区没有数据，则会阻塞；如果没有取完，则下次使用此函数的时候不会阻塞
 *                  应注意：当一次无法获得对方发送的全部数据，在数据不完整的时候，程序可能无法向下执行，可以考虑将数据放在缓冲区中，等数据全部接收完成的时候再使用
 *
 *                int send(int socket,char * buf,uint buflen,int flag);【仅TCP】
 *                  socket:  之前创建的socket
 *                  buf:     要发送的缓冲区
 *                  buflen:  要发送的长度
 *                  flag:    一般为0
 *                  返回值：   发送出去的长度
 *
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

    int    socket_fd, connect_fd;
    struct sockaddr_in     servaddr;
    char    buff[4096], send_buf[4096];
    int     n, send_len;
    //初始化Socket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //初始化
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT
    
    //将本地地址绑定到所创建的套接字上
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //开始监听是否有客户端连接
    if( listen(socket_fd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======waiting for client's request======\n");
    //阻塞直到有客户端连接，不然多浪费CPU资源。
    if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
    }
  
    while (1) {
        //接受客户端传过来的数据
        n = recv(connect_fd, buff, MAXLINE, 0);
        if (n < 0) {
            printf("接受失败！\n");
            break;
        } else {
            printf("客户端信息:%s\n", buff);
        }
        printf("请输入回复信息:");
        scanf("%s", send_buf);
        send_len = send(connect_fd, send_buf, MAXLINE, 0);
        if (send_len < 0) {
            printf("发送失败！\n");
            break;
        }
    }

    close(connect_fd);
    close(socket_fd);
}

