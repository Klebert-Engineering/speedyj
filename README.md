# speedyj

## About

Speedyj is a tiny C++ library for JSON serialization.

For optional Qt type support include `speedyj/speedyj-qt.hpp`.

## Building

```sh
mkdir build && cd build
cmake .. && cmake --build .
```

## Testing

```sh
ctest .
```

## Usage

```c++
#include "speedyj/speedyj.hpp"

namespace sj = speedyj;

/* Create a stream. */
sj::Stream s;

/* Serialize on object. */
s << sj::Object
  /* Key/value: */
  << "key" << "value"
  /* Nested array: */
  << "key-to-array" << sj::Array << 1.5 << sj::False << sj::True << sj::Null << sj::End
  /* Nested object: */
  << "nested-object" << sj:Object << "key" << "value" << sj::End
  /* Push std::vector as array: */
  << "vector" << std::vector<int>{{ 1, 2, 3, 4, 5, 6 }}
  /* Push std::map as object: */
  << "map" << std::map<std::string, int>{{"a", 1}, {"b", 2}}
  << sj::End;

/* Get the streams string. */
std::cout << s.str() << "\n";
```
