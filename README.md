# RSA KeyGen
A demonstratively key-gen for rsa encryption.

## Build
**dependencies**: c++20, make, git

```bash
git clone https://github.com/tomtamtam/rsa-keygen.git
cd rsa-keygen
make
```

## How to use
```cpp
./bin/rsa-keygen
```
With owh **path**:
```cpp
./bin/rsa-keygen ~/keys/test-key
```
**Flags**:
- h / -help             | help with commands
- -param value | set param [min, max, fermat_repeat] to given value

e.g.:
```cpp
./bin/rsa-keygen ~/keys/test-key -min 1000 -max 1000000000
```
