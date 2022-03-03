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
using namespace OpCoSerializer::Json;

struct TestTypeWithProperties final
{
    int i = 4;
    double d = 1.23;
    bool b = true;
    std::vector<double> v{1.2, 3.4};
    std::string s;

    bool operator==(TestTypeWithProperties const& other) const
    {
        return i == other.i && d == other.d && b == other.b && v == other.v;
    }

    static auto constexpr properties() { return std::make_tuple(
        MakeProperty(&TestTypeWithProperties::i, "integer"),
        MakeProperty(&TestTypeWithProperties::d, "double"),
        MakeProperty(&TestTypeWithProperties::b, "boolean"),
        MakeProperty(&TestTypeWithProperties::v, "vector"),
        MakeProperty(&TestTypeWithProperties::s, "string")
    );};
};

TEST(JsonSerializer, SerializesExpectedString)
{
    JsonSerializer serializer{};
    auto expected = "{\"integer\":5,\"double\":1.5,\"boolean\":true,\"vector\":[1.2],\"string\":\"test\"}";
    TestTypeWithProperties value = {
        5,
        1.5,
        true,
        std::vector<double>{1.2},
        "test"
    };

    auto serialized = serializer.Serialize(value);

    ASSERT_STREQ(expected, serialized.c_str());
}

TEST(JsonSerializer, DeerializesExpectedValue)
{
    JsonSerializer serializer{};
    std::string string{"{\"integer\":7,\"double\":4.2,\"boolean\":false,\"vector\":[1.2,3.4,4.5],\"string\":\"example\"}"};
    TestTypeWithProperties expected = {
        7,
        4.2,
        false,
        std::vector<double>{1.2, 3.4, 4.5},
        "example"
    };

    auto deserialized = serializer.Deserialize<TestTypeWithProperties>(string);

    ASSERT_EQ(expected, deserialized);
}

TEST(JsonSerializer, RoundTripTest)
{
    JsonSerializer serializer{};
    TestTypeWithProperties value = {
        42,
        3.14,
        true,
        std::vector<double>{1.2, 3.4, 4.5}
    };
    
    auto serialized = serializer.Serialize(value);
    auto deserialized = serializer.Deserialize<TestTypeWithProperties>(serialized);

    ASSERT_EQ(value, deserialized);
}
