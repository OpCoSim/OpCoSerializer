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
    /// deserialize any type of data. By default, this type will only
    /// support rapidjson supported types for Value(). To enable nested
    /// serialization, call these functions recursively.
    /// @tparam T The type.
    template <typename T>
    struct JsonTypeSerializer
    {
        /// Serializes the given value to a JSON value.
        /// @remarks For flexibility, the document is passed in so members
        /// (such as its allocator - required for some rapidjson functionality)
        /// is also available.
        /// @param document The document.
        /// @param value The value.
        static rapidjson::Value Serialize(rapidjson::Document&, T& value)
        {
            return rapidjson::Value(value);
        }

        /// Deserializes a value from the given JSON value.
        /// @param value The value.
        /// @returns The deserialized value.
        static T Deserialize(rapidjson::Value& value)
        {
            return value.Get<T>();
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
}

#endif // OPCOSERIALIZER_JSON_TYPE_SERIALIZER_HPP
