# OpCoSerializer

![version](https://img.shields.io/badge/Version-0.1-blue)
![license](https://img.shields.io/badge/License-MIT-blue)
![c++](https://img.shields.io/badge/C++-20-blue)

OpCoSerializer is a header-only C++ 20 serialization library.
It is mainly intended for use in [OpCoSim](https://github.com/OpCoSim/OpCoSim "OpCoSim repository link").

## Getting Started

Getting started with OpCoSerializer is as simple as adding the header files to
your include path and adding `#include "OpCoSerializer/OpCoSerializer.hpp"` to
your project.

Then add serialization property metadata to your types:

```cpp
using namespace OpCoSerializer;

struct TestTypeWithProperties final
{
    int i = 4;
    double d = 1.23;
    bool b = true;
    std::vector<double> v{1.2, 3.4};

    static auto constexpr properties = std::make_tuple(
        MakeProperty(&TestTypeWithProperties::i, "integer"),
        MakeProperty(&TestTypeWithProperties::d, "double"),
        MakeProperty(&TestTypeWithProperties::b, "boolean"),
        MakeProperty(&TestTypeWithProperties::v, "vector")
    );
};
```

Then instantiate a serializer and serialize away!

```cpp
using namespace OpCoSerializer;

JsonSerializer serializer{};
serializer.Serialize(value);
```

In order to be able to serialize custom types, make sure to read the docs for
[adding custom type serialization](./docs/AddingCustomTypeSerialization.md "Custom type serialization docs").
You can also follow the `SerializerBase` interface to create your own serializer
types.
You can find more info on the [adding custom serializers](././docs/AddingCustomSerializers.md "Adding custom serializers docs")
page.

## Tests

To build the tests, use the `CMakeLists.txt` file in the `test` directory.
For convenience, the `build.sh` script in the root of this repository can
be used to build the tests with `make`.
This script should be called from the root of the repository.

```sh
./build.sh
```

## Future Work

- Further standard library type support
- Add support for non-POD types and polymorphism
- Add more builtin serializers

## Acknowledgements

OpCoSerializer takes its inspiration from the following places:

- [Cereal](https://uscilab.github.io/cereal/ "Cereal repository link")
- [JSON serialization question](https://stackoverflow.com/q/17549906/10725298 "Stackoverflow link")
- [MSVC bug in above answer](https://stackoverflow.com/q/39086837/10725298 "Stackoverflow link")

OpCoSerializer makes use of the following third party projects:

- [rapidjson](https://github.com/Tencent/rapidjson "rapidjson repository link")

## License

This program is distributed under the [MIT License](./LICENSE "License link").
