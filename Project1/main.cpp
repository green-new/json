#include <iostream>
#include "json2.hpp"

int main() {
    std::cout << "Hello World";

    using str = std::string;
    using node = A<str>;
    using object = B<str>;
    using value = C<str, int>;

    object obj{};
    obj["foo"] = object{};

    return 0;
}