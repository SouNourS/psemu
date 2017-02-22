#include "bitstream.h"
#include "log.h"
#include "test.h"

class PlanetsideBitstream {
public:
    std::vector<uint8_t>& pData;
    int32_t streamPos;

    PlanetsideBitstream(std::vector<uint8_t>& pData) :
        pData(pData),
        streamPos(0) {

    }
};

void Write(PlanetsideBitstream *pThis, const uint8_t *pData, int numBits) {
    int remainingBits; // ebx@1
    PlanetsideBitstream *this_; // ebp@1
    int byteOffset; // eax@2
    int bitOffset; // ecx@2
    int bitsLeft; // edx@4
    int v8; // ebx@7
    int v9; // edi@7
    unsigned __int8 *this_pData; // ecx@9

    remainingBits = numBits;
    this_ = pThis;
    while (1) {
        byteOffset = this_->streamPos / 8;
        bitOffset = this_->streamPos % 8;
        if (!bitOffset && !(remainingBits & 7)) {
            memcpy(
                &this_->pData[byteOffset],
                pData,
                4 * ((unsigned int)remainingBits >> 5) + (((unsigned int)remainingBits >> 3) & 3));
            this_->streamPos += remainingBits;
            return;
        }
        bitsLeft = 8 - bitOffset;
        if (!bitOffset)
            this_->pData[byteOffset] = 0;
        if (remainingBits <= (unsigned int)bitsLeft)
            break;
        v8 = remainingBits - bitsLeft;
        v9 = v8;
        if (v8 >= this_->streamPos % 8)
            v8 = this_->streamPos % 8;
        this_pData = this_->pData.data();
        if ((unsigned int)bitsLeft >= 8)
            this_pData[byteOffset] = *pData;
        else
            this_pData[byteOffset] |= *pData >> v8;
        this_->streamPos += bitsLeft;
        if (v8)
            Write(this_, pData, v8);
        if (v9 == v8)
            return;
        remainingBits = v9 - v8;
        ++pData;
    }
    this_->pData[byteOffset] |= *pData << (bitsLeft - remainingBits);
    this_->streamPos += remainingBits;
}

void testBitstreamWriteBitsBasic() {
    static std::vector<uint8_t> expectedBuf = std::vector<uint8_t>({
        0x0B, 0xCA
    });

    std::vector<uint8_t> bitstreamBuf;
    BitStream bitstream(bitstreamBuf);

    uint32_t zero = 0;
    bitstream.writeBits((const uint8_t*)&zero, 4);

    uint32_t abc = 0xABC;
    bitstream.writeBits((const uint8_t*)&abc, 12);

    assertBuffersEqual(bitstreamBuf, expectedBuf);
}

void testBitstreamWriteBits() {
    std::vector<uint8_t> psBitstreamBuf(32);
    PlanetsideBitstream psBitstream(psBitstreamBuf);

    std::vector<uint8_t> bitstreamBuf(32);
    BitStream bitstream(bitstreamBuf);

    for (int32_t n = 1; n < 32; ++n) {
        memset(psBitstreamBuf.data(), 0x0, 32);
        psBitstream.streamPos = n;
        Write(&psBitstream, (const uint8_t*)&n, n);

        memset(bitstreamBuf.data(), 0x0, 32);
        bitstream.setPos(n);
        bitstream.writeBits((const uint8_t*)&n, n);

        assertBuffersEqual(psBitstreamBuf, bitstreamBuf);
    }
}

void testBitstreamReadBits() {
    std::vector<uint8_t> psBitstreamBuf(32);
    PlanetsideBitstream psBitstream(psBitstreamBuf);

    BitStream bitstream(psBitstreamBuf);

    for (int32_t n = 1; n < 32; ++n) {
        memset(psBitstreamBuf.data(), 0x0, 32);
        psBitstream.streamPos = n;
        Write(&psBitstream, (const uint8_t*)&n, n);

        int32_t readResult = 0;
        bitstream.setPos(n);
        bitstream.readBits((uint8_t*)&readResult, n);

        assertEqual(readResult, n);
    }
}

void testBitstream() {
    testBitstreamWriteBitsBasic();
    testBitstreamWriteBits();
    testBitstreamReadBits();

    // TODO: Test byte write/read
    // TODO: Test mixed bits and bytes write/read
    // TODO: Test primitive write/read
    // TODO: Test string write/read
}
