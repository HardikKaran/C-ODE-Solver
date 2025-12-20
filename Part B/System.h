#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <string>

class System {
public:
  virtual ~System() = default; // Base class destructor

  virtual void computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) = 0; // Define system's ODEs 
  virtual int getDimension() const = 0; // Number of equations

  virtual void setParams(const std::vector<double>& params) = 0; // Sets system parameters
  virtual int getNumParams() const = 0; // returns system parameters
  virtual std::string getName() const = 0; // Returns name to identify system
};

// non-linear system picked for comparison in report
class PredatorPreySystem : public System {
private:
  double alpha, beta, gamma, delta;
  
public:
  PredatorPreySystem() {}
  PredatorPreySystem(double a, double b, double g, double d);
  void computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) override;
  int getDimension() const override;

  void setParams(const std::vector<double>& params) override;
  int getNumParams() const override;
  std::string getName() const override;
};

class SIREpidemicSystem : public System {
private:
  double beta, gamma, mu;

public:
  SIREpidemicSystem() {}
  SIREpidemicSystem(double b, double g, double m);
  void computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) override;
  int getDimension() const override;

  void setParams(const std::vector<double>& params) override;
  int getNumParams() const override;
  std::string getName() const override;
};

class VanDerPolOscillator : public System {
private:
  double mu;

public:
  VanDerPolOscillator() {}
  VanDerPolOscillator(double m);
  void computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) override;
  int getDimension() const override;

  void setParams(const std::vector<double>& params) override;
  int getNumParams() const override;
  std::string getName() const override;  
};

class LorenzAttractorSystem : public System {
private:
  double sigma, rho, beta;

public:
  LorenzAttractorSystem() {}
  LorenzAttractorSystem(double s, double r, double b);
  void computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) override;
  int getDimension() const override;

  void setParams(const std::vector<double>& params) override;
  int getNumParams() const override;
  std::string getName() const override;
};

// linear system picked to analyse in both reports
class TwoCompartmentDiffusionSystem : public System {
private: 
  double k;

public: 
  TwoCompartmentDiffusionSystem() {}
  TwoCompartmentDiffusionSystem(double k);
  void computeDerivatives(const std::vector<double>& y, double t, std::vector<double>& dydt) override;
  int getDimension() const override;

  void setParams(const std::vector<double>& params) override;
  int getNumParams() const override;
  std::string getName() const override;
};

#endif