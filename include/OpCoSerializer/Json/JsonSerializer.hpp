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
#include "OpCoSerializer/Json/JsonSerializerSettings.hpp"
#include "OpCoSerializer/Json/JsonTypeSerializer.hpp"

namespace OpCoSerializer::Json
{
    /// Serializes objects to and from JSON.
    class JsonSerializer final
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
                    using PropertyType = typename std::remove_cvref<decltype(property)>::type;
                    using Type = typename std::remove_cvref<typename PropertyType::Type>::type;
                    auto propertyValue = value.*(property.member);
                    auto key = std::string(property.name);
                    document.AddMember(
                        rapidjson::Value(key.c_str(), key.size(), document.GetAllocator()),
                        JsonTypeSerializer<Type>::Serialize(document, propertyValue),
                        document.GetAllocator()
                    );
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
                using namespace rapidjson;

                // If possible, use the default instance of T. This makes partial deserialization
                // much more intuitive. Otherwise an unitialized instance is used.
                T value;
                if constexpr (std::is_default_constructible_v<T> && std::is_copy_assignable_v<T>)
                {
                    value = T{};
                }

                Document document;
                try
                {
                    document.Parse(serializedString.c_str());
                }
                catch (std::runtime_error& exception)
                {
                    auto message = std::string("Error whilst parsing JSON document from string - ") + std::string(exception.what());
                    throw OpCoSerializerException(message);
                }

                ForProperty<T>([&](auto& property) {
                    using PropertyType = typename std::remove_cvref<decltype(property)>::type;
                    using Type = typename std::remove_cvref<typename PropertyType::Type>::type;
                    auto iterator = document.FindMember(property.name);
                    if (iterator == document.MemberEnd() && _settings.propertiesRequired)
                    {
                        throw OpCoSerializerException(std::string("Missing property during deserialization - ") + property.name);
                    }

                    value.*(property.member) = JsonTypeSerializer<Type>::Deserialize(iterator->value);
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
