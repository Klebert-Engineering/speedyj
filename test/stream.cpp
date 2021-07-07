#include <catch2/catch.hpp>

#include "speedyj/speedyj.hpp"

namespace sj = speedyj;

TEST_CASE("Empty stream", "[empty]") {
    sj::Stream s;

    REQUIRE(s.str() == "");
}

TEST_CASE("Empty object", "[empty-object]") {
    sj::Stream s;
    s << sj::Object << sj::End;

    REQUIRE(s.str() == "{}");
}

TEST_CASE("Empty array", "[empty-array]") {
    sj::Stream s;
    s << sj::Array << sj::End;

    REQUIRE(s.str() == "[]");
}

TEST_CASE("Unclosed object", "[unclosed-object]") {
    sj::Stream s;
    s << sj::Object; /* Missing << sj::End! */

    REQUIRE_THROWS(s.str());
}

TEST_CASE("Object key without value", "[unfinished-object-kv]") {
    sj::Stream s;
    REQUIRE_THROWS(
        s << sj::Object << "key" << sj::End
    );
}

TEST_CASE("Unclosed array", "[unclosed-array]") {
    sj::Stream s;
    s << sj::Array; /* Missing << sj::End! */

    REQUIRE_THROWS(s.str());
}

TEST_CASE("Special values", "[special-value]") {
    sj::Stream s;
    s << sj::Array
      << sj::False /* -> false */
      << sj::True  /* -> true */
      << sj::Null  /* -> null */
      << sj::End;

    REQUIRE(s.str() == "[false,true,null]");
}

TEST_CASE("String escaping", "[string-escaping]") {
    sj::Stream s;
    s << sj::Array
      << "\""                    /* -> \" */
      << "\n\r\t"                /* -> \n\r\t */
      << std::string("\x00", 1)  /* -> \u0000 */
      << sj::End;

    REQUIRE(s.str() == "[\"\\\"\",\"\\n\\r\\t\",\"\\u0000\"]");
}

TEST_CASE("Numeric values", "[numeric-values]") {
    sj::Stream s;
    s << sj::Array
      << (int)1
      << (unsigned)1u
      << (long)1
      << (unsigned long)1u
      << (long long)1
      << (unsigned long long)1u
      << (double)1.1
      << (float)1.1f
      << sj::End;

    REQUIRE(s.str() == "[1,1,1,1,1,1,1.1,1.1]");
}

TEST_CASE("Boolean values", "[boolean-values]") {
    sj::Stream s;
    s << sj::Array
      << false
      << true
      << sj::End;

    REQUIRE(s.str() == "[false,true]");
}

TEST_CASE("Nested arrays", "[nested-arrays]") {
    sj::Stream s;
    for (auto i = 0; i < 10; ++i)
        s << sj::Array;
    for (auto i = 0; i < 10; ++i)
        s << sj::End;

    REQUIRE(s.str() == "[[[[[[[[[[]]]]]]]]]]");
}

TEST_CASE("Nested objects", "[nested-objects]") {
    sj::Stream s;
    for (auto i = 0; i < 10; ++i)
        s << sj::Object
          << "key";
    s << nullptr;
    for (auto i = 0; i < 10; ++i)
        s << sj::End;

    REQUIRE(s.str() == "{\"key\":{\"key\":{\"key\":{\"key\":{\"key\":"
                       "{\"key\":{\"key\":{\"key\":{\"key\":{\"key\":null}}}}}}}}}}");
}

TEST_CASE("Array specialization", "[array-spec]") {
    sj::Stream s;
    s << sj::Array;
    s << std::vector<int>{{ 1, 2, 3, 4, 5 }};
    s << std::deque<int>{{ 1, 2, 3, 4, 5 }};
    s << std::set<int>{{ 1, 2, 3, 4, 5 }};
    s << std::list<int>{{ 1, 2, 3, 4, 5 }};
    s << sj::End;

    REQUIRE(s.str() ==
            "["
            "[1,2,3,4,5],"
            "[1,2,3,4,5],"
            "[1,2,3,4,5],"
            "[1,2,3,4,5]"
            "]");
}

TEST_CASE("Object specialization", "[object-spec]") {
    sj::Stream s;
    s << sj::Array;
    s << std::map<std::string, int>{{"a", 1}, {"b", 2}, {"c", 3}};
    s << sj::End;

    REQUIRE(s.str() ==
            "["
            "{\"a\":1,\"b\":2,\"c\":3}"
            "]");
}
