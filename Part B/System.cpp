#include "System.h"
#include <stdexcept>

PredatorPreySystem::PredatorPreySystem(double a, double b, double g, double d) : alpha(a), beta(b), gamma(g), delta(d) {}

void PredatorPreySystem::computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) {
  double xPop = y[0]; // Prey population
  double yPop = y[1]; // Predator population
  
  dydt[0] = alpha * xPop - beta * xPop * yPop; // dx/dt
  dydt[1] = delta * xPop * yPop - gamma * yPop; // dy/dt
}

int PredatorPreySystem::getDimension() const { 
  return 2; 
}

int PredatorPreySystem::getNumParams() const { 
  return 4; 
}

std::string PredatorPreySystem::getName() const { 
  return "Predator-Prey"; 
}

void PredatorPreySystem::setParams(const std::vector<double>& params) {
  if (params.size() != 4) {
    throw std::invalid_argument("Predator Prey expects 4 parameters");
  }

  alpha = params[0];
  beta = params[1];
  gamma = params[2]; 
  delta = params[3];
}

SIREpidemicSystem::SIREpidemicSystem(double b, double g, double m) : beta(b), gamma(g), mu(m) {}

void SIREpidemicSystem::computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) {
  double S = y[0]; // Susceptible population
  double I = y[1]; // Infected population  
  double R = y[2]; // Recovered population
  double N = S + I + R; // Total population

  // SIR equations with vital dynamics
  dydt[0] = mu * N - beta * S * I - mu * S;  // dS/dt
  dydt[1] = beta * S * I - gamma * I - mu * I; // dI/dt  
  dydt[2] = gamma * I - mu * R; // dR/dt
}

int SIREpidemicSystem::getDimension() const { 
  return 3; // S, I, R - three equations
}

int SIREpidemicSystem::getNumParams() const { 
  return 3; 
}

std::string SIREpidemicSystem::getName() const { 
  return "SIR Epidemic"; 
}

void SIREpidemicSystem::setParams(const std::vector<double>& params) {
  if (params.size() != 3) {
    throw std::invalid_argument("SIR expects 3 parameters");
  }

  beta = params[0];
  gamma = params[1];
  mu = params[2];
}

VanDerPolOscillator::VanDerPolOscillator(double m) : mu(m) {}

void VanDerPolOscillator::computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) {
  double x = y[0]; // Position
  double yVel = y[1]; // Velocity
  
  dydt[0] = yVel; // dx/dt
  dydt[1] = mu * (1 - x * x) * yVel - x; // dy/dt
}

int VanDerPolOscillator::getDimension() const { 
  return 2; // x, y - 2 equations
}

int VanDerPolOscillator::getNumParams() const { 
  return 1; 
}

std::string VanDerPolOscillator::getName() const { 
  return "Van der Pol"; 
}

void VanDerPolOscillator::setParams(const std::vector<double>& params) {
  if (params.size() != 1) {
    throw std::invalid_argument("Van der Pol expects 1 parameter");
  }
  
  mu = params[0];
}

LorenzAttractorSystem::LorenzAttractorSystem(double s, double r, double b) : sigma(s), rho(r), beta(b) {}

void LorenzAttractorSystem::computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) {
  double x = y[0]; // Intensity of convection
  double yVal = y[1]; // Temp difference
  double z = y[2]; // Distortion of vertical temp profile

  dydt[0] = sigma * (yVal - x); // dx/dt
  dydt[1] = x * (rho - z) - yVal; // dy/dt
  dydt[2] = x * yVal - beta * z; // dz/dt
}

int LorenzAttractorSystem::getDimension() const {
  return 3;
}

int LorenzAttractorSystem::getNumParams() const { 
  return 3; 

}
std::string LorenzAttractorSystem::getName() const { 
  return "Lorenz Attractor"; 
}

void LorenzAttractorSystem::setParams(const std::vector<double>& params) {
  if (params.size() != 3) {
    throw std::invalid_argument("Lorenz expects 3 parameters");
  }
  
  sigma = params[0];
  rho = params[1];
  beta = params[2];
}

TwoCompartmentDiffusionSystem::TwoCompartmentDiffusionSystem(double k) : k(k) {}

void TwoCompartmentDiffusionSystem::computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) {
  dydt[0] = -k * (y[0] - y[1]); // dC1/dt
  dydt[1] = k * (y[0] - y[1]); // dC2/dt
}

int TwoCompartmentDiffusionSystem::getDimension() const { 
  return 2; // C1, C2 - 2 equations
}

int TwoCompartmentDiffusionSystem::getNumParams() const { 
  return 1; 
}

std::string TwoCompartmentDiffusionSystem::getName() const { 
  return "Two-Compartment Diffusion"; 
}

void TwoCompartmentDiffusionSystem::setParams(const std::vector<double>& params) {
  if (params.size() != 1) {
    throw std::invalid_argument("Diffusion expects 1 param");
  }
  
  k = params[0];
}