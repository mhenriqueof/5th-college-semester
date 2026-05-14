const ALPHABET: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const PADDING: char = '=';

fn pos(c: u8) -> u32 {
    ALPHABET.iter().position(|&x| x == c).unwrap() as u32
}

pub fn encode(input: &str) -> String {
    let bytes = input.as_bytes();
    let mut result = String::new();

    for chunk in bytes.chunks(3) {
        let b0 = chunk[0] as u32;
        let b1 = if chunk.len() > 1 {chunk[1] as u32} else {0};
        let b2 = if chunk.len() > 2 {chunk[2] as u32} else {0};

        let combined = (b0 << 16) | (b1 << 8) | b2;

        result.push(ALPHABET[((combined >> 18) & 0x3F) as usize] as char);
        result.push(ALPHABET[((combined >> 12) & 0x3F) as usize] as char);

        if chunk.len() > 1 {
            result.push(ALPHABET[((combined >> 6) & 0x3F) as usize] as char);
        } else {
            result.push(PADDING);
        }

        if chunk.len() > 2 {
            result.push(ALPHABET[(combined & 0x3F) as usize] as char);
        } else {
            result.push(PADDING);
        }
    }
    result
}

pub fn decode(input: &str) -> String {
    let bytes = input.as_bytes();
    let mut result = Vec::new();

    for chunk in bytes.chunks(4) {
        let b0 = pos(chunk[0]);
        let b1 = pos(chunk[1]);
        let b2 = if chunk.len() > 2 && chunk[2] != b'=' {pos(chunk[2])} else {0};
        let b3 = if chunk.len() > 3 && chunk[3] != b'=' {pos(chunk[3])} else {0};

        let combined = (b0 << 18) | (b1 << 12) | (b2 << 6) | b3;

        result.push(((combined >> 16) & 0xFF) as u8);

        if chunk.len() > 2 && chunk[2] != b'=' {
            result.push(((combined >> 8) & 0xFF) as u8);
        }
        if chunk.len() > 3 && chunk[3] != b'=' {
            result.push((combined & 0xFF) as u8);
        }
    }
    String::from_utf8(result).unwrap()
}

// Tests
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_encode_basic() {
        assert_eq!(encode("Man"), "TWFu");
    }

    #[test]
    fn test_encode_padding_one() {
        assert_eq!(encode("Ma"), "TWE=");
    }

    #[test]
    fn test_encode_padding_two() {
        assert_eq!(encode("M"), "TQ==");
    }

    #[test]
    fn test_decode_basic() {
        assert_eq!(decode("TWFu"), "Man");
    }

    #[test]
    fn test_decode_padding_one() {
        assert_eq!(decode("TWE="), "Ma");
    }

    #[test]
    fn test_decode_padding_two() {
        assert_eq!(decode("TQ=="), "M");
    }

    #[test]
    fn test_roundtrip() {
        let original = "Henrique";
        assert_eq!(decode(&encode(original)), original);
    }
}