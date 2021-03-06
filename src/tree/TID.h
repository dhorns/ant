#pragma once

#include "Rtypes.h"

#ifdef __CINT__

// simulate minimal cstdint for ROOTCINT
namespace std {
typedef UChar_t    uint8_t;
typedef UShort_t   uint16_t;
typedef UInt_t     uint32_t;
typedef ULong_t    uint64_t;
typedef Short_t    int16_t;
typedef Long_t     int64_t;
}

// disable the override keyword
#define override

#else

#include "base/std_ext/time.h"

#include <type_traits>
#include <tuple>
#include <cstdint>
#include <stdexcept>
#include <list>

static_assert(std::is_same<ULong_t, std::uint64_t>::value, "Type size mismatch");
static_assert(std::is_same<Long_t, std::int64_t>::value, "Type size mismatch");
#endif // __CINT__

#include <iomanip>
#include <ctime>

// if we want to change something
// of the data format defined by the ant::T* classes
#define ANT_UNPACKER_ROOT_VERSION 2

namespace ant {

struct TID
{

    std::uint32_t Flags;
    std::uint32_t Timestamp; // unix epoch
    std::uint32_t Lower;     // usually event counter
    std::uint32_t Reserved;  // unused

#ifndef __CINT__

    // you may append flags, but never remove or change order!
    enum class Flags_t : std::uint8_t {
        Invalid,    // simply invalid, unset, whatever...
        MC,         // this ID belongs to a MC event (not real data)
        AdHoc,      // this ID was "invented" by a reader since the
                    // input did not provide proper timestamp information
    };

    // ensure correct init in default constructor
    static_assert(static_cast<std::uint8_t>(Flags_t::Invalid) == 0, "Invalid flag should be first item in enum class");

    explicit TID(
            std::uint32_t timestamp,
            std::uint32_t lower = 0,
            const std::list<Flags_t>& flags={}
       )
        :
          Flags(0),
          Timestamp(timestamp),
          Lower(lower),
          Reserved(0)
    {
        for(const auto& f : flags) {
            Flags |= 1 << static_cast<std::uint8_t>(f);
        }
    }

    template<class Archive>
    void serialize(Archive& archive) {
        archive(Flags, Timestamp, Lower, Reserved);
    }

    bool IsInvalid() const {
        return Flags & (1 << static_cast<std::uint8_t>(Flags_t::Invalid));
    }

    bool isSet(const Flags_t& flag) const {
        return Flags & (1 << static_cast<std::uint8_t>(flag));
    }

    std::uint64_t Value() const {
        return (static_cast<std::uint64_t>(Timestamp) << 8*sizeof(std::uint32_t)) + Lower;
    }

    friend std::ostream& operator<<(std::ostream& s, const TID& o) {
        if(o.IsInvalid())
            return s << "INVALID";

        s << "(";
        if(o.Flags)
            s  << "flags=0x" << o.Flags << ",";
        s << "'" << std_ext::to_iso8601(o.Timestamp) <<"',";
        s << "0x" << std::hex <<  std::setw(sizeof(decltype(Lower))*2) << std::setfill('0')
          << o.Lower << std::dec;
        s  << ")" ;
        return s;
    }

    bool operator<(const TID& rhs) const
    {
        if(IsInvalid() || rhs.IsInvalid())
            return false;
        auto make_tuple = [] (const TID& id) {
            return std::tie(id.Flags, id.Timestamp, id.Lower);
        };
        return make_tuple(*this) < make_tuple(rhs);
    }

    bool operator!=(const TID& rhs) const
    {
        if(IsInvalid() || rhs.IsInvalid())
            return true;
        return *this < rhs || rhs < *this;
    }

    bool operator<=(const TID& rhs) const
    {
        return *this < rhs || *this == rhs;
    }

    bool operator>=(const TID& rhs) const
    {
        return *this > rhs || *this == rhs;
    }

    bool operator>(const TID& rhs) const
    {
        return rhs < *this;
    }

    bool operator==(const TID& rhs) const
    {
        return !(*this != rhs);
    }

    TID& operator++() {
        if(Lower == std::numeric_limits<decltype(Lower)>::max())
            ++Timestamp;
        ++Lower;
        return *this;
    }

    TID& operator--() {
        if(Lower == std::numeric_limits<decltype(Lower)>::min())
            --Timestamp;
        --Lower;
        return *this;
    }

#endif

    TID() : Flags(1 << static_cast<std::uint8_t>(Flags_t::Invalid)), // set the invalid flag by default
        Timestamp(0), Lower(0), Reserved(0) {}
    virtual ~TID() {}
    ClassDef(TID, ANT_UNPACKER_ROOT_VERSION)

}; // TID

template<typename ValueType>
struct TKeyValue
{
    std::uint32_t  Key;
    ValueType Value;

    TKeyValue(unsigned key, ValueType value) :
        Key(key), Value(std::move(value))
    {}

    friend std::ostream& operator<<(std::ostream& s, const TKeyValue& kv) {
        return s << kv.Key << "=" << kv.Value;
    }

    template<class Archive>
    void serialize(Archive& archive) {
        archive(Key, Value);
    }

    TKeyValue() : Key(), Value() {}
    // for some very strange reason,
    // ClassDef MUST NOT be used here
    // otherwise reading it from the tree gives a segfault!
    // Anyhow, reading/writing still works...
    // ClassDef(TKeyValue,0)
}; // TKeyValue<ValueType>

}
