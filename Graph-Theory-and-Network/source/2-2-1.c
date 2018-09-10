#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// オイラーグラフであるかを求める.
// オイラーグラフなら1, そうでないなら0を返す.
int is_euler(int **adjacent, int size){
    int i=0,  j=0; // ループ用
    int sides = 0;
    for(i=0; i<size; i++){
        sides = 0;
        for(j=0; j<size; j++) sides += adjacent[i][j];
        if(sides%2 != 0) return 0;
    }
    return 1;
}

int main( int argc, char *argv[] ) {
    int i, j;                     // ループ用
    int n1, n2;                   // ファイルから読み込むときの点番号
    int N = 0;                    // 点の数
    int M = 0;                    // 辺の数

    int **adjacent;               // 隣接行列(2次元ポインタ変数)

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
    if(is_euler(adjacent, N))
        fprintf(stdout, "これはオイラーグラフです.\n");
    else
        fprintf(stdout, "これはオイラーグラフではありません.\n");
    
    return 0;
}