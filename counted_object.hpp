#ifndef COUNTED_OBJECT_HPP
#define COUNTED_OBJECT_HPP

#include <cstddef>
#include <string>
#include <iostream>
#include <concepts>


namespace counted_object {

    template<typename T>
    struct loud_value {
        bool is_supressed{ true };
        T value;
        loud_value() : value(T{}) {
            if (!is_supressed) { std::cout << "Loud value default constructed with " << std::to_string(value) << std::endl; }
        }
        loud_value(const T& value) : value(value) {
            if (!is_supressed) { std::cout << "Loud value 'l-value' initialized with " << std::to_string(value) << std::endl; }
        }
        loud_value(T&& value) : value(std::move(value)) {
            if (!is_supressed) { std::cout << "Loud value 'r-value' initialized with " << std::to_string(value) << std::endl; }
        }
        loud_value& operator++() {
            if (!is_supressed) {
                std::cout << "Loud value pre-incremented" << std::endl;
            }
            ++value;
            return *this;
        }
        loud_value operator++(int) {
            if (!is_supressed) {
                std::cout << "Loud value post-incremented" << std::endl;
            }
            loud_value temp{ value };
            value++;
            return temp;
        }
        loud_value& operator--() {
            if (!is_supressed) {
                std::cout << "Loud value pre-decremented" << std::endl;
            }
            --value;
            return *this;
        }
        loud_value operator--(int) {
            if (!is_supressed) {
                std::cout << "Loud value post-decremented" << std::endl;
            }
            loud_value temp{ value };
            value--;
            return temp;
        }
        [[nodiscard]] std::string to_string() const noexcept {
            return std::to_string(value);
        }
        template<typename Str> requires std::is_convertible_v<Str, std::string>
        friend auto operator+(Str&& str, const loud_value& val) {
            return std::string(std::forward<Str>(str)).append(val.to_string());
        }
        template<typename Str> requires std::is_convertible_v<Str, std::string>
        friend auto operator+(const loud_value& val, Str&& str) {
            return std::string(std::forward<Str>(str)).append(val.to_string());
        }
    };

    struct stats {
        static inline loud_value<std::size_t> def_cons;
        static inline loud_value<std::size_t> copy_cons;
        static inline loud_value<std::size_t> copy_assg;
        static inline loud_value<std::size_t> move_cons;
        static inline loud_value<std::size_t> move_assg;

        static [[nodiscard]] std::string to_string() noexcept {
            std::string result;
            result += "Default construction : " + def_cons + '\n';
            result += "Copy construction    : " + copy_cons + '\n';
            result += "Copy assignment      : " + copy_assg + '\n';
            result += "Move construction    : " + move_cons + '\n';
            result += "Move assignment      : " + move_assg + '\n';
            return result;
        }

        static void reset() noexcept {
            def_cons = 0;
            copy_cons = 0;
            move_cons = 0;
            copy_assg = 0;
            move_assg = 0;
        }
        static void print(const std::string& message = "") {
            std::cout << std::endl;
            std::cout << message << std::endl;
            std::cout << to_string();
        }

        static void use_loud_value(bool cond) {
            if (cond == false) {
                def_cons.is_supressed = true;
                copy_cons.is_supressed = true;
                move_cons.is_supressed = true;
                copy_assg.is_supressed = true;
                move_assg.is_supressed = true;
            }
            else {
                def_cons.is_supressed =  false;
                copy_cons.is_supressed = false;
                move_cons.is_supressed = false;
                copy_assg.is_supressed = false;
                move_assg.is_supressed = false;
            }
        }
    };

    struct counted_object {
        counted_object() { stats::def_cons++; }
        counted_object(const counted_object& other) { stats::copy_cons++; }
        counted_object(counted_object&& other) noexcept { stats::move_cons++; }
        counted_object& operator=(const counted_object& other) { stats::copy_assg++; return *this; }
        counted_object& operator=(counted_object&& other) noexcept { stats::move_assg++; return *this; }
    };

} // namespace counted_object

// Macro for creating definitions of static members
#define CREATE_DEFINITIONS_FOR_TRACKERS \
counted_object::stats::def_cons = 0; \
counted_object::stats::copy_cons = 0; \
counted_object::stats::copy_assg = 0; \
counted_object::stats::move_cons = 0; \
counted_object::stats::move_assg = 0;

#endif // COUNTED_OBJECT_HPP
