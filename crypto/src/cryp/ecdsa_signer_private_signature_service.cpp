#include "cryp/ecdsa_signer_private_signature_service.h"
#include "cryp/ecdsa_signer_private_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            EcdsaSignerPrivateSignatureService::EcdsaSignerPrivateSignatureService(const EcdsaSignerPrivateCtx &ecdsaSignerPrivateCtx)
                : ecdsaSignerPrivateCtx(&ecdsaSignerPrivateCtx)
            {
            }

            CryptoPrimitiveId::AlgId EcdsaSignerPrivateSignatureService::GetRequiredHashAlgId() const noexcept
            {
            }

            std::size_t EcdsaSignerPrivateSignatureService::GetRequiredHashSize() const noexcept
            {
            }

            std::size_t EcdsaSignerPrivateSignatureService::GetSignatureSize() const noexcept
            {
            }

            std::size_t EcdsaSignerPrivateSignatureService::GetActualKeyBitLength() const noexcept
            {
            }

            CryptoObjectUid EcdsaSignerPrivateSignatureService::GetActualKeyCOUID() const noexcept
            {
            }

            AllowedUsageFlags EcdsaSignerPrivateSignatureService::GetAllowedUsage() const noexcept
            {
            }

            std::size_t EcdsaSignerPrivateSignatureService::GetMaxKeyBitLength() const noexcept
            {
            }

            std::size_t EcdsaSignerPrivateSignatureService::GetMinKeyBitLength() const noexcept
            {
            }

            bool EcdsaSignerPrivateSignatureService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
            {
            }

            bool EcdsaSignerPrivateSignatureService::IsKeyAvailable() const noexcept
            {
            }

        }
    }
}
