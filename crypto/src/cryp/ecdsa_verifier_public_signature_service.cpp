#include "cryp/ecdsa_verifier_public_signature_service.h"
#include "cryp/verifier_public_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            EcdsaVerifierPublicSignatureService::EcdsaVerifierPublicSignatureService(const EcdsaVerifierPublicCtx &ecdsaVerifierPublicCtx)
                : ecdsaVerifierPublicCtx(&ecdsaVerifierPublicCtx)
            {
            }

            CryptoPrimitiveId::AlgId EcdsaVerifierPublicSignatureService::GetRequiredHashAlgId() const noexcept
            {
            }

            std::size_t EcdsaVerifierPublicSignatureService::GetRequiredHashSize() const noexcept
            {
            }

            std::size_t EcdsaVerifierPublicSignatureService::GetSignatureSize() const noexcept
            {
            }

            std::size_t EcdsaVerifierPublicSignatureService::GetActualKeyBitLength() const noexcept
            {
            }

            CryptoObjectUid EcdsaVerifierPublicSignatureService::GetActualKeyCOUID() const noexcept
            {
            }

            AllowedUsageFlags EcdsaVerifierPublicSignatureService::GetAllowedUsage() const noexcept
            {
            }

            std::size_t EcdsaVerifierPublicSignatureService::GetMaxKeyBitLength() const noexcept
            {
            }

            std::size_t EcdsaVerifierPublicSignatureService::GetMinKeyBitLength() const noexcept
            {
            }

            bool EcdsaVerifierPublicSignatureService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
            {
            }

            bool EcdsaVerifierPublicSignatureService::IsKeyAvailable() const noexcept
            {
            }

        }
    }
}
