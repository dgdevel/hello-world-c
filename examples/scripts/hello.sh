#!/bin/sh
# the greeting: bare, and with a name argument.
set -eu
cd "$(dirname "$0")/../.."
BIN=./hello-world-c
[ -x "$BIN" ] || { echo "$BIN not found - build it first: make" >&2; exit 1; }

$BIN hello
$BIN hello world
name=${1:-$USER}
$BIN hello "$name"
