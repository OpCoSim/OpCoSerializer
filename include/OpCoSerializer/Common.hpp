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

#ifndef OPCOSERIALIZER_COMMON_HPP
#define OPCOSERIALIZER_COMMON_HPP

// Standard library includes
#include <cstdint>
#include <utility>
#include <tuple>
#include <type_traits>
#include <concepts>
#include <stdexcept>
#include <vector>

namespace OpCoSerializer
{
    /// An exception type for OpCoSerializer.
    struct OpCoSerializerException final : std::runtime_error
    {
        /// Initializes a new instance of the OpCoSerializerException type.
        /// @param message The message.
        explicit OpCoSerializerException(char const* message)
            : std::runtime_error(message)
        {
        }

        /// Initializes a new instance of the OpCoSerializerException type.
        /// @param message The message.
        explicit OpCoSerializerException(std::string const& message)
            : std::runtime_error(message)
        {
        }
    };

    /// Checks whether or not T has serializable properties.
    /// @tparam T The type to check.
    template <typename T>
    class HasSerializableProperties final
    {
        private:
            template<typename U>
            static auto test(int) -> decltype(U::SerializerProperties(), std::true_type{});

            template<typename>
            static std::false_type test(...);

        public:
            static constexpr bool value = std::is_same<decltype(test<T>(0)), std::true_type>::value;
    };

    /// Helper for the value of HasSerializableProperties<T>.
    template <typename T>
    bool constexpr HasSerializablePropertiesV = HasSerializableProperties<T>::value;

    /// Metadata for a propery to be serialized.
    template<typename Class, typename T>
    struct Property
    {
        /// The member type.
        using Type = T;

        /// The member pointer.
        T Class::* member;

        /// The propery name.
        char const* name;

        /// Initializes a new instance of the Property type.
        /// @param member The member pointer.
        /// @param name The property name.
        constexpr Property(T Class::* member, char const* name)
            : member{member},
              name{name}
        {
        }
    };

    /// Creates a property for the given member with a given name.
    /// @param member The member pointer.
    /// @param name The property name.
    /// @returns The created property.
    template<typename Class, typename T>
    constexpr auto MakeProperty(T Class::* member, const char* name)
    {
        return Property<Class, T>{member, name};
    }

    /// Forwards to MakeProperty, assuming the member name and serialization name
    /// are the same.
    #define OPCOSERIALIZER_PROPERTY(CLASS, MEMBER) OpCoSerializer::MakeProperty(&CLASS::MEMBER, #MEMBER)

    /// Calls the function f for each constant in the integer sequence.
    /// @param f The function.
    template <typename T, T... S, typename F>
    constexpr inline void ForSequence(std::integer_sequence<T, S...>, F&& f)
    {
        (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
    }

    /// Iterates the serializable properties of T and applies the function f to each.
    /// @param f The function.
    template <typename T, typename F>
    constexpr inline void ForProperty(F&& f)
    {
        auto constexpr numberOfProperties = std::tuple_size<decltype(T::SerializerProperties())>::value;
        ForSequence(std::make_index_sequence<numberOfProperties>{}, [&](auto i) {
            auto constexpr property = std::get<i>(T::SerializerProperties());
            f(property);
        });
    }
}

#endif // OPCOSERIALIZER_COMMON_HPP
