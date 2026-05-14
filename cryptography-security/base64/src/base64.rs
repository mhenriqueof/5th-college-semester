// The 64 printable ASCII characters used as the Base64 alphabet
const ALPHABET: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// The padding character used when input length is not a multiple of 3
const PADDING: char = '=';

// Returns the index of a Base64 character in the alphabet (0–63)
fn pos(c: u8) -> u32 {
    ALPHABET.iter().position(|&x| x == c).unwrap() as u32
}

// Encodes a string into Base64 format
pub fn encode(input: &str) -> String {
    let bytes = input.as_bytes();
    let mut result = String::new();

    // Process input in chunks of 3 bytes (24 bits)
    for chunk in bytes.chunks(3) {
        // Read up to 3 bytes, filling missing ones with 0
        let b0 = chunk[0] as u32;
        let b1 = if chunk.len() > 1 {chunk[1] as u32} else {0};
        let b2 = if chunk.len() > 2 {chunk[2] as u32} else {0};

        // Combine the 3 bytes into a single 24-bit number
        let combined = (b0 << 16) | (b1 << 8) | b2;

        // Extract four 6-bit indexes and map them to the alphabet
        result.push(ALPHABET[((combined >> 18) & 0x3F) as usize] as char);
        result.push(ALPHABET[((combined >> 12) & 0x3F) as usize] as char);

        // Add padding if the chunk has less than 3 bytes
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

// Decodes a Base64 encoded string back to its original form
pub fn decode(input: &str) -> String {
    let bytes = input.as_bytes();
    let mut result = Vec::new();

    // Process input in chunks of 4 Base64 characters (24 bits)
    for chunk in bytes.chunks(4) {
        // Convert each Base64 character back to its 6-bit index
        let b0 = pos(chunk[0]);
        let b1 = pos(chunk[1]);
        let b2 = if chunk.len() > 2 && chunk[2] != b'=' {pos(chunk[2])} else {0};
        let b3 = if chunk.len() > 3 && chunk[3] != b'=' {pos(chunk[3])} else {0};

        // Combine the four 6-bit indexes into a single 24-bit number
        let combined = (b0 << 18) | (b1 << 12) | (b2 << 6) | b3;

        // Extract the original 3 bytes from the 24-bit number
        result.push(((combined >> 16) & 0xFF) as u8);

        // Skip bytes that were added as padding during encoding
        if chunk.len() > 2 && chunk[2] != b'=' {
            result.push(((combined >> 8) & 0xFF) as u8);
        }
        if chunk.len() > 3 && chunk[3] != b'=' {
            result.push((combined & 0xFF) as u8);
        }
    }
    // Convert the byte vector back to a UTF-8 string
    String::from_utf8(result).unwrap()
}

// Tests
#[cfg(test)]
mod tests {
    use super::*;

    // Basic encoding with no padding needed
    #[test]
    fn test_encode_basic() {
        assert_eq!(encode("Man"), "TWFu");
    }

    // Encoding with one padding character
    #[test]
    fn test_encode_padding_one() {
        assert_eq!(encode("Ma"), "TWE=");
    }

    // Encoding with two padding characters
    #[test]
    fn test_encode_padding_two() {
        assert_eq!(encode("M"), "TQ==");
    }

    // Basic decoding with no padding
    #[test]
    fn test_decode_basic() {
        assert_eq!(decode("TWFu"), "Man");
    }

    // Decoding with one padding character
    #[test]
    fn test_decode_padding_one() {
        assert_eq!(decode("TWE="), "Ma");
    }

    // Decoding with two padding characters
    #[test]
    fn test_decode_padding_two() {
        assert_eq!(decode("TQ=="), "M");
    }

    // Full roundtrip: encoded string must decode back to the original
    #[test]
    fn test_roundtrip() {
        let original = "Henrique";
        assert_eq!(decode(&encode(original)), original);
    }
}