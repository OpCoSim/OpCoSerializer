# Adding Custom Serializers

OpCoSerializer provides serialization helpers.
This can be used to create custom serializer implementations using a type's
provided metadata about its serializable properties.

The informal interface for a serializer is as follows:

```cpp
class MySerializer final
{
    template <typename T>
    std::string Serialize(T const& value)
    {
        // Serialize a value to a string.
    }

    template <typename T>
    T Deserialize(std::string const& serializedString)
    {
        // Deserialize a value from a string.
    }
}
```

By using the `OpCoserializer::ForProperty<T>` free function, one can iterate through
`T`'s serializable properties and perform the desired serialization:

```cpp
class MySerializer final
{
    template <typename T>
    std::string Serialize(T const& value)
    {
        // Prerequsite setup...

        OpCoserializer::ForProperty<T>([&](auto& property) {
            // Type contains the type of the class member
            using Type = typename std::remove_cvref<decltype(property)>::type::Type;

            // Get the key and value...
            auto key = std::string(property.name);
            auto propertyValue = value.*(property.member);

            // Per property serialization logic here...
        });

        // Cleanup and return final string here...
    }

    template <typename T>
    T Deserialize(std::string const& serializedString)
    {
        // Can follow similar pattern to Serialize...
    }
}
```
