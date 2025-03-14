/*

This is an implementation of the AES algorithm, specifically ECB, CTR and CBC mode.
Block size can be chosen in aes.h - available choices are AES128, AES192, AES256.

The implementation is verified against the test vectors in:
  National Institute of Standards and Technology Special Publication 800-38A 2001 ED

ECB-AES128
----------

  plain-text:
    6bc1bee22e409f96e93d7e117393172a
    ae2d8a571e03ac9c9eb76fac45af8e51
    30c81c46a35ce411e5fbc1191a0a52ef
    f69f2445df4f9b17ad2b417be66c3710

  key:
    2b7e151628aed2a6abf7158809cf4f3c

  resulting cipher
    3ad77bb40d7a3660a89ecaf32466ef97 
    f5d3d58503b9699de785895a96fdbaaf 
    43b1cd7f598ece23881b00e3ed030688 
    7b0c785e27e8ad3f8223207104725dd4 


NOTE:   String length must be evenly divisible by 16byte (str_len % 16 == 0)
        You should pad the end of the string with zeros if this is not the case.
        For AES192/256 the key size is proportionally larger.

*/


/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
//#include <string.h> // CBC mode, for memset
//#include "aes.h"

/*****************************************************************************/
/* Defines:                                                                  */
/*****************************************************************************/
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4

#if defined(AES256) && (AES256 == 1)
    #define Nk 8
    #define Nr 14
#elif defined(AES192) && (AES192 == 1)
    #define Nk 6
    #define Nr 12
#else
    #define Nk 4        // The number of 32 bit words in a key.
    #define Nr 10       // The number of rounds in AES Cipher.
#endif

// jcallan@github points out that declaring Multiply as a function 
// reduces code size considerably with the Keil ARM compiler.
// See this link for more information: https://github.com/kokke/tiny-AES-C/pull/3
#ifndef MULTIPLY_AS_A_FUNCTION
  #define MULTIPLY_AS_A_FUNCTION 0
#endif




/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef unsigned char state_t[4][4];



// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM - 
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static const unsigned char sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  (unsigned char)0x63, (unsigned char)0x7c, (unsigned char)0x77, (unsigned char)0x7b, (unsigned char)0xf2, (unsigned char)0x6b, (unsigned char)0x6f, (unsigned char)0xc5, (unsigned char)0x30, (unsigned char)0x01, (unsigned char)0x67, (unsigned char)0x2b, (unsigned char)0xfe, (unsigned char)0xd7, (unsigned char)0xab, (unsigned char)0x76,
  (unsigned char)0xca, (unsigned char)0x82, (unsigned char)0xc9, (unsigned char)0x7d, (unsigned char)0xfa, (unsigned char)0x59, (unsigned char)0x47, (unsigned char)0xf0, (unsigned char)0xad, (unsigned char)0xd4, (unsigned char)0xa2, (unsigned char)0xaf, (unsigned char)0x9c, (unsigned char)0xa4, (unsigned char)0x72, (unsigned char)0xc0,
  (unsigned char)0xb7, (unsigned char)0xfd, (unsigned char)0x93, (unsigned char)0x26, (unsigned char)0x36, (unsigned char)0x3f, (unsigned char)0xf7, (unsigned char)0xcc, (unsigned char)0x34, (unsigned char)0xa5, (unsigned char)0xe5, (unsigned char)0xf1, (unsigned char)0x71, (unsigned char)0xd8, (unsigned char)0x31, (unsigned char)0x15,
  (unsigned char)0x04, (unsigned char)0xc7, (unsigned char)0x23, (unsigned char)0xc3, (unsigned char)0x18, (unsigned char)0x96, (unsigned char)0x05, (unsigned char)0x9a, (unsigned char)0x07, (unsigned char)0x12, (unsigned char)0x80, (unsigned char)0xe2, (unsigned char)0xeb, (unsigned char)0x27, (unsigned char)0xb2, (unsigned char)0x75,
  (unsigned char)0x09, (unsigned char)0x83, (unsigned char)0x2c, (unsigned char)0x1a, (unsigned char)0x1b, (unsigned char)0x6e, (unsigned char)0x5a, (unsigned char)0xa0, (unsigned char)0x52, (unsigned char)0x3b, (unsigned char)0xd6, (unsigned char)0xb3, (unsigned char)0x29, (unsigned char)0xe3, (unsigned char)0x2f, (unsigned char)0x84,
  (unsigned char)0x53, (unsigned char)0xd1, (unsigned char)0x00, (unsigned char)0xed, (unsigned char)0x20, (unsigned char)0xfc, (unsigned char)0xb1, (unsigned char)0x5b, (unsigned char)0x6a, (unsigned char)0xcb, (unsigned char)0xbe, (unsigned char)0x39, (unsigned char)0x4a, (unsigned char)0x4c, (unsigned char)0x58, (unsigned char)0xcf,
  (unsigned char)0xd0, (unsigned char)0xef, (unsigned char)0xaa, (unsigned char)0xfb, (unsigned char)0x43, (unsigned char)0x4d, (unsigned char)0x33, (unsigned char)0x85, (unsigned char)0x45, (unsigned char)0xf9, (unsigned char)0x02, (unsigned char)0x7f, (unsigned char)0x50, (unsigned char)0x3c, (unsigned char)0x9f, (unsigned char)0xa8,
  (unsigned char)0x51, (unsigned char)0xa3, (unsigned char)0x40, (unsigned char)0x8f, (unsigned char)0x92, (unsigned char)0x9d, (unsigned char)0x38, (unsigned char)0xf5, (unsigned char)0xbc, (unsigned char)0xb6, (unsigned char)0xda, (unsigned char)0x21, (unsigned char)0x10, (unsigned char)0xff, (unsigned char)0xf3, (unsigned char)0xd2,
  (unsigned char)0xcd, (unsigned char)0x0c, (unsigned char)0x13, (unsigned char)0xec, (unsigned char)0x5f, (unsigned char)0x97, (unsigned char)0x44, (unsigned char)0x17, (unsigned char)0xc4, (unsigned char)0xa7, (unsigned char)0x7e, (unsigned char)0x3d, (unsigned char)0x64, (unsigned char)0x5d, (unsigned char)0x19, (unsigned char)0x73,
  (unsigned char)0x60, (unsigned char)0x81, (unsigned char)0x4f, (unsigned char)0xdc, (unsigned char)0x22, (unsigned char)0x2a, (unsigned char)0x90, (unsigned char)0x88, (unsigned char)0x46, (unsigned char)0xee, (unsigned char)0xb8, (unsigned char)0x14, (unsigned char)0xde, (unsigned char)0x5e, (unsigned char)0x0b, (unsigned char)0xdb,
  (unsigned char)0xe0, (unsigned char)0x32, (unsigned char)0x3a, (unsigned char)0x0a, (unsigned char)0x49, (unsigned char)0x06, (unsigned char)0x24, (unsigned char)0x5c, (unsigned char)0xc2, (unsigned char)0xd3, (unsigned char)0xac, (unsigned char)0x62, (unsigned char)0x91, (unsigned char)0x95, (unsigned char)0xe4, (unsigned char)0x79,
  (unsigned char)0xe7, (unsigned char)0xc8, (unsigned char)0x37, (unsigned char)0x6d, (unsigned char)0x8d, (unsigned char)0xd5, (unsigned char)0x4e, (unsigned char)0xa9, (unsigned char)0x6c, (unsigned char)0x56, (unsigned char)0xf4, (unsigned char)0xea, (unsigned char)0x65, (unsigned char)0x7a, (unsigned char)0xae, (unsigned char)0x08,
  (unsigned char)0xba, (unsigned char)0x78, (unsigned char)0x25, (unsigned char)0x2e, (unsigned char)0x1c, (unsigned char)0xa6, (unsigned char)0xb4, (unsigned char)0xc6, (unsigned char)0xe8, (unsigned char)0xdd, (unsigned char)0x74, (unsigned char)0x1f, (unsigned char)0x4b, (unsigned char)0xbd, (unsigned char)0x8b, (unsigned char)0x8a,
  (unsigned char)0x70, (unsigned char)0x3e, (unsigned char)0xb5, (unsigned char)0x66, (unsigned char)0x48, (unsigned char)0x03, (unsigned char)0xf6, (unsigned char)0x0e, (unsigned char)0x61, (unsigned char)0x35, (unsigned char)0x57, (unsigned char)0xb9, (unsigned char)0x86, (unsigned char)0xc1, (unsigned char)0x1d, (unsigned char)0x9e,
  (unsigned char)0xe1, (unsigned char)0xf8, (unsigned char)0x98, (unsigned char)0x11, (unsigned char)0x69, (unsigned char)0xd9, (unsigned char)0x8e, (unsigned char)0x94, (unsigned char)0x9b, (unsigned char)0x1e, (unsigned char)0x87, (unsigned char)0xe9, (unsigned char)0xce, (unsigned char)0x55, (unsigned char)0x28, (unsigned char)0xdf,
  (unsigned char)0x8c, (unsigned char)0xa1, (unsigned char)0x89, (unsigned char)0x0d, (unsigned char)0xbf, (unsigned char)0xe6, (unsigned char)0x42, (unsigned char)0x68, (unsigned char)0x41, (unsigned char)0x99, (unsigned char)0x2d, (unsigned char)0x0f, (unsigned char)0xb0, (unsigned char)0x54, (unsigned char)0xbb, (unsigned char)0x16 };

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
static const unsigned char rsbox[256] = {
  (unsigned char)0x52, (unsigned char)0x09, (unsigned char)0x6a, (unsigned char)0xd5, (unsigned char)0x30, (unsigned char)0x36, (unsigned char)0xa5, (unsigned char)0x38, (unsigned char)0xbf, (unsigned char)0x40, (unsigned char)0xa3, (unsigned char)0x9e, (unsigned char)0x81, (unsigned char)0xf3, (unsigned char)0xd7, (unsigned char)0xfb,
  (unsigned char)0x7c, (unsigned char)0xe3, (unsigned char)0x39, (unsigned char)0x82, (unsigned char)0x9b, (unsigned char)0x2f, (unsigned char)0xff, (unsigned char)0x87, (unsigned char)0x34, (unsigned char)0x8e, (unsigned char)0x43, (unsigned char)0x44, (unsigned char)0xc4, (unsigned char)0xde, (unsigned char)0xe9, (unsigned char)0xcb,
  (unsigned char)0x54, (unsigned char)0x7b, (unsigned char)0x94, (unsigned char)0x32, (unsigned char)0xa6, (unsigned char)0xc2, (unsigned char)0x23, (unsigned char)0x3d, (unsigned char)0xee, (unsigned char)0x4c, (unsigned char)0x95, (unsigned char)0x0b, (unsigned char)0x42, (unsigned char)0xfa, (unsigned char)0xc3, (unsigned char)0x4e,
  (unsigned char)0x08, (unsigned char)0x2e, (unsigned char)0xa1, (unsigned char)0x66, (unsigned char)0x28, (unsigned char)0xd9, (unsigned char)0x24, (unsigned char)0xb2, (unsigned char)0x76, (unsigned char)0x5b, (unsigned char)0xa2, (unsigned char)0x49, (unsigned char)0x6d, (unsigned char)0x8b, (unsigned char)0xd1, (unsigned char)0x25,
  (unsigned char)0x72, (unsigned char)0xf8, (unsigned char)0xf6, (unsigned char)0x64, (unsigned char)0x86, (unsigned char)0x68, (unsigned char)0x98, (unsigned char)0x16, (unsigned char)0xd4, (unsigned char)0xa4, (unsigned char)0x5c, (unsigned char)0xcc, (unsigned char)0x5d, (unsigned char)0x65, (unsigned char)0xb6, (unsigned char)0x92,
  (unsigned char)0x6c, (unsigned char)0x70, (unsigned char)0x48, (unsigned char)0x50, (unsigned char)0xfd, (unsigned char)0xed, (unsigned char)0xb9, (unsigned char)0xda, (unsigned char)0x5e, (unsigned char)0x15, (unsigned char)0x46, (unsigned char)0x57, (unsigned char)0xa7, (unsigned char)0x8d, (unsigned char)0x9d, (unsigned char)0x84,
  (unsigned char)0x90, (unsigned char)0xd8, (unsigned char)0xab, (unsigned char)0x00, (unsigned char)0x8c, (unsigned char)0xbc, (unsigned char)0xd3, (unsigned char)0x0a, (unsigned char)0xf7, (unsigned char)0xe4, (unsigned char)0x58, (unsigned char)0x05, (unsigned char)0xb8, (unsigned char)0xb3, (unsigned char)0x45, (unsigned char)0x06,
  (unsigned char)0xd0, (unsigned char)0x2c, (unsigned char)0x1e, (unsigned char)0x8f, (unsigned char)0xca, (unsigned char)0x3f, (unsigned char)0x0f, (unsigned char)0x02, (unsigned char)0xc1, (unsigned char)0xaf, (unsigned char)0xbd, (unsigned char)0x03, (unsigned char)0x01, (unsigned char)0x13, (unsigned char)0x8a, (unsigned char)0x6b,
  (unsigned char)0x3a, (unsigned char)0x91, (unsigned char)0x11, (unsigned char)0x41, (unsigned char)0x4f, (unsigned char)0x67, (unsigned char)0xdc, (unsigned char)0xea, (unsigned char)0x97, (unsigned char)0xf2, (unsigned char)0xcf, (unsigned char)0xce, (unsigned char)0xf0, (unsigned char)0xb4, (unsigned char)0xe6, (unsigned char)0x73,
  (unsigned char)0x96, (unsigned char)0xac, (unsigned char)0x74, (unsigned char)0x22, (unsigned char)0xe7, (unsigned char)0xad, (unsigned char)0x35, (unsigned char)0x85, (unsigned char)0xe2, (unsigned char)0xf9, (unsigned char)0x37, (unsigned char)0xe8, (unsigned char)0x1c, (unsigned char)0x75, (unsigned char)0xdf, (unsigned char)0x6e,
  (unsigned char)0x47, (unsigned char)0xf1, (unsigned char)0x1a, (unsigned char)0x71, (unsigned char)0x1d, (unsigned char)0x29, (unsigned char)0xc5, (unsigned char)0x89, (unsigned char)0x6f, (unsigned char)0xb7, (unsigned char)0x62, (unsigned char)0x0e, (unsigned char)0xaa, (unsigned char)0x18, (unsigned char)0xbe, (unsigned char)0x1b,
  (unsigned char)0xfc, (unsigned char)0x56, (unsigned char)0x3e, (unsigned char)0x4b, (unsigned char)0xc6, (unsigned char)0xd2, (unsigned char)0x79, (unsigned char)0x20, (unsigned char)0x9a, (unsigned char)0xdb, (unsigned char)0xc0, (unsigned char)0xfe, (unsigned char)0x78, (unsigned char)0xcd, (unsigned char)0x5a, (unsigned char)0xf4,
  (unsigned char)0x1f, (unsigned char)0xdd, (unsigned char)0xa8, (unsigned char)0x33, (unsigned char)0x88, (unsigned char)0x07, (unsigned char)0xc7, (unsigned char)0x31, (unsigned char)0xb1, (unsigned char)0x12, (unsigned char)0x10, (unsigned char)0x59, (unsigned char)0x27, (unsigned char)0x80, (unsigned char)0xec, (unsigned char)0x5f,
  (unsigned char)0x60, (unsigned char)0x51, (unsigned char)0x7f, (unsigned char)0xa9, (unsigned char)0x19, (unsigned char)0xb5, (unsigned char)0x4a, (unsigned char)0x0d, (unsigned char)0x2d, (unsigned char)0xe5, (unsigned char)0x7a, (unsigned char)0x9f, (unsigned char)0x93, (unsigned char)0xc9, (unsigned char)0x9c, (unsigned char)0xef,
  (unsigned char)0xa0, (unsigned char)0xe0, (unsigned char)0x3b, (unsigned char)0x4d, (unsigned char)0xae, (unsigned char)0x2a, (unsigned char)0xf5, (unsigned char)0xb0, (unsigned char)0xc8, (unsigned char)0xeb, (unsigned char)0xbb, (unsigned char)0x3c, (unsigned char)0x83, (unsigned char)0x53, (unsigned char)0x99, (unsigned char)0x61,
  (unsigned char)0x17, (unsigned char)0x2b, (unsigned char)0x04, (unsigned char)0x7e, (unsigned char)0xba, (unsigned char)0x77, (unsigned char)0xd6, (unsigned char)0x26, (unsigned char)0xe1, (unsigned char)0x69, (unsigned char)0x14, (unsigned char)0x63, (unsigned char)0x55, (unsigned char)0x21, (unsigned char)0x0c, (unsigned char)0x7d };
#endif

// The round constant word array, Rcon[i], contains the values given by 
// x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
static const unsigned char Rcon[11] = {
  (unsigned char)0x8d, (unsigned char)0x01, (unsigned char)0x02, (unsigned char)0x04, (unsigned char)0x08, (unsigned char)0x10, (unsigned char)0x20, (unsigned char)0x40, (unsigned char)0x80, (unsigned char)0x1b, (unsigned char)0x36 };

/*
 * Jordan Goulder points out in PR #12 (https://github.com/kokke/tiny-AES-C/pull/12),
 * that you can remove most of the elements in the Rcon array, because they are unused.
 *
 * From Wikipedia's article on the Rijndael key schedule @ https://en.wikipedia.org/wiki/Rijndael_key_schedule#Rcon
 * 
 * "Only the first some of these constants are actually used – up to rcon[10] for AES-128 (as 11 round keys are needed), 
 *  up to rcon[8] for AES-192, up to rcon[7] for AES-256. rcon[0] is not used in AES algorithm."
 */


/*****************************************************************************/
/* Private functions:                                                        */
/*****************************************************************************/
/*
static unsigned char getSBoxValue(unsigned char num)
{
  return sbox[num];
}
*/
#define getSBoxValue(num) (sbox[(num)])

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states. 
static void KeyExpansion(unsigned char* RoundKey, const unsigned char* Key)
{
  unsigned int i, j, k;
  unsigned char tempa[4]; // Used for the column/row operations
  
  // The first round key is the key itself.
  for (i = 0; i < Nk; ++i)
  {
    RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
    RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
    RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
    RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
  }

  // All other round keys are found from the previous round keys.
  for (i = Nk; i < Nb * (Nr + 1); ++i)
  {
    {
      k = (i - 1) * 4;
      tempa[0]=RoundKey[k + 0];
      tempa[1]=RoundKey[k + 1];
      tempa[2]=RoundKey[k + 2];
      tempa[3]=RoundKey[k + 3];

    }

    if (i % Nk == 0)
    {
      // This function shifts the 4 bytes in a word to the left once.
      // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

      // Function RotWord()
      {
        const unsigned char u8tmp = tempa[0];
        tempa[0] = tempa[1];
        tempa[1] = tempa[2];
        tempa[2] = tempa[3];
        tempa[3] = u8tmp;
      }

      // SubWord() is a function that takes a four-byte input word and 
      // applies the S-box to each of the four bytes to produce an output word.

      // Function Subword()
      {
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
      }

      tempa[0] = tempa[0] ^ Rcon[i/Nk];
    }
#if defined(AES256) && (AES256 == 1)
    if (i % Nk == 4)
    {
      // Function Subword()
      {
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
      }
    }
#endif
    j = i * 4; k=(i - Nk) * 4;
    RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
    RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
    RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
    RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
  }
}

void AES_init_ctx(struct AES_ctx* ctx, const unsigned char* key)
{
  KeyExpansion(ctx->RoundKey, key);
}
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
void AES_init_ctx_iv(struct AES_ctx* ctx, const unsigned char* key, const unsigned char* iv)
{
  KeyExpansion(ctx->RoundKey, key);
  memcpy (ctx->Iv, iv, AES_BLOCKLEN);
}
void AES_ctx_set_iv(struct AES_ctx* ctx, const unsigned char* iv)
{
  memcpy (ctx->Iv, iv, AES_BLOCKLEN);
}
#endif

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void AddRoundKey(unsigned char round, state_t* state, const unsigned char* RoundKey)
{
  unsigned char i,j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
    }
  }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void SubBytes(state_t* state)
{
  unsigned char i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);
    }
  }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void ShiftRows(state_t* state)
{
  unsigned char temp;

  // Rotate first row 1 columns to left  
  temp           = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left  
  temp           = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp           = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp           = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

static unsigned char xtime(unsigned char x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(state_t* state)
{
  unsigned char i;
  unsigned char Tmp, Tm, t;
  for (i = 0; i < 4; ++i)
  {  
    t   = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
    Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
  }
}

// Multiply is used to multiply numbers in the field GF(2^8)
// Note: The last call to xtime() is unneeded, but often ends up generating a smaller binary
//       The compiler seems to be able to vectorize the operation better this way.
//       See https://github.com/kokke/tiny-AES-c/pull/34
#if MULTIPLY_AS_A_FUNCTION
static unsigned char Multiply(unsigned char x, unsigned char y)
{
  return (((y & 1) * x) ^
       ((y>>1 & 1) * xtime(x)) ^
       ((y>>2 & 1) * xtime(xtime(x))) ^
       ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
       ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
  }
#else
#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

#endif

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
/*
static unsigned char getSBoxInvert(unsigned char num)
{
  return rsbox[num];
}
*/
#define getSBoxInvert(num) (rsbox[(num)])

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
static void InvMixColumns(state_t* state)
{
  int i;
  unsigned char a, b, c, d;
  for (i = 0; i < 4; ++i)
  { 
    a = (*state)[i][0];
    b = (*state)[i][1];
    c = (*state)[i][2];
    d = (*state)[i][3];

    (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
  }
}


// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes(state_t* state)
{
  unsigned char i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxInvert((*state)[j][i]);
    }
  }
}

static void InvShiftRows(state_t* state)
{
  unsigned char temp;

  // Rotate first row 1 columns to right  
  temp = (*state)[3][1];
  (*state)[3][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[0][1];
  (*state)[0][1] = temp;

  // Rotate second row 2 columns to right 
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[1][3];
  (*state)[1][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[3][3];
  (*state)[3][3] = temp;
}
#endif // #if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)

// Cipher is the main function that encrypts the PlainText.
static void Cipher(state_t* state, const unsigned char* RoundKey)
{
  unsigned char round = 0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(0, state, RoundKey);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr rounds are executed in the loop below.
  // Last one without MixColumns()
  for (round = 1; ; ++round)
  {
    SubBytes(state);
    ShiftRows(state);
    if (round == Nr) {
      break;
    }
    MixColumns(state);
    AddRoundKey(round, state, RoundKey);
  }
  // Add round key to last round
  AddRoundKey(Nr, state, RoundKey);
}

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
static void InvCipher(state_t* state, const unsigned char* RoundKey)
{
  unsigned char round = 0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(Nr, state, RoundKey);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr rounds are executed in the loop below.
  // Last one without InvMixColumn()
  for (round = (Nr - 1); ; --round)
  {
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(round, state, RoundKey);
    if (round == 0) {
      break;
    }
    InvMixColumns(state);
  }

}
#endif // #if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)

/*****************************************************************************/
/* Public functions:                                                         */
/*****************************************************************************/
#if defined(ECB) && (ECB == 1)


void AES_ECB_encrypt(const struct AES_ctx* ctx, unsigned char* buf)
{
  // The next function call encrypts the PlainText with the Key using AES algorithm.
  Cipher((state_t*)buf, ctx->RoundKey);
}

void AES_ECB_decrypt(const struct AES_ctx* ctx, unsigned char* buf)
{
  // The next function call decrypts the PlainText with the Key using AES algorithm.
  InvCipher((state_t*)buf, ctx->RoundKey);
}


#endif // #if defined(ECB) && (ECB == 1)





#if defined(CBC) && (CBC == 1)


static void XorWithIv(unsigned char* buf, const unsigned char* Iv)
{
  unsigned char i;
  for (i = 0; i < AES_BLOCKLEN; ++i) // The block in AES is always 128bit no matter the key size
  {
    buf[i] ^= Iv[i];
  }
}

void AES_CBC_encrypt_buffer(struct AES_ctx *ctx, unsigned char* buf, unsigned int length)
{
  unsigned int i;
  unsigned char *Iv = ctx->Iv;
  for (i = 0; i < length; i += AES_BLOCKLEN)
  {
    XorWithIv(buf, Iv);
    Cipher((state_t*)buf, ctx->RoundKey);
    Iv = buf;
    buf += AES_BLOCKLEN;
  }
  /* store Iv in ctx for next call */
  memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, unsigned char* buf, unsigned int length)
{
  unsigned int i;
  unsigned char storeNextIv[AES_BLOCKLEN];
  for (i = 0; i < length; i += AES_BLOCKLEN)
  {
    memcpy(storeNextIv, buf, AES_BLOCKLEN);
    InvCipher((state_t*)buf, ctx->RoundKey);
    XorWithIv(buf, ctx->Iv);
    memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
    buf += AES_BLOCKLEN;
  }

}

#endif // #if defined(CBC) && (CBC == 1)



#if defined(CTR) && (CTR == 1)

/* Symmetrical operation: same function for encrypting as for decrypting. Note any IV/nonce should never be reused with the same key */
void AES_CTR_xcrypt_buffer(struct AES_ctx* ctx, unsigned char* buf, unsigned int length)
{
  unsigned char buffer[AES_BLOCKLEN];
  
  unsigned int i;
  int bi;
  for (i = 0, bi = AES_BLOCKLEN; i < length; ++i, ++bi)
  {
    if (bi == AES_BLOCKLEN) /* we need to regen xor compliment in buffer */
    {
      
      memcpy(buffer, ctx->Iv, AES_BLOCKLEN);
      Cipher((state_t*)buffer,ctx->RoundKey);

      /* Increment Iv and handle overflow */
      for (bi = (AES_BLOCKLEN - 1); bi >= 0; --bi)
      {
	/* inc will overflow */
        if (ctx->Iv[bi] == 255)
	{
          ctx->Iv[bi] = 0;
          continue;
        } 
        ctx->Iv[bi] += 1;
        break;   
      }
      bi = 0;
    }

    buf[i] = (buf[i] ^ buffer[bi]);
  }
}

#endif // #if defined(CTR) && (CTR == 1)