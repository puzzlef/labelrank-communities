Comparing approaches for *community detection* using **LabelRank algorithm**.

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
there is a *fixed upper limit* on the *number of labels per vertex*
(**labelset** **capacity**). Therefore we do not use the *cutoff operator*
(which removes low-weighted labels), but instead trim-off labels if they do not
fit within labelset capacity. *Labels* are **sorted by weight** such that only
low-weighted labels are eliminated. The input data used for each experiment
given below is available from the [SuiteSparse Matrix Collection]. All
experiments are done with guidance from [Prof. Kishore Kothapalli] and
[Prof. Dip Sankar Banerjee].

<br>


### Adjusting Parameters

In this experiment ([adjust-parameters]), we adjust the LabelRank parameters
`inflation` from `1.1 - 2.0` and `conditionalUpdate` from `0.1 - 1.0`, in order
the obtain parameter values that are suitable in general, or suitable to a
specific class of graphs. We use a fixed `labelset capacity` of `4`, and run the
algorithm for exactly `10 iterations`. We measure the **time taken** for the
*computation* (performed 5 times for averaging), and *measure* the **modularity**
**score** (one of the ways to measure quality of communities). This is repeated
for *seventeen* different graphs.

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

[adjust-parameters]: https://github.com/puzzlef/labelrank-communities/tree/adjust-parameters

<br>


### Adjusting Labelset capacity

In this experiment ([adjust-labelset-capacity]), the **labelset capacity** is
**adjusted from 1 to 16** (in multiples of 2), and the LabelRank algorithm is
performed (5 times for averaging). We run the algorithm for exactly `10 iterations`
with the following parameters: `inflation=1.5`, `conditionalUpdate=0.5`. We
measure the **time** **taken** for the *computation*, and *measure* the
**modularity score** (one of the ways to measure quality of communities).
This is repeated for *seventeen* other graphs.

From the results, we note that using a **labelset capacity of 1** yields not
only the **best performance** (as expected), but also ends up providing us with
the **highest modularity score**. The only exception is on the
`soc-LiveJournal1` where a labelset capacity of 4 yields and slightly higher
modularity compared to that with a labelset capacity of 1. If this holds for
other parameter values as well, it would indicate that **cheapest is also the**
**best**.

[adjust-labelset-capacity]: https://github.com/puzzlef/labelrank-communities/tree/adjust-labelset-capacity

<br>


### Adjusting Accumulator capacity

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

In this experiment ([adjust-accumulator-capacity]), we adjust the capacity of
accumulator labelset from `2` to `4093` in multiples of 2. This capacity is
always set to the highest prime number below a power of 2.  We choose the
LabelRank *parameters* as `inflation = 1.5` and `conditionalUpdate = 0.5`. In
addition, we use a fixed `labelset capacity` of `4`, and run the algorithm for
exactly `10 iterations`. Note that this labelset capacity is different from the
accumulator labelset capacity, and indicates the maximum number of labels
allowed per vertex. We measure the **time taken** for the *computation*
(performed 5 times for averaging), and *measure* the **modularity score** (one
of the ways to measure quality of communities). This is repeated for *seventeen*
different graphs.

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

[adjust-accumulator-capacity]: https://github.com/puzzlef/labelrank-communities/tree/adjust-accumulator-capacity

<br>


### Measuring Per-iteration modularity

In this experiment ([measure-per-iteration-modularity]), we observe the
**variation in modularity** of communities at **each iteration** on the
*LabelRank algorithm*. Each iteration is *re-run 5 times* for averaging, and
the **average time taken** and the **modularity** of community membership
obtained after the iteration is measured. We run the algorithm for exactly
`50 iterations` with the following parameters: `inflation=1.5`,
`conditionalUpdate=0.5`. This is repeated for *seventeen* different graphs.

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

[measure-per-iteration-modularity]: https://github.com/puzzlef/labelrank-communities/tree/measure-per-iteration-modularity

<br>


### Other Experiments

- [ordered-vs-unordered](https://github.com/puzzlef/labelrank-communities/tree/ordered-vs-unordered)

<br>
<br>


## References

- [LabelRankT: Incremental Community Detection in Dynamic Networks via Label Propagation](https://arxiv.org/abs/1305.2006)
- [LabelRank: A Stabilized Label Propagation Algorithm for Community Detection in Networks](https://arxiv.org/abs/1303.0868)
- [SuiteSparse Matrix Collection]

<br>
<br>


[![](https://i.imgur.com/3Xh28zd.jpg)](https://www.youtube.com/watch?v=jabC2s3dlI0)<br>
[![ORG](https://img.shields.io/badge/org-puzzlef-green?logo=Org)](https://puzzlef.github.io)
[![DOI](https://zenodo.org/badge/512477021.svg)](https://zenodo.org/badge/latestdoi/512477021)
![](https://ga-beacon.deno.dev/G-KD28SG54JQ:hbAybl6nQFOtmVxW4if3xw/github.com/puzzlef/labelrank-communities)

[Prof. Dip Sankar Banerjee]: https://sites.google.com/site/dipsankarban/
[Prof. Kishore Kothapalli]: https://faculty.iiit.ac.in/~kkishore/
[SuiteSparse Matrix Collection]: https://sparse.tamu.edu
[LabelRank]: https://arxiv.org/abs/1303.0868
