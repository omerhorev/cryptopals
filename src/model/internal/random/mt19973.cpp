//
// Created by omerh on 06/11/2019.
//

#include <model/internal/random/mt19937.h>

using namespace model;
using namespace model::internal;
using namespace model::internal::random;

void mt19937::seed(const unsigned char seed[], size_t number)
{

}

void mt19937::generate(unsigned char o_data[], size_t data_length)
{
    if (_state_used_vector_index >= state_vectors_count)
    {
        twist();
        _state_used_vector_index = 0;
    }

    

    _state_used_vector_index++;
}

void mt19937::twist()
{

}
