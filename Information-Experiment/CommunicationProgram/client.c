// title         : 情報実験 9回 クライアントプログラム

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define HOSTNAME "localhost"
#define PORT 10024

int main(){
  struct sockaddr_in saddr;
  int soc;
  char buf[BUF_SIZE];
  struct hostent *hp;

  // ソケットの作成.
  if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(1);
  }

  // 接続情報の初期化と追加
  memset((char*)&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(10024);
  saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // サーバに接続
  if(connect(soc, (struct sockaddr *)&saddr, (socklen_t)sizeof(saddr)) < 0){
    perror("connect");
    exit(1);
  }
  fprintf(stderr, "Connection established: socket %d\n", soc);

  // 標準入力で文字列が入力されるたびに
  while(fgets(buf, BUF_SIZE, stdin)){
    // fgetsで入力されたときの改行文字を消去
    if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';

    // サーバに送信して返答を待つ
    write(soc, buf, strlen(buf)+1); 
    fsync(soc);
    read(soc, buf, BUF_SIZE);

    /// 送信されてきた文字列に改行文字があれば取り除く
    if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';

    fprintf(stdout, "%s\n", buf);
  }

  //接続を閉じる 
  close(soc);  
  fprintf(stderr, "Disconnection established: socket %d\n", soc);
  return 0;
}
