#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void visit( int v, int *yet, int N, int **adjacent ){
    int w;
    yet[v] = 0;
    for ( w=0; w<N; w++ )
        if ( adjacent[v][w] == 1 && yet[w] == 1 )
            visit( w, yet, N, adjacent );
}

int connect_check( int N, int **adjacent ){
    int i, *YetToVisit, count = 0;
    YetToVisit = ( int * )malloc( sizeof( int ) * N );
    for ( i=0; i<N; i++ )
    YetToVisit[i] = 1;
    for ( i=0; i<N; i++ )
        if ( YetToVisit[i] == 1 ) {
            count++;
            visit( i, YetToVisit, N, adjacent );
    }
    free( YetToVisit );
    return ( count );
}

// オイラーグラフであるかを求める.
// オイラーグラフなら1, そうでないなら0を返す.
int is_euler(int **adjacent, int size){
    int i=0,  j=0; // ループ用
    int sides = 0; // 次数
    
    // 次数を数えるループ
    for(i=0; i<size; i++){
        sides = 0;
        for(j=0; j<size; j++) sides += adjacent[i][j];
        if(sides%2 != 0) return 0;
    }
    return 1;
}

// 経路探索をする関数. 
// size: 隣接行列の1辺辺りの長さ road: 隣接行列
// count: 歩いた回数 worked: 歩いた道順
int* search(int size, int **road, int count, int *worked, int start){
    int i, j;
    int bridge = -1;
    int* res = NULL;

    for(i=0; i<size; ++i){
        //start->iへの道が存在する場合
        if(road[start][i] > 0){
            road[start][i]--;
            road[i][start]--;
            
            // 橋なら保留する
            if(connect_check(size, road)>1){ 
                bridge = i;
                road[start][i]++;
                road[i][start]++;
                continue;
            }
            
            // 進んで探索する. 探索先で探索が失敗した場合, 続行する
            worked[count] = i;
            if((res=search(size, road, count+1, worked, i)) != NULL) return res; 
            else{
                road[start][i]++;
                road[i][start]++;
            }
        }
    }
    
    // 保留していた橋への探索
    if(bridge>-1){
        road[start][bridge]--;
        road[bridge][start]--;
        worked[count] = bridge;
        if((res=search(size, road, count+1, worked, bridge)) != NULL) return res; 
        else{
            road[start][bridge]++;
            road[bridge][start]++;
        }
    }
    
    // 現在地から行き先が無くなった場合, 全ての道を通っていた場合はその道順を返す.
    // まだ道が残っている場合, 探索失敗としてNULLを返す.
    for(i=0; i<size; ++i) for(j=0; j<size; ++j) if (road[i][j] != 0) return NULL;
    return worked;
}

// オイラー閉路の探索(フラーリーのアルゴリズム)
// 探索した順に数値が入った配列を返す.
// 探索に失敗した場合は返り値にNULLが入る.
int* fleury(int size, int sides,int **adjacent, int start){
    int n, i, j;
    int **road = NULL;
    int *worked = NULL;
    int *res = NULL;

    road = (int**) malloc(sizeof(int*)*size);
    for(i=0; i<size; ++i) road[i] = (int*) malloc(sizeof(int)*size);
    worked = (int*) malloc(sizeof(int)*sides);

    if(road == NULL) exit(1);
    if(worked == NULL) exit(1);

    if(!is_euler(adjacent, size)) return NULL;
    if(size <= start) return NULL;

    for(i=0; i<size; ++i) for(j=0; j<size; ++j) road[i][j] = adjacent[i][j];    
    for(i=sizeof(worked)-1; i>=0; --i) worked[i] = -1;

    res = search(size, road, 0, worked, start);

    return res;
} 

int main( int argc, char *argv[] ) {
    int i, j;                     // ループ用
    int n1, n2;                   // ファイルから読み込むときの点番号
    int N = 0;                    // 点の数
    int M = 0;                    // 辺の数

    const int START = 0;          // 探索を開始する場所

    int **adjacent = NULL;        // 隣接行列(2次元ポインタ変数)
    int *euler_circuit = NULL;    // オイラー閉路の配列を格納するためのポインタ

    FILE *fp;                     // ファイルから読み込むためのポインタ


    // 実行時引数のチェック
    if ( argc != 2 ) {
    fprintf( stderr, "Usage: %s graph_file\element_num", argv[0] );
    exit( 1 );
    }

    // 読み込みファイルを開く
    if (( fp = fopen( argv[1], "r" )) == NULL ) {
    fprintf( stderr, "%s not found.\element_num", argv[1] );
    exit( 1 );
    }

    // ファイルから点の数を読み込む
    fscanf( fp, "%d", &N );

    // N×Nの2次元配列になるように、mallocを実行する
    adjacent = ( int ** )malloc( sizeof( int * ) * N );
    for ( i=0; i<N; i++ ) {
    adjacent[i] = ( int * )malloc( sizeof( int ) * N );
    for ( j=0; j<N; j++ )
        adjacent[i][j] = 0;
    }

    // 2行目からは辺の情報があるので、それを繰り返し読み込む
    while( fscanf( fp, "%d %d", &n1, &n2 ) != EOF ) {
       // n1とn2の間に辺がある
        // adjacent[n1][n2] と adjacent[n2][n1] を1増やす
        adjacent[n1][n2]++;
        adjacent[n2][n1]++;
        M++;
    }

    // オイラーグラフであるかを表示
    if(is_euler(adjacent, N)){
        fprintf(stdout, "これはオイラーグラフです.\n");
        euler_circuit = fleury(N, M, adjacent, START);
        if(euler_circuit != NULL){
            fprintf(stdout, "\n[オイラー閉路]\n%d", START);
            for(i=0; i<M; ++i) fprintf(stdout, "->%d", euler_circuit[i]);
            fprintf(stdout, "\n");
        }
    } else{
        fprintf(stdout, "これはオイラーグラフではありません.\n");
    }
    
    // 使用したメモリの解放
    if(euler_circuit != NULL) free(euler_circuit);
    for ( i=0; i<N; i++ ) free(adjacent[i]);
    free(adjacent);

    return 0;
}