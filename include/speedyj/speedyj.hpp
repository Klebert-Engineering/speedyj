#pragma once

#if defined(_MSC_VER)
    #if defined(SPEEDYJ_BUILD)
        #define SPEEDYJ_EXPORT __declspec(dllexport)
    #else
        #define SPEEDYJ_EXPORT __declspec(dllimport)
    #endif
#else
    #define SPEEDYJ_EXPORT __attribute__ ((visibility ("default")))
#endif

#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace speedyj
{

struct Error : public std::runtime_error
{
    SPEEDYJ_EXPORT Error(const char*);
};

/**
 * Internal stream state.
 */
struct StreamState
{
    enum Type {
        Object,
        Array
    } type {Object};
    int itemIdx {0};

    SPEEDYJ_EXPORT StreamState(Type);
};

/**
 * Tiny wrapper over std::string stream for serializing
 * data directly to a json string.
 */
class Stream
{
public:
    SPEEDYJ_EXPORT Stream(Stream&&) = default;
    SPEEDYJ_EXPORT Stream& operator=(Stream&&) = default;

    /**
     * Create an empty json stream.
     *
     * Note: Push either Object or Array first, otherwise
     *       pushing values will throw.
     */
    SPEEDYJ_EXPORT Stream();

    /**
     * Returns the streams string value.
     */
    SPEEDYJ_EXPORT std::string str() const;

    /**
     * Push next value. Use operator << implementation.
     */
    SPEEDYJ_EXPORT Stream& push(const std::string&);
    SPEEDYJ_EXPORT Stream& push(unsigned long long);
    SPEEDYJ_EXPORT Stream& push(unsigned long);
    SPEEDYJ_EXPORT Stream& push(unsigned int);
    SPEEDYJ_EXPORT Stream& push(long long);
    SPEEDYJ_EXPORT Stream& push(long);
    SPEEDYJ_EXPORT Stream& push(int);
    SPEEDYJ_EXPORT Stream& push(double);

/* private */
    std::stringstream ss_;
    std::stack<StreamState, std::vector<StreamState>> state_;

    Stream& push(StreamState);
    Stream& pop();

    StreamState& state();
    const StreamState& state() const;
};

namespace impl {
template <class> struct TypeHelper;
}

template <class _Type>
Stream& operator<<(Stream& s, const _Type& v)
{
    impl::TypeHelper<_Type>::push(s, v);
    return s;
}

static constexpr struct Null_ {} Null;
static constexpr struct True_ {} True;
static constexpr struct False_ {} False;
static constexpr struct Array_ {} Array;
static constexpr struct Object_ {} Object;
static constexpr struct End_ {} End;

SPEEDYJ_EXPORT Stream& operator<<(Stream&, const Null_&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const True_&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const False_&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const Array_&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const Object_&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const End_&);

struct ScopedObject
{
    Stream& s_;

    ScopedObject(Stream& s) : s_(s) { s_ << Object; }
    ~ScopedObject() { s_ << End; }
};

struct ScopedArray
{
    Stream& s_;

    ScopedArray(Stream& s) : s_(s) { s_ << Array; }
    ~ScopedArray() { s_ << End; }
};

}

#include "speedyj-impl.hpp"
