#include "HDVParticle.hpp"
#include <random>

HDV::Particle::Particle(unsigned int dimensions, HDV::RandomGenerator& rand_gen)
{
  Randomise(dimensions, rand_gen);
}

void HDV::Particle::Randomise(unsigned int dimensions, HDV::RandomGenerator& rand_gen)
{
  RandomisePosition(dimensions, rand_gen);
  RandomiseDirection(dimensions, rand_gen);
}

void HDV::Particle::ResetNextDirection()
{
  m_newDirection = m_direction;
}

void HDV::Particle::AddNextDirection(HDV::Vec const& dir)
{
  m_newDirection += dir;
}

void HDV::Particle::SwapDirections()
{
  using std::swap;
  swap(m_direction, m_newDirection);
  m_direction.normalize();
}

void HDV::Particle::Step(HDV::Float speed)
{
  m_position += speed * m_direction;
}

HDV::Vec const& HDV::Particle::GetCurrentPosition() const
{
  return m_position;
}

HDV::Vec const& HDV::Particle::GetCurrentDirection() const
{
  return m_direction;
}

void HDV::Particle::RandomisePosition(unsigned int dimensions, HDV::RandomGenerator& rand_gen)
{
  std::uniform_real_distribution<HDV::Vec::value_type> dist(0,1);
  m_position = Vec::NullaryExpr(dimensions, [&]{return dist(rand_gen);});
}

void HDV::Particle::RandomiseDirection(unsigned int dimensions, HDV::RandomGenerator& rand_gen)
{
  // Generate random directions using Marsaglia (1972)
  HDV::NormalDistribution dist(0,1);
  m_direction = Vec::NullaryExpr(dimensions, [&]{return dist(rand_gen);});
  m_direction.normalize();
}

void HDV::Particle::Wrap()
{
  for (auto& pos : m_position)
  {
    pos = fmod(pos+floor(pos),static_cast<HDV::Float>(1.0)); 
  }
}

void HDV::Particle::Wobble(HDV::Float maxAngle)
{

}

HDV::Float HDV::Distance2(HDV::Particle const& lhs, HDV::Particle const& rhs)
{
  // First get the positive differences in position on each axis
  // If (per axis) the difference 'd' is > 0.5, set equal to 1-d (to account for wrapped boundary)
  // Return dot product with self
  HDV::Vec diff = Eigen::abs((lhs.GetCurrentPosition() - rhs.GetCurrentPosition()).array()).matrix();
  HDV::Vec halfs;
  halfs.resize(lhs.GetCurrentPosition().size());;
  halfs.setConstant(0.5f);
  diff -= halfs;
  diff = Eigen::abs(diff.array()).matrix();
  diff = halfs - diff;
  return diff.dot(diff);
}

