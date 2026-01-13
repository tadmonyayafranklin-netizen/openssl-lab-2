Here is a complete **README.md** you can put in your project folder.

---

# 🔐 AES-128-CBC Key Cracking using OpenSSL EVP

This project demonstrates how to brute-force an AES-128-CBC encryption key using the **OpenSSL EVP cryptographic API**.
The key is known to be an English word padded with spaces (`0x20`) to 16 bytes.

---

## 📌 Problem Description

We are given:

**Plaintext**

```
This is a top secret.
```

**Ciphertext (hex)**

```
8d20e5056a8d24d0462ce74e4904c1b5
13e10d1df4a2ef2ad4540fae1ca0aaf9
```

**Encryption details**

* Algorithm: AES-128-CBC
* IV: all zeros (16 bytes)
* Key: an English word padded with spaces to 16 bytes

Your task is to recover the original English word.

---

## 🧰 Requirements

You need the OpenSSL development libraries.

Install them on Ubuntu / WSL:

```bash
sudo apt update
sudo apt install libssl-dev
```

Verify installation:

```bash
ls /usr/include/openssl/evp.h
```

---

## 📂 Files

| File        | Description                                  |
| ----------- | -------------------------------------------- |
| `crack.c`   | Brute-force AES key search using OpenSSL EVP |
| `words.txt` | English dictionary file                      |

---

## 📥 Download the dictionary

```bash
wget https://raw.githubusercontent.com/dwyl/english-words/master/words.txt
```

---

## ⚙️ Compilation

Compile using OpenSSL:

```bash
gcc crack.c -o crack -lcrypto
```

If that fails, try:

```bash
gcc crack.c -o crack $(pkg-config --cflags --libs openssl)
```

---

## ▶️ Run the program

```bash
./crack
```

---

## 🧪 What the program does

For each word in the dictionary:

1. Pads the word with spaces (`0x20`) to 16 bytes
2. Encrypts the known plaintext using AES-128-CBC
3. Compares the result with the given ciphertext
4. Prints the key when a match is found

---

## ✅ Example Output

```
KEY FOUND: "secret"
Full AES key = "secret          "
```

(The spaces are real padding bytes.)

---

## 🔐 Why this works

AES-CBC with a known IV (all zeros) and known plaintext allows verification of any guessed key:

```
Encrypt(plaintext, key) == known_ciphertext
```

Since the key is from a dictionary, brute force becomes feasible.

---

## 📎 Notes

* The EVP API is used, not OpenSSL command-line tools (per assignment rules).
* Padding uses `0x20` (space), not zero bytes.
* The ciphertext is compared byte-for-byte for correctness.


