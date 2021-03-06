# Adding Custom Type Serialization To OpCoSerializer

This file explains how to extend the different serializers with the logic
required to serialize custom types.
This is done through the use of template specialization.

## JSON

By default, the JSON serializer supports the following C++ types:

- Numeric types (`int`, `double` etc.)
- `bool`
- `std::vector<T>` (where `T` must be supported)
- `std::string`

In order to add more type support, specialize the `OpCoSerializer::Json::JsonTypeSerializer<T>`
template type for the type you wish to support.
Note - it's important to inject the specialization into that exact namespace!

The expected interface is as follows (for example type `TExample`):

```cpp
struct Example
{
    // Struct that needs some custom serialization logic
};

namespace OpCoSerializer::Json
{
    struct JsonTypeSerializer<Example>
    {
        static rapidjson::Value Serialize(rapidjson::Document& document, Example& value)
        {
            // Serialization logic here. One can access the document too, although
            // this is not strictly neccesary. Call recursively for other 
            // specializations for nested serialization.
        }

        static Example Deserialize(rapidjson::Value& value)
        {
            // Deserialization logic here - return the deserialized type.
            // Call recursively for other specializations for nested deserialization.
        }
    };
}
```

The reference implementations and specializations can be found in the
[`OpCoSerializer/Json/JsonTypeSerializer.hpp`](./../include/OpCoSerializer/Json/JsonTypeSerializer.hpp "JsonTypeSerializer header")
file.
