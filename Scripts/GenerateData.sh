#!/bin/bash

# This script has the goal to generate all the data related to a PDF in order to be used in the BridgePTM platform.
# For each page the script should generate:
# - The thumbnail image of the page
# - The pyramid (3 levels)
# - The content-holders
# - The features and the descriptors

echo "Document: $1.pdf"

# 1- Generates the large page image whose tiles make the lowest level of detail
./Src/bridgeptm-content/bin/bin/mudraw -r 150 -o $1-%d.png $1.pdf

for image in "$1"*.png; 
do
	echo "Page: $image"
done
