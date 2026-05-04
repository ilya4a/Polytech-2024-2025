# Polytech‑2024‑2025

1. [**Digger**](#digger) – arcade tile‑based game (C++20, SDL2)
2. [**RSA Encryption**](#rsa-encryption-c-gmp) – cryptographic library / CLI tool (C, GMP)

Each project has its own build instructions and dependencies. See the corresponding sections below.

---

# Digger

Simplified arcade tile‑based game where you collect diamonds and escape from enemies.  
Written in C++20 with SDL2.

## Features

- **Tile‑map loaded from a text file** – walls, dirt, diamonds, player and enemy spawn points.
- **Enemy** – priority‑based decision making. Enemies chase the player, speed up when getting close.
- **Diamond collection** – increases score, displayed on screen.
- **Bullets** – destroy dirt tiles and can be used against enemies.
- **Linux only** – built with CMake, SDL2, SDL_ttf, SDL_image (and nlohmann_json).

![Gameplay demo](diger/assets/demo.gif)

## Build

```bash
cmake -B build && cmake --build build
```

---
# RSA Encryption (C + GMP)

A compact implementation of the RSA algorithm for key generation, encryption, and decryption using the GNU MP (GMP) library.
## Features

- Generate 1024‑bit RSA key pairs (fixed public exponent `e = 65537`)
- Store keys in a plain text file (`keys.txt`) indexed by an integer ID
- Load keys by ID from the file
- Encrypt a string (message) into a large integer
- Decrypt the integer back to the original string
- Interactive console menu (generate keys, encrypt, decrypt)
- Built‑in test routine to verify correctness

## Algorithms used

- Miller–Rabin primality test
- Modular exponentiation via square‑and‑multiply (`powm`)
- Extended Euclidean algorithm for modular inverse
- Random prime generation for RSA modulus `n = p·q`

All big‑number operations are handled by **GMP** (`mpz_t`, `mpz_import`, `mpz_export`, etc.).

## Examples

```text

Keys are stored as plain text, one line per key:


1 - code massage; 2 - decode massage; 3 - create new key; 0 - exit
1

enter key_id: 123

enter massage(until &): Hello, world!&

ok

codded massage:
119439663550585517199147749058874816998251429045655...
1 - code massage; 2 - decode massage; 3 - create new key; 0 - exit
2

enter key_id: 123

enter massage: 119439663550585517199147749058874816...

decoded massage: Hello, world!
1 - code massage; 2 - decode massage; 3 - create new key; 0 - exit
```