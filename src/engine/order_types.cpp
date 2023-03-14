#include "i_order_types.hpp"

#include <ostream>

std::ostream &operator<<(std::ostream &os, const order_id &id)
{
    return (os << id.agent << "," << id.seq);
}
