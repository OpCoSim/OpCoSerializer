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

    static auto constexpr properties()
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
