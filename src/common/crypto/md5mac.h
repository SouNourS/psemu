// NOTE (tfarley): Taken from old CryptoPP revision, credit to owner.
#ifndef CRYPTOPP_MD5MAC_H
#define CRYPTOPP_MD5MAC_H

/** \file
*/

#include "seckey.h"
#include "iterhash.h"

NAMESPACE_BEGIN(CryptoPP)

class CRYPTOPP_NO_VTABLE MD5MAC_Base : public FixedKeyLength<16>, public IteratedHash<word32, LittleEndian, 64, MessageAuthenticationCode>
{
public:
	static std::string StaticAlgorithmName() {return "MD5-MAC";}
	CRYPTOPP_CONSTANT(DIGESTSIZE = 16)

	//MD5MAC_Base() {SetStateSize(DIGESTSIZE);} // NOTE (tfarley): Removed this

	void UncheckedSetKey(const byte *userKey, unsigned int keylength, const NameValuePairs &params);
	void TruncatedFinal(byte *mac, size_t size);
	unsigned int DigestSize() const {return DIGESTSIZE;}

protected:
	static void Transform (word32 *buf, const word32 *in, const word32 *key);
	void HashEndianCorrectedBlock(const word32 *data) {Transform(m_digest, data, m_key+4);}
	void Init();
    word32* StateBuf() { return m_digest; } // NOTE (tfarley): Added this

	static const word32 T[12];
    FixedSizeSecBlock<word32, DIGESTSIZE> m_digest; // NOTE (tfarley): Added this
	FixedSizeSecBlock<word32, 12> m_key;
};

//! <a href="http://www.weidai.com/scan-mirror/mac.html#MD5-MAC">MD5-MAC</a>
DOCUMENTED_TYPEDEF(MessageAuthenticationCodeFinal<MD5MAC_Base>, MD5MAC)

NAMESPACE_END

#endif
