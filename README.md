# Universally Consistent AVX (UCAVX)

WORK IN PROGRESS

UCAVX is a single C header library with asm implementation files. It gives users the capability to help ensure identical program behavior between different CPUs while leveraging SIMD AVX instructions where possible.

## Purpose

SIMD instructions are not only difficult to call from C/C++, but some instructions are inconsistent across CPU implementations between AMD and Intel. Some older Intel CPUs even behave differently on certain instructions relative to newer Intel CPUs.

UCAVX seeks to remedy this by providing a way to quickly, dynamically determine if the CPU's SIMD AVX instructions are identical to the scalar instruction given and set function pointers to either point to the vector instruciton implementation or the scalar instruction implementation.

## License

UCAVX comes with the MIT-0 license and NO WARRENTY. See LICENSE for more details.

Do no evil.

## Credits

Written by Brian Billman. Original repository of Universally Consistent AVX (UCAVX) found at github.com/brian-catcow-b.
