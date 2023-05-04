#include "ara/crypto/common/mem_trusted_container.h"
#include "ara/crypto/cryp/crypto_provider.h"

namespace ara
{
    namespace crypto
    {
        using namespace internal;

        MemTrustedContainer::MemTrustedContainer(cryp::CryptoProvider &cryptoProvider, std::size_t capacity)
            : memIoInterface(*this), cryptoProvider(cryptoProvider),
              algId(0), objectType(CryptoObjectType::kUndefined),
              allowedUsageFlags(kAllowPrototypedOnly), isExportable(false), isSession(false)
        {
            keyMaterial.reserve(capacity);
        }

        IOInterface &MemTrustedContainer::GetIOInterface() noexcept
        {
            return memIoInterface;
        }

        const core::Vector<core::Byte> &MemTrustedContainer::getKeyMaterial() const noexcept
        {
            return this->keyMaterial;
        }

        void MemTrustedContainer::setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) noexcept
        {
            this->keyMaterial = keyMaterial;
        }

        const cryp::CryptoProvider &MemTrustedContainer::MyProvider() const noexcept
        {
            return this->cryptoProvider;
        }

        void MemTrustedContainer::setMyProvider(const cryp::CryptoProvider &cryptoProvider) noexcept
        {
            this->cryptoProvider = cryptoProvider;
        }

        CryptoAlgId MemTrustedContainer::getAlgId() const noexcept
        {
            return this->algId;
        }

        void MemTrustedContainer::setAlgId(CryptoAlgId algId) noexcept
        {
            this->algId = algId;
        }

        CryptoObjectType MemTrustedContainer::getObjectType() const noexcept
        {
            return this->objectType;
        }

        void MemTrustedContainer::setObjectType(CryptoObjectType objectType) noexcept
        {
            this->objectType = objectType;
        }

        CryptoObjectType MemTrustedContainer::getDependantObjectType() const noexcept
        {
            return this->dependantObjectType;
        }

        void MemTrustedContainer::setDependantObjectType(CryptoObjectType dependantObjectType) noexcept
        {
            this->dependantObjectType = dependantObjectType;
        }

        CryptoObjectUid MemTrustedContainer::getObjectUid() const noexcept
        {
            return this->objectUid;
        }

        void MemTrustedContainer::setObjectUid(CryptoObjectUid objectUid) noexcept
        {
            this->objectUid = objectUid;
        }

        CryptoObjectUid MemTrustedContainer::getDependantObjectUid() const noexcept
        {
            return this->dependantObjectUid;
        }

        void MemTrustedContainer::setDependantObjectUid(CryptoObjectUid dependantObjectUid) noexcept
        {
            this->dependantObjectUid = dependantObjectUid;
        }

        AllowedUsageFlags MemTrustedContainer::getAllowedUsageFlags() const noexcept
        {
            return this->allowedUsageFlags;
        }

        void MemTrustedContainer::setAllowedUsageFlags(AllowedUsageFlags allowedUsageFlags) noexcept
        {
            this->allowedUsageFlags = allowedUsageFlags;
        }

        bool MemTrustedContainer::getIsExportable() const noexcept
        {
            return this->isExportable;
        }

        void MemTrustedContainer::setIsExportable(bool isExportable) noexcept
        {
            this->isExportable = isExportable;
        }

        bool MemTrustedContainer::getIsSession() const noexcept
        {
            return this->isSession;
        }

        void MemTrustedContainer::setIsSession(bool isSession) noexcept
        {
            this->isSession = isSession;
        }

    }

}
