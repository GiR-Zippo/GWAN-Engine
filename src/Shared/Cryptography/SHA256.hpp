#ifndef _SHA256_H
#define _SHA256_H

#include "Define.hpp"
#include <string>
#include <openssl/sha.h>

class BigNumber;

class SHA256Hash
{
    public:
        SHA256Hash();
        ~SHA256Hash();

        void UpdateBigNumbers(BigNumber* bn0, ...);

        void UpdateData(const uint8 *dta, int len);
        void UpdateData(const std::string &str);

        void Initialize();
        void Finalize();

        uint8 *GetDigest() { return digest; };
        int GetLength() const { return SHA_DIGEST_LENGTH; };

    private:
        SHA256_CTX sha256;
        uint8 digest[SHA256_DIGEST_LENGTH];
};
#endif /* _SHA256_H */
