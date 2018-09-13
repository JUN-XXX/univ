// title         : 情報実験 9回 課題2 サーバプログラム


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
  char dict_buf[256];
  FILE* dict;

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
    
    // クライアントから文字列が送信されたら, 辞書を検索して送り返す
    while((ret = read(fd2, buf, 1024)) > 0){
      // 送られてきた文字列の終端に改行文字があるなら削除
      if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
      
      if((dict=fopen("dic-w.txt", "r")) == NULL){
        fprintf(stderr, "Can't open the dictionary file.\n");
	      continue;
      }
      
      fprintf(stderr, "read: %s\n", buf);
      
      while(1){
        if(fgets(dict_buf, 256, dict) != NULL){
          // 該当文字が辞書にあったら対応する日本語をバッファに書き込む
          if(strcmp(strtok(dict_buf, " ") , buf) == 0){
            strcpy(buf, strtok(NULL, " "));
            break;
          }
        } else{
          // 辞書に無かったら無かった旨をバッファに書き込む
          strcpy(buf, "一致しない");
          break;
        }
      }
      fclose(dict);

      // 辞書から読み込んだ日本語に改行文字があるなら取り除く
      if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
      fprintf(stderr, "write: %s\n", buf);
      
      // クライアントに送信する
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
