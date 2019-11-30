//
// Created by omerh on 29/11/2019.
//

#ifndef CRYPTOPALS_MD4_H
#define CRYPTOPALS_MD4_H

#include <cstddef>
#include <algorithm>
#include <model/internal/utils.h>
#include <model/exceptions.h>

namespace model
{
    namespace internal
    {
        namespace hash
        {
            template<class UIntType, class LengthType, UIntType A, UIntType B, UIntType C, UIntType D>
            class md4
            {
            private:
                using encoder = model::internal::endianness_encoder<UIntType, model::internal::endianness::little>;

            public:
                static const size_t hash_size = 16;

                /**
                 * Initializes the sha1 object
                 */
                md4() : _a(A), _b(B), _c(C), _d(D), _buffer_index(0), _buffer(), _message_length(0)
                {}

                /**
                 * Updates the state with more data
                 *
                 * @param data   The data to enter to the hash calculation
                 * @param length The length of the data
                 */
                void update(const unsigned char data[], size_t length)
                {
                    for (auto i = 0; i < length; ++i)
                    {
                        _buffer[_buffer_index++] = data[i];
                        if (_buffer_index == sizeof(_buffer))
                        {
                            update_state();
                        }
                    }

                    _message_length += length;
                }

                /**
                 * Digest all the updated data into a hash 160 bit hash
                 * @param o_hash The buffer to put the hash into.
                 * @param length The length of the buffer, must be at least 20 bytes
                 */
                void digest(unsigned char o_hash[], size_t length)
                {
                    pad_and_update();
                    encode_hash(o_hash, length);

                    reset();
                }

                /**
                 * Reset the internal state of the sha1 to default while discarding all previous updates
                 */
                void reset()
                {
                    _a = A;
                    _b = B;
                    _c = C;
                    _d = D;
                    _buffer_index = 0;
                    std::fill_n(_buffer, sizeof(_buffer), 0);
                    _message_length = 0;
                }

            private:

                void pad_and_update()
                {
                    //
                    // _buffer_index always points to the next writeable index. so we just need
                    //  to write to it 0x80 (1000 0000)
                    //
                    _buffer[_buffer_index++] = 0x80;

                    while (_buffer_index != sizeof(_buffer) - sizeof(LengthType))
                    {
                        if (_buffer_index == sizeof(_buffer))
                        {
                            update_state();
                        }

                        _buffer[_buffer_index++] = 0;
                    }

                    // Encode the length
                    model::internal::endianness_encoder<LengthType, model::internal::endianness::little>::encode(
                            _message_length * CHAR_BIT,
                            *((unsigned char (*)[sizeof(LengthType)]) &_buffer[sizeof(_buffer) - sizeof(LengthType)]));
                    _buffer_index += sizeof(LengthType);

                    update_state();
                }

                void update_state()
                {
                    if (_buffer_index != sizeof(_buffer))
                    {
                        throw model::internal_error("Cannot update state (the buffer is not full)");
                    }

                    UIntType x[16];

                    for (int i = 0; i < 16; ++i)
                    {
                        x[i] = encoder::decode(*((unsigned char (*)[sizeof(UIntType)]) (_buffer + i * 4)));
                    }

                    UIntType AA = _a;
                    UIntType BB = _b;
                    UIntType CC = _c;
                    UIntType DD = _d;

                    update_round_type1(_a, _b, _c, _d, x[0], 3);
                    update_round_type1(_d, _a, _b, _c, x[1], 7);
                    update_round_type1(_c, _d, _a, _b, x[2], 11);
                    update_round_type1(_b, _c, _d, _a, x[3], 19);
                    update_round_type1(_a, _b, _c, _d, x[4], 3);
                    update_round_type1(_d, _a, _b, _c, x[5], 7);
                    update_round_type1(_c, _d, _a, _b, x[6], 11);
                    update_round_type1(_b, _c, _d, _a, x[7], 19);
                    update_round_type1(_a, _b, _c, _d, x[8], 3);
                    update_round_type1(_d, _a, _b, _c, x[9], 7);
                    update_round_type1(_c, _d, _a, _b, x[10], 11);
                    update_round_type1(_b, _c, _d, _a, x[11], 19);
                    update_round_type1(_a, _b, _c, _d, x[12], 3);
                    update_round_type1(_d, _a, _b, _c, x[13], 7);
                    update_round_type1(_c, _d, _a, _b, x[14], 11);
                    update_round_type1(_b, _c, _d, _a, x[15], 19);

                    update_round_type2(_a, _b, _c, _d, x[0], 3);
                    update_round_type2(_d, _a, _b, _c, x[4], 5);
                    update_round_type2(_c, _d, _a, _b, x[8], 9);
                    update_round_type2(_b, _c, _d, _a, x[12], 13);
                    update_round_type2(_a, _b, _c, _d, x[1], 3);
                    update_round_type2(_d, _a, _b, _c, x[5], 5);
                    update_round_type2(_c, _d, _a, _b, x[9], 9);
                    update_round_type2(_b, _c, _d, _a, x[13], 13);
                    update_round_type2(_a, _b, _c, _d, x[2], 3);
                    update_round_type2(_d, _a, _b, _c, x[6], 5);
                    update_round_type2(_c, _d, _a, _b, x[10], 9);
                    update_round_type2(_b, _c, _d, _a, x[14], 13);
                    update_round_type2(_a, _b, _c, _d, x[3], 3);
                    update_round_type2(_d, _a, _b, _c, x[7], 5);
                    update_round_type2(_c, _d, _a, _b, x[11], 9);
                    update_round_type2(_b, _c, _d, _a, x[15], 13);

                    update_round_type3(_a, _b, _c, _d, x[0], 3);
                    update_round_type3(_d, _a, _b, _c, x[8], 9);
                    update_round_type3(_c, _d, _a, _b, x[4], 11);
                    update_round_type3(_b, _c, _d, _a, x[12], 15);
                    update_round_type3(_a, _b, _c, _d, x[2], 3);
                    update_round_type3(_d, _a, _b, _c, x[10], 9);
                    update_round_type3(_c, _d, _a, _b, x[6], 11);
                    update_round_type3(_b, _c, _d, _a, x[14], 15);
                    update_round_type3(_a, _b, _c, _d, x[1], 3);
                    update_round_type3(_d, _a, _b, _c, x[9], 9);
                    update_round_type3(_c, _d, _a, _b, x[5], 11);
                    update_round_type3(_b, _c, _d, _a, x[13], 15);
                    update_round_type3(_a, _b, _c, _d, x[3], 3);
                    update_round_type3(_d, _a, _b, _c, x[11], 9);
                    update_round_type3(_c, _d, _a, _b, x[7], 11);
                    update_round_type3(_b, _c, _d, _a, x[15], 15);

                    _a += AA;
                    _b += BB;
                    _c += CC;
                    _d += DD;

                    std::fill_n(_buffer,
                                sizeof(_buffer), 0);
                    _buffer_index = 0;
                }

                void encode_hash(const unsigned char o_hash[], size_t length) const
                {
                    if (length < hash_size)
                    {
                        throw std::length_error("Not enough space in the hash vector provided");
                    }

                    encoder::encode(_a, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 0]));
                    encoder::encode(_b, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 1]));
                    encoder::encode(_c, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 2]));
                    encoder::encode(_d, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 3]));
                }

                static constexpr UIntType F(UIntType X, UIntType Y, UIntType Z)
                { return (X & Y) | (~X & Z); }

                static constexpr UIntType G(UIntType X, UIntType Y, UIntType Z)
                { return (X & Y) | (X & Z) | (Y & Z); }

                static constexpr UIntType H(UIntType X, UIntType Y, UIntType Z)
                { return X ^ Y ^ Z; }

                static constexpr void update_round_type1(UIntType &a,
                                                         UIntType &b,
                                                         UIntType &c,
                                                         UIntType &d,
                                                         UIntType x,
                                                         UIntType s)
                {
                    a += F(b, c, d) + x;
                    a = model::internal::circular_left_shift(a, s);
                }

                static constexpr void update_round_type2(UIntType &a,
                                                         UIntType &b,
                                                         UIntType &c,
                                                         UIntType &d,
                                                         UIntType x,
                                                         UIntType s)
                {
                    a += G(b, c, d) + x + 0x5A827999;
                    a = model::internal::circular_left_shift(a, s);
                }

                static constexpr void update_round_type3(UIntType &a,
                                                         UIntType &b,
                                                         UIntType &c,
                                                         UIntType &d,
                                                         UIntType x,
                                                         UIntType s)
                {
                    a += H(b, c, d) + x + 0x6ED9EBA1;
                    a = model::internal::circular_left_shift(a, s);
                }

            private:

                LengthType _message_length;

                size_t _buffer_index;
                unsigned char _buffer[64];

                UIntType _a;
                UIntType _b;
                UIntType _c;
                UIntType _d;
            };

        };
    }
}

#endif //CRYPTOPALS_MD4_H
