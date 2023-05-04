#ifndef _FILE_KEYSLOT_H_
#define _FILE_KEYSLOT_H_

#include "ara/crypto/keys/keyslot.h"
#include "ara/crypto/common/file_io_interface.h"

#define DEFAULT_PROVIDER_INSTANCE_SPECIFIER CRYPTOPP_CRYPTO_PROVIDER

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            namespace internal
            {
                class FileKeySlot : public KeySlot
                {
                public:
                    using Uptr = std::unique_ptr<FileKeySlot>;

                    FileKeySlot(cryp::CryptoProvider &cryptoProvider, const KeySlotPrototypeProps &keySlotPrototypeProps);

                    core::Result<void> Clear() noexcept override;

                    core::Result<KeySlotContentProps> GetContentProps() const noexcept override;

                    KeySlotContentProps &GetContentPropsUpdate() noexcept;

                    core::Result<cryp::CryptoProvider::Uptr> MyProvider() const noexcept override;

                    core::Result<KeySlotPrototypeProps> GetPrototypedProps() const noexcept override;

                    bool IsEmpty() const noexcept override;

                    core::Result<IOInterface::Uptr> Open(bool subscribeForUpdates = false, bool writeable = false) noexcept override;

                    core::Result<void> SaveCopy(const IOInterface &container) noexcept override;

                    core::Vector<core::Byte> getKeyMaterial() const noexcept;

                    void setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) noexcept;

                    bool IsModified() const noexcept;

                    bool IsWritable() const noexcept;

                    void setProvider(const cryp::CryptoProvider &cryptoProvider) noexcept;

                private:
                    cryp::CryptoProvider &cryptoProvider;

                    KeySlotPrototypeProps keySlotPrototypeProps;

                    KeySlotContentProps keySlotContentProps;

                    core::Vector<core::Byte> keyMaterial;

                    crypto::internal::FileIOInterface fileIoInterface;

                    bool isOpened;

                    bool isWritable;

                    bool isModified;
                };
            }
        }
    }
}

#endif /* _FILE_KEYSLOT_H_ */