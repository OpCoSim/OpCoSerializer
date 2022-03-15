// Copyright (c) 2022 OpCoSim
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef OPCOSERIALIZER_JSON_TYPE_SERIALIZER_HPP
#define OPCOSERIALIZER_JSON_TYPE_SERIALIZER_HPP

namespace OpCoSerializer::Json
{
    /// Provides Json serialization and serialization logic for a type.
    /// @remarks Specialize this type in order to be able serialize or
    /// deserialize any type of data. By default, this type will support:
    /// - rapidjson supported numeric and boolean types
    /// - Types that have OpCoSerializer properties that are recursively serializable.
    /// @tparam T The type.
    template <typename T>
    struct JsonTypeSerializer
    {        
        // We autodetect types with serialization properties and just
        // attempt to serialize each as an object. This will work
        // when each property type (recursively) in T is serializable.
        // This means that types that nest serializable properties do not
        // require specializations to deserialize as this would be laborious.

        /// Serializes the given value to a JSON value.
        /// @remarks For flexibility, the document is passed in so members
        /// (such as its allocator - required for some rapidjson functionality)
        /// is also available.
        /// @param document The document.
        /// @param value The value.
        static rapidjson::Value Serialize(rapidjson::Document& document, T& value)
        {
            if constexpr (HasSerializablePropertiesV<T>)
            {
                rapidjson::Value object;
                object.SetObject();

                ForProperty<T>([&](auto& property) {
                    using PropertyType = typename std::remove_cvref<decltype(property)>::type;
                    using Type = std::remove_cvref<typename PropertyType::Type>::type;
                    auto propertyValue = value.*(property.member);
                    auto key = std::string(property.name);
                    object.AddMember(
                        rapidjson::Value(key.c_str(), key.size(), document.GetAllocator()),
                        JsonTypeSerializer<Type>::Serialize(document, propertyValue),
                        document.GetAllocator()
                    );
                });

                return object;
            }
            else if constexpr (std::is_enum_v<T>)
            {
                return rapidjson::Value(static_cast<int32_t>(value));
            }
            else
            {
                return rapidjson::Value(value);
            }
        }

        /// Deserializes a value from the given JSON value.
        /// @param value The value.
        /// @returns The deserialized value.
        static T Deserialize(rapidjson::Value& value)
        {
            if constexpr (HasSerializablePropertiesV<T>)
            {
                T deserialized;
                if constexpr (std::is_default_constructible_v<T> && std::is_copy_assignable_v<T>)
                {
                    deserialized = T{};
                }

                ForProperty<T>([&](auto& property) {
                    using PropertyType = typename std::remove_cvref<decltype(property)>::type;
                    using Type = std::remove_cvref<typename PropertyType::Type>::type;
                    auto iterator = value.FindMember(property.name);
                    if (iterator == value.MemberEnd())
                    {
                        throw OpCoSerializerException(std::string("Missing property during deserialization - ") + property.name);
                    }

                    deserialized.*(property.member) = JsonTypeSerializer<Type>::Deserialize(iterator->value);
                });

                return deserialized;
            }
            else if constexpr (std::is_enum_v<T>)
            {
                return static_cast<T>(value.Get<int32_t>());
            }
            else
            {
                return value.Get<T>();
            }
        }
    };

    /// Partial JsonTypeSerializer specialization for a vector of a given type.
    /// @remarks This will forward calls to a JsonTypeSerializer<TElement>, thus
    /// serialization must be defined for TElement (or TElement must be natively
    /// supported by rapidjson).
    /// @tparam TElement The type of the elements in the vector.
    template <typename TElement>
    struct JsonTypeSerializer<std::vector<TElement>>
    {
        static rapidjson::Value Serialize(rapidjson::Document& document, std::vector<TElement>& value)
        {
            using namespace rapidjson;
            
            Value array;
            array.SetArray();

            for (auto& element : value)
            {
                array.PushBack(
                    JsonTypeSerializer<TElement>::Serialize(document, element),
                    document.GetAllocator()
                );
            }

            return array;
        }

        static std::vector<TElement> Deserialize(rapidjson::Value& value)
        {
            auto array = value.GetArray();
            typename std::vector<TElement>::size_type i = 0;
            std::vector<TElement> vector(array.Size());

            for (auto& jsonElement : array)
            {
                vector[i] = JsonTypeSerializer<TElement>::Deserialize(jsonElement);
                ++i;
            }

            return vector;
        }
    };

    /// JsonTypeSerializer specialization for a C++ string.
    template <>
    struct JsonTypeSerializer<std::string>
    {
        static rapidjson::Value Serialize(rapidjson::Document& document, std::string& value)
        {
            rapidjson::Value string;
            string.SetString(value.c_str(), value.size(), document.GetAllocator());
            return string;
        }

        static std::string Deserialize(rapidjson::Value& value)
        {
            return std::string(value.GetString());
        }
    };
}

#endif // OPCOSERIALIZER_JSON_TYPE_SERIALIZER_HPP
