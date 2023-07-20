#ifndef YRLF_LAZY_CONSTRUCTED_
#define YRLF_LAZY_CONSTRUCTED_

#include <utility>
#include <type_traits>

template <typename T>
struct lazy_constructed {
    private:
        std::aligned_storage_t<sizeof(T), alignof(T)> storage;

    public:
        lazy_constructed() noexcept = default;
        ~lazy_constructed() noexcept = default;

        lazy_constructed(const lazy_constructed&) = delete;
        lazy_constructed(lazy_constructed&&) = delete;
        lazy_constructed& operator=(const lazy_constructed&) = delete;
        lazy_constructed& operator=(lazy_constructed&&) = delete;

        T& get() noexcept {
            return reinterpret_cast<T&>(storage);
        }

        const T& get() const noexcept {
            return reinterpret_cast<const T&>(storage);
        }

        T& operator*() noexcept {
            return get();
        }

        const T& operator*() const noexcept {
            return get();
        }

        T* operator->() noexcept {
            return &get();
        }

        const T* operator->() const noexcept {
            return &get();
        }

        template <typename... Args>
        void construct(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
            new (&storage) T(std::forward<Args>(args)...);
        }

        void destruct() noexcept(std::is_nothrow_destructible_v<T>) {
            get().~T();
        }
};

#endif
