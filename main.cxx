#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include "src/main.hxx"

using namespace std;




template <size_t N, class G>
void performLabelrank(const G& x, float M, int repeat) {
  using K = typename G::key_type;
  int iterations  = 10;
  float inflation = 1.5f;
  float conditionalUpdate = 0.5f;
  LabelrankResult<K> a = labelrankSeq<N>(x, {repeat, iterations, inflation, conditionalUpdate});
  auto fc = [&](auto u) { return a.membership[u]; };
  auto Q  = modularity(x, fc, M, 1.0f);
  printf("[%09.3f ms; %01.6f modularity] labelrankSeq {lset_capacity: %zu}\n", a.time, Q, N);
}

template <class G>
void adjustLabelsetCapacity(const G& x, int repeat) {
  auto M = edgeWeight(x)/2;
  auto Q = modularity(x, M, 1.0f);
  printf("[%01.6f modularity] noop\n", Q);
  performLabelrank<1>(x, M, repeat);
  performLabelrank<2>(x, M, repeat);
  performLabelrank<4>(x, M, repeat);
  performLabelrank<8>(x, M, repeat);
  performLabelrank<16>(x, M, repeat);
}


int main(int argc, char **argv) {
  using K = int;
  using V = float;
  char *file = argv[1];
  int repeat = argc>2? stoi(argv[2]) : 5;
  OutDiGraph<K, None, V> x; V w = 1;
  printf("Loading graph %s ...\n", file);
  readMtxW(x, file); println(x);
  auto y  = symmetricize(x); print(y); printf(" (symmetricize)\n");
  auto fl = [](auto u) { return true; };
  selfLoopU(y, w, fl); print(y); printf(" (selfLoopAllVertices)\n");
  adjustLabelsetCapacity(y, repeat);
  printf("\n");
  return 0;
}
