#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include "src/main.hxx"

using namespace std;


// The problem with using a large accumulator labelset is that it is not
// feasible on a GPU, where we have a large number of threads, but a small
// amount of working memory (shared memory). To alleviate this, we may use
// a hash table instead. To guarentee that we have the space to store labels
// (in the worst case, where most labels among the neighbors are unique), we
// must reserve enough space for all the labels, asusming they are all unique.
// For high-degree vertices, this would have to be done in the global memory
// instead (which is slow). In addition, we would have to use atomicCAS()
// operations in order to avoid collisions which can further drop performance.
// Therefore, here my idea is to look if we can simply do away with large
// hash tables and collision resolution altogether, by simply considering
// identical hash keys as identical labels. This may lead to bad communities,
// but that is what this experiment is for. If they do yield good communities
// it can be a big win in performance. Note that such a scheme is only likely
// to cause issues only in the first few iterations, when we have a large
// number of labels. In addition, as there is potential for multiple labels to
// be combined, we will consider the new label to be the hash key. We can
// adjust the hash function or the capacity of the hash table to observe the
// impact on modularity, which could an indicator of the amount of (ignored)
// label collisions that might have occured behind the scenes.

// Prime numbers just below 2, 3, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096.
const vector<int> PRIMES = {2, 3, 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093};


template <class G>
void adjustOptions(const G& x, int repeat) {
  using K = typename G::key_type;
  int iterations  = 10;
  float inflation = 1.5f;
  float conditionalUpdate = 0.5f;
  auto M = edgeWeight(x)/2;
  auto Q = modularity(x, M, 1.0f);
  printf("[%01.6f modularity] noop\n", Q);
  do {
    size_t accumulatorCapacity = x.span();
    LabelrankResult<K> a = labelrankSeq<4>(x, {repeat, iterations, accumulatorCapacity, inflation, conditionalUpdate});
    auto fc = [&](auto u) { return a.membership[u]; };
    auto Q  = modularity(x, fc, M, 1.0f);
    printf("[%09.3f ms; %01.6f modularity] labelrankSeq\n", a.time, Q);
  } while (0);
  for (size_t accumulatorCapacity : PRIMES) {
    LabelrankResult<K> a = labelrankSeq<4>(x, {repeat, iterations, accumulatorCapacity, inflation, conditionalUpdate});
    auto fc = [&](auto u) { return a.membership[u]; };
    auto Q  = modularity(x, fc, M, 1.0f);
    printf("[%09.3f ms; %01.6f modularity] labelrankSeq {acc_capacity: %d}\n", a.time, Q, accumulatorCapacity);
  }
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
  adjustOptions(y, repeat);
  printf("\n");
  return 0;
}
