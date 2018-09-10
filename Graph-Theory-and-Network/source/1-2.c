#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
  int i, j;                     // ループ用
  int n1, n2;                   // ファイルから読み込むときの点番号
  int N = 0;                    // 点の数
  int M = 0;                    // 辺の数

  int **adjacent;               // 隣接行列(2次元ポインタ変数)
  int *degree;                  // 次数配列(1次元ポインタ変数)
  float ave_degree = 0;         // 次数の平均

  FILE *fp;                     // ファイルから読み込むためのポインタ

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
  for ( i=0; i<N; i++ ) {
    adjacent[i] = ( int * )malloc( sizeof( int ) * N );
    for ( j=0; j<N; j++ )
      adjacent[i][j] = 0;
  }
  // 配列を確保するため、mallocを実行する
  degree = (int *)malloc( sizeof( int ) * N);
  for ( i=0; i<N; i++)
    degree[i] = 0;

  // 2行目からは辺の情報があるので、それを繰り返し読み込む
  while( fscanf( fp, "%d %d", &n1, &n2 ) != EOF ) {
    // n1とn2の間に辺がある
    // adjacent[n1][n2] と adjacent[n2][n1] を1増やす
    adjacent[n1][n2]++;
    adjacent[n2][n1]++;
    M++;
  }

  // 次数の計算
  for ( i=0; i<N; i++ )
      for ( j=0; j<N; j++ )
        degree[i] += adjacent[i][j];

  // 次数の平均の計算
  for ( i=0; i<N; i++ )
    ave_degree += degree[i];
  ave_degree /= N;

  // グラフの表示
  fprintf( stdout, "[Adjacent Matrix]\n" );
  for ( i=0; i<N; i++ ) {
    for ( j=0; j<N; j++ )
      fprintf( stdout, "%d ", adjacent[i][j] );
    fprintf( stdout, "\n" );
  }
  fprintf( stdout, "\n" );
  
  // 次数の表示
  fprintf( stdout, "[Degree]\n" );
  for ( i=0; i<N; i++ )
    fprintf( stdout, "%d\n", degree[i] );
  fprintf( stdout, "\n" );

  // 次数の平均の表示
  fprintf( stdout, "[Average degree]\n" );
  fprintf( stdout, "%3f\n", ave_degree );
  fprintf( stdout, "\n" );
  

  // ファイルを閉じる
  fclose( fp );

  // 確保したメモリを解放する
  for ( i=0; i<N; i++ )
    free( adjacent[i] );
  free( adjacent );

  return 0;
}
