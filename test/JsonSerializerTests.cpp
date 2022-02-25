#include <gtest/gtest.h>
#include "OpCoSerializer/OpCoSerializer.hpp"

using namespace OpCoSerializer;

struct TestTypeWithProperties final
{
    int i = 4;
    double d = 1.23;
    bool b = true;

    bool operator==(TestTypeWithProperties const& other) const
    {
        return i == other.i && d == other.d && b == other.b;
    }

    static auto constexpr properties = std::make_tuple(
        MakeProperty(&TestTypeWithProperties::i, "integer"),
        MakeProperty(&TestTypeWithProperties::d, "double"),
        MakeProperty(&TestTypeWithProperties::b, "boolean")
    );
};

TEST(JsonSerializer, SerializesExpectedString)
{
    TestTypeWithProperties value{};
    JsonSerializer serializer{};
    auto expected = "{\"integer\":4,\"double\":1.23,\"boolean\":true}";

    auto serialized = serializer.Serialize(value);

    ASSERT_STREQ(expected, serialized.c_str());
}

TEST(JsonSerializer, DeerializesExpectedValue)
{
    JsonSerializer serializer{};
    std::string string{"{\"integer\":7,\"double\":4.2,\"boolean\":false}"};
    TestTypeWithProperties expected = {
        7,
        4.2,
        false
    };

    auto deserialized = serializer.Deserialize<TestTypeWithProperties>(string);
}
