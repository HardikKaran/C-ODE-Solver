% Housekeeping
clc; clear; close all;

% Load data from output.txt to arrays
data = load('output.txt');
t_cpp = data(:, 1);
prey_cpp = data(:, 2);
pred_cpp = data(:, 3);

% Define Predator-Prey Parameters 
alpha = 1.1; 
beta  = 0.4;  
delta = 0.1; 
gamma = 0.4;  

% Define the ODE system (Lotka-Volterra)
odeSys = @(t, y) [
    alpha * y(1) - beta * y(1) * y(2);   
    delta * y(1) * y(2) - gamma * y(2)   
];


% Initial conditions from the C++ file (first row)
ic = [prey_cpp(1); pred_cpp(1)];

% Run ode45 using the same time points as the C++ output for direct comparison
options = odeset('RelTol', 1e-6, 'AbsTol', 1e-6);
[tODE, yODE] = ode45(odeSys, t_cpp, ic, options);

% Error Calculation
errPrey = prey_cpp - yODE(:, 1);
errPred = pred_cpp - yODE(:, 2);
errSum = abs(errPrey) + abs(errPred);

% C++ and ode45 plots
figure(1);
hold on;

% Plot ode45 
plot(tODE, yODE(:, 1), 'r-', 'LineWidth', 2, 'DisplayName', 'ode45 Prey');
plot(tODE, yODE(:, 2), 'g-', 'LineWidth', 2, 'DisplayName', 'ode45 Predator');

% Plot C++ Output 
plot(t_cpp, prey_cpp, 'b--', 'LineWidth', 2, 'DisplayName', 'C++ Prey');
plot(t_cpp, pred_cpp, 'k--', 'LineWidth', 2, 'DisplayName', 'C++ Predator');

title('Comparison: C++ Implementation vs MATLAB ode45 (Lotka-Volterra)');
xlabel('Time (t)');
ylabel('Population Size');
legend('Location', 'best');
grid on;
hold off;

% Error analysis plot
figure('Name', 'Error Analysis', 'Color', 'w');
plot(t_cpp, errSum, 'r.-', 'DisplayName', 'Total Absolute Error');
title('Total Error (C++ Value - ode45 Value)');
xlabel('Time (t)');
ylabel('Absolute Error');
grid on;