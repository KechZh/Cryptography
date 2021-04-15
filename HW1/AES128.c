#include <stdio.h>
#include <string.h>
#include <time.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define max_size 20 * 1024 * 1024

void handleErrors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext, char *mode){
    EVP_CIPHER_CTX *ctx;

    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if(strcmp(mode, "ecb") == 0)
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv))
            handleErrors();
    if(strcmp(mode, "cbc") == 0)
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
            handleErrors();
    if(strcmp(mode, "ctr") == 0)
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv))
            handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext, char *mode){
    EVP_CIPHER_CTX *ctx;

    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if(strcmp(mode, "ecb") == 0)
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv))
            handleErrors();
    if(strcmp(mode, "cbc") == 0)
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
            handleErrors();
    if(strcmp(mode, "ctr") == 0)
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv))
            handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main(){

    unsigned char *key;
    unsigned char *iv;

    unsigned char *input = malloc(max_size);
    unsigned char *output = malloc(max_size);

    int input_len;
    int output_len;

    FILE *rp, *wp;
    char filename[32];
    char op;
    char mode[8];

    char str[32];

    printf("Enter key (16 char) : ");
    fgets(str, 32, stdin);
    str[strlen(str) - 1] = 0;
    key = (unsigned char *)str;
    if(strlen(key) != 16){
        printf("Length of key is not 16.\n");
        return 0;
    }

    printf("Enter iv (16 char) : ");
    fgets(str, 32, stdin);
    str[strlen(str) - 1] = 0;
    iv = (unsigned char *)str;
    if(strlen(iv) != 16){
        printf("Length of iv is not 16.\n");
        return 0;
    }

    printf("Enter file name : ");
    fgets(filename, 32, stdin);
    filename[strlen(filename) - 1] = 0;
    if((rp = fopen(filename, "r")) == NULL){
        printf("Cannot find file.\n");
        return 0;
    }

    printf("Enter operation (e or d) : ");
    op = fgetc(stdin);
    fgetc(stdin);
    if(op != 'e' && op != 'd'){
        printf("No operation.\n");
        return 0;
    }

    printf("Enter mode (ecb, cbc or ctr) : ");
    fgets(mode, 8, stdin);
    mode[strlen(mode) - 1] = 0;
    if(strcmp(mode, "ecb") != 0 && strcmp(mode, "cbc") != 0 && strcmp(mode, "ctr") != 0){
        printf("No mode.\n");
        return 0;
    }

    clock_t t1, t2;

    if(op == 'e'){
        wp = fopen("ctext.txt", "w");

        fseek(rp, 0, SEEK_END);
        input_len = ftell(rp);
        fseek(rp, 0, SEEK_SET);

        fread(input, 1, input_len, rp);

        t1 = clock();
        output_len = encrypt(input, input_len, key, iv, output, mode);
        t2 = clock();

        fwrite(output, 1, output_len, wp);

        printf("Encrypt rate : %f MB/s\n", ((double)input_len / (1024 * 1024)) / ((double)(t2 - t1) / CLOCKS_PER_SEC));

        fclose(wp);
    }

    if(op == 'd'){
        wp = fopen("ptext.txt", "w");

        fseek(rp, 0, SEEK_END);
        input_len = ftell(rp);
        fseek(rp, 0, SEEK_SET);

        fread(input, 1, input_len, rp);

        t1 = clock();
        output_len = decrypt(input, input_len, key, iv, output, mode);
        output[output_len] = 0;
        t2 = clock();

        fwrite(output, 1, output_len, wp);

        printf("Decrypt rate : %f MB/s\n", ((double)input_len / (1024 * 1024)) / ((double)(t2 - t1) / CLOCKS_PER_SEC));

        fclose(wp);
    }

    fclose(rp);

    return 0;
}