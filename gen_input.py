import random, string

random.seed(1)
N = 100000

with open("input.txt", "w", encoding="utf-8") as f:
    for i in range(N):
        f.write(f"user_{i:06d}\n")