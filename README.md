# FileCompressionAlgo — Lossless File Compression via Huffman Coding

A C++ implementation of **Huffman Coding**, a greedy algorithm that compresses text by replacing fixed-length ASCII characters with variable-length binary codes, achieving lossless data compression.

---

## Demo / Architecture Diagram

```
Input Text: "aabbbcccc"
                │
                ▼
  ┌─────────────────────────┐
  │  Build Frequency Table  │
  │  a:2  b:3  c:4          │
  └────────────┬────────────┘
               │
               ▼
  ┌─────────────────────────┐
  │  Min-Heap (Priority Q)  │
  │  [a:2] [b:3] [c:4]      │
  └────────────┬────────────┘
               │
               ▼
  ┌─────────────────────────┐
  │   Build Huffman Tree    │
  │         [*:9]           │
  │        /      \         │
  │     [*:5]    [c:4]      │
  │    /     \              │
  │  [a:2]  [b:3]           │
  └────────────┬────────────┘
               │
               ▼
  ┌─────────────────────────┐
  │  Assign Bit Codes       │
  │  c → 1   b → 01         │
  │  a → 00                 │
  └────────────┬────────────┘
               │
               ▼
  ┌─────────────────────────┐
  │  Encode + Pack to Bytes │
  │  "aabbbcccc"            │
  │  → 00 00 01 01 01 1 1 1 1│
  │       → byte array      │
  └─────────────────────────┘
```

---

## Why I Built This

Standard files store every character as an 8-bit ASCII value, regardless of how often it appears. That wastes space — a character appearing thousands of times uses the same bit-width as one that appears once. Huffman Coding solves this by assigning **shorter codes to more frequent characters** and longer codes to rarer ones, shrinking the overall bit count and reducing file size without losing any data.

---

## Key Technical Highlights

- **Greedy min-heap construction** — a `std::priority_queue` (min-heap) repeatedly extracts the two lowest-frequency nodes and merges them, building an optimal prefix-free binary tree in O(n log n).
- **Prefix-free codes** — codes stored in an `unordered_map<char, string>` are guaranteed collision-free because every character sits only at a leaf node of the Huffman tree.
- **Bit-level packing** — the encoded bit-string is padded to the nearest byte boundary and a leading byte records the exact padding length, enabling correct decoding later.
- **`std::bitset`-powered serialisation** — 8-character substrings of the encoded bit-string are converted to `uint8_t` bytes via `std::bitset<8>`, producing a compact binary byte array ready for file I/O.
- **Single-pass frequency count** — character frequencies are tallied in one O(n) loop over the input, keeping pre-processing minimal before tree construction begins.

---

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++17 |
| Core Data Structures | `std::priority_queue`, `std::unordered_map` |
| Bit Manipulation | `std::bitset<8>` |
| Compression Algorithm | Huffman Coding (greedy) |
| Build Toolchain | g++ / any C++17-compliant compiler |

---

## How to Run Locally

### Prerequisites
- g++ (GCC 7+) or any C++17-compliant compiler

### Build

```bash
# Clone the repository
git clone https://github.com/deepgodhani/FileCompressionAlgo.git
cd FileCompressionAlgo

# Compile
g++ -std=c++17 -O2 -o huffman huffman.cpp
```

### Run

```bash
./huffman
```

> **Note:** The `main()` function is currently a stub. To compress a file, instantiate the `Huffman` class, set the `text` member to your input string, and call `compression()`:
>
> ```cpp
> Huffman h("output.bin");
> h.text = "your input text here";
> h.compression();
> ```

---

## Architecture Overview

The project is structured around two classes:

| Class / Struct | Responsibility |
|---|---|
| `Node` | A single node in the Huffman binary tree (holds character, frequency, left/right child pointers) |
| `cmp` | Custom comparator for the min-heap — orders nodes by ascending frequency |
| `Huffman` | Orchestrates the full pipeline: frequency counting → heap building → tree construction → code generation → encoding → byte serialisation |

**Pipeline (inside `Huffman`):**

1. `freqFromText()` — counts character frequencies from input text.
2. `buildHeap()` — pushes all `Node*` objects into the min-heap.
3. `buildBinaryTree()` — merges nodes until a single root remains.
4. `bitsFromTree()` — DFS traversal assigns `'0'`/`'1'` codes to each leaf.
5. `encodeTxt()` — replaces each character with its bit code, pads to 8-bit boundary, prepends padding length byte.
6. `buildByteArray()` — converts the padded bit-string into a `vector<uint8_t>` for binary file output.

---

## Known Limitations / What I'd Improve

- **No decompression yet** — the decoder (which needs the Huffman tree or code table stored alongside the compressed data) is not implemented.
- **No actual file I/O** — `buildByteArray()` returns a byte vector but never writes it to disk; adding `std::ofstream` in binary mode would complete the pipeline.
- **`main()` is a stub** — there is no CLI interface; adding argument parsing (e.g. `./huffman compress input.txt output.bin`) would make the tool usable without modifying source code.
- **Memory management** — tree nodes are allocated with `new` but never freed; switching to `std::unique_ptr` or an arena allocator would eliminate memory leaks.
- **Single-character input edge case** — if the input contains only one unique character, the tree has a single node with no children; `buildBits()` would produce an empty code for it, requiring a special-case handler.
- **Large file performance** — the entire file is read into a `std::string`; streaming chunk-based processing would be needed for very large files.
