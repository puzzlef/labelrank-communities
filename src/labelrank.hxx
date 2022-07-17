#pragma once




// LABELRANK-OPTIONS
// -----------------

struct LabelrankOptions {
  int    repeat;
  int    maxIterations;
  size_t accumulatorCapacity;
  float  inflation;
  float  conditionalUpdate;

  LabelrankOptions(int repeat=1, int maxIterations=500, size_t accumulatorCapacity=13, float inflation=1.2f, float conditionalUpdate=0.3f) :
  repeat(repeat), maxIterations(maxIterations), accumulatorCapacity(accumulatorCapacity), inflation(inflation), conditionalUpdate(conditionalUpdate) {}
};




// LABELRANK-RESULT
// -----------------

template <class K>
struct LabelrankResult {
  vector<K> membership;
  int   iterations;
  float time;

  LabelrankResult(vector<K>&& membership, int iterations=0, float time=0) :
  membership(membership), iterations(iterations), time(time) {}

  LabelrankResult(vector<K>& membership, int iterations=0, float time=0) :
  membership(move(membership)), iterations(iterations), time(time) {}
};
