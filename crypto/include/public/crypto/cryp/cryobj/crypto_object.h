#ifndef _CRYPTO_OBJECT_H_
#define _CRYPTO_OBJECT_H_

#include <memory>
#include "crypto/common/base_id_types.h"
#include "crypto/common/crypto_object_uid.h"
#include "crypto/cryp/cryobj/crypto_primitive_id.h"
#include "crypto/common/crypto_error_domain.h"
#include "crypto/common/io_interface.h"
#include "ara/core/result.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoObject
            {
            public:
                using Sptrc = std::shared_ptr<const CryptoObject>;

                using Sptr = std::shared_ptr<CryptoObject>;

                struct COIdentifier
                {
                    CryptoObjectType mCOType;
                    CryptoObjectUid mCouid;
                };

                virtual ~CryptoObject() noexcept = default;

                // todo
                template <class ConcreteObject>
                static core::Result<typename ConcreteObject::Sptrc> Downcast(CryptoObject::Sptrc &&object) noexcept
                {
                    // auto derived_ptr = dynamic_cast<const ConcreteObject *>(object.get());
                    // if (!derived_ptr)
                    // {
                    //     return core::Result<typename ConcreteObject::Sptrc>::FromError(CryptoErrc::kInvalidArgument);
                    // }

                    // auto derived_object = std::shared_ptr<const ConcreteObject>(derived_ptr);
                    // object.release();
                    // return core::Result<typename ConcreteObject::Sptrc>::FromValue(std::move(derived_object));
                }

                virtual CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept = 0;

                virtual COIdentifier GetObjectId() const noexcept = 0;

                virtual std::size_t GetPayloadSize() const noexcept = 0;

                virtual COIdentifier HasDependence() const noexcept = 0;

                virtual bool IsExportable() const noexcept = 0;

                virtual bool IsSession() const noexcept = 0;

                virtual core::Result<void> Save(IOInterface &container) const noexcept = 0;

                CryptoObject &operator=(const CryptoObject &other) = default;

                CryptoObject &operator=(CryptoObject &&other) = default;
            };
        }

    }

}
#endif /* _CRYPTO_OBJECT_H_ */