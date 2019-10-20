//
// Created by omerh on 06/09/2019.
//

#ifndef CRYPTOPALS_ECB_BLOCK_PARSER_H
#define CRYPTOPALS_ECB_BLOCK_PARSER_H

#include <cstddef>
#include <algorithm>
#include <stdexcept>

namespace breaks
{
    namespace internal
    {
        /**
         * Use for parsing and handling encrypted data divided into blocks
         */
        class ecb_block_parser
        {
        public:
            struct block
            {
                block(unsigned char *d, size_t i, size_t l) : base_data(d), length(l), index(i)
                {}

                unsigned char *base_data;
                size_t length;
                size_t index;

                /**
                 * @return Returns the data in this block
                 */
                unsigned char *data() const
                { return base_data + index * length; }

                /**
                 * @param b The other block to compare to
                 * @return  Whether the two blocks are equal in their content
                 */
                bool operator==(const block &b) const
                { return length == b.length && std::equal(data(), data() + length, b.data()); }
            };

            struct block_iterator : public std::iterator<std::random_access_iterator_tag, block>
            {
                explicit block_iterator(block b) : _block(b)
                {}

                auto operator*()
                { return _block; }

                bool operator==(const block_iterator &iter) const
                { return _block == iter._block; }

                bool operator!=(const block_iterator &iter) const
                { return !(*this == iter); }

                const block_iterator operator++(int)
                {
                    block ret = _block;

                    ++(*this);
                    return block_iterator(ret);
                }

                const block_iterator operator++()
                {
                    _block.index += 1;
                    return *this;
                }

                const block_iterator operator--(int)
                {
                    block ret = _block;
                    --(*this);
                    return block_iterator(ret);
                }

                const block_iterator operator--()
                {
                    _block.index -= 1;
                    return *this;
                }

                block_iterator operator+(int offset) const
                { return block_iterator(block(_block.base_data, _block.index + offset, _block.length)); }

                block_iterator operator+=(int offset)
                {
                    _block.index += offset;
                    return *this;
                }

                auto operator-(const block_iterator &other) const
                { return _block.index - other._block.index; }

                auto operator-(int count) const
                { return block_iterator(block(_block.base_data, _block.index - count, _block.length)); }

                auto operator-=(int offset)
                {
                    _block.index -= offset;
                    return *this;
                }

                block _block;
            };

            /**
             * @param buffer       The data we are working on
             * @param length       The length of the data we are working on
             * @param block_size   The block size
             */
            ecb_block_parser(unsigned char buffer[], size_t length, size_t block_size) :
                    _cipher(buffer), _blocks_count(length / block_size), _block_size(block_size)
            {
                if (length % block_size != 0)
                {
                    throw std::length_error("The length of the buffer must be a multiplicand of the block size");
                }
            }

            /**
             * Returns a block in a specific index
             *
             * @param index The index of the block (first, second, etc)
             * @return The block in the index specified
             */
            block get(size_t index) const
            { return {_cipher, index, _block_size}; }

            /**
             * @return an iterator pointing to the start of the buffer
             */
            block_iterator begin()
            { return block_iterator(block(_cipher, 0, _block_size)); };

            /**
             * @return an iterator pointing to the end of the buffer (one block after the last)
             */
            block_iterator end()
            { return block_iterator(block(_cipher, _blocks_count, _block_size)); }

            /**
             * Returns a block in a specific index
             *
             * @param index The index of the block (first, second, etc)
             * @return The block in the index specified
             */
            block operator[](size_t index)
            { return get(index); }

            size_t blocks_count()
            { return end() - begin(); }

        private:
            unsigned char *_cipher;
            size_t _blocks_count;
            size_t _block_size;
        };
    }
}

#endif //CRYPTOPALS_ECB_BLOCK_PARSER_H
