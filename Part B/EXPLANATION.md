# C++ ODE Solver — Code Explained Simply

A beginner-friendly tour of this project: **what the code does** (solving ODEs
with numerical methods) and **the C++ ideas it uses** (inheritance, polymorphism,
abstract classes, smart pointers, and more). Each concept comes with a short real
snippet from the code and an everyday analogy.

---

## 1. What this program does

In one sentence: **it reads a small config file, picks one of 5 physics/biology
models and one of 2 maths methods, marches the model forward through time, and
writes the results to a file.**

The whole job is driven by a plain text file called **`parameters.txt`** with
exactly **3 lines**:

```
[ODE] [Scheme] [T] [dt]      <- line 1: which model, which method, total time, step size
[param1] [param2] ...        <- line 2: the model's settings (e.g. growth rates)
[ic0] [ic1] ...              <- line 3: starting values at time t = 0
```

You choose the model and method using ID numbers:

| ID | ODE System | Variables | Parameters |
|----|------------|-----------|------------|
| 1 | Predator-Prey | x, y (prey, predator) | α, β, γ, δ |
| 2 | SIR Epidemic (with vital dynamics) | S, I, R (susceptible, infected, recovered) | β, γ, µ |
| 3 | Van der Pol Oscillator | x, y (position, velocity) | µ |
| 4 | Lorenz Attractor | x, y, z (convection, temp difference, distortion) | σ, ρ, β |
| 5 | Two-Compartment Diffusion | C1, C2 (concentration 1, concentration 2) | k |

| ID | Integration Scheme |
|----|--------------------|
| 1 | Forward Euler |
| 2 | Runge-Kutta 4 (RK4) |

The answers are written, one time-step per line, to **`output.txt`**.

---

## 2. The maths: solving ODEs

An **ODE** (Ordinary Differential Equation) is just **a rule for how fast things
change**. For example, "the prey population grows in proportion to how many prey
there are, but shrinks when predators eat them." The ODE doesn't directly tell
you *how many* prey there are at time 50 — it tells you the *rate of change*. To
get the actual numbers, we **start from the known values at t = 0 and take lots
of tiny steps forward in time** (each step is `dt` seconds long). This is what a
numerical solver does.

Every model knows how to compute its own rates of change. For example, the
Predator-Prey model:

```cpp
dydt[0] = alpha * xPop - beta * xPop * yPop;   // how fast prey changes
dydt[1] = delta * xPop * yPop - gamma * yPop;  // how fast predators change
```

The two methods below differ in **how cleverly they use those rates** to take a step.

### Forward (Explicit) Euler — the simple step

> **Analogy:** walking in fog using only the slope of the ground *right under your
> feet*. You point yourself downhill and take one step. Simple, but if the hill
> curves you'll drift off course.

It computes the rate *once* and steps straight ahead:

```cpp
system.computeDerivatives(y, t, dydt);
for (int i = 0; i < dim; i++) {
    y[i] += dt * dydt[i];   // new value = old value + step * slope
}
t += dt;
```

### Runge-Kutta 4 (RK4) — the accurate step

> **Analogy:** before committing to a step, you peek at the slope at the start,
> twice in the middle, and at the end of the step, then take a **weighted average**
> of all four. You "look ahead" before moving, so you follow curves far more
> faithfully.

It samples the slope four times (`k1`–`k4`) and blends them:

```cpp
system.computeDerivatives(y, t, k1);                         // slope at start
// ... k2 and k3 sampled at the midpoint, k4 at the end ...
for (int i = 0; i < dim; i++) {
    y[i] += (dt / 6.0) * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]); // weighted average
}
t += dt;
```

**Why RK4?** It is dramatically more accurate per step. In the project's report,
RK4 output was benchmarked against MATLAB's trusted `ode45` solver on a linear
model (Two-Compartment Diffusion) and a non-linear one (Predator-Prey), and the
curves overlapped almost perfectly (errors around `1e-3`–`1e-2`).

---

## 3. Key C++ concepts (with analogies)

This project is a great showcase of **Object-Oriented Programming (OOP)**. The
big idea: the `Solver` doesn't care *which* model or method it's using — it just
talks to generic "shapes" and lets the specific class fill in the details.

### Abstract classes / interfaces — the blank contract

> **Analogy:** a job contract or a blank form. It lists the duties everyone *must*
> perform, but doesn't do them itself. You can't hire "a contract" — you hire a
> real person who has signed it.

`System` and `Integrator` are **abstract base classes**. The `= 0` makes a
function **pure virtual**, meaning "every model *must* provide this — the base
class refuses to." You can never create a plain `System`; only a real model.

```cpp
class System {
public:
  virtual ~System() = default;
  // "= 0" => every ODE model MUST define these. The base class won't.
  virtual void computeDerivatives(const std::vector<double>& y, double t,
                                  std::vector<double>& dydt) = 0;
  virtual int getDimension() const = 0;
  virtual std::string getName() const = 0;
};
```

### Inheritance — building on a common shape

> **Analogy:** a screwdriver and a wrench are both "tools." They inherit the
> general idea of a tool but each does its own specific job.

A **derived class** inherits from a base class using `: public`. Here five ODE
models inherit from `System`, and two methods inherit from `Integrator`:

```cpp
class PredatorPreySystem : public System { ... };  // "is a" System
class RungeKutta4 : public Integrator { ... };      // "is a" Integrator
```

### Polymorphism — one button, many behaviours

> **Analogy:** a single "play" button on a universal remote. You press the same
> button, but what happens depends on which device is connected. The remote
> doesn't need to know the difference.

This is the heart of the design. The `Solver` holds a pointer to the *generic*
`Integrator`, then calls `step(...)`. Because `step` is `virtual`, C++ decides
**at runtime** whether to run the Euler version or the RK4 version:

```cpp
integrator->step(*system, yCurr, t, dt);   // runs Euler OR RK4 — decided at runtime
```

The payoff is **extensibility**: to add a new model you just write a new class
that inherits from `System`. The solver loop never changes.

### `override` — the safety net

When a derived class re-implements a base function it adds `override`. It's a
promise to the compiler: *"this really does match a virtual function above."* If
you misspell the name, the compiler catches it instead of silently doing nothing.

```cpp
void computeDerivatives(...) override;   // must match System's version exactly
```

### Encapsulation — hide the engine, show the dashboard

> **Analogy:** a car. You use the steering wheel and pedals (the public interface)
> without touching the engine internals (private parts).

Each model keeps its parameters `private` and exposes only `public` methods:

```cpp
class LorenzAttractorSystem : public System {
private:
  double sigma, rho, beta;   // hidden internals
public:
  void setParams(const std::vector<double>& params) override;  // controlled access
};
```

### Dynamic / smart pointers + heap allocation — the auto-returning ticket

> **Analogy:** a cloakroom ticket. You hand over your coat (the object), get a
> ticket (the pointer), and when you leave the ticket *automatically* gets your
> coat back and frees the hook. You never forget your coat.

When the program runs, it doesn't know in advance *which* model it needs, so it
creates the object **on the heap** (a flexible memory pool sized at runtime,
unlike the fixed "stack"). It uses a **smart pointer**, `std::unique_ptr`, which
**owns** the object and automatically deletes it when it goes out of scope.

```cpp
std::unique_ptr<System> system;            // a "ticket" that owns a System
system = std::make_unique<PredatorPreySystem>();  // build the object on the heap
```

`std::make_unique` is the modern, safe way to allocate on the heap (no raw `new`
/ `delete` needed).

### "Does C++ have a garbage collector?" — No, and it doesn't need one here

Languages like **Java and Python have a garbage collector**: a background process
that periodically hunts down unused objects and frees their memory for you.

**C++ has no garbage collector.** Instead it uses a principle called **RAII**
(Resource Acquisition Is Initialization): *an object cleans up after itself the
moment it goes out of scope.* Smart pointers like `unique_ptr` apply RAII to heap
memory — when the `Solver` is destroyed, its `unique_ptr` members automatically
free their objects. No leaks, no manual `delete`, no GC pauses.

For this to work correctly with inheritance, the base class needs a **virtual
destructor**, which ensures the *correct* derived destructor runs during cleanup:

```cpp
virtual ~System() = default;   // virtual => the right cleanup runs for the real model
```

> **In short:** GC = a cleaner who comes around occasionally (Java/Python).
> RAII = every object tidies its own desk the instant it leaves (C++).

### Factory pattern — turning an ID number into an object

> **Analogy:** a vending machine. You press "1" and the right snack drops out.
> You don't assemble the snack yourself.

The `Solver` reads an integer ID from the file and a `switch` builds the matching
object. This keeps all the "which-class-to-create" logic in one tidy place:

```cpp
void Solver::createSystem(int odeID) {
  switch (odeID) {
    case 1: system = std::make_unique<PredatorPreySystem>(); break;
    case 2: system = std::make_unique<SIREpidemicSystem>();  break;
    // ... cases 3, 4, 5 ...
    default: throw std::invalid_argument("Error: Unknown ODE.");
  }
}
```

### Header guards & splitting `.h` from `.cpp`

> **Analogy:** a recipe card vs the actual cooking. The **`.h` header** lists the
> ingredients and steps (the *declarations* — what exists). The **`.cpp` source**
> does the cooking (the *definitions* — the real logic).

This separation (e.g. `System.h` vs `System.cpp`) keeps interfaces clean and
speeds up compiling. Each header is wrapped in an **include guard** so it's never
pasted in twice by accident:

```cpp
#ifndef SYSTEM_H   // "if not already defined..."
#define SYSTEM_H   // "...define it now"
// ... declarations ...
#endif
```

### Exception handling & validation — the bouncer at the door

> **Analogy:** a bouncer checking IDs. Bad input never gets inside to cause
> chaos — it's stopped at the door with a clear reason.

If the input file is missing, malformed, or has the wrong number of values, the
code **throws** an exception, which `main` **catches** and reports cleanly:

```cpp
if (params.size() != 4) {
  throw std::invalid_argument("Predator Prey expects 4 parameters");
}
```

```cpp
try {
  solver.initialise(paramsFile);
  solver.run(outFile);
} catch (const std::exception& e) {
  std::cerr << "Error: " << e.what() << std::endl;   // friendly message, no crash
  return 1;
}
```

### Pass-by-reference, `const`, and `std::vector`

- **`std::vector<double>`** is a resizable array — perfect for holding a system's
  values when we don't know the size until runtime.
- **Pass-by-reference (`&`)** hands a function the *original* data instead of
  copying it — fast, and lets the function update it in place.
- **`const`** is a read-only promise: "I'll look at this but won't change it."

```cpp
//        read-only input ----v                       v---- output we fill in
void computeDerivatives(const std::vector<double>& y, double t,
                        std::vector<double>& dydt);
```

---

## 4. File-by-file breakdown

### `main.cpp` — the front door
The entry point. Creates a `Solver`, tells it to read `parameters.txt` and write
`output.txt`, all wrapped in a `try/catch` so any error prints a clean message.
**Concepts:** exception handling.

### `System.h` — the model interface (declarations)
Declares the abstract `System` base class (pure virtual functions + virtual
destructor) and the **5 derived model classes**, each with its private
parameters. **Concepts:** abstract classes, inheritance, encapsulation, header
guards, virtual destructor.

### `System.cpp` — the model maths (definitions)
Implements each model's `computeDerivatives` (the actual ODE equations),
`setParams` (with validation), and helpers like `getDimension`/`getName`.
**Concepts:** polymorphism (each class overrides the interface), validation/exceptions.

### `Integrator.h` — the method interface (declarations)
Declares the abstract `Integrator` base class with the pure virtual `step(...)`,
plus the derived `ForwardEuler` and `RungeKutta4`. **Concepts:** abstract classes,
inheritance, `override`.

### `Integrator.cpp` — the numerical methods (definitions)
Implements `ForwardEuler::step` (one simple step) and `RungeKutta4::step` (the
four-sample weighted step). **Concepts:** polymorphism, `std::vector`.

### `Solver.h` — the orchestrator interface (declarations)
Declares the `Solver` class. Crucially, it holds the two **smart pointers**
(`std::unique_ptr<System>` and `std::unique_ptr<Integrator>`) and declares the
factory helpers and the public `initialise`/`run`. **Concepts:** smart pointers,
encapsulation.

### `Solver.cpp` — the conductor (definitions)
The brain. `createSystem`/`createIntegrator` are the **factory** switches;
`initialise` reads and **validates** the file then builds the objects on the
**heap**; `run` executes the `while (t < T)` time-stepping loop, repeatedly
calling `integrator->step(...)` (**polymorphism**) and writing each result.
**Concepts:** factory pattern, heap allocation, polymorphism, file I/O, validation.

### Supporting files (not C++ source)
- **`parameters1.txt` / `parameters2.txt`** — sample inputs used in the report
  (diffusion and predator-prey test cases). Rename to `parameters.txt` to run.
- **`output.txt`** — the results table produced by a run (time + values per line).
- **`odeSolverComparisonLinear.m` / `odeSolverComparisonNonLinear.m`** — MATLAB
  scripts that compare this solver's output against `ode45` for validation.
- **`ODE_solver.exe`** — the already-compiled program.

---

## 5. How it all fits together (data flow)

```
parameters.txt
      |
      v
Solver::initialise(...)        -> opens file, VALIDATES every line
      |                           -> createSystem(id)     (factory builds the model on the heap)
      |                           -> createIntegrator(id) (factory builds the method on the heap)
      v
Solver::run(...)               -> loop while (t < T):
      |                                integrator->step(*system, y, t, dt)   <-- polymorphism
      |                                     |  calls  system.computeDerivatives(...)  <-- the model's ODE
      |                                     v
      |                                writes one line of results
      v
output.txt
```

The key insight: `Solver::run` only ever talks to the **generic** `System` and
`Integrator`. Swapping models or methods changes nothing in this loop — that's
the power of inheritance + polymorphism.

---

## 6. How to build & run

Put all `.cpp` and `.h` files in one folder, then with the GCC compiler (C++17):

```bash
g++ -Wall -Wpedantic -std=c++17 *.cpp -o ODE_solver
./ODE_solver
```

This compiles `main.cpp`, `Solver.cpp`, `System.cpp`, and `Integrator.cpp` into
one program. It needs a valid **`parameters.txt`** in the same folder. On success
it prints a confirmation and writes the answers to **`output.txt`**.
