#ifndef _UPDATES_OBSERVER_H_
#define _UPDATES_OBSERVER_H_

#include "ara/crypto/keys/elementary_types.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            class UpdatesObserver
            {
            public:
                using Uptr = std::unique_ptr<UpdatesObserver>;

                virtual ~UpdatesObserver() noexcept = default;

                virtual void OnUpdate(const TransactionScope &updatedSlots) noexcept = 0;

                UpdatesObserver &operator=(const UpdatesObserver &other) = default;

                UpdatesObserver &operator=(UpdatesObserver &&other) = default;
            };
        }
    }
}

#endif /* _UPDATES_OBSERVER_H_ */