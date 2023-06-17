#ifndef _SIGNATURE_SERVICE_H_
#define _SIGNATURE_SERVICE_H_

#include <memory>
#include "crypto/cryp/extension_service.h"
#include "crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SignatureService : public ExtensionService
            {
            public:
                using Sptr = std::shared_ptr<SignatureService>;

                virtual CryptoPrimitiveId::AlgId GetRequiredHashAlgId() const noexcept = 0;

                virtual std::size_t GetRequiredHashSize() const noexcept = 0;

                virtual std::size_t GetSignatureSize() const noexcept = 0;
            };
        }
    }
}

#endif /* _SIGNATURE_SERVICE_H_ */