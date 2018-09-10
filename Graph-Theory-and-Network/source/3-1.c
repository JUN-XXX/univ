#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define BUF_SIZE 1024
#define INF 100000

// ダイクストラ法で最短経路を求める関数
// 引数 route: 探索結果を格納する配列
//      w: 重みを格納した隣接行列.
//      size: adjacent, wの1辺あたりの大きさ
//      s, t: 探索開始点, 探索終了点
// 返り値: 最短経路を逆順にした配列を指すポインタ
int* dijkstra(int* route, int** adjacent, float** w, int size, int s, int t){
    float *l = (float*) malloc(sizeof(float)*size);   // 距離ラベル
    int *parent = (int*) malloc(sizeof(int)*size);    // 親ラベル
    // 最短経路確定点集合を表現する配列 1なら含まれている 0なら含まれていない
    int *u = (int*) malloc(sizeof(int)*size);       
    float d;                                          // 距離を格納する一時変数
    int x;                                            // 距離ラベル最小点の記録用変数
    int i, y;                                         // ループ用

    // 初期化
    for(i=0; i<size; ++i){
        l[i] = INF;
        parent[i] = -1;
        u[i] = 0;
    }
    l[s] = 0;

    do{
        // V-Uから距離ラベル最小点xを選択する. 
        // Uに含まれていない, つまりU[i]=0である点を探索している.
        for(i=0, d=INF; i<size; ++i) if((u[i]==0) && (d>=l[i])) {
            d = l[i];
            x = i;
        }

        // x = tなら終了
        if(x == t) break;

        // V-Uに含まれるxの隣接点 yについて親ラベルを更新する.
        for(y=0; y<size; ++y){
            if((u[y]==0) && (adjacent[y][x] != 0)){
                if(l[y] > (l[x]+w[x][y])){
                    l[y] = l[x]+w[x][y];
                    parent[y] = x;  
                } 
            }
        }

        u[x] = 1; //探索済みとする
    } while(1);

    // 終点からparentを辿って最短経路を作成する.
    x = t;
    for(i=0;;++i){
        route[i] = x;
        if(x == s) break;
        x = parent[x];
    
    }

    // メモリの解放
    free(l);
    free(parent);
    free(u);
    return route;
}

int main( int argc, char *argv[] ) {
    int N = 0;                    // 点の数
    int M = 0;                    // 辺の数
    int **adjacent;               // 隣接行列(2次元ポインタ変数)
    float **weight;                   // 次数配列(2次元ポインタ変数)
    int start, finish;            // 始点と終点
    int *route;                   // ルートを格納するポインタ変数

    int i, j;                     // ループ用
    int n1, n2;                   // ファイルから読み込むときの点番号
    float n3;                     // ファイルから読み込むときの重み
    FILE *fp;                     // ファイルから読み込むためのポインタ
    char buf[BUF_SIZE];           // 標準入力/出力用のバッファ
    char shaping[BUF_SIZE];       // 出力時文字列成形用のバッファ
    float sum_dist;               // 最短距離の計算用

    // 実行時引数のチェック
    if ( argc != 2 ) {
        fprintf( stderr, "Usage: %s graph_file\n", argv[0] );
        exit( 1 );
    }

    // 読み込みファイルを開く
    if (( fp = fopen( argv[1], "r" )) == NULL ) {
        fprintf( stderr, "%s not found.\n", argv[1] );
        exit( 1 );
    }

    // ファイルから点の数を読み込む
    fscanf( fp, "%d", &N );

    // N×Nの2次元配列になるように、mallocを実行する
    adjacent = ( int ** )malloc( sizeof( int * ) * N );
    weight =  ( float ** )malloc( sizeof( float * ) * N );
    for ( i=0; i<N; i++ ) {
        adjacent[i] = ( int * )malloc( sizeof( int ) * N );
        weight[i] = ( float * )malloc( sizeof( float ) * N );
        for ( j=0; j<N; j++ ){
            adjacent[i][j] = 0;
            weight[i][j] = 0.0;
        }
    }
    
    // 2行目からは辺の情報があるので、それを繰り返し読み込む
    while( fscanf( fp, "%d %d %f", &n1, &n2 ,&n3) != EOF ) {
        // n1とn2の間に辺がある
        // adjacent[n1][n2] と adjacent[n2][n1] を1増やす
        adjacent[n1][n2]++;
        adjacent[n2][n1]++;
        M++;
        weight[n1][n2] = n3;
        weight[n2][n1] = n3;
    }

    //キーボードから始点と終点を入力
    while(1){
        fprintf(stdout, "探索を開始する点を選択してください: ");
        if(fscanf(stdin, "%s", &buf) == EOF){
            fprintf(stderr, "入力エラーです.\n");
            continue;
        }
        start = atoi(buf);
        if(start >= N || 0 > start || (start==0 && strcmp(buf, "0"))){
            fprintf(stderr, "その点番号は存在しません.\n");
            continue;
        }
        fprintf(stdout, "探索を終了する点を選択してください: ");
        if(fscanf(stdin, "%s", &buf) == EOF){
            fprintf(stderr, "入力エラーです.\n");
            continue;
        }
        finish = atoi(buf);
        if(finish >= N || 0 > finish || (finish==0 && strcmp(buf, "0"))){
            fprintf(stderr, "その点番号は存在しません.\n");
            continue;
        }
        if(start == finish){
            fprintf(stderr, "探索開始点と同じ点を選択しています.\n");
            continue;
        }
        break;
    }

    // ダイクストラ法による探索
    route = malloc(sizeof(int)*N);
    route = dijkstra(route, adjacent, weight, N, start, finish);
    memset(buf, 0, sizeof(buf));
    memset(shaping, 0, sizeof(shaping));

    // 探索に失敗したらエラーメッセージを出力
    // 探索に成功したら経路情報を出力
    if(route == NULL) fprintf(stderr, "探索に失敗しました.\n");
    else for(i=0;;++i){
        sprintf(shaping, "%d", route[i]);
        strcat(shaping, buf);
        strcpy(buf, shaping);
        if (route[i] == start) break;
        sprintf(shaping, " -> ");
        strcat(shaping, buf);
        strcpy(buf, shaping);
    }
    fprintf(stdout, "[最短経路]\n", buf);
    fprintf(stdout, "%s\n", buf);


    sum_dist = 0;
    for(i=0; route[i] != start; ++i) sum_dist += weight[route[i+1]][route[i]];
    fprintf(stdout, "[最短距離]\n", buf);
    fprintf(stdout, "%f\n", sum_dist);


    // メモリの解放
    for(i=0; i<N; i++){
        free(adjacent[i]);
        free(weight[i]);
    }
    free(adjacent);
    free(weight);

    free(route);

    return 0;
}
