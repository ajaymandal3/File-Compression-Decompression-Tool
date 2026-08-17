# File-Compression-Decompression-Tool

Lossless file compressor built from scratch in C++ using Huffman Coding — works on any file type with zero external dependencies.

## Features

- Compress and decompress any file type — the compressor works on raw bytes, so it isn't limited to plain text
- Fully **lossless** — decompressed output is verified byte-for-byte identical to the original
- Builds the Huffman tree with a min-heap (`std::priority_queue`) and generates binary codes recursively
- Self-contained compressed files — the Huffman tree is serialized into the output file itself, so no external metadata is needed to decompress
- Simple interactive CLI menu, no configuration required
- Clean error handling for missing files, empty files, and corrupted compressed data
- Zero external dependencies — standard C++17 only

## Project Structure

```text
FileCompressionTool/
├── include/
│   ├── Compressor.hpp
│   ├── HuffmanNode.hpp
│   └── HuffmanTree.hpp
├── src/
│   ├── Compressor.cpp
│   ├── HuffmanNode.cpp
│   ├── HuffmanTree.cpp
│   └── main.cpp
├── data/
│   └── sample_input.txt
├── build/
│   └── huffman              # generated binary output
└── README.md
```

Headers are separated from implementation files (`include/` vs `src/`), following standard C++ project conventions. All build artifacts are kept in `build/`, which is meant to be excluded from version control.

## How It Works

1. **Read** the input file as raw bytes (no assumptions about content).
2. **Count** how often each byte value (0–255) occurs.
3. **Build a Huffman tree** using a min-heap: repeatedly merge the two least-frequent nodes until a single root remains. Frequent bytes end up near the root (short codes); rare bytes end up deeper (longer codes).
4. **Generate binary codes** by walking the tree recursively — left = `0`, right = `1`.
5. **Encode** the file by replacing every byte with its code and packing the resulting bits into real bytes.
6. **Serialize the tree** directly into the compressed file, along with the original byte count, so decompression is fully self-contained.
7. **Decompress** by rebuilding the tree from the saved shape, then walking it bit-by-bit — every time a leaf is reached, that's one decoded byte — until the original byte count is reached.

Because Huffman codes are _prefix codes_ (no code is a prefix of another), decoding is always unambiguous, which is what guarantees the process is fully lossless.

## Build

Requires a C++17-compatible compiler (g++, clang++, or MSVC).

```bash
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -o build/huffman src/main.cpp src/HuffmanNode.cpp src/HuffmanTree.cpp src/Compressor.cpp
```

## Run

```bash
./build/huffman
```

```
==================================================
             HUFFMAN FILE COMPRESSOR
==================================================

1. Compress File
2. Decompress File
3. Exit

Enter your choice:
```

Try it on the included sample file:

```text
Enter input file path:
> data/sample_input.txt

Enter output file path:
> build/sample_input.huff
```

Then decompress it back:

```text
Enter compressed file path:
> build/sample_input.huff

Enter output file path:
> build/sample_input_decompressed.txt
```

## Complexity

|       | Complexity | Notes                                                     |
| ----- | ---------- | --------------------------------------------------------- |
| Time  | O(n log k) | n = file size, k = number of distinct byte values (≤ 256) |
| Space | O(n + k)   | file buffer, encoded bit-stream, and the tree/code table  |

## Design Notes

- Compression effectiveness depends on how skewed the input's byte frequency distribution is. Text and uncompressed binaries typically compress well; already-compressed formats (PNG, ZIP) or random data can end up slightly _larger_ than the original, since Huffman coding needs repeating patterns to exploit and there's a small fixed overhead from storing the tree.
- The single-distinct-character edge case (a file made of only one repeated byte) is handled explicitly, since a standard Huffman tree requires at least two leaves to generate a valid code.

## Concepts Demonstrated

`std::priority_queue` (min-heap) · recursion · binary trees · bitwise operations · binary file I/O (`ifstream`/`ofstream`) · `std::unordered_map` · encapsulation and constructors/destructors in C++ classes

## License

MIT
