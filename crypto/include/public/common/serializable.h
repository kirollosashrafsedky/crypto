#ifndef _SERRIALIZABLE_H_
#define _SERRIALIZABLE_H_

#include <iostream>
#include <stdint.h>
#include "ara/core/vector.h"
#include "ara/core/utility.h"
#include "ara/core/result.h"
#include "common/base_id_types.h"

namespace ara
{
    namespace crypto
    {
        class Serializable
        {
        public:
            using FormatId = std::uint32_t;

            static const FormatId kFormatDefault = 0;

            static const FormatId kFormatDerEncoded = 2;

            static const FormatId kFormatPemEncoded = 3;

            static const FormatId kFormatRawValueOnly = 1;

            virtual ~Serializable() noexcept = default;

            virtual core::Result<core::Vector<core::Byte>> ExportPublicly(FormatId formatId = kFormatDefault) const noexcept = 0;

            template <typename Alloc = std::allocator<uint8_t>>
            core::Result<ByteVector<Alloc>> ExportPublicly(FormatId formatId = kFormatDefault) const noexcept;

            Serializable &operator=(const Serializable &other) = default;

            Serializable &operator=(Serializable &&other) = default;
        };
    }
}

#endif /* _SERRIALIZABLE_H_ */