# JPEG-like-Compression
It's a jpeg compression algorithm, but it just includes the part of encode and decode, it cannot make a raw image into real form of jpeg image whose side name is .jpg.
QF(Quality Factor) can decides the output quality of the image, if the value is higher, the performance is better.

Limitation of input image:
1. gray scale
2. raw image
3. length：512，width：512

Usage:
1.
  1. image should be in the same place with the main program.
  2. QF(Quality Factor) can only changed by the source code in line 38.
  3. filename can only modified manually in line 2830, 2966, 3044, 3344.
  4. filename in line 2966 and 3044 should be the same.



