from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding
import time

ori_path = 'Cryptography/HW1/Text/text.txt'
ct_path = 'Cryptography/HW1/Text/ctext.txt'
pt_path = 'Cryptography/HW1/Text/ptext.txt'

key = b'01234567890123456789012345678901'
iv = b'0123456789012345'

def encrypt(plaintext, mode, key, iv):
    if mode == 1:
        cipher = Cipher(algorithms.AES(key), modes.CBC(iv))
    if mode == 2:
        cipher = Cipher(algorithms.AES(key), modes.CTR(iv))
    if mode == 3:
        cipher = Cipher(algorithms.ChaCha20(key, iv), None)

    encryptor = cipher.encryptor()
    padder = padding.PKCS7(256).padder()

    plaintext = plaintext.encode()

    plaintext = padder.update(plaintext) + padder.finalize()
    ciphertext = encryptor.update(plaintext) + encryptor.finalize()

    return ciphertext

def decrypt(ciphertext, mode, key, iv):
    if mode == 1:
        cipher = Cipher(algorithms.AES(key), modes.CBC(iv))
    if mode == 2:
        cipher = Cipher(algorithms.AES(key), modes.CTR(iv))
    if mode == 3:
        cipher = Cipher(algorithms.ChaCha20(key, iv), None)

    decryptor = cipher.decryptor()
    unpadder = padding.PKCS7(256).unpadder()

    plaintext = decryptor.update(ciphertext) + decryptor.finalize()
    plaintext = unpadder.update(plaintext) + unpadder.finalize()

    plaintext = plaintext.decode()

    return plaintext

def main():
    while True:
        op = input('E : Encrypt \nD : Decrypt \nEnter operation : ')
        mode = int(input('1 : AES-CBC \n2 : AES-CTR \n3 : ChaCha20 \nEnter mode : '))

        if (op == 'E' or op == 'D') and (mode >= 1 and mode <= 3):
            break

    if op == 'E':
        f = open(ori_path, mode = 'r')
        plaintext = f.read()
        f.close()

        start_time = time.time()
        ciphertext = encrypt(plaintext, mode, key, iv)
        end_time = time.time()

        print('Encrypt Success')
        print('Encrypt Rate : ' + str((len(plaintext) / (1024 * 1024)) / (end_time - start_time)) + ' MB/s')

        f = open(ct_path, mode = 'wb')
        f.write(ciphertext)
        f.close()

    if op == 'D':
        f = open(ct_path, mode = 'rb')
        ciphertext = f.read()
        f.close()

        start_time = time.time()
        plaintext = decrypt(ciphertext, mode, key, iv)
        end_time = time.time()

        print('Decrypt Success')
        print('Decrypt Rate : ' + str((len(ciphertext) / (1024 * 1024)) / (end_time - start_time)) + ' MB/s')

        f = open(pt_path, mode = 'w')
        f.write(plaintext)
        f.close()

if __name__ == '__main__':
    main()