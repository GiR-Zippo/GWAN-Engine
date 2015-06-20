#include "SHA256.hpp"
#include "BigNumber.hpp"
#include <stdarg.h>

SHA256Hash::SHA256Hash()
{
    SHA256_Init(&sha256);
    memset(digest, 0, SHA256_DIGEST_LENGTH * sizeof(uint8));
}

SHA256Hash::~SHA256Hash()
{
    SHA256_Init(&sha256);
}

void SHA256Hash::UpdateData(const uint8 *dta, int len)
{
    SHA256_Update(&sha256, dta, len);
}

void SHA256Hash::UpdateData(const std::string &str)
{
    UpdateData((uint8 const*)str.c_str(), str.length());
}

void SHA256Hash::UpdateBigNumbers(BigNumber* bn0, ...)
{
    va_list v;
    BigNumber* bn;

    va_start(v, bn0);
    bn = bn0;
    while (bn)
    {
        UpdateData(bn->AsByteArray().get(), bn->GetNumBytes());
        bn = va_arg(v, BigNumber*);
    }
    va_end(v);
}

void SHA256Hash::Initialize()
{
    SHA256_Init(&sha256);
}

void SHA256Hash::Finalize()
{
    SHA256_Final(digest, &sha256);
}
