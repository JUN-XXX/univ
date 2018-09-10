#include <stdio.h>
#include <stdlib.h>

/*
// Windows上ではsrand48, drand48が動作しなかったため記述
void srand48(int seed) {
	srand(seed);
}

double drand48() {
	return ((double)(rand()) / RAND_MAX);
}
*/

int main( int argc, char *argv[] ) {
  int i, j;                     // ループ用
  int n1, n2;                   // ファイルから読み込むときの点番号
  int N = 0;                    // 点の数
  int M = 0;                    // 辺の数

  int **adjacent;               // 隣接行列(2次元ポインタ変数)
  int *degree;                  // 次数配列(1次元ポインタ変数)
  float ave_degree = 0;         // 次数の平均
  double ave_move = 0;

  FILE *fp;                     // ファイルから読み込むためのポインタ

  srand48(4);                   // randomのseed値を設定  

  // ランダムに移動をする関数
  // 引数は順に隣接行列, グラフの点の数, スタートの点, ゴールの点, 結果を標準出力にはき出すか(boolean)
  // 返り値は移動した回数.
  int random_walk( int **adjacent, int N, int start, int end, int dose_print ){
    int pos = start;
    int next = 0;
    int count = 0;

    if( dose_print ) 
      fprintf( stdout, "%d", pos );    
    while( 1 ) {
      count += 1;
      do { 
        next = (int) ( drand48() * N );
      } while( !adjacent[pos][next] );

      if( dose_print ) 
        fprintf( stdout, " -> %d", next );
      if ( next == end ) break;
      pos = next;
    }
    if( dose_print ) 
      fprintf( stdout, "\n" );    
    return count; 
  }

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

  // 10000回実行した平均を計算
  for( i=0; i<10000; i++ )
    ave_move += random_walk(adjacent, N, 0, N-1, 0);
  ave_move /= 10000;

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
  fprintf( stdout, "%g\n", ave_degree );
  fprintf( stdout, "\n" );

  // ランダムに移動した経路を表示
  random_walk(adjacent, N, 0, N-1, 1);
  fprintf( stdout, "\n" );

  // 10000回実行した平均移動回数を表示
  fprintf( stdout, "[Average moving count]\n" );  
  fprintf( stdout, "%lf\n", ave_move );

  // ファイルを閉じる
  fclose( fp );

  // 確保したメモリを解放する
  for ( i=0; i<N; i++ )
    free( adjacent[i] );
  free( adjacent );

  return 0;
}
