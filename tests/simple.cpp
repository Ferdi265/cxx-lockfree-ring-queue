#include <cassert>
#include <optional>

#include "lockfree_ring_queue.hpp"

int main() {
    lockfree_ring_queue<int, 10> queue;

    assert(queue.push(42));
    assert(queue.push(10));

    assert(queue.pop() == 42);
    assert(queue.pop() == 10);
    assert(queue.pop() == std::nullopt);
}
