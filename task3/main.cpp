#include "spy.hpp"
#include "managed_ops.hpp"
#include "commons_assert.hpp"

#include <vector>
#include <iostream>
#include <memory>

namespace mpg::detail {

template <size_t pad_size>
struct Pad {
  char pad[pad_size];
};

template <>
struct Pad<0> {};

template <mpg::Enable options, size_t pad_size>
class Logger : public mpg::Operations<options>, private Pad<pad_size> {
 private:
  std::vector<unsigned int>* got_;

 public:
  explicit Logger(std::vector<unsigned int>* got)
      : got_(got) {}

  void operator()(unsigned int num) {
      got_->push_back(num);
  }
};

template <mpg::Enable options, size_t pad_size>
class LoggerChecker {
 public:
  Logger<options, pad_size> getLogger() {
      return Logger<options, pad_size>{&got_};
  }

  std::vector<unsigned int> pollValues() {
      auto result = got_;
      got_.clear();
      return result;
  }

 private:
  std::vector<unsigned int> got_;
};

template <mpg::Enable options>
struct Counter : public mpg::Operations<options> {
  int x = 0;
  bool isPositive() const {
      return x > 0;
  }

  Counter(int x) :x(x) {}
  Counter() {}
};

}

constexpr auto semiregular_opt = mpg::Enable{};

template <size_t pad_size = 0>
void testCopyable() {
    using mpg::detail::LoggerChecker;
    using mpg::detail::Counter;


    auto s = Spy<Counter<semiregular_opt>>{};
    ensure(eq, s->isPositive(), false); // works without logger

    static_assert(std::copyable<Counter<semiregular_opt>>);
    static_assert(std::movable<Counter<semiregular_opt>>);

    s = Spy{ Counter<semiregular_opt>(1) };
    ensure(eq, s->x, 1); // construction and copy construction work

    LoggerChecker<semiregular_opt, pad_size> checker;
    s.setLogger(checker.getLogger());
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{});

    s->x = 0;
    s->isPositive() && s->x--;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{1, 1});

    s->x++ + s->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{2});

    auto s2 = s; // copy construction
    // logger must be copied too

    s2->isPositive() && s2->x--;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{2});

    s->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{1});

    // what about move assignment?
    s = Spy { Counter<semiregular_opt>{} }; // no logger
    s->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{});

    // which results are expected in the following expression?
    //    s->isPositive() && ((s = s2), 1);
    // ¯\_(ツ)_/¯

    // but this one is ok
    (s2->isPositive(), (s = s2)->isPositive());
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{ 1, 1 });

    LoggerChecker<semiregular_opt, pad_size> another_checker;
    s.setLogger(another_checker.getLogger()); // what about changing loggers?

    s->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{});
    ensure(eq, another_checker.pollValues(), std::vector<unsigned int>{ 1 });

    s2 = std::move(s); // what about move assignment?
    s2->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{});
    ensure(eq, another_checker.pollValues(), std::vector<unsigned int>{ 1 });

    (s2 = s2)->x++;
    ensure(eq, another_checker.pollValues(), std::vector<unsigned int>{ 1 });

    (s2 = std::move(s2))->x++;
    //ensure(eq, another_checker.pollValues(), std::vector<unsigned int>{ 1 });

    auto s3 = Spy { Counter<semiregular_opt>{} }; // no logger
    s2 = s3; // copy assignment
    s2->x++;
    //ensure(eq, another_checker.pollValues(), std::vector<unsigned int>{});
}

constexpr auto move_only_opt = mpg::Enable{ .copy_constructor = false, .copy_assignment = false };

template <size_t pad_size = 0>
void testMovable() {
    using mpg::detail::LoggerChecker;
    using mpg::detail::Counter;


    auto s = Spy<Counter<move_only_opt>>{};
    ensure(eq, s->isPositive(), false); // works without logger

    s = Spy{ Counter<move_only_opt>(1) };
    ensure(eq, s->x, 1); // construction and move construction work

    LoggerChecker<move_only_opt, pad_size> checker;
    s.setLogger(checker.getLogger());
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{});

    s->x = 0;
    s->isPositive() && s->x--;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{1, 1});

    s->x++ + s->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{2});

    auto s2 = std::move(s); // move construction
    // logger must be moved too

    s2->isPositive() && s2->x--;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{2});

    // what about move assignment?
    s2 = Spy { Counter<move_only_opt>{} }; // no logger
    s2->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{});

    s2.setLogger(checker.getLogger());
    (s2 = std::move(s2))->x++;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{ 1 });
}

template <size_t pad_size = 0>
void testLvalueLogger() {
    using mpg::detail::LoggerChecker;
    using mpg::detail::Counter;

    auto s = Spy<Counter<semiregular_opt>>{};

    LoggerChecker<semiregular_opt, pad_size> checker;
    auto logger = checker.getLogger();
    s.setLogger(logger);
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{});

    s->x = 0;
    s->isPositive() && s->x--;
    ensure(eq, checker.pollValues(), std::vector<unsigned int>{1, 1});
}

void logFunction(unsigned int) {}

void testFunctionPointer() {
    using mpg::detail::LoggerChecker;
    using mpg::detail::Counter;

    auto s = Spy<Counter<semiregular_opt>>{};
    s.setLogger(&logFunction);
    s->x++;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////                                                                      /////
/////    Test Function                                                     /////
/////                                                                      /////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

long long counter = 0;

struct CounterIncrementer {
  void operator()() {
      counter++;
  }
};

struct VectorHolder {
  VectorHolder() = delete;
  VectorHolder(std::vector<long  long>* vPtr) : longs(vPtr) {};

  void operator()() {
      longs->push_back(counter);
  }
  std::vector<long long>* longs;
};


void checkIncrementor() {
    auto inc = CounterIncrementer();
    Function<void(void)> f = Function<void(void)>(inc);
    f();
    assert(counter == 1);
};

void checkVectorHolder() {
    std::vector<long long> v;
    auto vh = VectorHolder(&v);
    Function<void(void)> f = Function<void(void)>(vh);
    f();
    assert(v.size() == 1);
    f = Function<void(void)>(std::move(vh));
    f();
    assert(v.size() == 2);
}


int main() {

    testCopyable(); // will likely trigger SBO
    testCopyable<256>(); // will likely not trigger SBO

    testMovable(); // will likely trigger SBO
    testMovable<256>(); // will likely not trigger SBO

    testLvalueLogger(); // will likely trigger SBO
    testLvalueLogger<256>(); // will likely not trigger SBO
    
    testFunctionPointer();
    
    checkIncrementor();
    checkVectorHolder();

    return 0;
}