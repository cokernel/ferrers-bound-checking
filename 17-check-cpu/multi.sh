#!/bin/bash
for n in $(seq 1 15); do 
    (geng -cb $n | biplabg | ./ferrers-check2) &
done
