#!/bin/bash
n=$1
mod=$2
pause=$3
max=$((mod-1))

for res in $(seq 0 "$max"); do
    echo "starting process $res/$max"
    (geng -cb "$n" "$res/$mod" | biplabg | ./ferrers-check2) &
    sleep "$pause"
done

wait
