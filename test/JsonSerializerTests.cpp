#include <gtest/gtest.h>
#include "OpCoSerializer/OpCoSerializer.hpp"

using namespace OpCoSerializer;

struct TestTypeWithProperties final
{
    int i = 4;
    double d = 1.23;
    bool b = true;
    std::string s = "test";

    static auto constexpr properties = std::make_tuple(
        MakeProperty(&TestTypeWithProperties::i, "integer"),
        MakeProperty(&TestTypeWithProperties::d, "double"),
        MakeProperty(&TestTypeWithProperties::b, "boolean"),
        MakeProperty(&TestTypeWithProperties::s, "weight")
    );
};

TEST(JsonSerializer, SerializesExpectedString)
{
    TestTypeWithProperties value{};
    JsonSerializer serializer{};
    auto expected = "";

    auto serialized = serializer.Serialize(value);

    ASSERT_STREQ(expected, serialized.c_str());
}
