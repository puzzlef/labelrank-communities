Comparing the effect of adjustment of labelset capacity of the LabelRank
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

In this experiment the **labelset capacity** is **adjusted from 1 to 16** (in
multiples of 2), and the LabelRank algorithm is performed (5 times for
averaging). We run the algorithm for exactly `10 iterations` with the following
parameters: `inflation=1.5`, `conditionalUpdate=0.5`. We measure the **time**
**taken** for the *computation*, and *measure* the **modularity score** (one of
the ways to measure quality of communities). This is repeated for *seventeen*
other graphs.

From the results, we note that using a **labelset capacity of 1** yields not
only the **best performance** (as expected), but also ends up providing us with
the **highest modularity score**. The only exception is on the
`soc-LiveJournal1` where a labelset capacity of 4 yields and slightly higher
modularity compared to that with a labelset capacity of 1. If this holds for
other parameter values as well, it would indicate that **cheapest is also the**
**best**.

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
# [00886.283 ms; 0.682887 modularity] labelrankSeq {lset_capacity: 1}
# [01546.772 ms; 0.656402 modularity] labelrankSeq {lset_capacity: 2}
# [03129.802 ms; 0.621151 modularity] labelrankSeq {lset_capacity: 4}
# [06412.741 ms; 0.586805 modularity] labelrankSeq {lset_capacity: 8}
# [12712.884 ms; 0.622585 modularity] labelrankSeq {lset_capacity: 16}
#
# Loading graph /home/subhajit/data/web-BerkStan.mtx ...
# order: 685230 size: 7600595 [directed] {}
# order: 685230 size: 13298940 [directed] {} (symmetricize)
# order: 685230 size: 13984170 [directed] {} (selfLoopAllVertices)
# ...
```

[![](https://i.imgur.com/XWBNk8z.png)][sheetp]
[![](https://i.imgur.com/uy9VSTv.png)][sheetp]

<br>
<br>


## References

- [LabelRankT: Incremental Community Detection in Dynamic Networks via Label Propagation](https://arxiv.org/abs/1305.2006)
- [LabelRank: A Stabilized Label Propagation Algorithm for Community Detection in Networks](https://arxiv.org/abs/1303.0868)
- [SuiteSparse Matrix Collection]

<br>
<br>

[![](https://i.imgur.com/sBfj27E.jpg)](https://www.youtube.com/watch?v=UIf4KJXo45g)<br>
[![DOI](https://zenodo.org/badge/512471326.svg)](https://zenodo.org/badge/latestdoi/512471326)


[Prof. Dip Sankar Banerjee]: https://sites.google.com/site/dipsankarban/
[Prof. Kishore Kothapalli]: https://faculty.iiit.ac.in/~kkishore/
[SuiteSparse Matrix Collection]: https://sparse.tamu.edu
[LabelRank]: https://arxiv.org/abs/1303.0868
[gist]: https://gist.github.com/wolfram77/42326b17747eced1a67add9140b745ea
[charts]: https://imgur.com/a/wyKJ8Q6
[sheets]: https://docs.google.com/spreadsheets/d/1F4LiOW2e46UBQVr0hWBGZk7p_jPZyVeOwDuVnwLvt2I/edit?usp=sharing
[sheetp]: https://docs.google.com/spreadsheets/d/e/2PACX-1vQUV6cjRepZUP3H1G85wixiutORoVGGtEWP_AD69poElPWEpMdjv5TttKVjrThC3bCNn5mxmnRtehsH/pubhtml
