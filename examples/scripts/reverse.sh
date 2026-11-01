#!/bin/sh
# reverse: arguments as one line, then stdin reversal with -.
set -eu
cd "$(dirname "$0")/../.."
BIN=./hello-world-c
[ -x "$BIN" ] || { echo "$BIN not found - build it first: make" >&2; exit 1; }

$BIN reverse hello world
printf 'through the pipe\n' | $BIN reverse -
