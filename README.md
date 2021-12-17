# huffman-compressor
:mortar_board: Tool for compression and decompression of binary files using Huffman algorithm.

:thumbsup: Tested on macOS BigSur 11.4.

## Table of contents 
  * [Build](#build)
  * [Compress files](#compress-files)
  * [Decompress files](#decompress-files)

## Build
```bash
git clone https://github.com/golju/huffman-compressor.git
cd huffman-compressor
make
```

## Compress files
To compress file use `-c` key and write `input` and `output` paths:
```bash
./huffman -c data/voina-i-mir.txt data/compressed-voina-i-mir.bin
```
Compare the sizes:
```
voina-i-mir.txt : 3 675 057 B
compressed-voina-i-mir.bin : 2 271 747 B
```

## Decompress files
To decompress file write `input` and `new_output` paths:
```bash
./huffman data/compressed-voina-i-mir.bin data/decompressed-voina-i-mir.txt
```
To compare two files binary:
```bash
diff data/voina-i-mir.txt data/decompressed-voina-i-mir.txt
```
