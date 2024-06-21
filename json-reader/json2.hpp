#pragma once

#include <iostream>
#include <map>
#include <memory>

// node class
template<typename T>
struct A {
    A() = default;
    virtual ~A() = default;

    virtual A<T>& operator[](T name) = 0;
    virtual A<T>& operator=(const A<T>& other) = 0;
    virtual A<T>* clone() = 0;

    T name{};
};

// value class (for an additional node-like class)
template<typename T, typename K>
struct C : public A<T> {
    C() : A<T>() {}
    ~C() {}

    A<T>& operator[](T name) override {
        return *this;
    }
    A<T>& operator=(const A<T>& other) {
        value = ((const C<T, K>&)other).value;
        return *this;
    }

    A<T>* clone() {
        return new C<T, K>(*this);
    }

    K value;
};

// object class
template<typename T>
struct B : public A<T> {
    using node_ptr = std::unique_ptr<A<T>>;
    B() : A<T>() {}
    ~B() {}

    // this must return nonconst node reference, as it could be a value or array or object.
    A<T>& operator[](T name) override {
        return *props[name];
    }

    A<T>& operator=(const A<T>& other) override {
        // make sure other is of type object.
        props.clear();
        for (const auto& [key, value] : ((B<T>&)other).props) {
            props.insert({ key, std::make_unique<A<T>>(value->clone()) });
        }
        return *this;
    }

    A<T>* clone() {
        return new B<T>(*this);
    }

    std::map<T, node_ptr> props{};
};