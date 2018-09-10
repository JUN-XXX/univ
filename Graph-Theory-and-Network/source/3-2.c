#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define BUF_SIZE 1024
#define INF 100000

// プリム法で最小探索木を求める関数
// 引数 route: 探索結果を格納する配列
//      dist: 重みを格納した隣接行列.
//      size: adjacent, wの1辺あたりの大きさ
//      s: 探索開始点
int* prim(int** route, int** adjacent, float** dist, int size, int s){
    // 最短経路確定点集合を表現する配列 1なら含まれている 0なら含まれていない
    // つまり, 1ならUに含まれ, 0ならV-Uに含まれている.
    int *u;
    int **t;     // 隣接行列.探索した最短経路を保存する.
    float d;     // 最小重みを求める際に重みを保存する一時変数
    int v1, v2;  // 最小重みを求める際に両辺の点を保存する変数 eの代わりに保存
    int i, j;    // ループ用

    // メモリ確保
    u = (int*) malloc(sizeof(int)*size);
    
    // 初期化
    memset(u, 0, sizeof(u));
    u[s] = 1;
    t = route;
    
    while(1){
        // V=Uであるかの検査
        for(i=0; i<size; ++i) if(!u[i]) break;
        if(i>=size) break; 

        // カットセットC(U, V=U)に含まれる最小重みの辺e(実際に保存しているのはv1,v2)を求める.
        d=INF; v1=-1; v2=-1;
        for(i=0; i<size; ++i){
            if(!u[i]) continue;
            for(j=0; j<size; ++j){
                if(u[j]) continue;
                if(!adjacent[i][j]) continue;
                if(d >= dist[i][j]){
                    d = dist[i][j];
                    v1 = i;
                    v2 = j;
                }
            }
        }

        // tにe(保存していたv1とv2)を追加
        t[v1][v2] = t[v2][v1] = 1;

        if(!u[v1]) u[v1] = 1;
        if(!u[v2]) u[v2] = 1;
    }

    // メモリ解放
    free(u);
}

int main( int argc, char *argv[] ) {
    int N = 0;                    // 点の数
    int M = 0;                    // 辺の数
    int **adjacent;               // 隣接行列(2次元ポインタ変数)
    float **dist;                 // 次数配列(2次元ポインタ変数)
    int start;                    // 始点
    int **route;                  // ルートを格納するポインタ変数

    int i, j;                     // ループ用
    int n1, n2;                   // ファイルから読み込むときの点番号
    float n3;                     // ファイルから読み込むときの重み
    FILE *fp;                     // ファイルから読み込むためのポインタ
    char buf[BUF_SIZE];           // 標準入力から読み込みをするときのバッファ

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
    dist =  ( float ** )malloc( sizeof( float * ) * N );
    for ( i=0; i<N; i++ ) {
        adjacent[i] = ( int * )malloc( sizeof( int ) * N );
        dist[i] = ( float * )malloc( sizeof( float ) * N );
        for ( j=0; j<N; j++ ){
            adjacent[i][j] = 0;
            dist[i][j] = 0.0;
        }
    }
    
    // 2行目からは辺の情報があるので、それを繰り返し読み込む
    while( fscanf( fp, "%d %d %f", &n1, &n2 ,&n3) != EOF ) {
        // n1とn2の間に辺がある
        // adjacent[n1][n2] と adjacent[n2][n1] を1増やす
        adjacent[n1][n2]++;
        adjacent[n2][n1]++;
        M++;
        dist[n1][n2] = n3;
        dist[n2][n1] = n3;
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
        break;
    }

    // プリム法による探索
    route = (int**)malloc(sizeof(int*)*N);
    for(i=0; i<N; ++i) route[i] = (int*)malloc(sizeof(int)*N);
    prim(route, adjacent, dist, N, start);

    if(route == NULL) fprintf(stderr, "探索に失敗しました.\n");
    else {
        fprintf(stdout, "[最少全域木に含まれる辺情報]\n");
        for(i=0; i<N; ++i) for(j=i+1; j<N; ++j){
           if(route[i][j]) fprintf(stdout, "(%d, %d) ", i, j);
       } 
    }
    fprintf(stdout, "\n");

    // メモリの解放
    for(i=0; i<N; i++){
        free(adjacent[i]);
        free(dist[i]);
        free(route[i]);
    }
    free(adjacent);
    free(dist);
    free(route);


    return 0;
}
