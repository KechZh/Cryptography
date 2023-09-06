str = 'ABCD 1234\n'
text = [str for i in range(110 * 1024 * 1024 // len(str))]

f = open('Cryptography/HW1/Text/text.txt', mode = 'w')
f.write(''.join(text))
f.close()
