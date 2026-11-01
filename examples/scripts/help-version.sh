#!/bin/sh
# the built-ins: help and version are subcommands like everything else.
set -eu
cd "$(dirname "$0")/../.."
BIN=./hello-world-c
[ -x "$BIN" ] || { echo "$BIN not found - build it first: make" >&2; exit 1; }

$BIN help
$BIN version
