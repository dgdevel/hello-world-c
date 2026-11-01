#!/bin/sh
# echo: arguments on one line, then stdin passthrough with -.
set -eu
cd "$(dirname "$0")/../.."
BIN=./hello-world-c
[ -x "$BIN" ] || { echo "$BIN not found - build it first: make" >&2; exit 1; }

$BIN echo one two three
printf 'through the pipe\n' | $BIN echo -
