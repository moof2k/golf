/*
 *  RudeUnicode.h
 *  golf
 *
 *  Created by Robert Rose on 10/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

typedef enum
{
	kLittleEndian,
	kBigEndian,
} EUnicodeByteOrder;

// This function will attempt to decode a UTF-8 encoded character in the buffer.
// If the encoding is invalid, the function returns -1.
int DecodeUTF8(const char *encodedBuffer, unsigned int *outCharLength);

// This function will encode the value into the buffer.
// If the value is invalid, the function returns -1, else the encoded length.
int EncodeUTF8(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength);

// This function will attempt to decode a UTF-16 encoded character in the buffer.
// If the encoding is invalid, the function returns -1.
int DecodeUTF16(const char *encodedBuffer, unsigned int *outCharLength, EUnicodeByteOrder byteOrder = kLittleEndian);

// This function will encode the value into the buffer.
// If the value is invalid, the function returns -1, else the encoded length.
int EncodeUTF16(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength, EUnicodeByteOrder byteOrder = kLittleEndian);
