#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CAPACITY_MAX   2147483647
#define M_MAX                 100
#define N_MAX                   8

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* 連結性を確認する関数（深さ優先探索のため）
   返り値: なし
   隣接行列だけでは計算できないので，辺容量とフローも引数に追加する */
void visit( int N, int **adjacent, int **capacity, int **flow, int v,
            int *yet, int *father )
{
  int w;
  int r = v; 
  int *search = (int*) malloc(sizeof(int)*N);
  int *next_search = (int*) malloc(sizeof(int)*N);
  for(w=0; w<N; w++) search[w] = 0; 
  for(w=0; w<N; w++) next_search[w] = 0; 

  yet[v] =  0;
  search[v] = 1; // 現在の階層であるrootのフラグを1にする

  while(1){
    for ( v=0; v<N; v++) {
      // searchフラグの立っているものに対して探索を行う
      if(search[v]==0) continue;
      else search[v] = 0;

      for ( w=0; w<N; w++ ) {
        if ( adjacent[v][w] == 1 && yet[w] == 1 ) {
          if((flow[v][w] < capacity[v][w]) || (flow[w][v] > 0)){
            father[w] = v;
            next_search[w] = 1;
            yet[w] =  0;
          }
        }
      }
    }

    // これ以上探索ができなくなったら終了
    for(w=0; w<N; w++)
      if(next_search[w] == 1) break;
    if(w == N) break;

    // next_searchをsearchにコピー
    for(w=0; w<N; w++){
      search[w] = next_search[w];
      next_search[w] = 0;
    }
  }

  free(next_search);
  free(search);
      /*****/
}


/* s→tフロー増加道を探索する関数
   返り値: 見つかった場合は1，見つからなければ0を返す */
int find_flow_arg_path( int N, int **adjacent, int **capacity, int **flow,
			int *path, int s, int t )
{
  /*
    関数作成のポイント
    (1) adjacent[][]を利用して，深さ（または幅）優先探索を実施．
      ・ フローが増加する辺のみを利用して探索を進める．
      ・ father[]を利用して，深さ（または幅）優先探索木を作成する．
      ・ 親となる点がない場合には，fatherの値を-1としておく．
    (2) father[]を辿って，sからtへの点列をpath[]に格納する．
      ・ fatherはtからsへの方向にのみ辿れることに注意せよ．
      ・ 例えば，フロー増加道が 0 -> 1 -> 8 -> 9 の場合，
         path[0]=0, path[1]=1, path[2]=8, path[3]=9 となる．
	 それ以外の要素には特に何も入れなくて良い．
	 （path[]の終わりは，値がtとなることで確認できるため．）
    (3) フロー増加道が見つからなかった場合には，father[]を辿っている
	際に値が-1となっている．その場合にはpath[]配列作成を中断し，
	return 0とすればよい．
  */
  int i, j, ret, depth;
  int *yet = (int*) malloc(sizeof(int)*N);  
  int *father = (int*) malloc(sizeof(int)*N);
  for(i=0; i<N; i++) yet[i] = 1;
  for(i=0; i<N; i++) father[i] = -1;

  visit(N, adjacent, capacity, flow, s, yet, father);

  depth = 0;
  ret= 1;
  // 探索した要素数を数える
  for(i=t; (father[i]!=-1) && (father[i]!=s); i=father[i], depth++);
  // fatherからpathに直す
  path[depth+1] = t;
  for(i=depth; i>=0; --i){
     path[i] = father[path[i+1]];
     if(path[i] == -1){
       ret = 0;
       break;
     }
  }
  free(father);
  free(yet);
  return ret;
} 
  /*****/


/* フロー増加量を算出する関数
   返り値: フロー増加量 */
int calc_increasable_flow( int N, int **capacity, int **flow,
			   int *path, int t )
{
  /*
    関数作成のポイント
    この関数はそれほど難しくないであろう．
    path[]に格納されたフロー増加道を辿り，フローの可能増加量を算出する．
    なお，path[]の終わりには終端点「t」が入っていることに注意．
  */
  int i;
  int amount = CAPACITY_MAX;

  for(i=0; path[i]!=t; i++){
    // 正方向なら容量と既存フローの差を比べる
    if(capacity[path[i]][path[i+1]] > 0)
      amount = MIN(amount, capacity[path[i]][path[i+1]] - flow[path[i]][path[i+1]]);
    // 逆方向なら既存フローと比べる
    else
      amount = MIN(amount, flow[path[i+1]][path[i]]);
  }
  
  return amount;
  /*****/
}

/* フローを増加させる関数
   返り値: なし */
void increase_flow( int N, int **capacity, int **flow,
		    int *path, int t, int e_min )
{
  /*
    関数作成のポイント
    この関数はそれほど難しくないであろう．
    e_minで指定された分だけ，フロー増加道上のフローを増加させる．
  */
  int i;
  for(i=0; path[i]!=t; i++)
    if(capacity[path[i]][path[i+1]] > 0)
      flow[path[i]][path[i+1]] += e_min;
    else
      flow[path[i+1]][path[i]] -= e_min;
  return;
  /*****/
}


/* フロー増加道を表示する関数
   返り値: なし */
void print_path( int N, int *path, int t )
{
  int n = 0;

  while ( n < N && path[n] != t )
    fprintf( stdout, "%d ", path[n++] );
  fprintf( stdout, "%d ", path[n] );
  fprintf( stdout, "\n" );
}
  


int main( int argc, char *argv[] )
{
  int i, j, N, M = 0, a, b, c;
  int n1, n2, n3;      // ファイルから読み込むときの点番号
  int s, t;        /* 始点と終点 */
  int e_min;       /* フロー増加量を記憶する変数 */
  int **adjacent;  /* 隣接行列 */
  int **capacity;  /* 辺容量 */
  int **flow;      /* 辺のフロー */
  int *path;       /* フロー増加道記憶配列 */
  int F = 0;       /* s→tフロー */
  FILE *fp;

  if ( argc != 4 ) {
    fprintf( stderr, "Usage: %s data s_node t_node\n", argv[0] );
    exit( 1 );
  }

  if ( ( fp = fopen( argv[1], "r" ) ) == NULL ) {
    fprintf( stderr, "File open error.\n" );
    exit( 1 );
  }

  s = atoi( argv[2] );
  t = atoi( argv[3] );


  /* 点の数を読み込む */
  fscanf( fp, "%d", &N );
  /*****/

  /* 指定した始点と終点が存在しない場合はエラー */
  if ( s < 0 || s >= N || t < 0 || t >= N ) {
    fprintf( stderr, "Node should satisfy 0 <= s, t < %d\n", N );
    exit( 1 );
  }
  
  /* 配列を malloc() により動的に確保する */
  path = ( int * )malloc( sizeof( int ) * N );  
  adjacent = ( int ** )malloc( sizeof( int * ) * N );
  capacity = ( int ** )malloc( sizeof( int * ) * N );
  flow = ( int ** )malloc( sizeof( int * ) * N );
  for ( i=0; i<N; i++ ) {
    path[i] = 0;
    adjacent[i] = ( int * )malloc( sizeof( int ) * N );
    capacity[i] = ( int * )malloc( sizeof( int ) * N );
    flow[i] = ( int * )malloc( sizeof( int ) * N );
    for ( j=0; j<N; j++ ){
      adjacent[i][j] = 0;
      capacity[i][j] = 0;
      flow[i][j] = 0;
    }
  }
  /*****/
  
  /* ファイルからデータを読み込む */
  while( fscanf( fp, "%d %d %d", &n1, &n2, &n3) != EOF ) {
    adjacent[n1][n2]++;
    adjacent[n2][n1]++;
    M++;
    capacity[n1][n2] += n3;
  }
  /*****/

  // s→tフロー増加道が見つからなくなるまで，フロー増加を繰り返す */
  while ( find_flow_arg_path( N, adjacent, capacity, flow, path, s, t ) ) {

    /* calc_increasable_flow()を使ってフロー増加量を算出し，
       e_minに代入する */
    e_min = calc_increasable_flow(N, capacity, flow, path, t);
    /*****/

    /* increase_flow()を使ってフロー増加道に沿ってフローを増加させる */
    increase_flow(N, capacity, flow, path, t, e_min);
    /*****/

    /* s→tフローを増加させる */
    F += e_min;
    /*****/

    /* フロー増加道の確認と増加フローの表示（確認用） */
    print_path( N, path, t );
    fprintf( stdout, "e_min = %d\n", e_min );
  }

  /* 最終表示 */
  fprintf( stdout, "Max Flow (%d, %d) is %d\n", s, t, F );


  /* 配列の開放 */
  for ( i=0; i<N; i++ ) {
    free(adjacent[i]);
    free(capacity[i]);
    free(flow[i]);
  }
  free(adjacent);
  free(capacity);
  free(flow);
  free(path);
  /*****/

  return 0;
}
