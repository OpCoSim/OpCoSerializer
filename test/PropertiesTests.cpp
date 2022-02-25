#include <gtest/gtest.h>
#include "OpCoSerializer/OpCoSerializer.hpp"

using namespace OpCoSerializer;

struct TestType final
{
    int value = 0;
};

TEST(MakeProperty, ReturnsExpectedProperty)
{
    auto constexpr name = "value";

    auto constexpr property = MakeProperty(&TestType::value, name);

    ASSERT_EQ(&TestType::value, property.member);
    ASSERT_STREQ(name, property.name);
}
