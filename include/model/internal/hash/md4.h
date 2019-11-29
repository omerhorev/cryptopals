//
// Created by omerh on 29/11/2019.
//

#ifndef CRYPTOPALS_MD4_H
#define CRYPTOPALS_MD4_H

#include <cstddef>
#include <model/exceptions.h>
#include <model/internal/utils.h>

namespace model
{
    namespace internal
    {
        namespace hash
        {
            template<class UIntType, class LengthType, UIntType H0, UIntType H1, UIntType H2, UIntType H3, UIntType H4>
            class md4
            {

            public:
                static const size_t hash_size = 20;

                /**
                 * Initializes the sha1 object
                 */
                md4() : _h0(H0), _h1(H1), _h2(H2), _h3(H3), _h4(H4), _buffer_index(0), _buffer(),
                         _message_length(0)
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
                    _h0 = H0;
                    _h1 = H1;
                    _h2 = H2;
                    _h3 = H3;
                    _h4 = H4;
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
                    model::internal::endianness_encoder<LengthType, model::internal::endianness::big>::encode(
                            _message_length * CHAR_BIT,
                            *((unsigned char (*)[sizeof(LengthType)]) &_buffer[sizeof(_buffer) -
                                                                               sizeof(LengthType)]));
                    _buffer_index += sizeof(LengthType);

                    update_state();
                }

                void update_state()
                {
                    using encoder = model::internal::endianness_encoder<UIntType, model::internal::endianness::big>;

                    if (_buffer_index != sizeof(_buffer))
                    {
                        throw model::internal_error("Cannot update state (the buffer is not full)");
                    }

                    UIntType W[80] = {0};
                    UIntType A = _h0, B = _h1, C = _h2, D = _h3, E = _h4;

                    for (unsigned int i = 0; i < 16; ++i)
                    {
                        W[i] = encoder::decode(
                                *((unsigned char (*)[sizeof(UIntType)]) &_buffer[i * sizeof(UIntType)]));
                    }

                    for (unsigned int t = 16; t < 80; ++t)
                    {
                        W[t] = model::internal::circular_left_shift(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
                    }

                    for (unsigned int t = 0; t < 80; ++t)
                    {
                        UIntType temp =
                                model::internal::circular_left_shift(A, 5) + f(t, B, C, D) + E + W[t] + K(t);

                        E = D;
                        D = C;
                        C = circular_left_shift(B, 30);
                        B = A;
                        A = temp;
                    }

                    _h0 += A;
                    _h1 += B;
                    _h2 += C;
                    _h3 += D;
                    _h4 += E;

                    std::fill_n(_buffer, sizeof(_buffer), 0);
                    _buffer_index = 0;
                }

                void encode_hash(const unsigned char o_hash[], size_t length) const
                {
                    if (length < hash_size)
                    {
                        throw std::length_error("Not enough space in the hash vector provided");
                    }

                    model::internal::endianness_encoder<UIntType, model::internal::endianness::big>::encode(
                            _h0, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 0]));
                    model::internal::endianness_encoder<UIntType, model::internal::endianness::big>::encode(
                            _h1, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 1]));
                    model::internal::endianness_encoder<UIntType, model::internal::endianness::big>::encode(
                            _h2, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 2]));
                    model::internal::endianness_encoder<UIntType, model::internal::endianness::big>::encode(
                            _h3, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 3]));
                    model::internal::endianness_encoder<UIntType, model::internal::endianness::big>::encode(
                            _h4, *((unsigned char (*)[sizeof(UIntType)]) &o_hash[sizeof(UIntType) * 4]));
                }

                constexpr UIntType f(unsigned int t, UIntType B, UIntType C, UIntType D) const
                {
                    if (t >= 0 && t < 20) return (B & C) | ((~B) & D);
                    else if (t >= 20 && t < 40) return B ^ C ^ D;
                    else if (t >= 40 && t < 60) return (B & C) | (B & D) | (C & D);
                    else if (t >= 60 && t < 80) return B ^ C ^ D;
                    else throw internal_error("Invalid round for sha1::f");
                }

                constexpr UIntType K(unsigned int t) const
                {
                    if (t >= 0 && t < 20) return 0x5A827999;
                    else if (t >= 20 && t < 40) return 0x6ED9EBA1;
                    else if (t >= 40 && t < 60) return 0x8F1BBCDC;
                    else if (t >= 60 && t < 80) return 0xCA62C1D6;

                    else throw internal_error("Invalid round for sha1::K");
                }


            private:

                LengthType _message_length;

                size_t _buffer_index;
                unsigned char _buffer[64];

                UIntType _h0;
                UIntType _h1;
                UIntType _h2;
                UIntType _h3;
                UIntType _h4;
            };

        };
    }
}
}

#endif //CRYPTOPALS_MD4_H
