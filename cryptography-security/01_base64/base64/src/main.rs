mod base64;

fn main() {
    let original = "Man";

    let encoded = base64::encode(original);
    println!("Original: {}", original);
    println!("Encoded:  {}", encoded);

    let decoded = base64::decode(&encoded);
    println!("Decoded:  {}", decoded);
}
