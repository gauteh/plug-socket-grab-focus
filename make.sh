#! /usr/bin/bash

flags1=$(pkg-config --libs --cflags glibmm-2.4)
flags2=$(pkg-config --libs --cflags gtkmm-3.0)

g++ $flags1 $flags2 -o sock sock.cc
g++ $flags1 $flags2 -o plug plug.cc
