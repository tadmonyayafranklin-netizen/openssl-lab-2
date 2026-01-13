#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

#define PLAINTEXT "This is a top secret."
#define PLAINTEXT_LEN 21

unsigned char ciphertext_hex[] =
"53616c7465645f5f43f821cf72591120150e4caadf447f6127be828fbaf0820f3736811f32dc2adeb0bb9c2a0259f97f";

void hex_to_bytes(const char *hex, unsigned char *out, int len) {
    for (int i = 0; i < len; i++)
        sscanf(hex + 2*i, "%2hhx", &out[i]);
}

int main() {
    FILE *dict = fopen("words.txt", "r");
    if (!dict) {
        perror("dictionary");
        return 1;
    }

    int cipher_len = strlen((char*)ciphertext_hex) / 2;
    unsigned char cipher[cipher_len];
    hex_to_bytes((char*)ciphertext_hex, cipher, cipher_len);

    // OpenSSL salt is bytes 8..15
    unsigned char *salt = cipher + 8;
    unsigned char *real_cipher = cipher + 16;
    int real_cipher_len = cipher_len - 16;

    char word[64];

    while (fgets(word, sizeof(word), dict)) {
        word[strcspn(word, "\r\n")] = 0;
        int len = strlen(word);
        if (len > 16) continue;

        // pad password to 16 bytes with spaces
        unsigned char password[16];
        memset(password, 0x20, 16);
        memcpy(password, word, len);

        unsigned char key[32], iv[16];

        EVP_BytesToKey(
            EVP_aes_256_cbc(),
            EVP_md5(),
            salt,
            password,
            16,
            1,
            key,
            iv
        );

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        unsigned char out[128];
        int outlen, finallen;

        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
        EVP_DecryptUpdate(ctx, out, &outlen, real_cipher, real_cipher_len);
        EVP_DecryptFinal_ex(ctx, out + outlen, &finallen);

        int total = outlen + finallen;

        if (total == PLAINTEXT_LEN &&
            memcmp(out, PLAINTEXT, PLAINTEXT_LEN) == 0) {
            printf("KEY FOUND: %s\n", word);
            break;
        }

        EVP_CIPHER_CTX_free(ctx);
    }

    fclose(dict);
    return 0;
}
