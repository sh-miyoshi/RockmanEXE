#pragma once

#include <string>
#include <wmmintrin.h>

// Please set USE_AES_NI to 1, if you want to use hardware accelerator.
#define USE_AES_NI 1
// #define USE_AES_NI 0

// FILE_READ_SIZE is a read byte size from file at once
#define FILE_READ_SIZE (1024)

#define AES_BLOCK_SIZE (16)

namespace aes {
enum Mode {
    AES_ECB_ZERO,
    AES_ECB_PKCS_5,
    AES_CBC_ZERO,
    AES_CBC_PKCS_5,
    AES_CTR,
};

class AESError {
  public:
    bool success;
    std::string message;

	AESError() : success(true) {}
    ~AESError() {}
};

class AES {
    static const unsigned int MAX_NR = 14; // max no of rounds

    class EncryptBase {
      public:
        virtual int Encrypt(char *res, const char *readBuf, unsigned int readSize) = 0;
        virtual void Decrypt(char *res, const char *readBuf, unsigned int readSize) = 0;
        virtual bool Finalize(char *res) { return false; }
    };

    class EncryptECB : public EncryptBase {
        bool paddingFlag;
        AES *obj;

      public:
        EncryptECB(AES *obj);
        ~EncryptECB();

        int Encrypt(char *res, const char *readBuf, unsigned int readSize);
        void Decrypt(char *res, const char *readBuf, unsigned int readSize);
        bool Finalize(char *res);
    };

    class EncryptCBC : public EncryptBase {
        bool paddingFlag;
        AES *obj;
#if USE_AES_NI
        __m128i vec;
#else
        unsigned char vec[AES_BLOCK_SIZE];
#endif

      public:
#if USE_AES_NI
        EncryptCBC(AES *obj, __m128i iv);
#else
        EncryptCBC(AES *obj, const unsigned char *iv);
#endif
        ~EncryptCBC();

        int Encrypt(char *res, const char *readBuf, unsigned int readSize);
        void Decrypt(char *res, const char *readBuf, unsigned int readSize);
        bool Finalize(char *res);
    };

    class EncryptCTR : public EncryptBase {
        AES *obj;
#if USE_AES_NI
        __m128i vec;
#else
        unsigned char vec[AES_BLOCK_SIZE];
#endif

      public:
#if USE_AES_NI
        EncryptCTR(AES *obj, __m128i iv);
#else
        EncryptCTR(AES *obj, const unsigned char *iv);
#endif
        ~EncryptCTR();

        int Encrypt(char *res, const char *readBuf, unsigned int readSize);
        void Decrypt(char *res, const char *readBuf, unsigned int readSize);
    };

    unsigned int Nr; // number of rounds
	AESError initError;
    Mode mode;

    void Init(Mode mode, const unsigned char *key, unsigned int keyBitLen, unsigned char *iv);
    void SetPadding(char *data, int size);
    int GetDataSizeWithoutPadding(const char *data);
	AESError FileOpen(FILE **fp, std::string fname, std::string mode);
#if USE_AES_NI
    __m128i encKey[MAX_NR + 2], decKey[MAX_NR + 2];
    __m128i iv;
    __m128i AES_128_ASSIST(__m128i temp1, __m128i temp2);
    void AES_256_ASSIST_1(__m128i &temp1, __m128i &temp2);
    void AES_256_ASSIST_2(__m128i &temp1, __m128i &temp3);
    void AES_128_Key_Expansion(__m128i *key, const unsigned char *userKey);
    void AES_256_Key_Expansion(__m128i *key, const unsigned char *userKey);

    __m128i EncryptCore(__m128i data);
    __m128i DecryptCore(__m128i data);
#else
    unsigned char iv[AES_BLOCK_SIZE]; // initialize vector
    unsigned char roundKey[AES_BLOCK_SIZE * (MAX_NR + 1)];

    inline void ExtMul(unsigned char &x, unsigned char data, int n);
    void SubWord(unsigned char *w);
    void RotWord(unsigned char *w);
    void KeyExpansion(const unsigned char *userKey, int wordkeyBitLength);

    inline void SubBytes(unsigned char *data);
    inline void ShiftRows(unsigned char *data);
    inline void MixColumns(unsigned char *data);
    inline void InvSubBytes(unsigned char *data);
    inline void InvShiftRows(unsigned char *data);
    inline void InvMixColumns(unsigned char *data);
    inline void AddRoundKey(unsigned char *data, int n);

    void EncryptCore(unsigned char *data);
    void DecryptCore(unsigned char *data);
#endif
  public:
    // generate initialize vector
    static void GenerateIV(unsigned char *iv, Mode mode);
    static void GenerateIV(unsigned char *iv, std::string passpharse, Mode mode);

    AES(Mode mode, const unsigned char *key, unsigned int keyBitLen, unsigned char *iv);
    ~AES() {}

	AESError EncryptFile(std::string in_fname, std::string out_fname);
	AESError DecryptFile(std::string in_fname, std::string out_fname);

	AESError EncryptString(char *result, const char *data, unsigned int length);
	AESError DecryptString(char *result, const char *data, unsigned int length);
};
}; // namespace aes
