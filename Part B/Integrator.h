#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "System.h"
#include <vector>

class Integrator {
public:
  virtual ~Integrator() = default;
  virtual void step(System& system, std::vector<double>& y, double& t, double dt) = 0; // Mandatory definition for sub-classes to contain numerical method step
};

// Define explicit Euler method
class ForwardEuler : public Integrator {
public:
  void step(System& system, std::vector<double>& y, double& t, double dt) override;
};

// Define RK4 method scheme
class RungeKutta4 : public Integrator {
public:
  void step(System& system, std::vector<double>& y, double& t, double dt) override;
};

#endif