#include "watalab.h"
#define BUFSIZE 1024

int watalab_tcp_connect(const char *hostname, int port)
{
  int sock;
  int ret;
  struct sockaddr_in addr;
  struct hostent *host;
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  addr.sin_family = AF_INET;
  host = gethostbyname(hostname);
  addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
  addr.sin_port = htons(port);
  ret = connect(sock, (struct sockaddr *)&addr, sizeof addr);
  if(ret < 0){
    return -1;
  }else{
    return sock;
  }
}

int watalab_do_talk(int sock)
{
  fd_set fds;
  char buf[1024];
  
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  FD_SET(sock, &fds);
  
  select(sock + 1, &fds, NULL, NULL, NULL);
  
  if(FD_ISSET(0, &fds) != 0){
    int ret;
    ret = read(0, buf, BUFSIZE);
    write(sock, buf, ret);
  }
  
  if(FD_ISSET(sock, &fds) != 0){
    int ret;
    ret = recv(sock, buf, BUFSIZE, 0);
    if(ret > 0){
      write(1, buf, ret);
    }else{
      return -1;
    }
  }
  
  return 1;
}



int main(int argc, char** argv)
{
  int sock;
  int port;
  int ret;
  
  if(argc !=3){
   printf("usage: %s [ip address][port]\r\n", argv[0]);
   return -1;
  }
  
  port = atoi(argv[2]);
  
  sock = watalab_tcp_connect(argv[1], 11111);
  
  ret = 1;
  while(ret == 1){
    ret = watalab_do_talk(sock);
  }
  
  close(sock);
  
  return 0;
}
