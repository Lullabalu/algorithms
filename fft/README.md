# Polynomial + NTT

Polynomial arithmetic modulo 998244353.

## Features
- addition and subtraction
- multiplication (via NTT)
- division (quotient)
- remainder
- polynomial inverse (mod x^k)

## Representation
- `odds[i]` — coefficient of x^i
- `degree` — number of coefficients

example:
1 + x → {1, 1}

## NTT
- used instead of FFT
- works modulo 998244353
- `invert = false` → direct transform
- `invert = true` → inverse transform

## Notes
- NTT sizes must be powers of two
- inverse exists only if constant term ≠ 0