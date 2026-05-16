# AES-256

Implementation of the AES-256 encryption and decryption algorithm from scratch in Rust, using CBC mode of operation, without any external cryptographic libraries.

## About

AES (Advanced Encryption Standard) is the most widely used symmetric encryption algorithm in the world. This project implements AES-256 manually to demonstrate understanding of:

- S-Box substitution and its inverse
- ShiftRows and its inverse
- MixColumns using Galois Field GF(2⁸) arithmetic
- Key expansion (key schedule) for 256-bit keys
- CBC (Cipher Block Chaining) mode with PKCS7 padding
- Key derivation from a password using SHA-256

## How to Run

```bash
cargo run
```

## Tests

```bash
cargo test
```

All 6 tests cover:

- SubBytes roundtrip (encrypt → decrypt)
- ShiftRows roundtrip (encrypt → decrypt)
- Single block encrypt and decrypt
- Full CBC roundtrip (encrypt → decrypt)
- Decryption with wrong key produces incorrect output
- Same plaintext with different IVs produces different ciphertexts

## Project Structure

```
src/
├── main.rs      # Entry point: key derivation, encryption and decryption example
├── aes.rs       # AES-256 core algorithm (SubBytes, ShiftRows, MixColumns, KeyExpansion)
├── cbc.rs       # CBC mode of operation with PKCS7 padding
└── tests.rs     # Automated tests
```

## How It Works

**Key Derivation:**
The user password is hashed with SHA-256 to produce a 32-byte key suitable for AES-256.

**Encryption (CBC mode):**
1. Apply PKCS7 padding to the plaintext
2. For each 16-byte block, XOR with the previous ciphertext block (or IV for the first block)
3. Encrypt the result using AES-256
4. The output of each block becomes the input XOR for the next

**Decryption (CBC mode):**
1. For each 16-byte block, decrypt using AES-256
2. XOR the result with the previous ciphertext block (or IV for the first block)
3. Remove PKCS7 padding from the final block

**AES-256 Round Structure (14 rounds):**
1. AddRoundKey
2. SubBytes
3. ShiftRows
4. MixColumns (skipped in the final round)
5. AddRoundKey

---

Thanks!
