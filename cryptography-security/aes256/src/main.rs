mod aes;
mod cbc;

#[cfg(test)]
mod tests;

use sha2::{Sha256, Digest};
use cbc::{cbc_encrypt, cbc_decrypt};

fn main() {
    let password = "my_secret_password";
    let plaintext = "no ceu tem pao(interrogacao)";

    // Derive a 32-byte key from the password using SHA-256
    let mut hasher = Sha256::new();
    hasher.update(password.as_bytes());
    let key_vec = hasher.finalize();
    let mut key = [0u8; 32];
    key.copy_from_slice(&key_vec);

    // IV: 16 bytes fixed for demonstration purposes
    let iv: [u8; 16] = [
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
    ];

    println!("Original:  {}", plaintext);

    // Encrypt
    let encrypted = cbc_encrypt(plaintext.as_bytes(), &key, &iv);
    let hex: String = encrypted.iter().map(|b| format!("{:02x}", b)).collect();
    println!("Encrypted: {}", hex);

    // Decrypt
    let decrypted_bytes = cbc_decrypt(&encrypted, &key, &iv);
    let decrypted = String::from_utf8(decrypted_bytes).unwrap();
    println!("Decrypted: {}", decrypted);
}
