#include "common/mem_io_interface.h"
#include "common/mem_trusted_container.h"

namespace ara
{
    namespace crypto
    {
        using namespace internal;

        MemIOInterface::MemIOInterface(MemTrustedContainer &memTrustedContainer)
            : memTrustedContainer(&memTrustedContainer)
        {
        }

        AllowedUsageFlags MemIOInterface::GetAllowedUsage() const noexcept
        {
            return this->memTrustedContainer->getAllowedUsageFlags();
        }

        std::size_t MemIOInterface::GetCapacity() const noexcept
        {
            return this->memTrustedContainer->getKeyMaterial().capacity();
        }

        CryptoObjectType MemIOInterface::GetCryptoObjectType() const noexcept
        {
            return this->memTrustedContainer->getObjectType();
        }

        CryptoObjectUid MemIOInterface::GetObjectId() const noexcept
        {
            return this->memTrustedContainer->getObjectUid();
        }

        std::size_t MemIOInterface::GetPayloadSize() const noexcept
        {
            return this->memTrustedContainer->getKeyMaterial().size();
        }

        CryptoAlgId MemIOInterface::GetPrimitiveId() const noexcept
        {
            return this->memTrustedContainer->getAlgId();
        }

        CryptoObjectType MemIOInterface::GetTypeRestriction() const noexcept
        {
            return CryptoObjectType::kUndefined;
        }

        bool MemIOInterface::IsObjectExportable() const noexcept
        {
            return this->memTrustedContainer->getIsExportable();
        }

        bool MemIOInterface::IsObjectSession() const noexcept
        {
            return this->memTrustedContainer->getIsSession();
        }

        bool MemIOInterface::IsVolatile() const noexcept
        {
            return true;
        }

        bool MemIOInterface::IsValid() const noexcept
        {
            return true;
        }

        bool MemIOInterface::IsWritable() const noexcept
        {
            return true;
        }

        core::Vector<core::Byte> MemIOInterface::getKeyMaterial() const noexcept
        {
            return this->memTrustedContainer->getKeyMaterial();
        }

        void MemIOInterface::setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) const noexcept
        {
            this->memTrustedContainer->setKeyMaterial(keyMaterial);
        }

        void MemIOInterface::SetAllowedUsage(AllowedUsageFlags allowedUsageFlags) noexcept
        {
            this->memTrustedContainer->setAllowedUsageFlags(allowedUsageFlags);
        }

        void MemIOInterface::SetCryptoObjectType(CryptoObjectType cryptoObjectType) noexcept
        {
            this->memTrustedContainer->setObjectType(cryptoObjectType);
        }

        void MemIOInterface::SetObjectId(CryptoObjectUid cryptoObjectUid) noexcept
        {
            this->memTrustedContainer->setObjectUid(cryptoObjectUid);
        }

        void MemIOInterface::SetPrimitiveId(CryptoAlgId primitiveId) noexcept
        {
            this->memTrustedContainer->setAlgId(primitiveId);
        }

        void MemIOInterface::setExportable(bool isExportable) const noexcept
        {
            this->memTrustedContainer->setIsExportable(isExportable);
        }

        std::shared_ptr<const cryp::CryptoProvider> MemIOInterface::getProvider() const noexcept
        {
            return this->memTrustedContainer->MyProvider();
        }

        void MemIOInterface::setProvider(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider) noexcept
        {
            this->memTrustedContainer->setMyProvider(cryptoProvider);
        }

        CryptoObjectUid MemIOInterface::GetDependantObjectId() const noexcept
        {
            return this->memTrustedContainer->getDependantObjectUid();
        }

        void MemIOInterface::SetDependantObjectId(CryptoObjectUid dependantObjectUid) noexcept
        {
            this->memTrustedContainer->setDependantObjectUid(dependantObjectUid);
        }

        CryptoObjectType MemIOInterface::GetDependantObjectType() const noexcept
        {
            return this->memTrustedContainer->getDependantObjectType();
        }

        void MemIOInterface::SetDependantObjectType(CryptoObjectType dependantObjectType) noexcept
        {
            this->memTrustedContainer->setDependantObjectType(dependantObjectType);
        }

    }

}
