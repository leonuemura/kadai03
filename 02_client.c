#include "watalab.h"
#define BUFSIZE 1024



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
