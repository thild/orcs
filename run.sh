#!/bin/bash

./build.sh

echo "dealII"
./orcs -t traces/dealII.CFP.PP200M

echo "calculix"
./orcs -t traces/calculix.CFP.PP200M

echo "namd"
./orcs -t traces/namd.CFP.PP200M

echo "gromacs"
./orcs -t traces/gromacs.CFP.PP200M

echo "libquantum"
./orcs -t traces/libquantum.CINT.PP200M

echo "astar"
./orcs -t traces/astar.CINT.PP200M
