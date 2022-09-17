Comparing the effect of adjustment of parameter of the LabelRank algorithm for
community detection.

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

In this experiment we adjust the LabelRank parameters `inflation` from `1.1 -
2.0` and `conditionalUpdate` from `0.1 - 1.0`, in order the obtain parameter
values that are suitable in general, or suitable to a specific class of graphs.
We use a fixed `labelset capacity` of `4`, and run the algorithm for exactly `10
iterations`. We measure the **time** **taken** for the *computation* (performed
5 times for averaging), and *measure* the **modularity score** (one of the ways
to measure quality of communities). This is repeated for *seventeen* different
graphs.

From the results, we observe that using `inflation = 1.1 / 1.5` along with
`conditionalUpdate = 1.0 / 0.7-0.9` seems to give the **highest possible values
of modularity**. Time taken for completion of *10 iterations* is higher for
`inflation = 1.1, conditionalUpdate = 1.0` than with
`inflation = 1.5 / conditionalUpdate = 0.7-0.9`. Looking specifically at
**web graphs**, the best choice of parameters seems to be
`inflation = 1.1, conditionalUpdate = 1.0`. For **social/collaboration networks**,
a choice of `inflation = 1.3-1.5, conditionalUpdate = 1.0` would be suitable.
For **road networks**, selecting `inflation = 1.5, conditionalUpdate = 0.7-0.9`
looks to be the best. Note that these paramter choices might differ if a
different *labelset capacity* is used.

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
# [03741.334 ms; 0.390458 modularity] labelrankSeq {inflation: 2.0, cond_update: 1.0}
# [04401.621 ms; 0.368602 modularity] labelrankSeq {inflation: 1.9, cond_update: 1.0}
# [04335.695 ms; 0.321369 modularity] labelrankSeq {inflation: 1.8, cond_update: 1.0}
# ...
# [02264.883 ms; 0.403640 modularity] labelrankSeq {inflation: 1.3, cond_update: 0.1}
# [02231.660 ms; 0.416514 modularity] labelrankSeq {inflation: 1.2, cond_update: 0.1}
# [02208.212 ms; 0.441764 modularity] labelrankSeq {inflation: 1.1, cond_update: 0.1}
#
# Loading graph /home/subhajit/data/web-BerkStan.mtx ...
# order: 685230 size: 7600595 [directed] {}
# order: 685230 size: 13298940 [directed] {} (symmetricize)
# order: 685230 size: 13984170 [directed] {} (selfLoopAllVertices)
# [0.048410 modularity] noop
# [08309.834 ms; 0.410866 modularity] labelrankSeq {inflation: 2.0, cond_update: 1.0}
# [10264.402 ms; 0.441517 modularity] labelrankSeq {inflation: 1.9, cond_update: 1.0}
# [09834.699 ms; 0.332533 modularity] labelrankSeq {inflation: 1.8, cond_update: 1.0}
# ...
```

[![](https://i.imgur.com/4ajsNyf.png)][sheetp]
[![](https://i.imgur.com/3PcfPaZ.png)][sheetp]

<br>
<br>


## References

- [LabelRankT: Incremental Community Detection in Dynamic Networks via Label Propagation](https://arxiv.org/abs/1305.2006)
- [LabelRank: A Stabilized Label Propagation Algorithm for Community Detection in Networks](https://arxiv.org/abs/1303.0868)
- [SuiteSparse Matrix Collection]

<br>
<br>

[![](https://i.imgur.com/FvgjEIX.jpg)](https://www.youtube.com/watch?v=4rrtQMCIrrk)<br>
[![ORG](https://img.shields.io/badge/org-puzzlef-green?logo=Org)](https://puzzlef.github.io)
[![DOI](https://zenodo.org/badge/512458995.svg)](https://zenodo.org/badge/latestdoi/512458995)


[Prof. Dip Sankar Banerjee]: https://sites.google.com/site/dipsankarban/
[Prof. Kishore Kothapalli]: https://faculty.iiit.ac.in/~kkishore/
[SuiteSparse Matrix Collection]: https://sparse.tamu.edu
[LabelRank]: https://arxiv.org/abs/1303.0868
[gist]: https://gist.github.com/wolfram77/e9137741753d366014927e59c2fe6b6a
[charts]: https://imgur.com/a/mUh31mV
[sheets]: https://docs.google.com/spreadsheets/d/16_vhQWnmD4BxDHheO8AKEIbWOtOBssS349oRSGAQpTw/edit?usp=sharing
[sheetp]: https://docs.google.com/spreadsheets/d/e/2PACX-1vQA75KLWP9FkdRplra-WqUKutW-ALvc4rJann11Kq_EGqs_EL6qMbp_O00YSJTsNBe8-HKP0K7HDcEN/pubhtml
