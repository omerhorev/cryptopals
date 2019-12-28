//
// Created by omerh on 20/12/2019.
//

#ifndef CRYPTOPALS_INTERNAL_DIFFIE_HELLMAN_H
#define CRYPTOPALS_INTERNAL_DIFFIE_HELLMAN_H

#include <cstddef>
#include <math/numbers.h>

namespace model
{
    namespace internal
    {
        template<size_t NumbersSizeInBits>
        class diffie_hellman
        {
            static_assert(NumbersSizeInBits % 8 == 0, "only bits count that are multiplicand of 8 are supported");

        public:
            using num_t = math::num_t<NumbersSizeInBits>;

            /**
             * Initializes the mathematical engine of the diffie-hellman protocol
             *
             * @param p The universal parameter p
             * @param g The universal paramater g
             */
            diffie_hellman(const num_t p, const num_t g) : _p(p), _g(g)
            {}

            /**
             * Seeds the engine with a private key
             *
             * @param private_key The private key to seed with
             */
            void seed(const num_t &private_key)
            {
                _private_key = private_key;
            }

            /**
             * Generates a public key from the parameters provided
             *
             * @return The public key generated
             */
            num_t generate_public_key()
            {
                num_t _ = _g;
                _.apply_modular_power(_private_key, _p);

                return _;
            }

            /**
             * Provided a public key of another, generates a session key
             *
             * @param other_public The other persons public key
             * @return The session key
             */
            num_t generate_session_key(const num_t &other_public)
            {
                num_t _ = other_public;
                _.apply_modular_power(_private_key, _p);

                return _;
            }

            num_t get_p() const
            { return _p; }

            num_t get_g() const
            { return _g; }

        private:
            num_t _p, _g;
            num_t _private_key;
        };
    }
}

#endif //CRYPTOPALS_INTERNAL_DIFFIE_HELLMAN_H
