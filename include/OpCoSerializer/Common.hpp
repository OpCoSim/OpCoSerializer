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

namespace OpCoSerializer
{
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
    #define OPCOSERIALIZER_PROPERTY(CLASS, MEMBER) MakeProperty(&CLASS::MEMBER, #MEMBER)

    /// A base class for a serializer.
    class SerializerBase
    {
        protected:
            /// Calls the function f for each constant in the integer sequence.
            /// @param f The function.
            template <typename T, T... S, typename F>
            constexpr void ForSequence(std::integer_sequence<T, S...>, F&& f)
            {
                (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
            }
    };
}

#endif // OPCOSERIALIZER_COMMON_HPP
