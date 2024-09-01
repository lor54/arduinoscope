#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <input_file>"
    exit 1
fi

inputFile="$1"

numChannels=$(awk '{print NF}' "$inputFile" | sort -nu | tail -n 1)

plotCommand="plot "

for i in $(seq 1 $numChannels); do
    plotCommand+="\"$inputFile\" using $i with lines title \"Channel $i\""
    if [ $i -lt $numChannels ]; then
        plotCommand+=", "
    fi
done

gnuplot -e "$plotCommand" -p
