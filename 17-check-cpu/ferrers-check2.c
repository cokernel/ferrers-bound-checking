#include <lapacke.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>

#define MAX_NODES 32

/* input:
 *  char *graph6: graph6 representation of connected bipartite graph on n nodes filtered through biplabg
 *  ssize_t len: length of graph6 representation
 *  double **laplacian: workspace to hold MAX_NODES x MAX_NODES entries (does not need to be zeroed)
 *
 * side effect:
 *  the first n * n entries in laplacian will contain the laplacian of the graph represented by graph6
 *
 * output:
 *  double ferrers: the conjectured Ferrers bound for the tree number of the graph represented by graph6
 */
double construct_laplacian_ferrers(char *graph6, ssize_t len, double *laplacian) {
    int rstart;
    uint32_t n;

    int row = 0;
    int col = 1;
    char byte;

    int seenblue = 0;
    int green = 0;
    int blue = 0;
    double ferrers_numerator;
    double ferrers_denominator;
    double ferrers;

    /* reset workspace */
    for (int i = 0; i < MAX_NODES * MAX_NODES; i++) {
        laplacian[i] = 0;
    }

    /* get size of graph
     *
     * Note: for our purposes, n will not come anywhere near 63, hence the definition of MAX_NODES.
     */
    if (graph6[1] == 126) {
        /* 258048 <= n <= 68719476735 */
        rstart = 6;
        n = ((graph6[2]-63) << 3) | ((graph6[3]-63) << 2) | ((graph6[4]-63) << 1) | (graph6[5]-63);
    }
    else if (graph6[0] == 126) {
        /* 63 <= n <= 258047 */
        rstart = 4;
        n = ((graph6[1]-63) << 2) | ((graph6[2]-63) << 1) | (graph6[3]-63);
    }
    else  {
        rstart = 1;
        n = graph6[0]-63;
    }

    /* construct laplacian */
    for (int frame = rstart; frame < len-1; frame++) {
        byte = graph6[frame] - 63;
        /* we're only using 6 bits per byte */
        for (int r = 5; r >= 0; r--) {
            if (byte & (1 << r)) {
                /* edge row-col is present */

                /* note: we label bipartite graphs canonically, so the first edge
                 * found identifies the number of green vertices. */
                if (!seenblue) {
                    seenblue = 1;
                    green = col;
                    blue = n - col;
                }

                /* increase row-row and col-col */
                laplacian[row * n + row]++;
                laplacian[col * n + col]++;

                /* decrease row-col and col-row */
                laplacian[row * n + col]--;
                laplacian[col * n + row]--;
            }
            row++;
            if (row == col) {
                row = 0;
                col++;
            }
        }
    }

    /* construct ferrers bound */
    ferrers_numerator = 1;
    ferrers_denominator = (double)(blue * green);
    for (int i = 0; i < n; i++) {
        ferrers_numerator *= laplacian[i * n + i];
    }
    ferrers = ferrers_numerator / ferrers_denominator;
    
    return ferrers;
}

int main() {
    uint32_t n;

    char *graph6 = NULL;
    char byte;
    size_t linesize = 0;
    ssize_t linelen;
    
    double *laplacian;
    double *laplacian_merge;
    double *laplacian_copy;
    double *diagonal;
    double *subdiagonal;
    double *tau;

    double ferrers;
    double trees_raw;
    int trees;

    laplacian = calloc(MAX_NODES * MAX_NODES, sizeof(double));
    laplacian_merge = calloc(MAX_NODES * MAX_NODES, sizeof(double));
    laplacian_copy = calloc(MAX_NODES * MAX_NODES, sizeof(double));
    diagonal = calloc(MAX_NODES, sizeof(double));
    subdiagonal = calloc(MAX_NODES - 1, sizeof(double));
    tau = calloc(MAX_NODES - 1, sizeof(double));


    while ((linelen = getline(&graph6, &linesize, stdin)) != -1) {
        /* peek at n */
        n = graph6[0]-63;
        ferrers = construct_laplacian_ferrers(graph6, linelen, laplacian);

        /* Use LAPACKE to get tree number */
        lapack_int info;
        lapack_int lda = (lapack_int)n;
        for (int i = 0; i < n * n; i++) {
            laplacian_copy[i] = laplacian[i];
        }

        info = LAPACKE_dsytrd(LAPACK_ROW_MAJOR, 'U', n, laplacian_copy, lda, diagonal, subdiagonal, tau);
        info = LAPACKE_dsterf(n, diagonal, subdiagonal);
        
        trees_raw = 1;
        for (int i = 1; i < n; i++) {
            trees_raw *= diagonal[i];
        }
        trees_raw /= n;
        trees = (int)(trees_raw + 0.5);

        /*
        if (trees <= ferrers) {
            printf("ok %s %d %f\n", graph6, trees, ferrers);
        }
        else {
        */
        if (trees > ferrers) {
            printf("not ok %s\n", graph6);
            exit(1);
        }
    }

    printf("ok %d\n", n);
    return 0;
}
