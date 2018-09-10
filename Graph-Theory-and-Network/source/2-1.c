#include <stdio.h>
#include <stdlib.h>
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

// グラフの最小次数を求める.
int minimum_dimension(int **adjacent, int size){
    int i=0,  j=0; // ループ用
    int sigma = size;
    int sum = 0;
    for(i=0; i<size; i++){
        sum = 0;
        for(j=0; j<size; j++) sum += adjacent[i][j];
        if(sigma > sum) sigma=sum;
    }
    return sigma;
}

int element_num(int val, int size){
    int i = 0; // ループ用
    int e = 0; // 要素数

    if(val == 0) return 0;
    for(i=0; i<size; i++){
        e += (int)((val%(int)pow(2, i+1))/pow(2, i));
    }
    return e;
}

int point_connectivity(int **adjacent, int size){
    int x = 1;
    int sigma = minimum_dimension(adjacent, size);

    int i, j, k; // ループ用
    int num;
    int *calams = NULL; // 部分集合に該当する配列番号を保存する配列
    int **adjacent_prime = NULL; // 部分集合配列

    if(sigma == 1) return x;

    do{
        // 生成した数字の要素数がxならば
        for(num=0; num<size*size; num++){
            if(element_num(num, size) == x){
                
                calams = (int *) malloc(sizeof(int)*x);

                adjacent_prime = ( int ** )malloc( sizeof( int * ) * x );
                for ( i=0; i<x; i++ ) adjacent_prime[i] = ( int * )malloc( sizeof( int ) * x );
                

                // 部分集合に使用する元の配列の番号を抜粋
                for(i=0, k=0; i<size; i++){
                    if((int)((num%(int)pow(2, i+1))/pow(2, i))) calams[k++] = i;
                }
                
                // 部分集合の配列を作成
                for(i=0; i<x; i++)
                    for(j=0; j<x; j++)
                        adjacent_prime[i][j] = adjacent[calams[i]][calams[j]];

                //分離集合ならbreak
                if(connect_check(x, adjacent_prime) > 1) break;
                
                free(calams);
                for(i=0; i<x; i++) free(adjacent_prime[i]);
                free(adjacent_prime);
            }
        }
        x++;
        if(x == sigma) break;
    }while(1);

    return x;

}

int main( int argc, char *argv[] ) {
    int i, j;                     // ループ用
    int n1, n2;                   // ファイルから読み込むときの点番号
    int N = 0;                    // 点の数
    int M = 0;                    // 辺の数
    int pc;                       // point_connectivityの結果を保存

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

    pc = point_connectivity(adjacent, N);
    fprintf(stdout, "点連結度は%dです.\n", pc);
    if(pc == minimum_dimension(adjacent, N))
        fprintf(stdout, "点連結度は最小次数と一致します.\n");
    return 0;
}