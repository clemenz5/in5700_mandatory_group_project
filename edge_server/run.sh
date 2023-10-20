#!/bin/bash
make all
out/clang-release/edge_server -m -u Qtenv -n .:../inet4/src:../inet4/examples:../inet4/tutorials:../inet4/showcases --image-path=../inet4/images -l ../inet4/src/INET omnetpp.ini