#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

void sumatoria(vector<int> & resultado, vector<int> inicial, int nt, int nth){
  int n = inicial.size();
  int *res = resultado.data();
  int *ini = inicial.data();

  #pragma omp target enter data map(to: ini[:n], res[:n])
  #pragma omp target teams distribute parallel for num_teams(nt) thread_limit(nth)
  for(int i = 0; i < n; i++){
    res[i] = ini[i] + 1;
  }
  #pragma omp target exit data map(delete: ini[:n]) map(from: res[:n])

}

int main(){

  vector<int> v1(4, 1);
  vector<int> v2 = v1;

  sumatoria(v2, v1, 1, 4);
  
  cout << "El resultado contiene:" << endl;
  for(int i = 0; i < v2.size(); i++){
    cout << v2[i] << " ";
  }
}