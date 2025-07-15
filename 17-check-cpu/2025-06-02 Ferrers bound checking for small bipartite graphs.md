# Ferrers bound checking for small bipartite graphs

## Experiment beginning 2025-06-02

### Order 1 <= n <= 15

Command: `time bash [multi.sh] | tee [multi-results.txt]`

Result: no counterexamples found

Timing: 

```
bash multi.sh  0.00s user 0.00s system 49% cpu 0.012 total
tee multi-results.txt  0.00s user 0.00s system 0% cpu 1:20:28.37 total
```

### Order 16

Command: `time bash [split.sh] 16 4 5 | tee [16-4-5.txt]`

Result: no counterexamples found

Timing:

```
bash split.sh 16 4 5  149575.37s user 716.99s system 272% cpu 15:20:04.47 total
tee 16-4-5.txt  0.00s user 0.00s system 0% cpu 15:20:04.47 total
```

### Order 17

Command: `time bash [split.sh] 17 4 5 | tee [17-4-5.txt]`

Result: no counterexamples found

Timing:

```
bash split.sh 17 4 5  5142709.78s user 24750.01s system 263% cpu 544:20:45.01 total
tee 17-4-5.txt  0.00s user 0.00s system 0% cpu 544:20:45.01 total
```
