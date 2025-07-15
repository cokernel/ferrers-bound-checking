#!/bin/bash
# linux
#clang -o ferrers-check ferrers-check.c $(pkgconf --cflags --libs lapacke)
#clang -o ferrers-check2 ferrers-check2.c $(pkgconf --cflags --libs lapacke)
#clang -o ferrers-check3 ferrers-check3.c -pthread $(pkgconf --cflags --libs lapacke)
#clang -o ferrers-check-parallel ferrers-check-parallel.c -pthread $(pkgconf --cflags --libs lapacke)
#clang -o ferrers-check2-omp ferrers-check2-omp.c -fopenmp=libomp $(pkgconf --cflags --libs lapacke)
# macos
#clang -o ferrers-factor ferrers-factor.c -L/opt/homebrew/opt/lapack/lib -I/opt/homebrew/opt/lapack/include -llapacke
#clang -o ferrers-check-parallel ferrers-check-parallel.c -L/opt/homebrew/opt/lapack/lib -I/opt/homebrew/opt/lapack/include -llapacke
#clang -o ferrers-check ferrers-check.c -L/opt/homebrew/opt/lapack/lib -I/opt/homebrew/opt/lapack/include -llapacke
clang -o ferrers-check2 ferrers-check2.c -L/opt/homebrew/opt/lapack/lib -I/opt/homebrew/opt/lapack/include -llapacke
