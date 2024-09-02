#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <input_file>"
    exit 1
fi

inputFile="$1"

numChannels=$(awk '{gsub(/\t/, ""); gsub(/[ \t]+$/, ""); print NF}' "$inputFile" | sort -nu | tail -n 1)
numChannels=$(($numChannels - 1))

plotCommand="plot "

for i in $(seq 0 $(($numChannels - 1))); do
    plotCommand+="\"$inputFile\" using $(($i + 1)) with lines title \"Channel $i\""
    if [ $i -lt $(($numChannels - 1)) ]; then
        plotCommand+=", "
    fi
done

echo $numChannels

gnuplot -e "$plotCommand" -p
