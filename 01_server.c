#include "watalab.h"
#define BUFSIZE 1024

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
  int sock_listen;
  
  sock_listen = watalab_tcp_listen(11111);
  
  while(true){
    int ret;
    int sock_client;
    
    sock_client = watalab_accept(sock_listen);
    ret = 1;
    while(ret == 1){
      ret = watalab_do_talk(sock_client);
    }
    close(sock_client);
  }
  close(sock_listen);
}
