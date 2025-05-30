#ifndef _HDV_DEF_HPP_
#define _HDV_DEF_HPP_

#include <random>
#include <Eigen/Dense>

namespace HDV
{
  // Base floating point type used in calculations
  using Float = float;
  
  // Column vectors, taken from Eigen
  using Vec = Eigen::Vector<HDV::Float, Eigen::Dynamic>;
  
  // Random generator and distribution used anytime randomness is needed
  using RandomGenerator = std::mt19937_64;
  using NormalDistribution = std::normal_distribution<HDV::Float>;
}

#endif // #ifndef _HDV_DEF_HPP_

