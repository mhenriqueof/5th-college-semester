#[cfg(test)]
mod tests {
    use sha2::{Sha256, Digest};
    use crate::aes::{key_expansion, encrypt_block, decrypt_block, sub_bytes, sub_bytes_inv, shift_rows, shift_rows_inv};
    use crate::cbc::{cbc_encrypt, cbc_decrypt};

    // Helper: derive a 32-byte key from a password using SHA-256
    fn make_key(password: &str) -> [u8; 32] {
        let mut hasher = Sha256::new();
        hasher.update(password.as_bytes());
        let result = hasher.finalize();
        let mut key = [0u8; 32];
        key.copy_from_slice(&result);
        key
    }

    // Helper: fixed IV for testing
    fn make_iv() -> [u8; 16] {
        [
            0x00, 0x01, 0x02, 0x03,
            0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b,
            0x0c, 0x0d, 0x0e, 0x0f,
        ]
    }

    #[test]
    fn test_sub_bytes_roundtrip() {
        let mut state = [0u8; 16];
        for i in 0..16 { state[i] = i as u8; }
        let original = state;
        sub_bytes(&mut state);
        sub_bytes_inv(&mut state);
        assert_eq!(state, original);
    }

    #[test]
    fn test_shift_rows_roundtrip() {
        let mut state = [0u8; 16];
        for i in 0..16 { state[i] = i as u8; }
        let original = state;
        shift_rows(&mut state);
        shift_rows_inv(&mut state);
        assert_eq!(state, original);
    }

    #[test]
    fn test_encrypt_decrypt_block() {
        let key = make_key("test_password");
        let expanded = key_expansion(&key);
        let block: [u8; 16] = [
            0x00, 0x11, 0x22, 0x33,
            0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xaa, 0xbb,
            0xcc, 0xdd, 0xee, 0xff,
        ];
        let encrypted = encrypt_block(&block, &expanded);
        let decrypted = decrypt_block(&encrypted, &expanded);
        assert_eq!(decrypted, block);
    }

    #[test]
    fn test_cbc_roundtrip() {
        let key = make_key("minha_senha_secreta");
        let iv = make_iv();
        let plaintext = b"Henrique AES256!";
        let encrypted = cbc_encrypt(plaintext, &key, &iv);
        let decrypted = cbc_decrypt(&encrypted, &key, &iv);
        assert_eq!(decrypted, plaintext);
    }

    #[test]
    fn test_cbc_different_keys() {
        let key1 = make_key("senha_um");
        let key2 = make_key("senha_dois");
        let iv = make_iv();
        let plaintext = b"mensagem secreta";
        let encrypted = cbc_encrypt(plaintext, &key1, &iv);
        let decrypted = cbc_decrypt(&encrypted, &key2, &iv);
        assert_ne!(decrypted, plaintext.to_vec());
    }

    #[test]
    fn test_cbc_different_ivs() {
        let key = make_key("mesma_senha");
        let iv1 = make_iv();
        let iv2 = [0xff; 16];
        let plaintext = b"mensagem secreta";
        let enc1 = cbc_encrypt(plaintext, &key, &iv1);
        let enc2 = cbc_encrypt(plaintext, &key, &iv2);
        assert_ne!(enc1, enc2);
    }
}
