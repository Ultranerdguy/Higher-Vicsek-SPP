#ifndef _HDV_PARTICLE_HPP_
#define _HDV_PARTICLE_HPP_

#include "HDVDef.hpp"

namespace HDV
{
  class Particle
  {
  public:
    constexpr Particle() = default;
    Particle(unsigned int dimensions, HDV::RandomGenerator& rand_gen);

    void Randomise(unsigned int dimensions, HDV::RandomGenerator& rand_gen);

    void ResetNextDirection();
    void AddNextDirection(HDV::Vec const& dir);
    void SwapDirections();
    void Step(HDV::Float speed);
    void RandomiseDirection(unsigned int dimensions, HDV::RandomGenerator& rand_gen);
    void Wrap();
    void Wobble(HDV::Float maxAngle);
    HDV::Vec const& GetCurrentPosition() const;
    HDV::Vec const& GetCurrentDirection() const;

  private:
    void RandomisePosition(unsigned int dimensions, HDV::RandomGenerator& rand_gen);


    HDV::Vec m_position{};
    HDV::Vec m_direction{};
    HDV::Vec m_newDirection{}; // Direction to be set to at the end of this simulation tick
  };

  // Return the squared distance between particles
  HDV::Float Distance2(Particle const& lhs, Particle const& rhs);
}

#endif // #ifndef _HDV_PARTICLE_HPP_

