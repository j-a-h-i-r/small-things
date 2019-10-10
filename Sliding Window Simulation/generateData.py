import random

size = 1000
bits = []

for x in range(size):
    r = random.randint(0, 1)
    bits.append(str(r))

with open(r'test_data.txt', 'w') as file:
    file.write("".join(bits))
