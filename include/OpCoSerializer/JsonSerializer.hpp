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

#ifndef OPCOSERIALIZER_JSON_SERIALIZER_HPP
#define OPCOSERIALIZER_JSON_SERIALIZER_HPP

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "OpCoSerializer/Common.hpp"

namespace OpCoSerializer
{
    /// Generically serializes a key and value to the given document.
    /// @remarks Specialize this function in order to be able serialize any type
    /// of data. By default, this template will only support rapidjson supported
    /// types for Value().
    /// @tparam T The type of value.
    /// @param document The document.
    /// @param key The key.
    /// @param value The value.
    template <typename T>
    void AddJsonValue(rapidjson::Document& document, std::string&& key, T&& value)
    {
        document.AddMember(
            rapidjson::Value(key.c_str(), key.size(), document.GetAllocator()),
            rapidjson::Value(value),
            document.GetAllocator()
        );
    }

    /// Serializes objects to and from JSON.
    struct JsonSerializer final : SerializerBase
    {
        /// Serializes the given value to JSON.
        /// @param value The value.
        /// @returns The serialized string.
        template <typename T>
        std::string Serialize(T const& value) const
        {
            rapidjson::Document document;
            document.SetObject();

            auto constexpr numberOfProperties = std::tuple_size<decltype(T::properties)>::value;
            ForSequence(std::make_index_sequence<numberOfProperties>{}, [&](auto i) {
                auto constexpr property = std::get<i>(T::properties);
                auto propertyValue = value.*(property.member);
                AddJsonValue(document, std::string(property.name), propertyValue);
            });

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            document.Accept(writer);
            return buffer.GetString();
        }
    };
}

#endif // OPCOSERIALIZER_JSON_SERIALIZER_HPP