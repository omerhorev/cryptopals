//
// Created by omerh on 24/08/2019.
//

#include <gtest/gtest.h>
#include "utils/cookie_encoding.h"

struct example_object
{
    int foo;
    char baz[4];
    float zap;
};

namespace utils
{
    template<>
    class cookie_encoder<example_object> : internal::encode_cookie_base
    {
    public:
        std::string encode(const example_object &val)
        {
            std::string s;
            s += encode_field("baz", val.baz);
            s += encode_field("foo", val.foo);
            s += encode_field("zap", val.zap);

            return s;
        }

        void decode(const std::string &string, example_object &obj)
        {
            decode_field(string, "baz", obj.baz);
            decode_field(string, "foo", obj.foo);
            decode_field(string, "zap", obj.zap);
        }
    };
}


TEST(cookie, encode)
{
    example_object obj1 = {};

    obj1.zap = 0.5;
    obj1.baz[0] = 'a';
    obj1.baz[1] = 'b';
    obj1.baz[2] = 0;
    obj1.foo = 3;

    example_object obj2 = {};
    utils::decode_as_cookie(utils::encode_as_cookie(obj1), obj2);

    ASSERT_EQ(obj1.zap, obj2.zap);
    ASSERT_EQ(obj1.foo, obj2.foo);
    ASSERT_STREQ(obj1.baz, obj2.baz);

}