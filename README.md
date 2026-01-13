# 🔐 AES‑256‑CBC Dictionary Attack (OpenSSL Compatible)

This project demonstrates how to recover an AES encryption key when the key is a short English word, using a dictionary attack against OpenSSL‑generated ciphertext.

The ciphertext was produced using:

```
openssl enc -aes-256-cbc -salt -pass pass:<word>
```

The password is:

* An English word
* Shorter than 16 characters
* Padded with ASCII spaces (0x20) to 16 bytes before being used
* Processed through OpenSSL’s EVP_BytesToKey KDF (MD5 + salt)

---

## 📄 Given Data

**Plaintext**

```
This is a top secret.
```

**Ciphertext (hex)**

```
53616c7465645f5f43f821cf72591120150e4caadf447f6127be828fbaf0820f3736811f32dc2adeb0bb9c2a0259f97f
```

The ciphertext begins with:

```
53616c7465645f5f = "Salted__"
```

This indicates OpenSSL salted encryption was used.

---

## 🔑 Cryptographic Model

OpenSSL performs the following steps:

```
Salt = bytes 8..15 of ciphertext
Key, IV = EVP_BytesToKey(
              AES‑256‑CBC,
              MD5,
              Password (padded with spaces),
              Salt
          )
```

Then AES‑256‑CBC is applied.

Our cracking program replicates this process exactly.

---

## 🧠 Attack Strategy

We:

1. Extract the salt from the ciphertext
2. Try each English word from a dictionary
3. Pad it to 16 bytes with spaces
4. Use EVP_BytesToKey to derive key and IV
5. Decrypt the ciphertext
6. Check whether the result matches
   `This is a top secret.`

When it matches, the key is found.

---

## 🧪 Build Instructions

Make sure OpenSSL development headers are installed.

### On Linux / WSL

```bash
sudo apt install libssl-dev
```

Compile the Makefile using:

```bash
make
```

---

## 📁 Files

| File                | Description                                   |
| ------------------- | --------------------------------------------- |
| `crack.c`           | C program that performs the dictionary attack |
| `words.txt` | Word list used for brute‑force                |
| `README.md`         | This file                                     |

---

## ▶ Running the attack

```bash
./enc
```

When the correct key is found, it will print:

```
KEY FOUND: <word>
```

---

## 🔍 Verifying with OpenSSL

Once the key is found (for example `secret`), verify:

```bash
echo "53616c7465645f5f43f821cf72591120150e4caadf447f6127be828fbaf0820f3736811f32dc2adeb0bb9c2a0259f97f" | xxd -r -p > ct.bin

openssl enc -aes-256-cbc -d -in ct.bin -pass pass:secret
```

Output:

```
This is a top secret.
```

---

## 🧩 Why this works

OpenSSL’s `enc` command does NOT directly use the password as the AES key.
It uses a **weak MD5‑based key derivation function**, which makes short dictionary words vulnerable to brute‑force attacks.

This project demonstrates that weakness.

---

## ⚠ Security Lesson

Never use:

```
openssl enc -aes-256-cbc -pass pass:password
```

for real security.

Always use:

* PBKDF2
* bcrypt
* scrypt
* Argon2

