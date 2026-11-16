# Universally Consistent AVX (UCX)

WORK IN PROGRESS

UCX is a single C header library with asm implementation files. It gives users the capability to help ensure identical program behavior between different CPUs while leveraging SIMD AVX instructions where possible.

## Purpose

SIMD instructions are not only difficult to call from C/C++, but some instructions are inconsistent across CPU implementations between AMD and Intel. Some older Intel CPUs even behave differently on certain instructions relative to newer Intel CPUs.

UCX seeks to remedy this by providing a way to quickly, dynamically determine if the CPU's SIMD AVX instructions are identical to the scalar instruction given and set function pointers to either point to the vector instruciton implementation or the scalar instruction implementation.

## License

UCX comes with NO LICENSE and NO WARRENTY. The code can be copied and used freely however anyone chooses. There is a section in each source file that asks nicely to be left in wherever UCX is found, but it is not required.

Do no evil.

## Credits

Written by Brian Billman. Original repository of Universally Consistent AVX (UCX) found at github.com/brian-catcow-b.
