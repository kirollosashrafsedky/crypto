#include "common/uuid.h"

namespace ara
{
    namespace crypto
    {
        bool Uuid::IsNil() const noexcept
        {
            return (this->mQwordLs == 0 && this->mQwordMs == 0);
        }
    }
}