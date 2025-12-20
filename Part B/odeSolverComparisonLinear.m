% Housekeeping
clc; clear; close all;

% Load data from output.txt to arrays
data = load('output.txt');
t_cpp = data(:, 1);
c1_cpp = data(:, 2);
c2_cpp = data(:, 3);

k = 0.5; 

% Define the ODE system (Two-Compartment Diffusion Model)
odeSys = @(t, y) [-k * (y(1) - y(2)); k * (y(1) - y(2))];

% Initial conditions from the file
ic = [c1_cpp(1); c2_cpp(1)];

% Run ode45 for benchmark solution
options = odeset('RelTol', 1e-6, 'AbsTol', 1e-6);
[tODE, yOde] = ode45(odeSys, t_cpp, ic, options);

% Absolute error between C++ output and ode45
errC1 = c1_cpp - yOde(:, 1);
errC2 = c2_cpp - yOde(:, 2);
errSum = abs(errC1) + abs(errC1); % More representative of system's error

% C++ and ode45 plots
figure(1);
hold on;

% Plot ode45
plot(tODE, yOde(:, 1), 'b-', 'LineWidth', 2, 'DisplayName', 'ode45 C1');
plot(tODE, yOde(:, 2), 'b-', 'LineWidth', 2, 'DisplayName', 'ode45 C2');

% Plot C++ output 
plot(t_cpp, c1_cpp, 'r--', 'LineWidth', 2, 'DisplayName', 'C++ Output C1');
plot(t_cpp, c2_cpp, 'r--', 'LineWidth', 2, 'DisplayName', 'C++ Output C2');

title('Comparison: C++ Implementation vs MATLAB ode45');
xlabel('Time (s)');
ylabel('Concentration (mol/dm^3)');
legend('Location', 'best');
grid on;
hold off;

% Error analysis plot
figure(2);
plot(t_cpp, errSum, 'b.-', 'DisplayName', 'Error');

title('Total Error (C++ Value - ode45 Value)');
xlabel('Time (s)');
ylabel('Absolute Error');
grid on;