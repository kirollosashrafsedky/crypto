#ifndef _CRYPTO_SERVICE_H_
#define _CRYPTO_SERVICE_H_

#include <iostream>
#include <memory>
#include <stdint.h>
#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoService : public ExtensionService
            {
            public:
                using Uptr = std::unique_ptr<CryptoService>;

                virtual std::size_t GetBlockSize() const noexcept = 0;

                virtual std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept = 0;

                virtual std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept = 0;
            };
        }
    }
}

#endif /* _CRYPTO_SERVICE_H_ */