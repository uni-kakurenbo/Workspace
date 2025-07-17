#include <iostream>
#include <ranges>
#include <functional>
#include <generator>
#include <print>

#include "numeric/bit.hpp"
#include "data_structure/dynamic_set.hpp"

namespace primitive {


// 集合 (UNIQUE = false なら多重集合)
template<class T, bool UNIQUE = true>
struct set {
  private:
    uni::dynamic_set<T> _impl;

  public:
    template<class... Args>
    set(Args&&... values) {
        this->_impl.template assign<UNIQUE>(std::forward<Args>(values)...);
    }

    template<class U>
    set(std::initializer_list<U> values) {
        this->_impl.template assign<UNIQUE>(values);
    }

    template<class Set>
    inline auto swap(Set&& other) noexcept {
        this->_impl.swap(other._impl);
    }

    // 濃度
    inline auto size() const noexcept { return this->_impl.size(); }

    // 属する
    template<class U>
    inline auto contains(U&& val) noexcept(NO_EXCEPT) {
        return this->_impl.contains(std::forward<U>(val));
    }

    // 位数
    template<class Counter = std::size_t, class U>
        requires
            requires(T element, U val) {
                { element.contains(val) } -> std::convertible_to<Counter>;
            }
    inline auto order(U&& val) const noexcept(NO_EXCEPT) {
        Counter counter = 0;
        for(const auto& element : this->_impl) {
            counter += static_cast<Counter>(element.contains(std::forward<U>(val)));
        }
        return counter;
    }

    //　集合演算
    template<class Rhs>
    inline set& operator|=(Rhs&& rhs) noexcept(NO_EXCEPT) {
        set res;
        std::ranges::set_union(*this, std::forward<Rhs>(rhs), std::inserter(res, res.end()));
        this->swap(res);
        return *this;
    }

    template<class Rhs>
    inline set& operator&=(Rhs&& rhs) noexcept(NO_EXCEPT) {
        set res;
        std::ranges::set_intersection(*this, std::forward<Rhs>(rhs), std::inserter(res, res.end()));
        this->swap(res);
        return *this;
    }

    template<class Rhs>
    inline set& operator^=(Rhs&& rhs) noexcept(NO_EXCEPT) {
        set res;
        std::ranges::set_symmetric_difference(*this, std::forward<Rhs>(rhs), std::inserter(res, res.end()));
        this->swap(res);
        return *this;
    }


    template<class... Args>
    inline set operator|(set<Args...> rhs) noexcept(NO_EXCEPT) { return rhs |= *this; }

    template<class... Args>
    inline set operator&(set<Args...> rhs) noexcept(NO_EXCEPT) { return rhs &= *this; }

    template<class... Args>
    inline set operator^(set<Args...> rhs) noexcept(NO_EXCEPT) { return rhs ^= *this; }

    // 部分集合判定 (半順序)
    template<class... Args>
    inline auto operator<=>(const set<Args...>& rhs) const noexcept(NO_EXCEPT) {
        const bool leq = this->size() <= rhs.size() && std::ranges::includes(rhs, *this);
        const bool geq = rhs.size() <= this->size() && std::ranges::includes(*this, rhs);

        if(leq) {
            if(geq) return std::partial_ordering::equivalent;
            return std::partial_ordering::less;
        }

        if(geq) return std::partial_ordering::greater;

        return std::partial_ordering::unordered;
    }

    // Iterators
    inline auto begin() noexcept { return this->_impl.begin(); }
    inline auto end() noexcept { return this->_impl.end(); }

    inline auto begin() const noexcept { return this->_impl.begin(); }
    inline auto end() const noexcept { return this->_impl.end(); }
};


} // namespace primitive


namespace model {


template<class T>
struct rational_number {
  private:
    T _numerator, _denominator;

    auto _normalize() {
        if (this->_denominator < 0) {
            this->_numerator = -_numerator;
            this->_denominator = -_denominator;
        }

        auto gcd = std::gcd(this->_numerator, this->_denominator);

        this->_numerator /= gcd;
        this->_denominator /= gcd;

        return *this;
    }

  public:
    rational_number(T numerator = 0, T denominator = 1) : _numerator(numerator), _denominator(denominator) {
        assert(denominator != 0);
        this->_normalize();
    }


    inline auto numerator() const { return this->_numerator; }
    inline auto denominator() const { return this->_denominator; }

    inline auto& numerator() { return this->_numerator; }
    inline auto& denominator() { return this->_denominator; }


    auto operator+=(const rational_number& other) {
        this->_numerator = this->_numerator * other._denominator + other._numerator * this->_denominator;
        this->_denominator *= other._denominator;
        return this->_normalize();
    }

    auto operator-=(const rational_number& other) {
        this->_numerator = this->_numerator * other._denominator - other._numerator * this->_denominator;
        this->_denominator *= other._denominator;
        return this->_normalize();
    }

    auto operator*=(const rational_number& other) {
        this->_numerator *= other._numerator;
        this->_denominator *= other._denominator;
        return this->_normalize();
    }

    auto operator/=(const rational_number& other) {
        if (other._numerator == 0) throw std::invalid_argument("Cannot divide by zero.");
        this->_numerator *= other._denominator;
        this->_denominator *= other._numerator;
        return this->_normalize();
    }


    friend inline rational_number operator+(rational_number lhs, const rational_number& rhs) { return lhs += rhs; }
    friend inline rational_number operator-(rational_number lhs, const rational_number& rhs) { return lhs -= rhs; }
    friend inline rational_number operator*(rational_number lhs, const rational_number& rhs) { return lhs *= rhs; }
    friend inline rational_number operator/(rational_number lhs, const rational_number& rhs) { return lhs /= rhs; }

    friend inline rational_number operator+(const rational_number& lhs) {
        return rational_number(lhs._numerator, lhs._denominator);
    }

    friend inline rational_number operator-(const rational_number& rhs) {
        return rational_number(-rhs._numerator, rhs._denominator);
    }


    friend inline auto operator<=>(const rational_number& lhs, const rational_number& rhs) {
        return (lhs._numerator * rhs._denominator) <=> (rhs._numerator * lhs._denominator);
    }


    template<class U>
    auto to() const { return static_cast<U>(this->_numerator) / static_cast<U>(this->_denominator); }

    auto to_string() const {
        if( this->_denominator == 1) return std::format("{}", this->_numerator);
        return std::format("{}/{}", this->_numerator, this->_denominator);
    }
};


} // namespace model


template<class T>
struct std::formatter<model::rational_number<T>> : std::formatter<const char*> {
    auto format(const model::rational_number<T>& s, auto& ctx) const {
        return std::ranges::copy(s.to_string(), ctx.out()).out;
    }
};



namespace functional {


auto to_double = [](auto&& value) { return value * 2; };
auto add_one = [](auto&& value) { return value + 1; };


} // namespace functional


namespace ranges {


template <typename F>
struct closure_type : public std::ranges::range_adaptor_closure<closure_type<F>> {
  private:
    F f;

  public:
    constexpr closure_type(F f) : f(f) { }

    template <std::ranges::viewable_range R>
        requires std::invocable<F const&, R>
    constexpr auto operator()(R&& r) const {
        return f(std::forward<R>(r));
    }
};


template <typename F>
struct adaptor_type {
  private:
    F f;

  public:
    constexpr adaptor_type(F f) : f(f) { }

    template <typename... Args>
    constexpr auto operator()(Args&&... args) const {
        if constexpr (std::invocable<F const&, Args...>) {
            return f(std::forward<Args>(args)...);
        }
        else {
            return closure_type(std::bind_back(f, std::forward<Args>(args)...));
        }
    }
};


} // namespace ranges


namespace generator {


template<class T>
inline constexpr ranges::adaptor_type natural_numbers
    = [](const T& start) { return std::views::iota(start); };

template<class T>
inline constexpr ranges::adaptor_type integers
    = []() -> std::generator<T> {
        co_yield 0;
        for(auto n : natural_numbers<T>(1)) {
            co_yield n;
            co_yield -n;
        }
    };

template<class T>
inline constexpr ranges::adaptor_type even_numbers
    = []() { return integers<T>() | std::views::transform(functional::to_double); };

template<class T>
inline constexpr ranges::adaptor_type odd_numbers
    = []() { return even_numbers<T>() | std::views::transform(functional::add_one); };

template<class T>
inline constexpr ranges::adaptor_type rational_numbers
    = []() -> std::generator<model::rational_number<T>> {
        co_yield 0;

        for(auto n : generator::natural_numbers<T>(1)) {
            for(auto i : generator::natural_numbers<T>(1) | std::views::take(n - 1)) {
                if(std::gcd(i, n - i) != 1) continue;
                co_yield model::rational_number(i, n - i);
                co_yield -model::rational_number(i, n - i);
            }
        }
    };


} // namespace generator


signed main() {
    primitive::set<int> st = {1, 2, 3, 4, 5};
    std::println("{}", st);

    std::println("{}", generator::natural_numbers<int>(0) | std::views::take(10));
    std::println("{}", generator::integers<int>() | std::views::take(10));
    std::println("{}", generator::even_numbers<int>() | std::views::take(10));
    std::println("{}", generator::odd_numbers<int>() | std::views::take(10));
    std::println("{}", generator::rational_numbers<int>() | std::views::take(10));
}
