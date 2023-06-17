#ifndef _SIGNATURE_H_
#define _SIGNATURE_H_

#include "crypto/cryp/cryobj/crypto_object.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class Signature : public CryptoObject
            {
            public:
                using Sptrc = std::shared_ptr<const Signature>;

                static const CryptoObjectType kObjectType = CryptoObjectType::kSignature;

                virtual CryptoPrimitiveId::AlgId GetHashAlgId() const noexcept = 0;

                virtual std::size_t GetRequiredHashSize() const noexcept = 0;
            };
        }
    }
}

#endif /* _SIGNATURE_H_ */