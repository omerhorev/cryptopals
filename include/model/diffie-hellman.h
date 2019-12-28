//
// Created by omerh on 20/12/2019.
//

#ifndef CRYPTOPALS_DIFFIE_HELLMAN_H
#define CRYPTOPALS_DIFFIE_HELLMAN_H

#include <cstddef>
#include <algorithm>
#include "math/numbers.h"
#include "internal/diffie-hellman/diffie-hellman.h"

namespace model
{
    template<size_t BitsSize>
    struct diffie_hellman_defaults;

    template<>
    struct diffie_hellman_defaults<512>
    {
        static const size_t SizeInBits = 512;
        using num_t = math::num_t<SizeInBits>;

        static const num_t default_p()
        {
            static const unsigned char _[] = {0x7c, 0xa6, 0x82, 0xce, 0x8e, 0x12, 0xca, 0xba, 0x26, 0xef, 0xcc, 0xf7,
                                              0x11, 0x0e, 0x52, 0x6d, 0xb0, 0x78, 0xb0, 0x5e, 0xde, 0xcb, 0xcd, 0x1e,
                                              0xb4, 0xa2, 0x08, 0xf3, 0xae, 0x16, 0x17, 0xae, 0x01, 0xf3, 0x5b, 0x91,
                                              0xa4, 0x7e, 0x6d, 0xf6, 0x34, 0x13, 0xc5, 0xe1, 0x2e, 0xd0, 0x89, 0x9b,
                                              0xcd, 0x13, 0x2a, 0xcd, 0x50, 0xd9, 0x91, 0x51, 0xbd, 0xc4, 0x3e, 0xe7,
                                              0x37, 0x59, 0x2e, 0x17,};

            return num_t(_);
        }

        static const num_t default_q()
        {
            static const unsigned char _[] = {0x67, 0x84, 0x71, 0xb2, 0x7a, 0x9c, 0xf4, 0x4e, 0xe9, 0x1a, 0x49, 0xc5,
                                              0x14, 0x7d, 0xb1, 0xa9, 0xaa, 0xf2, 0x44, 0xf0, 0x5a, 0x43, 0x4d, 0x64,
                                              0x86, 0x93, 0x1d, 0x2d, 0x14, 0x27, 0x1b, 0x9e, 0x35, 0x03, 0x0b, 0x71,
                                              0xfd, 0x73, 0xda, 0x17, 0x90, 0x69, 0xb3, 0x2e, 0x29, 0x35, 0x63, 0x0e,
                                              0x1c, 0x20, 0x62, 0x35, 0x4d, 0x0d, 0xa2, 0x0a, 0x6c, 0x41, 0x6e, 0x50,
                                              0xbe, 0x79, 0x4c, 0xa4,};

            return num_t(_);
        }
    };

    template<size_t SizeInBits>
    class diffie_hellman
    {
        static_assert(SizeInBits % 8 == 0, "only bits count that are multiplicand of 8 are supported");

    private:
        static const size_t SizeInBytes = SizeInBits / 8;
        using defaults = diffie_hellman_defaults<SizeInBits>;
        using dh_t = internal::diffie_hellman<SizeInBits>;
        using number_t = typename internal::diffie_hellman<SizeInBits>::num_t;

    public:
        diffie_hellman() : _dh(defaults::default_p(), defaults::default_q())
        {}

        diffie_hellman(const unsigned char (&p)[SizeInBytes], const unsigned char (&g)[SizeInBytes]) : _dh(p, g)
        {}

        void seed(const unsigned char key[], size_t length)
        {
            if (length != SizeInBytes)
            {
                throw std::length_error("Invalid key length");
            }

            auto number = *((number_t *) key);
            _dh.seed(number);
        }

        void generate_public_key(unsigned char (&key)[SizeInBytes])
        {
            auto &number = *((number_t *) key);
            number = _dh.generate_public_key();
        }

        void generate_public_key(unsigned char key[], size_t length)
        {
            if (length != SizeInBytes)
            {
                throw std::length_error("Invalid key length");
            }

            auto k = _dh.generate_public_key();
            std::copy_n((unsigned char *) &k, SizeInBytes, key);
        }

        void generate_session_key(const unsigned char (&public_key)[SizeInBytes],
                                  unsigned char (&session_key)[SizeInBytes])
        {
            auto &number = *((number_t *) public_key);
            auto k = _dh.generate_session_key(number);
            auto _ = (unsigned char *) &k;
            std::copy_n(_, SizeInBytes, session_key);
        }

        void generate_session_key(const unsigned char public_key[], size_t public_key_length,
                                  unsigned char session_key[], size_t session_key_length)
        {
            auto &number = *((number_t *) public_key);

            auto k = _dh.generate_session_key(number);
            std::copy_n((unsigned char *) &k, SizeInBytes, session_key);
        }

        void get_p(unsigned char o_p[], size_t length) const
        {
            if (length != SizeInBytes)
            { throw std::length_error("Invalid key length"); }

            auto &number = *((number_t *) o_p);

            number = _dh.get_p();
        }

        void get_g(unsigned char o_g[], size_t length) const
        {
            if (length != SizeInBytes)
            { throw std::length_error("Invalid key length"); }

            auto &number = *((number_t *) o_g);

            number = _dh.get_g();
        }

    private:
        model::internal::diffie_hellman<SizeInBits> _dh;
    };
}

#endif //CRYPTOPALS_DIFFIE_HELLMAN_H
