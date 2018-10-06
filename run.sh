#!/bin/bash

./build.sh

echo "==========================="
echo "dealII"
echo "==========================="
./orcs -t traces/dealII.CFP.PP200M
echo ""
echo ""

echo "==========================="
echo "calculix"
echo "==========================="
./orcs -t traces/calculix.CFP.PP200M
echo ""
echo ""

echo "==========================="
echo "namd"
echo "==========================="
./orcs -t traces/namd.CFP.PP200M
echo ""
echo ""

echo "==========================="
echo "gromacs"
echo "==========================="
./orcs -t traces/gromacs.CFP.PP200M
echo ""
echo ""

echo "==========================="
echo "libquantum"
echo "==========================="
./orcs -t traces/libquantum.CINT.PP200M
echo ""
echo ""

echo "==========================="
echo "astar"
echo "==========================="
./orcs -t traces/astar.CINT.PP200M
echo ""
echo ""
