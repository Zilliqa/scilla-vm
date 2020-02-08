# Scilla Virtual Machine

A compiled execution backend for Scilla.

The compiled execution backend is divided into two parts:
  1. The JIT driver (JITD), which is responsible for invoking the
    scilla-compiler to produce LLVM-IR from Scilla source, then JIT
    compiling it to machine code, and finally executing it.
  2. Scilla run-time library (SRTL), which the contract code interacts
    with for performing pre-compiled operations, such as interact with
    the blockchain etc.

Design docs for the [VM architecture](https://github.com/Zilliqa/scilla-backend/wiki/Scilla-Backend-Design)
and the planned [interaction with the blockchain](https://github.com/Zilliqa/scilla-backend/wiki/Interaction-of-State-Variables-with-Blockchain)
are available.

## Build and install

The following ubuntu packages must first be installed.
    - `sudo apt-get install llvm-9-dev libboost-dev libboost-test-dev libjsoncpp-dev libboost-test-dev`

We suggest building ScillaVM in a directory that is *not* the source directory.
  * `$git clone https://github.com/Zilliqa/scilla-vm.git`
  * `$cd scilla-vm; mkdir build; cd build`
  * `$cmake ..` configures the project.
    Additional (optional) flags:
    - `-DCMAKE_INSTALL_PREFIX=/where/to/install/scilla-vm`: To specify an install directory other
    than the default (which requires root permissions).
    - `-DLLVM_DIR=/path/to/llvm/install/lib/cmake/llvm`: scilla-vm uses LLVM.
    If your LLVM is installed in a non-default directory, use this flag.
    - `-DCMAKE_BUILD_TYPE=[Debug|Release|RelWithDebInfo|MinSizeRel]`: The default build is `Debug`.
  * `$make` builds the entire project. You can find the built files in `bin/` and `lib/`.
  * `$make install` installs the project.
  We suggest to provide your installation path as described earlier and not install in a system
  directory.

## Developer Notes
All public headers are placed in [include](./include). Source files for the JIT driver are
in [libjitd](./libjitd) while the sources for Scilla RTL are in [libsrtl](./libsrtl).

### Coding Standards
This project uses the [LLVM coding standards](https://llvm.org/docs/CodingStandards.html).

For convenience, the naming convention is summarized below. All names (with allowed exceptions)
must be in [camel case](https://en.wikipedia.org/wiki/Camel_case)
  - Types (structs, classes etc), namespaces and filenames begin with a capital letter.
  - Function and method names being with a small letter.
  - Variable (local, global and class members) names being with a capital letter.
  - Functions that serve as Scilla builtins, accessible from the JIT'ed code
    must start with an `_` and be in [snake case](https://en.wikipedia.org/wiki/Snake_case).

A few other points to note
  - Add function [comments](https://llvm.org/docs/CodingStandards.html#doxygen-use-in-documentation-comments)
  at declaration points rather than definition points. Definition points
  can have more details if necessary.
  - Include system headers first, then library headers and lastly
  project headers. Each category separated by a line. `clang-format`
  takes care of arranging them in alphabetical order.
  - Since this library is designed to be part of an ever running process
  in the Zilliqa network, do not `abort()`, `exit()` or `assert()`.
  Always use the provided `CREATE_ERROR` and `CREATE_ERROR_SLOC` macros.
  This throws an exception (with information on where it was thrown from)
  and can be caught and handled by the blockchain.

To conform to the coding style and good programming practices, CMake targets `clang-format`
and `clang-tidy` are provided, which when run as `make clang-format` and `make clang-tidy`
in the build directory, will auto format all source files.

They can also be manually run from the command line:
  - ```clang-format -style=LLVM -i `find . -name "*.cpp" -o -name "*.h" | xargs```
  - ```clang-tidy `find . -name "*.cpp" -o -name "*.h" | xargs` -p build/```

### Testsuite

The testsuite, built by default (but not installed), is based on `boost_unit_framework`
and can be run from the project root as
```bash
build/testsuite/scilla-testsuite -- --testsuite_src testsuite
```

A few command line options are provided below for quick reference.
See `--help` for all options provided by `boost`. These options must all be provided prior to `--` in the command line.

  - `--list_content`: Lists the testsuite hierarchy.
  - `--run_test` : Run specific tests (as listed by `--list_content`). See details
  [here](https://www.boost.org/doc/libs/1_72_0/libs/test/doc/html/boost_test/runtime_config/test_unit_filtering.html)
  - `--log_level=all`: To enable the full log to be printed.

The `--testsuite_src` option following `--` is a [custom argument](https://www.boost.org/doc/libs/1_70_0/libs/test/doc/html/boost_test/runtime_config/custom_command_line_arguments.html)
who's argument tells the testsuite where to find the test inputs. The test
inputs are present in the `testsuite` directory of the project source.

For convenience a `CMake` target `runtests` has been provided to run the testsuite. This can be
executed as `make runtests` in the build directory.

#### Expression execution tests
The directory `testsuite/expr` contains text LLVM-IR files, all generated by `expr-compiler`.
We maintain the following conventions:
  - Each test file has the input Scilla expression as a comment at the beginning.
  Such a file can be generated using the script `scripts/gen_expr_test.sh`
  by providing the path to Scilla expression file (`.scilexp`) in the
  compiler repository testsuite. (It will implicitly look for the corresponding
  `.gold` file already generated by the compiler in the compiler testsuite).
  - Each test LLVM-IR file (`foo.ll`) must have a corresponding `foo.ll.result`
  file that contains the expected output on executing the code.
  The testsuite will match against it.
