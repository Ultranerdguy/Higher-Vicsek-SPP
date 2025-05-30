#ifndef _HDV_TESTS_UTIL_HPP_
#define _HDV_TESTS_UTIL_HPP_

constexpr bool Equal(double l, double r, double d) noexcept {
  return (l-d) < r && r < (l+d);
}

#endif // #ifndef _HDV_TESTS_UTIL_HPP_
