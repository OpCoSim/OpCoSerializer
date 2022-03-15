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

#include <gtest/gtest.h>
#include "OpCoSerializer/OpCoSerializer.hpp"

using namespace OpCoSerializer;

struct TestType final
{
    int value = 0;
};

struct WithProperties final
{
    int i = 4;

    static auto constexpr SerializerProperties()
    {
        return std::make_tuple(
            MakeProperty(&WithProperties::i, "integer")
        );
    };
};

struct WithoutProperties final
{
};

TEST(MakeProperty, ReturnsExpectedProperty)
{
    auto constexpr name = "value";

    auto constexpr property = MakeProperty(&TestType::value, name);

    ASSERT_EQ(&TestType::value, property.member);
    ASSERT_STREQ(name, property.name);
}

TEST(HasSerializablePropertiesV, WhenHasSerializablePropertiesIsTrue)
{
    ASSERT_TRUE(HasSerializablePropertiesV<WithProperties>);
}

TEST(HasSerializablePropertiesV, WhenDoesNotHaveSerializablePropertiesIsFalse)
{
    ASSERT_FALSE(HasSerializablePropertiesV<WithoutProperties>);
}
