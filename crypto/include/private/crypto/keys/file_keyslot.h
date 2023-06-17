#ifndef _FILE_KEYSLOT_H_
#define _FILE_KEYSLOT_H_

#include "crypto/keys/keyslot.h"
#include "crypto/common/file_io_interface.h"

#define DEFAULT_PROVIDER_INSTANCE_SPECIFIER CRYPTOPP_CRYPTO_PROVIDER

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            namespace internal
            {
                class MainKeyStorageProvider;

                class FileKeySlot : public KeySlot
                {
                public:
                    using Sptr = std::shared_ptr<FileKeySlot>;

                    using Sptrc = std::shared_ptr<const FileKeySlot>;

                    FileKeySlot(const core::InstanceSpecifier &iSpecify, std::shared_ptr<MainKeyStorageProvider> keyStorageProvider, std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, const KeySlotPrototypeProps &keySlotPrototypeProps, const std::string &fileName);

                    FileKeySlot(const core::InstanceSpecifier &iSpecify);

                    FileKeySlot(const FileKeySlot &FileKeySlot);

                    core::Result<void> Clear() noexcept override;

                    core::Result<KeySlotContentProps> GetContentProps() const noexcept override;

                    KeySlotContentProps &GetContentPropsUpdate() noexcept;

                    core::Result<cryp::CryptoProvider::Sptrc> MyProvider() const noexcept override;

                    core::Result<KeySlotPrototypeProps> GetPrototypedProps() const noexcept override;

                    bool IsEmpty() const noexcept override;

                    core::Result<IOInterface::Sptr> Open(bool subscribeForUpdates = false, bool writeable = false) noexcept override;

                    core::Result<void> SaveCopy(const IOInterface &container) noexcept override;

                    core::Vector<core::Byte> getKeyMaterial() const noexcept;

                    core::Result<void> setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) noexcept;

                    bool IsModified() const noexcept;

                    bool IsWritable() const noexcept;

                    core::Result<void> setProvider(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider) noexcept;

                    bool saveFile() noexcept;

                    core::InstanceSpecifier getIspecifier() const noexcept;

                    FileKeySlot &operator=(const FileKeySlot &other) = default;

                    FileKeySlot &operator=(FileKeySlot &&other) = default;

                private:
                    core::InstanceSpecifier iSpecify;

                    std::shared_ptr<MainKeyStorageProvider> keyStorageProvider;

                    std::shared_ptr<const cryp::CryptoProvider> cryptoProvider;

                    KeySlotPrototypeProps keySlotPrototypeProps;

                    KeySlotContentProps keySlotContentProps;

                    core::Vector<core::Byte> keyMaterial;

                    std::shared_ptr<crypto::internal::FileIOInterface> fileIoInterface;

                    bool isOpened;

                    bool isWritable;

                    bool isModified;

                    std::string fileName;
                };
            }
        }
    }
}

#endif /* _FILE_KEYSLOT_H_ */