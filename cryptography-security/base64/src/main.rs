mod base64;

fn main() {
    let original = "Man";

    // Encode the original string to Base64
    let encoded = base64::encode(original);
    println!("Original: {}", original);
    println!("Encoded:  {}", encoded);

    // Decode the Base64 string back to the original
    let decoded = base64::decode(&encoded);
    println!("Decoded:  {}", decoded);
}
