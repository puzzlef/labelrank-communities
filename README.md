Effect of adjusting accumulator hashtable capacity of the LabelRank algorithm
for community detection.

[LabelRank] is an algorithm for **detecting communities in graphs**. *Community*
*detection* helps us understand the *natural divisions in a network* in an
**unsupervised manner**. It is used in **e-commerce** for *customer*
*segmentation* and *advertising*, in **communication networks** for *multicast*
*routing* and setting up of *mobile networks*, and in **healthcare** for
*epidemic causality*, setting up *health programmes*, and *fraud detection* is
hospitals. *Community detection* is an **NP-hard** problem, but heuristics exist
to solve it (such as this). *LabelRank* is an **iterative algorithm** that is
based on the concept of *propagation of weighted labels* on a *weighted*
*(directed) network*, where the highest weight label determines the *community*
*membership* of each vertex.

Our implementation of LabelRank **differs** from the original algorithm in that
there is a *fixed upper limit* on the *number of labels per vertex* (**labelset**
**capacity**). Therefore we do not use the *cutoff operator* (which removes
low-weighted labels), but instead trim-off labels if they do not fit within
labelset capacity. *Labels* are **sorted by weight** such that only low-weighted
labels are eliminated.

An **accumulator labelset** is a *hash table* that is used to *combine labels*
from the neighbors of each vertex in the graph. Its **capacity** is always a
**prime number**, and the *hash key* is obtained by simply finding the *modulo*
*of the label id* with the capacity of the accumulator labelset. In order to
accomodate all labels in the worst case, the accumulator labelset is **normally**
**initialized** with a *capacity of all the vertices* in the graph. The problem
with using a **large accumulator labelset** is that it is **not feasible on a**
**GPU**, where we have a large number of *threads*, but a small amount of *working*
*memory* (shared memory). For *high-degree vertices*, this would have to be done
in the **global memory** instead (which is slow). In addition, we would have to
use `atomicCAS()` operations in order to **avoid collisions** which can further
drop performance.

Therefore, here my idea is to look if we can simply **do away** with **large**
**hash tables** and **collision resolution** altogether, by simply *considering*
*identical hash keys* as *identical labels*. This may lead to *bad communities*,
but that is what this experiment is for. If they do yield good communities it
can be a big win in performance. Note that such a scheme is only likely to cause
issues only in the *first few iterations*, when we have a large number of
labels. In addition, as there is potential for multiple labels to be combined,
we will consider the *new label* to be the *hash key*.

In this experiment we adjust the capacity of accumulator labelset from `2` to
`4093` in multiples of 2. This capacity is always set to the highest prime
number below a power of 2.  We choose the LabelRank *parameters* as
`inflation = 1.5` and `conditionalUpdate = 0.5`. In addition, we use a fixed
`labelset capacity` of `4`, and run the algorithm for exactly `10 iterations`.
Note that this labelset capacity is different from the accumulator labelset capacity,
and indicates the maximum number of labels allowed per vertex. We measure the
**time taken** for the *computation* (performed 5 times for averaging), and
*measure* the **modularity score** (one of the ways to measure quality of
communities). This is repeated for *seventeen* different graphs.

From the results, we observe that choosing a **lower accumulator labelset**
**capacity** allows the computation for **complete faster** (likely due to higher
cache hits). However with higher accumulator labelset capacities, the time taken
may increase beyond the time required for a full size accumulator labelset. This
is because of the additional modulus (`%`) operator required with a limited
capacity accumulator labelset (my expectaction is that this would not
significantly affect performance in a GPU). Again, a similar effect is observed
with **modularity** (in the average case), which **increases with increasing**
**accumulator labelset capacity**. It appears that using an **accumulator labelset**
**capacity** of `61 / 127` would **yield a good enough modularity**. In some
cases, using a smaller accumulator labelset capacity yeilds an even better
modularity than full-size labelsets (but these are exception cases i think).
Note that choices might differ if a different *labelset capacity* is used. It
would be interesting to implement this kind of collision-ignoring hash table on
a GPU, and observe its impact on LabelRank as well as the Louvain algorithm for
community detection.

All outputs are saved in a [gist] and a small part of the output is listed here.
Some [charts] are also included below, generated from [sheets]. The input data
used for this experiment is available from the [SuiteSparse Matrix Collection].
This experiment was done with guidance from [Prof. Kishore Kothapalli] and
[Prof. Dip Sankar Banerjee].

<br>

```bash
$ g++ -std=c++17 -O3 main.cxx
$ ./a.out ~/data/web-Stanford.mtx
$ ./a.out ~/data/web-BerkStan.mtx
$ ...

# Loading graph /home/subhajit/data/web-Stanford.mtx ...
# order: 281903 size: 2312497 [directed] {}
# order: 281903 size: 3985272 [directed] {} (symmetricize)
# order: 281903 size: 4267175 [directed] {} (selfLoopAllVertices)
# [0.065665 modularity] noop
# [02887.805 ms; 0.621151 modularity] labelrankSeq
# [00745.747 ms; 0.079785 modularity] labelrankSeq {acc_capacity: 2}
# [01106.795 ms; 0.132295 modularity] labelrankSeq {acc_capacity: 3}
# [02811.777 ms; 0.455311 modularity] labelrankSeq {acc_capacity: 7}
# [02933.439 ms; 0.533203 modularity] labelrankSeq {acc_capacity: 13}
# [03158.569 ms; 0.570127 modularity] labelrankSeq {acc_capacity: 31}
# [03474.475 ms; 0.581564 modularity] labelrankSeq {acc_capacity: 61}
# [03422.630 ms; 0.615646 modularity] labelrankSeq {acc_capacity: 127}
# [03482.629 ms; 0.607741 modularity] labelrankSeq {acc_capacity: 251}
# [03477.951 ms; 0.609800 modularity] labelrankSeq {acc_capacity: 509}
# [03606.549 ms; 0.615743 modularity] labelrankSeq {acc_capacity: 1021}
# [03592.647 ms; 0.616814 modularity] labelrankSeq {acc_capacity: 2039}
# [03593.207 ms; 0.620943 modularity] labelrankSeq {acc_capacity: 4093}
#
# Loading graph /home/subhajit/data/web-BerkStan.mtx ...
# order: 685230 size: 7600595 [directed] {}
# order: 685230 size: 13298940 [directed] {} (symmetricize)
# order: 685230 size: 13984170 [directed] {} (selfLoopAllVertices)
# [0.048410 modularity] noop
# [05051.676 ms; 0.642543 modularity] labelrankSeq
# [01033.231 ms; 0.227399 modularity] labelrankSeq {acc_capacity: 2}
# [01436.724 ms; 0.325241 modularity] labelrankSeq {acc_capacity: 3}
# [05384.903 ms; 0.507040 modularity] labelrankSeq {acc_capacity: 7}
# ...
```

[![](https://i.imgur.com/faBohhP.png)][sheetp]
[![](https://i.imgur.com/7f9oVDw.png)][sheetp]

<br>
<br>


## References

- [LabelRankT: Incremental Community Detection in Dynamic Networks via Label Propagation](https://arxiv.org/abs/1305.2006)
- [LabelRank: A Stabilized Label Propagation Algorithm for Community Detection in Networks](https://arxiv.org/abs/1303.0868)
- [SuiteSparse Matrix Collection]

<br>
<br>

[![](https://i.imgur.com/r9CbaNA.jpg)](https://www.youtube.com/watch?v=4uXWszmV0_Q)<br>


[Prof. Dip Sankar Banerjee]: https://sites.google.com/site/dipsankarban/
[Prof. Kishore Kothapalli]: https://faculty.iiit.ac.in/~kkishore/
[SuiteSparse Matrix Collection]: https://sparse.tamu.edu
[LabelRank]: https://arxiv.org/abs/1303.0868
[gist]: https://gist.github.com/wolfram77/52a8f10d9800a02ed27a34575198ef15
[charts]: https://imgur.com/a/z4xlfy4
[sheets]: https://docs.google.com/spreadsheets/d/16rPs-ERVQqnMoaCqaYizni8cdUwyU9Pf3A-mnovmYsM/edit?usp=sharing
[sheetp]: https://docs.google.com/spreadsheets/d/e/2PACX-1vSwZpgdzMXwPcVAUpTjCdgkrQpp_F-ui0V4dnyz6-WeUcuPoylnfQDwPGlgOYfWQ512hmAwwjneCYbn/pubhtml
