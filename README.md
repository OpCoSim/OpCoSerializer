# OpCoSerializer

![version](https://img.shields.io/badge/Version-0.1-blue)
![license](https://img.shields.io/badge/License-MIT-blue)
![c++](https://img.shields.io/badge/C++-20-blue)

OpCoSerializer is a header-only C++ 20 serialization library.
It is mainly intended for use in [OpCoSim](https://github.com/OpCoSim/OpCoSim "OpCoSim repository link").

## Tests

To build the tests, use the `CMakeLists.txt` file in the `test` directory.
For convenience, the `build.sh` script in the root of this repository can
be used to build the tests with `make`.
This script should be called from the root of the repository.

```sh
./build.sh
```

## Acknowledgements

OpCoSerializer takes its inspiration from the following:

- [Cereal](https://uscilab.github.io/cereal/ "Cereal repository link")
- [JSON serialization question](https://stackoverflow.com/q/17549906/10725298 "Stackoverflow link")
- [MSVC bug](https://stackoverflow.com/q/39086837/10725298 "Stackoverflow link")

## License

This program is distributed under the [MIT License](./LICENSE "License link").
