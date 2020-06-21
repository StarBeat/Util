#include "AesEncryption.h"
namespace Util
{
void AesEncryption::setIv()
{
	AES_ctx_set_iv(&ctx, iv);
}

void AesEncryption::setKey(uint8_t* key)
{
	AES_init_ctx(&ctx, key);
#if CBC || CTR
	setIv();
#endif
}

void AesEncryption::decrypt(uint8_t* buf, int len)
{
#if CBC
	AES_CBC_decrypt_buffer(&ctx, buf, len);
#elif CTR
	AES_CTR_xcrypt_buffer(&ctx, buf, len);
#elif ECB
	AES_ECB_decrypt(&ctx, buf);
#endif // CBC

}

void AesEncryption::encrypt(uint8_t* buf, int len)
{
#if CBC
	AES_CBC_encrypt_buffer(&ctx, buf, len);
#elif CTR
	AES_CTR_xcrypt_buffer(&ctx, buf, len);
#elif ECB
	AES_ECB_encrypt(&ctx, buf);
#endif // CBC
}

}
