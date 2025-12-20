#include "Integrator.h"
#include <vector>

void ForwardEuler::step(System& system, std::vector<double>& y, double& t, double dt) {
  int dim = system.getDimension();
  std::vector<double> dydt(dim); // Vector of size dim to store derivatives
  
  system.computeDerivatives(y, t, dydt);
  
  // Compute y_next using Forward Euler
  for (int i = 0; i < dim; i++) {
    y[i] += dt * dydt[i];
  }
  
  t += dt; // Increment by step size
}

void RungeKutta4::step(System& system, std::vector<double>& y, double& t, double dt) {
  int dim = system.getDimension();

  std::vector<double> k1(dim), k2(dim), k3(dim), k4(dim);
  std::vector<double> y_tmp(dim); 
  
  // k1
  system.computeDerivatives(y, t, k1);
  
  // k2
  for (int i = 0; i < dim; i++) y_tmp[i] = y[i] + 0.5 * dt * k1[i]; 
  system.computeDerivatives(y_tmp, t + 0.5 * dt, k2);
  
  // k3
  for (int i = 0; i < dim; i++) y_tmp[i] = y[i] + 0.5 * dt * k2[i];
  system.computeDerivatives(y_tmp, t + 0.5 * dt, k3);
  
  // k4
  for (int i = 0; i < dim; i++) y_tmp[i] = y[i] + dt * k3[i];
  system.computeDerivatives(y_tmp, t + dt, k4);
  
  // weighted sum
  for (int i = 0; i < dim; i++) {
    y[i] += (dt / 6.0) * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
  }
  
  t += dt; 
}