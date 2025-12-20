#ifndef SOLVER_H
#define SOLVER_H

#include "System.h"
#include "Integrator.h"
#include <vector>
#include <string>
#include <memory>

class Solver {
private:
  // Pointers to active ODE system and integration scheme
  std::unique_ptr<System> system;
  std::unique_ptr<Integrator> integrator;

  double T, dt;
  std::vector<double> params;
  std::vector<double> initialConditions;
  
  // Internal helper to create objects based on IDs
  void createSystem(int odeID);
  void createIntegrator(int schemeID);

public:
  Solver();
  ~Solver() = default;
  
  // validates input from parameters.txt
  void initialise(const std::string& inputParamsFile);

  // Writes to output.txt
  void run(const std::string& outputFile);
};

#endif