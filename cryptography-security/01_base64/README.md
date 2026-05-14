# Base64

Implementation of the Base64 encoding and decoding algorithm from scratch in Rust, without any external libraries.

## About

Base64 is an encoding scheme that converts binary data into a text format using 64 printable ASCII characters. It is widely used in cryptography, data transmission and security protocols.

This project implements the algorithm manually:
- Bitwise operations (shifts and masks)
- Byte manipulation
- Padding logic (`=`)

## How to Run

```bash
cargo run
```

## Tests

```bash
cargo test
```

All 7 tests cover:
- Basic encoding and decoding
- Padding with one `=`
- Padding with two `==`
- Full roundtrip (encode → decode)

## Project Structure

```
src/
├── main.rs      # Entry point and usage example
└── base64.rs    # Base64 algorithm and tests
```

## How It Works

**Encoding:**
1. Convert the input string to bytes
2. Group bytes in chunks of 3 (24 bits)
3. Split into four 6-bit indexes
4. Map each index to the Base64 alphabet
5. Add `=` padding if needed

**Decoding:**
1. Group characters in chunks of 4
2. Convert each character to its 6-bit index
3. Combine into 24 bits
4. Extract the original 3 bytes

---

Thanks!
