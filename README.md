# hello-world-c

Welcome to `hello-world-c` — a lightweight C project template that comes pre-loaded with several handy shell utilities. Whether you need to greet someone, reverse some text, or keep your project up to date, this toolkit has you covered.

## Getting Started

Get up and running in seconds:

```sh
make
./hello-world-c hello
```

## What's Inside?

Here are the tools available to you:

- **`hello [name]`**: Sends a friendly greeting. Leave out the name for a generic "hello world!".
- **`echo <text...>`**: Classic echo functionality. Use `-` to read from standard input.
- **`reverse <text...>`**: Flips your text backwards. Supports standard input via `-`.
- **`wc`**: Quick stats on your input (lines, words, and bytes).
- **`update [--yes]`**: Scans GitHub for fresh releases and handles installation.
- **`help`**: Stuck? This shows you how to use everything.
- **`version`**: Check which version you're running.

## Building the Project

### Requirements
To build this project, you'll need:
- A C11-compatible compiler.
- The `pthreads` library.

We've kept it dependency-free—all necessary code lives in `src/`, so you won't need to hunt for external libraries.

### Useful Commands
```sh
make              # Build the main binary
make check        # Run our 43-check test suite to ensure everything is working
make check-vg     # Run tests through Valgrind to catch memory leaks (see test/valgrind.log)
make asan         # Create a sanitizer build for deeper debugging
make fuzz         # Set up libFuzzer harnesses (see fuzz/README.md for details)
```

## A Peek Under the Hood

Here is how the project is organized:

- **`src/`**: Where the magic is. 
    - `hello.h`: The core header containing versioning and APIs.
    - `buf.c`: Our custom dynamic buffer system for efficient string handling.
    - `strutil.c`: Essential string helpers like trimming and reversing.
    - `cli.c`: A flexible long-option parser that handles arguments gracefully.
    - `crash_report.c/.h`: A failsafe signal handler. More details in `docs/crash-report.md`.
    - `update.c`: Powers the update command by checking GitHub via curl.
    - `main.c`: The entry point that connects subcommands to their logic.
- **`test/`**: Contains `selfcheck.c`, our comprehensive test suite.
- **`fuzz/`**: Everything needed for fuzz testing, including seeds and harnesses.
- **`tools/`**: Helper scripts for packaging and releases.
- **`docs/`**: Detailed guides on releasing and handling crashes.
- **`examples/`**: Simple scripts to show what the tool can do.

## License
This project is proudly distributed under the European Union Public Licence v1.2 (EUPL-1.2).
