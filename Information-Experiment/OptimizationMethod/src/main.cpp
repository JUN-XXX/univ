#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric> 
#include <iostream>
# include <random>
#include <chrono>

#define INF 0x7fffffff // for type of 32bit-int
// long型でも使用

using namespace std;

using dot_t = int;
using costarray_t = vector< vector<dot_t> >;
using routearray_t = vector<dot_t>;

routearray_t complete_enumration(const costarray_t cost_array, int first){
    const int SIZE = cost_array.size(); // cost_arrayの1辺の長さ
    long cost;                                      // コストを格納する変数
    long min_cost = INF;                            // 最小コストを格納する変数
    routearray_t route(SIZE);                   // ルートを格納する変数
    routearray_t min_route(SIZE);    // 最小コストになるルートを格納する変数
    
    // route に 0, 1, ... を設定し, first_pointを探索行列から外す 
    iota(route.begin(), route.end(), 0);
    route.erase(remove(route.begin(), route.end(), first), route.end()); 
    min_route[0] = first;

    do{ 
        // 現在の探索順でコストを計算する
        cost = cost_array[first][route[0]];
        for(int i=0; i<SIZE-1; ++i){
            cost += cost_array[route[i]][route[i+1]];
        }
        // 最小コストを更新したら, そのコストとルートを記憶する
        if(min_cost > cost){
            min_cost = cost;
            copy(route.begin(), route.end(), min_route.begin()+1);
        }
    // ルートを更新する
    } while( next_permutation(route.begin(), route.end()) ); 
    return min_route;
}

routearray_t nearest_addition(const costarray_t cost_array, int first){
    const int SIZE = cost_array.size();             // cost_arrayの1辺の長さ
    routearray_t route;                   // ルートを格納する変数
    routearray_t unsearched(SIZE);

    // routeにfirstを登録
    route.push_back(first);

    // unsearched に 0, 1, ... を設定し, firstを探索行列から外す 
    iota(unsearched.begin(), unsearched.end(), 0);
    unsearched.erase(remove(unsearched.begin(), unsearched.end(), first), 
                            unsearched.end()); 
    

    int goal;
    int relay;
    while(route.size()<SIZE){
        int cost = INF;
        for(auto r: route) for(auto u: unsearched) if(cost > cost_array[r][u]){
            cost = cost_array[r][u];
            goal = r;
            relay = u;
        }
        route.insert(find(route.begin(), route.end(), goal), relay);
        unsearched.erase(remove(unsearched.begin(), unsearched.end(), relay), 
                                unsearched.end());    
    }

    return route;
}

/*
routearray_t greedy(costarray_t cost_array, int first){
    return min_route;
}*/

routearray_t nearest_neighbor(const costarray_t cost_array, dot_t first){
    const int SIZE = cost_array.size();
    routearray_t route;
    routearray_t unsearched(SIZE);

    route.push_back(first);
    iota(unsearched.begin(), unsearched.end(), 0);
    unsearched.erase(remove(unsearched.begin(), unsearched.end(), first), 
                            unsearched.end()); 
    
    dot_t now = first;
    while(route.size()<SIZE){
        long cost = INF;
        dot_t next = -1;
        for(auto u: unsearched) if(cost >= cost_array[now][u]){
            cost = cost_array[now][u];
            next = u;
        }
        route.push_back(next);
        unsearched.erase(remove(unsearched.begin(), unsearched.end(), next), 
                                unsearched.end()); 
        now = next;
    }

    return route;
}

costarray_t make_random_costarray(const int size){
    costarray_t costarray = vector<vector<int>>(size, vector<int>(size, 0));
    random_device rnd;
    mt19937 mt(rnd());
    uniform_int_distribution<int> rand_cost(1,1000000);
    int r;
    for(int i=0; i<size; ++i) for(int j=i; j<size; j++){
        r = rand_cost(mt);
        costarray[i][j] = r;
        costarray[j][i] = r;
    }
    return costarray;
}

void show(routearray_t route){    
    cout << "route: " ;
    for(auto x : route) cout << x ;
    cout << "\n";
}

int main(){
    int start = 0;
    vector<int> citys_size;
    vector<int> cycle;

    // 都市数をそれぞれcycle数だけ回し、計算にかかった時間を出力
    if(true){
        citys_size = {4, 6, 8, 10};
        cycle      = {1000000, 100000, 10000, 100};
        for(int i=0; i<citys_size.size(); ++i){
            costarray_t costarray = make_random_costarray(citys_size[i]);
            auto start_time = chrono::system_clock::now(); 
            for(int c=0; c<cycle[i]; c++) routearray_t ce = complete_enumration(costarray, start);
            auto end_time = chrono::system_clock::now(); 
            cout << chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count() << ", "; 
        }
        cout << endl;

        citys_size = {4, 6, 8, 10, 16, 32, 64, 128, 256};
        cycle      = {1000000, 1000000, 1000000, 100000, 100000, 10000, 10000, 1000, 100};
        for(int i=0; i<citys_size.size(); ++i){
            costarray_t costarray = make_random_costarray(citys_size[i]);
            auto start_time = chrono::system_clock::now(); 
            for(int c=0; c<cycle[i]; c++) routearray_t na = nearest_addition(costarray, start);
            auto end_time = chrono::system_clock::now(); 
            cout << chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count() << ", "; 
        }
        cout << endl;

        citys_size = {4, 6, 8, 10, 16, 32, 64, 128, 256};
        cycle      = {1000000, 1000000, 1000000, 1000000, 100000, 100000, 100000, 10000, 10000};
        for(int i=0; i<citys_size.size(); ++i){
            costarray_t costarray = make_random_costarray(citys_size[i]);
            auto start_time = chrono::system_clock::now(); 
            for(int c=0; c<cycle[i]; c++) routearray_t nn = nearest_neighbor(costarray, start);
            auto end_time = chrono::system_clock::now(); 
            cout << chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count() << ", "; 
        }
        cout << endl;
    }

    // 都市数が8のときの、経路の平均コストを算出する
	// (不具合あり)
    if(false){
        long ce_sum=0, na_sum=0, nn_sum=0;
        long ce_ave=0, na_ave=0, nn_ave=0;
        for(int c=0; c<100; c++) {
            costarray_t costarray = make_random_costarray(8);
            routearray_t ce = complete_enumration(costarray, start);
            routearray_t na = nearest_addition(costarray, start);
            routearray_t nn = nearest_neighbor(costarray, start);
            ce_sum = costarray[ce[0]][ce[ce.size()-1]];
            na_sum = costarray[na[0]][na[na.size()-1]];
            nn_sum = costarray[nn[0]][nn[nn.size()-1]];
            for(int i=0; i<7; ++i) ce_sum += costarray[ce[i]][ce[i+1]]; 
            for(int i=0; i<7; ++i) na_sum += costarray[na[i]][na[i+1]]; 
            for(int i=0; i<7; ++i) nn_sum += costarray[nn[i]][nn[i+1]]; 
            ce_ave += ce_sum;
            na_ave += na_sum;
            nn_ave += nn_sum;
        }
        cout << "citys: " << 8 << " " << ce_ave << " " << na_ave << " " << nn_ave << endl;
    }

    return 0;
}