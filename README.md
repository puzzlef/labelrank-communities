Variation of modularity with progression of iterations of the LabelRank
algorithm for community detection.

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

In this experiment, we observe the **variation in modularity** of communities at
**each iteration** on the *LabelRank algorithm*. Each iteration is *re-run 5*
*times* for averaging, and the **average time taken** and the **modularity** of
community membership obtained after the iteration is measured. We run the
algorithm for exactly `50 iterations` with the following parameters:
`inflation=1.5`, `conditionalUpdate=0.5`. This is repeated for *seventeen*
different graphs.

From the results, we note that **most of the increase in modularity is in the**
**first few iterations**, with more and more *gradual increases* in the *later*
*iterations* on average (as expected). **Road networks** are seen to have the
**most gradual increase** in modularity, however all the **other classes** of
graph seem to **converge much faster** (within `10 iterations`). We also note
that with some graphs, the modularity seems to *peak in early iterations*, and
later *drops down a bit* and then *increases back up*. We then take a look at
the **per iteration time** taken. This is **initially higher** (`~1.5x` that of
later iterations), and seems to **settle to a low fixed time in the later
iterations**. This **difference in time** taken **between the early** and the
**later iterations** is observed to be the **least** in case of **social**
**networks**. It should however be noted that this behavior may change with a
different choice of parameters (*inflation*, *conditionalUpdate*).

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
# [00466.622 ms; 001 iteration; 0.323808 modularity] labelrankSeq
# [00411.452 ms; 002 iteration; 0.618470 modularity] labelrankSeq
# [00337.436 ms; 003 iteration; 0.669953 modularity] labelrankSeq
# ...
# [00207.755 ms; 048 iteration; 0.621806 modularity] labelrankSeq
# [00204.260 ms; 049 iteration; 0.621734 modularity] labelrankSeq
# [00210.423 ms; 050 iteration; 0.621788 modularity] labelrankSeq
#
# Loading graph /home/subhajit/data/web-BerkStan.mtx ...
# order: 685230 size: 7600595 [directed] {}
# order: 685230 size: 13298940 [directed] {} (symmetricize)
# order: 685230 size: 13984170 [directed] {} (selfLoopAllVertices)
# [0.048410 modularity] noop
# [00809.768 ms; 001 iteration; 0.311566 modularity] labelrankSeq
# [00661.264 ms; 002 iteration; 0.652659 modularity] labelrankSeq
# [00515.153 ms; 003 iteration; 0.665593 modularity] labelrankSeq
```

[![](https://i.imgur.com/vRfVRiv.png)][sheetp]
[![](https://i.imgur.com/pyP1rua.png)][sheetp]

<br>
<br>


## References

- [LabelRankT: Incremental Community Detection in Dynamic Networks via Label Propagation](https://arxiv.org/abs/1305.2006)
- [LabelRank: A Stabilized Label Propagation Algorithm for Community Detection in Networks](https://arxiv.org/abs/1303.0868)
- [SuiteSparse Matrix Collection]

<br>
<br>

[![](https://i.imgur.com/3Xh28zd.jpg)](https://www.youtube.com/watch?v=jabC2s3dlI0)<br>
[![DOI](https://zenodo.org/badge/512477021.svg)](https://zenodo.org/badge/latestdoi/512477021)


[Prof. Dip Sankar Banerjee]: https://sites.google.com/site/dipsankarban/
[Prof. Kishore Kothapalli]: https://faculty.iiit.ac.in/~kkishore/
[SuiteSparse Matrix Collection]: https://sparse.tamu.edu
[LabelRank]: https://arxiv.org/abs/1303.0868
[gist]: https://gist.github.com/wolfram77/bdbbca481ab5f5c28e8f0c458fe70bbb
[charts]: https://imgur.com/a/EoSFi7m
[sheets]: https://docs.google.com/spreadsheets/d/1G7_kUnC-08tnzqHFMnuNKb1o-A3_UcHNrNY1hYBKb2k/edit?usp=sharing
[sheetp]: https://docs.google.com/spreadsheets/d/e/2PACX-1vRTgWekRXMQnKceVf5Eq4YrK2ibUxSOU8cRrkoVSEqQGYXPew2sKyWekSaf5FzxMJoWeLuEU6U8tL0U/pubhtml
