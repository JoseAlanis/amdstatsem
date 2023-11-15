# Load required libraries
library(plotly)
library(purrr)
library(dplyr)

# Set seed for reproducibility
set.seed(123)

# Generate some data
n <- 100
x <- rnorm(n)
y <- 5*x + 3 + rnorm(n, sd = 0.5) # Increase slope for a steeper descent

# Define a range of potential slopes and intercepts
slopes <- seq(4, 6, length.out = 100)
intercepts <- seq(2.5, 3.5, length.out = 100)

# Prepare a matrix to store the RMSE values
rmse_matrix <- matrix(nrow = length(slopes), ncol = length(intercepts))

# Calculate RMSE for each combination of slope and intercept
for (i in seq_along(slopes)) {
  for (j in seq_along(intercepts)) {
    preds <- slopes[i]*x + intercepts[j]
    rmse_matrix[i, j] <- sqrt(mean((preds - y)^2))
  }
}

# Find the coordinates of the minimum RMSE
min_rmse_coords <- which(rmse_matrix == min(rmse_matrix), arr.ind = TRUE)
min_slope <- slopes[min_rmse_coords[1, 1]]
min_intercept <- intercepts[min_rmse_coords[1, 2]]

# Create surface plot
fig <- plot_ly(x = intercepts, y = slopes, z = rmse_matrix, type = "surface")

# Add point for the global minimum
fig <- fig %>% add_markers(x = c(min_intercept), y = c(min_slope), z = c(min(rmse_matrix)),
                         mode = "markers", marker = list(size = 5, color = 'red'),name="Global Minima")

fig <- fig %>% layout(scene = list(xaxis = list(title = 'alpha'),
                                   yaxis = list(title = 'beta'),
                                   zaxis = list(title = 'RMSE')))
fig


