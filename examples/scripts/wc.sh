#!/bin/sh
# wc: count lines, words and bytes of a stdin sample.
set -eu
cd "$(dirname "$0")/../.."
BIN=./hello-world-c
[ -x "$BIN" ] || { echo "$BIN not found - build it first: make" >&2; exit 1; }

printf 'one two three\nfour five\nsix\n' | $BIN wc
printf 'tab-separated\tfields\tcount too\n' | $BIN wc
