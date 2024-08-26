#!/bin/bash

# Define the starting directory
start_dir="/eos/totem/data/cmstotem/2018/90m/RECO_copy/"

# Find all files and format their paths
files=$(xrdfs root://eostotem.cern.ch/$start_dir find "$start_dir" -type f | sed "s/^/'/; s/$/',/")

# Print the formatted paths, joining them with newlines
echo "$files" | paste -sd '\n' -