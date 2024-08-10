#include "counted_object.hpp"
#include <vector>

int main() {
    counted_object::stats::use_loud_value(true);
    counted_object::stats::print("No object yet!");
    auto test1 = counted_object::counted_object();
    counted_object::stats::print("Plain creation!");
    auto test2 = counted_object::counted_object();
    counted_object::stats::print("Another plain creation!");
    auto test3 = test1;
    counted_object::stats::print("Hopefully copy construction!");
    test2 = test3;
    counted_object::stats::print("Hopefully copy assignment!");
    auto test4 = std::move(test3);
    counted_object::stats::print("Hopefully move construction!");
    test2 = std::move(test4);
    counted_object::stats::print("Hopefully move assignment!");
    
    counted_object::stats::reset();
    counted_object::stats::use_loud_value(false);

    auto v = std::vector<counted_object::counted_object>(10);
    for (auto i = 0; i < 1000; ++i) {
        v.push_back(counted_object::counted_object{});
    }
    counted_object::stats::print("Added 1000 counted objects on top of 10 previous ones!");

    counted_object::stats::reset();
    v = std::vector<counted_object::counted_object>();
    for (auto i = 0; i < 100; ++i) {
        auto temp = counted_object::counted_object();
        v.push_back(std::move(temp));
    }

    counted_object::stats::print("Constructed 1 object and push_back called 100 with move on it!");

    std::println(std::cout, "DONE!");
}