# cut-cpp

A C++ implementation of the `cut` command for extracting fields and characters from text.

# Building and installing

See the [BUILDING](BUILDING.md) document.

## Quick Start

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
./build/cut-cpp --help
```

## Usage

```bash
# Extract fields 1 and 3 (tab-delimited)
echo -e "one\ttwo\tthree" | ./build/cut-cpp -f 1,3
# Output: one  three

# Extract with custom delimiter
echo "a:b:c:d" | ./build/cut-cpp -d : -f 2,4
# Output: b:d

# Extract characters 1-5
echo "hello world" | ./build/cut-cpp -c 1-5
# Output: hello

# From file
./build/cut-cpp -f 1,3 data.txt

# Suppress lines without delimiter
echo -e "a,b,c\nd" | ./build/cut-cpp -d , -f 1 -s
# Output: a
```

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.

# Licensing

This project uses dual licensing:

1. **Open Source License**: GNU General Public License v3.0 (GPL-3.0)
   - See [LICENSE](LICENSE) for details
   - Free for open source projects and personal use

2. **Commercial License**
   - See [COMMERCIAL_LICENSE](COMMERCIAL_LICENSE) for details
   - Required for commercial use, including integration into commercial products
   - Contact: admin@nerdev.in

If you use this software in a commercial product or service, you must
obtain a commercial license.
