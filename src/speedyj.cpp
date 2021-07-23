#include "speedyj/speedyj.hpp"

#include <limits>
#include <ostream>
#include <cassert>
#include <string>

namespace speedyj
{

#define IS_EVEN(x) (bool)((x) % 2 == 0)

static auto wantsEscape(int c)
{
    switch (c) {
    case '"':
    case '\\':
    case '\n':
    case '\r':
    case 0x08: /* backspace */
    case 0x0c: /* form-feed */
    case '\t':
        return true;

    default:
        return c < ' ' || c > std::numeric_limits<signed char>::max();
    }
}

static auto writeEscapeSeq(const char* begin, const char* end, std::ostream& os)
{

    while (begin != end) {
        auto size = 2; /* Size of backslash escape seq. */
        char buf[7];   /* Sequence buffer. */
        buf[0] = '\\'; /* Default to backslash escape. */

        switch (auto c = *begin) {
        case '"':
            buf[1] = '"';
            break;

        case '\\':
            buf[1] = '\\';
            break;

        case '\n':
            buf[1] = 'n';
            break;

        case '\r':
            buf[1] = 'r';
            break;

        case 0x08: /* backspace */
            buf[1] = 'b';
            break;

        case 0x0c: /* form-feed */
            buf[1] = 'f';
            break;

        case '\t':
            buf[1] = 't';
            break;

        default:
            if (c < ' ' || c > std::numeric_limits<signed char>::max()) {
                /* Not optimal for multibyte codepoints, but
                 * it should work. */
                size = snprintf(buf, sizeof(buf), "\\u%04X",
                                (unsigned)(c < 0 ? std::numeric_limits<unsigned char>::max() + 1u + c : c));
            } else {
                return begin;
            }
        }

        os.write(buf, size);
        ++begin;
    }

    return begin;
}

static auto writeRange(const char* begin, const char* end, std::ostream& os)
{
    os.write(begin, end - begin);
    return end;
}

static auto writeEncoded(const std::string& s, std::ostream& os)
{
    auto begin = s.data();
    const auto end = s.data() + s.size();

    while (begin != end) {
        auto cursor = begin;
        while (cursor != end && !wantsEscape(*cursor))
            ++cursor;

        begin = writeRange(begin, cursor, os);
        if (begin == end)
            break;

        begin = writeEscapeSeq(begin, end, os);
    }

    assert(begin == end);
}

static void next(Stream& s)
{
    if (s.state_.empty())
        return;
   
    if (s.state().itemIdx > 0) {
        switch (s.state().type) {
        case StreamState::Array:
            s.ss_.put(',');
            break;
        case StreamState::Object:
            s.ss_.put(IS_EVEN(s.state().itemIdx) ? ',' : ':');
            break;
        }
    }

    s.state().itemIdx++;
}

static void checkedNext(Stream& s)
{
    if (s.state_.empty())
        throw Error("checkedNext: Stack is empty");

    next(s);
}

static void begin(Stream& s)
{
    switch (s.state().type) {
    case StreamState::Array:
        s.ss_.put('[');
        break;
    case StreamState::Object:
        s.ss_.put('{');
        break;
    }
}

static void finish(Stream& s)
{
    switch (s.state().type) {
    case StreamState::Array:
        s.ss_.put(']');
        break;
    case StreamState::Object:
        if (!IS_EVEN(s.state().itemIdx))
            throw Error("finish: Key value count missmatch");
        s.ss_.put('}');
        break;
    }
}

Error::Error(const char* msg)
    : std::runtime_error(msg)
{}

StreamState::StreamState(StreamState::Type t)
    : type(t)
    , itemIdx(0)
{}

Stream::Stream()
{}

std::string Stream::str() const
{
    if (!state_.empty())
        throw Error("str: State stack is not empty");
    return ss_.str();
}

Stream& Stream::push(const std::string& s)
{
    checkedNext(*this);
    ss_.put('"');
    writeEncoded(s, ss_);
    ss_.put('"');
    return *this;
}

Stream& Stream::push(unsigned long long u)
{
    checkedNext(*this);
    ss_ << u;
    return *this;
}

Stream& Stream::push(unsigned long u)
{
    checkedNext(*this);
    ss_ << u;
    return *this;
}

Stream& Stream::push(unsigned int u)
{
    checkedNext(*this);
    ss_ << u;
    return *this;
}

Stream& Stream::push(long long s)
{
    checkedNext(*this);
    ss_ << s;
    return *this;
}

Stream& Stream::push(long s)
{
    checkedNext(*this);
    ss_ << s;
    return *this;
}

Stream& Stream::push(int s)
{
    checkedNext(*this);
    ss_ << s;
    return *this;
}

Stream& Stream::push(double f)
{
    checkedNext(*this);
    ss_ << std::to_string(f);
    return *this;
}

Stream& Stream::push(StreamState s)
{
    next(*this);
    state_.push(s);
    begin(*this);

    return *this;
}

Stream& Stream::pop()
{
    if (state_.empty())
        throw Error("pop: Stack is empty");

    finish(*this);
    state_.pop();

    return *this;
}

StreamState& Stream::state()
{
    if (state_.empty())
        throw Error("state: Stack is empty");

    return state_.top();
}

const StreamState& Stream::state() const
{
    if (state_.empty())
        throw Error("state: Stack is empty");

    return state_.top();
}

/* Stream Operators */

speedyj::Stream& operator<<(speedyj::Stream& s, const Null_&)
{
    next(s);
    s.ss_.write("null", 4);
    return s;
}

speedyj::Stream& operator<<(speedyj::Stream& s, const True_&)
{
    next(s);
    s.ss_.write("true", 4);
    return s;
}

speedyj::Stream& operator<<(speedyj::Stream& s, const False_&)
{
    next(s);
    s.ss_.write("false", 5);
    return s;
}

speedyj::Stream& operator<<(speedyj::Stream& s, const Array_&)
{
    return s.push(StreamState(StreamState::Array));
}

speedyj::Stream& operator<<(speedyj::Stream& s, const Object_&)
{
    return s.push(StreamState(StreamState::Object));
}

speedyj::Stream& operator<<(speedyj::Stream& s, const End_&)
{
    return s.pop();
}

}
