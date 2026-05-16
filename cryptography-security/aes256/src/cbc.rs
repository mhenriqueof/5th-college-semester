use crate::aes::{key_expansion, encrypt_block, decrypt_block};

// Encrypt plaintext using AES-256 in CBC mode
pub fn cbc_encrypt(plaintext: &[u8], key: &[u8; 32], iv: &[u8; 16]) -> Vec<u8> {
    let expanded_key = key_expansion(key);
    let mut ciphertext = Vec::new();
    let mut prev_block = *iv;

    // Apply PKCS7 padding to the plaintext
    let mut padded = plaintext.to_vec();
    let pad_len = 16 - (plaintext.len() % 16);
    padded.extend(vec![pad_len as u8; pad_len]);

    for chunk in padded.chunks(16) {
        let mut block = [0u8; 16];
        block.copy_from_slice(chunk);

        // CBC: XOR block with previous ciphertext block (or IV for first block)
        for i in 0..16 {
            block[i] ^= prev_block[i];
        }

        let encrypted = encrypt_block(&block, &expanded_key);
        prev_block = encrypted;
        ciphertext.extend_from_slice(&encrypted);
    }
    ciphertext
}

// Decrypt ciphertext using AES-256 in CBC mode
pub fn cbc_decrypt(ciphertext: &[u8], key: &[u8; 32], iv: &[u8; 16]) -> Vec<u8> {
    let expanded_key = key_expansion(key);
    let mut plaintext = Vec::new();
    let mut prev_block = *iv;

    for chunk in ciphertext.chunks(16) {
        let mut block = [0u8; 16];
        block.copy_from_slice(chunk);

        let decrypted = decrypt_block(&block, &expanded_key);

        // CBC: XOR decrypted block with previous ciphertext block (or IV for first block)
        let mut plain_block = [0u8; 16];
        for i in 0..16 {
            plain_block[i] = decrypted[i] ^ prev_block[i];
        }
        prev_block = block;
        plaintext.extend_from_slice(&plain_block);
    }

    // Remove PKCS7 padding
    if let Some(&pad_len) = plaintext.last() {
        let pad_len = pad_len as usize;
        if pad_len <= 16 {
            plaintext.truncate(plaintext.len() - pad_len);
        }
    }
    plaintext
}