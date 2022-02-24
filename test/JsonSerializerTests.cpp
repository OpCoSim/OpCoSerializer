#include <gtest/gtest.h>
#include "OpCoSerializer/OpCoSerializer.hpp"

using namespace OpCoSerializer;

struct TestTypeWithProperties final
{
    int i = 4;
    double d = 1.23;
    bool b = true;

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
