import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Original data
x = np.array([0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5])
y = np.array([0.3, 2.23265, 3.25635, 2.88963, 1.30496, -0.75235, -2.31473, -2.64736, -1.5938, 0.350442, 2.27096, 3.2645, 2.8638, 1.2573, -0.799437, -2.33909, -2.63753, -1.55441, 0.400869, 2.30871, 3.27182, 2.83724, 1.20936, -0.846214, -2.3627, -2.62688, -1.5145, 0.451268, 2.34589, 3.2783, 2.80997, 1.16116, -0.892667, -2.38556, -2.6154, -1.47407, 0.501624, 2.38249, 3.28393, 2.78198, 1.11272, -0.938783, -2.40766, -2.60309, -1.43315, 0.551923, 2.41851, 3.28873, 2.7533, 1.06405, -0.984548])

# Define the sinusoidal function
def sinusoidal(x, A, B, C, D):
    return A * np.sin(B * x + C) + D

# Fit the sinusoidal function to the data
params, params_covariance = curve_fit(sinusoidal, x, y, p0=[1, 1, 0, 0])

# Generate predictions
y_pred = sinusoidal(x, params[0], params[1], params[2], params[3])

# Plot the original function and predicted values
plt.plot(x, y, label='function', color='blue')
plt.plot(x, y_pred, label='predict', color='orange')
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
plt.title('Sinusoidal Approximation')
plt.grid(True)
plt.show()
