
# Computer verification that no small counterexamples to the Ferrers bound conjecture exist

## Abstract

The Ferrers invariant of a bipartite graph is defined in terms of its degree sequence and bipartition. The *Ferrers bound conjecture* is the conjecture, due to Ehrenborg, that the number of spanning trees of a bipartite graph is bounded above by its Ferrers invariant. We verify that a counterexample to the conjecture must have at least 18 vertices.

## Introduction

In this note, a companion piece to another by the author (in preparation) we study an upper bound conjectured by Ehrenborg (2006) for the number of spanning trees of bipartite graphs. All graphs are simple, undirected and finite. Graph theory terminology which we do not define here can be found in any standard graph theory text. Linear algebra terminology which we do not define here can be found in Horn and Johnson (2012).

Let $G = (V, E)$ be a bipartite graph with bipartition $V = X \sqcup Y$. We call $X$ and $Y$ the *parts* of $G$ and say that $G$ has *order* $|V|$ and *size* $|E|$. A *spanning tree* of $G$ is a subgraph of $G$ which is a tree and which contains all vertices of $G$. The *tree number* of $G$, denoted by $T(G)$, is the number of distinct spanning trees of $G$.

**Definition**. Let $G$ be a bipartite graph with bipartition $V = X \sqcup Y$. Then the *Ferrers invariant of $G$*, denoted by $F(G)$, is the nonnegative rational quantity
$$F(G) = \frac{1}{|X||Y|}\prod_{v\in V}\deg_G(v).$$
**Conjecture** (Ehrenborg 2006). Let $G$ be a bipartite graph. Then $T(G) \le F(G)$.

This conjecture, which we call the *Ferrers bound conjecture*, is known to hold for some infinite classes of bipartite graphs and is open in general. In this note we focus on the search for small counterexamples.

Schmidt (2009) verified by a computer calculation that a counterexample to the Ferrers bound conjecture must have at least 14 vertices. In 2020, the author, using nauty (McKay and Piperno, 2013) and LAPACK (Anderson et al., 1999), replicated Schmidt's result and increased the minimum order of a counterexample to 18 vertices. However, the artifacts for these calculations were not preserved. Therefore, we replicate the results and provide our code so that others may verify the results for themselves and, we hope, extend and improve them.

**Definition** Let $G$ be a graph of order $n$ and label the vertices $v_1, \cdots, v_n$. Then the *Laplacian* of $G$ is the $n \times n$ matrix $L = (\ell_{i,j})$ such that
$$
\ell_{i,j} = \begin{cases}
\deg_G(v_i) & i = j \\
-1 & i\ne j \text{ and } v_i v_j\in E(G) \\
0 & \text{otherwise.}
\end{cases}
$$
The Laplacian of a graph is positive semidefinite. The matrix-tree theorem, implicit in Kirchhoff (1847) and proved by Brooks et al. (1940), shows that the spectrum of the Laplacian determines the tree number. We give a more modern statement, following Cvetković, Doob, and Sachs (1980).

**Theorem** (Brooks et al. 1940). Let $G$ be a graph of order $n$ with Laplacian $L$. Suppose $L$ has eigenvalues $\lambda_1 \ge \cdots \ge \lambda_n = 0$. Then
$$ T(G) = \frac{1}{n} \prod_{i=1}^{n-1}\lambda_i.$$
Equivalently,
$$ T(G) = \det(L(i)),$$
where $L(i)$ is a principal submatrix of $L$.

Observe that disconnected graphs have zero spanning trees, so the conjecture is trivially true for such graphs.
## Methods

We use the programs `geng` and `biplabg`, included in nauty, to generate connected bipartite graphs and canonically label them according to their coloring. We generate a stream of graphs represented in [`graph6` format](https://users.cecs.anu.edu.au/~bdm/data/formats.txt) and process them with a C program which uses LAPACK to determine the spectrum. The canonical labeling allows the C program to determine the orders of the parts of each graph. The sequence is as follows:

1. Generate bipartite graph $G$ with canonical labeling in graph6 format.
2. Decode the graph and construct the Laplacian $L$, also storing the order of each part.
3. Compute the conjectured Ferrers bound. The numerator is the product of the diagonal entries of $L$, and the denominator is the product of the orders of each part.
4. Compute the tree number using LAPACK. We provide some more detail below.
5. Verify that $T(G) \le F(G)$ for the given graph. If $T(G) > F(G)$, then output "not ok" followed by the graph6 representation of the graph, then halt.

If no counterexamples were found, the C program will output "ok" followed by the order of the final graph verified. The program is intended to be provided with streams of graphs of the same order.
### Computing the tree number

For the LAPACK calculation, we first use [`DSYTRD`](https://www.netlib.org/lapack/explore-html-3.6.1/d3/db6/group__double_s_ycomputational_gaefcd0b153f8e0c36b510af4364a12cd2.html) to reduce $L$ to real symmetric tridiagonal form, then we follow with [`DSTERF`](https://www.netlib.org/lapack/explore-html-3.6.1/d9/df2/dsterf_8f_af0616552c11358ae8298d0ac18ac023c.html) to compute the eigenvalues of the resulting tridiagonal matrix. Then we can read the eigenvalues from the diagonal of the output matrix and determine $T(G)$ using the eigenvalue form of the matrix-tree theorem.

### Installation

This program requires the following:

* nauty, which can be obtained from the [nauty and Traces page](https://pallini.di.uniroma1.it/)
* LAPACK, which can be obtained on macOS with the [Homebrew](https://brew.sh/) command `brew install lapack`
* A C compiler, such as [Clang](https://clang.llvm.org/).

The directory [17-check-cpu] includes the code used for the calculation. To compile the code with Clang on macOS, use the command `bash build.sh`. This will generate the executable `ferrers-check2`.

To run parallel tests of very small graphs, $1 \le |V(G)| \le 15$, use the command `bash multi.sh`.

To run parallel tests of graphs of order 16 or 17, the `split.sh` command is provided. This accepts three arguments:
* *n* - order of graphs to test
* *mod* - number of processes to use (note: because of the way `geng` works, the number of graphs each process will receive can vary)
* *pause* - time to wait between process launches (this is not strictly necessary but can reduce fan noise; all processes run concurrently aside from the staggered starts).
Example invocation: to test graphs of order 17 in 4 processes, waiting 5 seconds between process launches, use the command `bash split.sh 17 4 5`.
## Results

An experiment was performed between June 2, 2025 and June 26, 2025 on a computer with the following specifications:
* model: MacBook Pro, 16-inch, November 2023
* chip: Apple M3 Max
* memory: 36 GB
* macOS version: Sonoma 14.6.1

The following commands were used:
* `time bash [multi.sh] | tee [multi-results.txt]` ($1 \le |V(G)| \le 15$)
* `time bash [split.sh] 16 4 5 | tee [16-4-5.txt]` ($|V(G)| = 16$)
* `time bash [split.sh] 17 4 5 | tee [17-4-5.txt]` ($|V(G)| = 17$)

No counterexamples were found.

* The check for $1\le |V(G)| \le 15$ ran in 1 hour, 21 minutes.
* The check for $|V(G)| = 16$ ran in 15 hours, 20 minutes.
* The check for $|V(G)| = 17$ ran in 22 days, 16 hours.

All timings are given in wall-clock time.
## Discussion

We have demonstrated that it is possible to verify the Ferrers bound conjecture for very small bipartite graphs on consumer hardware in 2025. This places a constraint on the minimum order of a possible counterexample to the conjecture.

Because our program computes spectra of many small matrices, we chose to use CPU parallelization. In future work, we hope to use a strategy such as that outlined by Cosnuau (2014) to perform this work on the GPU. Additionally, the author has proved in a companion piece (in preparation) that minimal counterexamples to the conjecture must be 2-connected, which could be used to reduce the size of the dataset that must be processed by the C program.
## Bibliography

* E. Anderson, Z. Bai, C. Bischof, S. Blackford, J. Demmel, J. Dongarra, J. Du Croz, A. Greenbaum, S. Hammarling, A. McKenney, and D. Sorensen. _LAPACK Users' Guide_, Third. Philadelphia, PA: Society for Industrial and Applied Mathematics, 1999. ISBN: 0-89871-447-8 (paperback).
* R. L. Brooks, C. A. B. Smith, A. H. Stone, and W. T. Tutte. "The dissection of rectangles into squares", _Duke Math. J._, **6.1** (1940), 312&ndash;340.
* A. Cosnuau. "Computation on GPU of eigenvalues and eigenvectors of a large number of small Hermitian matrices", _Procedia Computer Science_ **29** (2014), 800&ndash;810.
* D. M. Cvetković, M. Doob, and H. Sachs. _Spectra of graphs: theory and applications_. New York: Academic Press, 1980. ISBN: 0-12-195150-2.
* R. Ehrenborg, personal communication, 2006.
* R. Ehrenborg and S. van Willigenburg. "Enumerative properties of Ferrers graphs", _Discrete Comput. Geom._ **32.4** (2004), 481&ndash;492. ISSN: 0179-5376.
* P. L. Hammer, U. N. Peled, and X. Sun. "Difference graphs", _Discrete Applied Mathematics_ **28.1** (1990), 35&ndash;44. ISSN: 0166-218X. DOI: [https://doi.org/10.1016/0166-218X(90)90092-Q](https://doi.org/10.1016/0166-218X(90)90092-Q). URL: [https://www.sciencedirect.com/science/article/pii/0166218X9090092Q](https://www.sciencedirect.com/science/article/pii/0166218X9090092Q).
* R. A. Horn  and C. R. Johnson. _Matrix analysis_, Cambridge University Press, 2012.
* G. Kirchhoff. "Über die Auflösung der Gleichungen, auf welche man bei der Untersuchung der linearen Vertheilung galvanischer Ströme geführt wird", _Annalen der Physik_ **148.12** (1847), 497&ndash;508.
* B. D. McKay and A. Piperno, "Practical Graph Isomorphism, II", _J. Symbolic Computation_ (2013) **60** 94-112. [https://doi.org/10.1016/j.jsc.2013.09.003](https://doi.org/10.1016/j.jsc.2013.09.003). Preprint version at [arxiv.org](https://arxiv.org/abs/1301.1493).
* J. Schmidt, personal communication, 2009.
* M. Slone, "Minimal Ferrers-bad graphs cannot have long ears", in preparation.

## Author

MLE Slone (m.slone@uky.edu), Research and Discovery Division, [University of Kentucky Libraries](https://libraries.uky.edu)
