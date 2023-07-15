#ifndef YRLF_LOCKFREE_RING_QUEUE_
#define YRLF_LOCKFREE_RING_QUEUE_

#include <cstddef>
#include <atomic>
#include <array>
#include <optional>
#include <type_traits>

#include "lazy-constructed.hpp"

template <typename T, size_t N>
struct lockfree_ring_queue {
    private:
        std::array<lazy_constructed<T>, N> data;
        std::atomic<size_t> write_head;
        std::atomic<size_t> read_head;

    public:
        lockfree_ring_queue() noexcept {
            read_head.store(0);
            write_head.store(0);
        }

        ~lockfree_ring_queue() noexcept(std::is_nothrow_destructible_v<T>) {
            for (size_t i = read_head.load(), end = write_head.load(); i != end; i++) {
                data[i % N].destruct();
            }
        }

        lockfree_ring_queue(const lockfree_ring_queue&) = delete;
        lockfree_ring_queue(lockfree_ring_queue&&) = delete;
        lockfree_ring_queue& operator=(const lockfree_ring_queue&) = delete;
        lockfree_ring_queue& operator=(lockfree_ring_queue&&) = delete;

        bool can_pop() const noexcept {
            size_t read_index = read_head.load();
            size_t write_index = write_head.load();

            return read_index != write_index;
        }

        bool can_push() const noexcept {
            size_t read_index = read_head.load();
            size_t write_index = write_head.load();

            return read_index + N != write_index;
        }

        std::optional<T> pop() noexcept(std::is_nothrow_destructible_v<T> && std::is_nothrow_move_constructible_v<T>) {
            size_t read_index = read_head.load();
            size_t write_index = write_head.load();

            if (read_index == write_index) {
                // no data available
                return std::nullopt;
            }

            std::optional<T> value;
            value = std::move(data[read_index % N].get());
            data[read_index % N].destruct();

            read_head.store(read_index + 1);

            return value;
        }

        bool push(T value) noexcept(std::is_nothrow_destructible_v<T> && std::is_nothrow_move_constructible_v<T>) {
            size_t read_index = read_head.load();
            size_t write_index = write_head.load();

            if (read_index + N == write_index) {
                // no space available
                return false;
            }

            data[write_index % N].construct(std::move(value));

            write_head.store(write_index + 1);

            return true;
        }
};

#endif
