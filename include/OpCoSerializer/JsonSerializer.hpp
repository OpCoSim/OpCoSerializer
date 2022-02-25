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
#include "rapidjson/prettywriter.h"
#include "OpCoSerializer/Common.hpp"

namespace OpCoSerializer
{
    /// Configuration for a JsonSerializer.
    struct JsonSerializerSettings final
    {
        /// Indicates when deserializing, if a member is not present,
        /// whether or not an exception should be thrown. 
        bool propertiesRequired = false;

        /// Whether or not to serialize JSON to a prettier indented string.
        bool pretty = false;
    };

    /// Generically serializes a key and value to the given document.
    /// @remarks Specialize this function in order to be able serialize any type
    /// of data. By default, this template will only support rapidjson supported
    /// types for Value().
    /// @tparam T The type of value.
    /// @param document The document.
    /// @param key The key.
    /// @param value The value.
    template <typename T>
    inline void AddJsonValue(rapidjson::Document& document, std::string&& key, T&& value)
    {
        document.AddMember(
            rapidjson::Value(key.c_str(), key.size(), document.GetAllocator()),
            rapidjson::Value(value),
            document.GetAllocator()
        );
    }

    /// Generically gets a generically typed value from a JSON value.
    /// @remarks Specialize this function in order to be able deserialize any type
    /// of data. By default, this template will only support rapidjson supported
    /// types for generic values.
    /// @tparam T The type of value.
    /// @param value The value.
    /// @returns The deserialized value.
    template <typename T>
    inline T GetValueFromJson(rapidjson::Value& value)
    {
        return value.Get<T>();
    }

    /// Serializes objects to and from JSON.
    class JsonSerializer final : public SerializerBase
    {
        public:
            /// Initializes a new instance of the JsonSerializer type.
            /// @param settings The settings.
            explicit JsonSerializer(JsonSerializerSettings&& settings = JsonSerializerSettings())
                : _settings(settings)
            {
            }

            /// Serializes the given value to JSON.
            /// @tparam T the type of the value to serialize.
            /// @param value The value.
            /// @returns The serialized string.
            template <typename T>
            std::string Serialize(T const& value)
            {
                using namespace rapidjson;

                Document document;
                document.SetObject();

                ForProperty<T>([&](auto& property) {
                    auto propertyValue = value.*(property.member);
                    AddJsonValue(document, std::string(property.name), propertyValue);
                });

                return _settings.pretty
                    ? StringifyDocument<PrettyWriter<StringBuffer>>(document)
                    : StringifyDocument<Writer<StringBuffer>>(document);
            }

            /// Deserializes the string to a value of type T.
            /// @tparam T the type of the value to deserialize to.
            /// @param serializedString The serialized string.
            /// @returns The deserialized value.
            template <typename T>
            T Deserialize(std::string const& serializedString)
            {
                // If possible, use the default instance of T. This makes partial deserialization
                // much more intuitive. Otherwise an unitialized instance is used.
                T value;
                if constexpr (std::is_default_constructible_v<T> && std::is_copy_assignable_v<T>)
                {
                    value = T{};
                }

                rapidjson::Document document;
                document.Parse(serializedString.c_str());

                ForProperty<T>([&](auto& property) {
                    auto iterator = document.FindMember(property.name);
                    if (iterator == document.MemberEnd() && _settings.propertiesRequired)
                    {
                        throw OpCoSerializerException(std::string("Missing property during deserialization - ") + property.name);
                    }

                    using Type = typename std::remove_cvref<decltype(property)>::type::Type;
                    value.*(property.member) = GetValueFromJson<Type>(iterator->value);
                });

                return value;
            }

        private:
            JsonSerializerSettings _settings;

            template <typename TWriter>
            std::string StringifyDocument(rapidjson::Document& document)
            {
                rapidjson::StringBuffer buffer;
                TWriter writer(buffer);
                document.Accept(writer);
                return buffer.GetString();
            }
    };
}

#endif // OPCOSERIALIZER_JSON_SERIALIZER_HPP
