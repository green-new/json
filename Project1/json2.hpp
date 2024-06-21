#pragma once

#include <iostream>
#include <map>
#include <memory>

// node class
template<typename T>
class A {
public:
    A() = default;
    virtual ~A() = default;
    virtual A<T>& operator[](T name) { return *this; }
    auto clone() {
        return A<T>(clone_impl());
    }
protected:
    virtual A<T>* clone_impl() const = 0;
public:
    T name{};
};

// value class (for an additional node-like class)
template<typename T, typename K>
class C : public A<T> {
    using node_ptr = std::unique_ptr<A<T>>;
public:
    C() : A<T>() {}
    C(C const& other) : value(other.value) { }
    C(C&& other) = default;
    C<T, K>& operator=(const C<T, K>& other) {
        value = other.value;
        return *this;
    }
    C<T, K>& operator=(C<T, K>&& other) = default;
    ~C() {}
public:
    C<T, K> operator=(K& newValue) {
        value = newValue;
    }
protected:
    virtual C<T, K>* clone_impl() const override {
        return new C<T, K>(*this);
    }
public:
    K value{};
};

// object class
template<typename T>
class B : public A<T> {
public:
    B() : A<T>() {}
    B(B<T> const& other) {
        props.clear();
        for (const auto& [key, value] : other.props) {
            if (value) {
                props.insert({ key, value->clone() });
            } else {
                props.insert({ key, node_ptr()});
            }
        }
    }
    B(B<T>&& other) = default;
    B<T>& operator=(B<T> const& other) {
        *this = B<T>(other);
        return *this;
    }
    B<T>& operator=(B<T>&& other) = default;
    ~B() {}

    // this must return nonconst node reference, as it could be a value or array or object.
    std::unique_ptr<A<T>>& operator[](T name) override {
        if (props.find(name) == props.end()) {
            props.insert({ name, node_ptr()});
        }
        return props.at(name);
    }

protected:
    virtual B<T>* clone_impl() const override {
        return new B<T>(*this);
    }
public:
    std::map<T, std::unique_ptr<A<T>>> props{};
};