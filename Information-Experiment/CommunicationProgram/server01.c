// title         : 情報実験 9回 課題1 サーバプログラム


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 10024

int main(){
  int i;
  int fd1, fd2;
  struct sockaddr_in saddr; // server
  struct sockaddr_in caddr; // client
  
  int len;
  int ret;
  char buf[1024];

  // 受付ソケットの作成
  if((fd1=socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(1);
  }

  // 接続情報の初期化と追加
  memset((char *)&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(PORT);
  saddr.sin_addr.s_addr = INADDR_ANY;

  // ソケットの命名
  if(bind(fd1, (struct sockaddr *) &saddr, (socklen_t)sizeof(saddr)) < 0){
    perror("bind");
    exit(1);
  }

  // 待ち受け開始
  if(listen(fd1, 1) < 0){
    perror("listen");
    exit(1);
  }

  while(1){
    // 接続受付
    len = sizeof(caddr);
    if((fd2 = accept(fd1, (struct sockaddr *)&caddr, (socklen_t *)&len)) < 0){
      perror("accept");
      exit(1);
    }
    fprintf(stderr, "Connection established: socket %d used.\n", fd2);
    
    // クライアントから文字列が送信されたら, 小文字を大文字に, 大文字を小文字に直して送り返す
    while((ret = read(fd2, buf, 1024)) > 0){
      fprintf(stderr, "read: %s\n", buf);
      for(i=0; i<ret; ++i){
        if(isalpha(buf[i])){
          if((buf[i]>= 'a') && (buf[i]<='z')) buf[i] = toupper(buf[i]);
          else if((buf[i]>= 'A') && (buf[i]<='Z')) buf[i] = tolower(buf[i]);
        }
      }
      fprintf(stderr, "write: %s\n", buf);
      write(fd2, buf, strlen(buf)+1);
      fsync(fd2);
    }

    // クライアントとの接続を終了
    fprintf(stderr, "Disconnection established: socket %d used.\n", fd2);
    close(fd2);
  }
  
  // 待ち受けの終了
  close(fd1);
  return 0;
}
