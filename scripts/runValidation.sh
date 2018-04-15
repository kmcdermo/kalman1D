#!/bin/bash

infile=${1:-"valtree.root"}
outdir=${2:-"output"}
moveInput=${3:-1}

root -b -l -q validation/runValidation.C\(\"${infile}\",\"${outdir}\",${moveInput}\)