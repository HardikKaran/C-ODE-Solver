#include <iostream>
#include "Solver.h" 
#include <fstream>
#include <iomanip>
#include <stdexcept>
// #include <cmath>

Solver::Solver() : T(0), dt(0) {}

void Solver::createSystem(int odeID) {
  // ODE systems the solver can handle (from Appendix)
  switch (odeID) {
    case 1: system = std::make_unique<PredatorPreySystem>(); break;
    case 2: system = std::make_unique<SIREpidemicSystem>(); break;
    case 3: system = std::make_unique<VanDerPolOscillator>(); break;
    case 4: system = std::make_unique<LorenzAttractorSystem>(); break;
    case 5: system = std::make_unique<TwoCompartmentDiffusionSystem>(); break; 
    default: throw std::invalid_argument("Error: Unknown ODE."); // Outside of the 5 ODEs to pick from
  }
}

void Solver::createIntegrator(int schemeID) {
  // integration schemes solver can handle
  switch (schemeID) {
    case 1: integrator = std::make_unique<ForwardEuler>(); break;
    case 2: integrator = std::make_unique<RungeKutta4>(); break;
    default: throw std::invalid_argument("Error: Unknown Integrator."); // Outside of the 2 integrators 
  }
}

void Solver::initialise(const std::string& inputParamsFile) {
  std::ifstream paramFile(inputParamsFile); // Creates input stream for data to be inputted from file
  
  // Check if file exists/opens
  if (!paramFile.is_open()) {
    throw std::runtime_error("Error: file not found.");
  }

  int odeID, schemeID;
  
  // Check input of line 1: [ODE] [TimeScheme] [T] [dt]
  if (!(paramFile >> odeID >> schemeID >> T >> dt)) {
    throw std::runtime_error("Error: Failed to read Line 1.");
  }

  // Validate time steps 
  if (dt <= 0 || T <= 0) {
    throw std::range_error("Error: T > 0 or dt > 0.");
  }
  if (dt > T) {
    std::cerr << "Error: dt > T" << std::endl;
  }

  createSystem(odeID);
  createIntegrator(schemeID);

  // Get line 2 of input file (parameters)
  int numParams = system->getNumParams();
  std::vector<double> p(numParams);

  // Check if sufficient parameters exist for the problem at hand
  for (int i = 0; i < numParams; ++i) {
    if (!(paramFile >> p[i])) {
      throw std::runtime_error("Error: Insufficient parameters.");
    }
  }

  system->setParams(p);

  // Get line 3 of input file (initial conditions)
  int numVars = system->getDimension();
  initialConditions.resize(numVars);
  for (int i = 0; i < numVars; ++i) {
    if (!(paramFile >> initialConditions[i])) {
      throw std::runtime_error("Error: Insufficient initial conditions.");
    }
  }

  paramFile.close(); 
}

void Solver::run(const std::string& outputFile) {
  std::ofstream resultFile(outputFile);
  
  double t = 0.0;
  std::vector<double> yCurr = initialConditions;

  // Write initial state (t=0)
  resultFile << std::fixed << std::setprecision(6);
  resultFile << t;
  for (double val : yCurr) {
    resultFile << " " << val;
    // std::cout << "t = " << t << std::endl;
  }
  
  resultFile << "\n";

  // Loop untik the end of T
  while (t < T) {
    integrator->step(*system, yCurr, t, dt);

    // Write step output to output file
    resultFile << t;
    for (double val : yCurr) resultFile << " " << val;
    resultFile << "\n";
  }
}