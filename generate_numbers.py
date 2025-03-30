import random

def generate_numbers(file_name, count):
    with open(file_name, 'w') as f:
        for _ in range(count):
            f.write(f"{random.randint(1, 1000000)}\n")

# Generate files of different sizes
generate_numbers("random_10000.txt", 10000)
generate_numbers("random_20000.txt", 20000)
generate_numbers("random_100000.txt", 100000)