#!/bin/bash

numChannels=$(awk '{print NF}' output.txt | sort -nu | tail -n 1)

plotCommand="plot "

for i in $(seq 1 $numChannels); do
    plotCommand+="\"output.txt\" using $i with lines title \"Channel $i\""
    if [ $i -lt $numChannels ]; then
        plotCommand+=", "
    fi
done

gnuplot -e "$plotCommand" -p
