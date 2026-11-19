# Image Steganography (BMP)

A command-line tool in **C** that hides a secret file inside a 24-bit BMP image by modifying the least significant bits (LSB) of pixel bytes, and extracts it back.

## Features
- Encode: hide a secret file (`.txt`, `.c`, `.sh`) inside a BMP image
- Decode: recover the hidden file from the stego image
- Magic string to confirm an image contains hidden data
- Checks that the image has enough capacity for the secret file

## Concepts used
Bitwise operations, LSB encoding, BMP header structure, binary file handling, structures, command-line arguments.

## Project structure
```
steganography/
├── main.c
├── encode.c / encode.h
├── decode.c / decode.h
├── common.h
├── types.h
├── beautiful.bmp    # sample cover image
├── secret.txt       # sample secret file
└── README.md
```

## Build
```bash
gcc *.c -o stego
```

## Run
Encode:
```bash
./stego -e beautiful.bmp secret.txt stego.bmp
```
Decode:
```bash
./stego -d stego.bmp decoded
```

## How it works
Each secret bit replaces the LSB of one image byte. The magic string, file extension size, extension, file size and then file data are encoded in that order after the 54-byte BMP header.

## Author
DHANUSH - [GitHub](https://github.com/dhanuingit)
