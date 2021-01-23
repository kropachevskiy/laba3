// Copyright 2020 kropachevskiy <kropachev-95@mail.ru>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_
#include <atomic>
#include <string>
#include <utility>

template <typename T>
class SharedPtr {
 private:
  T* Pointer;

  std::atomic_uint* Counter;

 public:
  SharedPtr() : Pointer{nullptr}, Counter{nullptr} {};

  explicit SharedPtr(T* ptr)
      : Pointer{ptr}, Counter{new std::atomic_uint{1}} {};

  SharedPtr(const SharedPtr& r) : Pointer{r.Pointer}, Counter{r.Counter} {
    ++(*Counter);
  };

  SharedPtr(SharedPtr&& r)
      : Pointer{std::move(r.Pointer)}, Counter{std::move(r.Counter)} {
    ++(*Counter);
  };

  ~SharedPtr() {
    if (*Counter == 1) {
      delete Counter;
      Counter = nullptr;
      Pointer = nullptr;
    } else {
      --(*Counter);
    }
  };

  auto operator=(const SharedPtr& r) -> SharedPtr& {
    this->reset();
    this->Pointer = r.Pointer;
    this->Counter = r.Counter;
    if (Counter != nullptr) {
      ++(*Counter);
    }
    return *this;
  };

  auto operator=(SharedPtr&& r) -> SharedPtr& {
    this->reset();
    this->Pointer = r.Pointer;
    this->Counter = r.Counter;
    if (Counter != nullptr) {
      ++(*Counter);
    }
    return *this;
  };

  auto operator*() const -> T& { return *Pointer; };

  auto operator->() const -> T* { return Pointer; };

  void reset() {
    if (Counter != nullptr) {
      if (*Counter != 1) {
        --(*Counter);
      } else {
        delete Pointer;
        delete Counter;
      }
      Pointer = nullptr;
      Counter = nullptr;
    }
  };

  void swap(SharedPtr& r) {
    SharedPtr<T> temp{r};
    r = *this;
    *this = temp;
  };

  auto use_count() const -> std::size_t { return *Counter; };
};
#endif  // INCLUDE_SHAREDPTR_HPP_