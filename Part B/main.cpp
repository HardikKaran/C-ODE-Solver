#include "Solver.h"
#include <iostream>

/*
ODE Solver
Author: Hardik Karan (02559704)
Date: December 2025
Description: A modular solver for 5 ODE systems using RK4 and Explixit Euler methods.
*/

int main() {
    try {
        Solver solver;

        std::string paramsFile = "parameters.txt";
        std::string outFile = "output.txt";

        // Required input filename: parameters.txt
        solver.initialise(paramsFile);

        // Write results to output.txt
        solver.run(outFile);

        std::cout << "Simulation complete!" << std::endl;

    } catch (const std::exception& e) {
        // Catch any errors thrown during file reading or validation
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}