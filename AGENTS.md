# hello-world-c Agents Guide

This document gives context and instructions for agents that work on the `hello-world-c` project.

## Project Overview
`hello-world-c` is a template C project with a small set of shell utilities: greeting, echo, reverse, wc, and a self-update stub. It is meant to be copied and extended, so every feature stays small and dependency-free.

## Technical Stack
- Language: C (C11)
- Dependencies: none (pthread only)
- Build system: GNU Make
- Platforms: linux (native plus docker-built deb/rpm/arch packages), windows and macos via the cross-builds in `tools/`.

## Project Structure
- `src/`: the source code.
    - `main.c`: entry point and subcommand dispatch (hello, echo, reverse, wc, update, help, version).
    - `hello.h`: the public header - version macro and the APIs of the modules below.
    - `buf.c`: the `buf_t` dynamic buffer (`buf_init`/`buf_free`/`buf_append`/`buf_appendf`/`buf_reset`).
    - `strutil.c`: byte-oriented string helpers (`str_trim`, `str_reverse_n`, `str_starts`, `str_upper_ascii`).
    - `cli.c`: the long-option parser (`--name`, `--name value`, `--name=value`, lone `--`).
    - `crash_report.c`/`.h`: fatal-signal handler (SIGSEGV/SIGBUS/SIGFPE/SIGABRT), optional backtrace.
    - `update.c`: `hello-world-c update` - latest-release check via the curl binary; download step is a documented stub.
- `test/selfcheck.c`: the 43-check self-check suite (`make check`).
- `fuzz/`: libFuzzer harnesses and seed corpora (see `fuzz/README.md`).
- `tools/`: build, release, and packaging utilities (`release.sh`, `package.sh`, `check-ascii.sh`, `pkg/`).
- `docs/`: release and crash-report documentation.
- `examples/`: sample usage and scripts.

## Build and Test
### Building
Run `make` to build the `hello-world-c` binary.
```bash
make
```

### Testing
Run the self-check test suite.
```bash
make check
```

Optional passes: `make check-vg` (valgrind), `make asan` (sanitizer build), `make fuzz` (libFuzzer, needs clang).

## Development Guidelines
- ASCII-only sources, enforced by the `check-ascii` gate that runs before every compilation.
- Buffer work goes through `src/buf.c` - no ad-hoc realloc loops elsewhere.
- New subcommand: a `cmd_<name>` in `src/main.c`, one dispatch line, one usage entry. Reusable logic goes in a module under `src/` with its API in `hello.h`.
- Code style: follow the existing C11 patterns, lowercase comments.
- The `update` download step is a stub on purpose; keep it honest in docs until wired.
