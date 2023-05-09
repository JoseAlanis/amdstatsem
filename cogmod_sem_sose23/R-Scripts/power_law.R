# Load all packages

library(plyr)
library(tidyverse)
library(ggplot2)

# Make Datapoints for the functions. 
# N = Number of Steps, alpha = learning rate 

N <- 1:100
alpha <- seq(0.1, 1,0.2)

# Define functions for exponential and power law


# Exponential Law

power_law_new <- function(N,alpha)
{
  
  RT = exp(-alpha*N)
  
  return(RT)
  
  
}

# Task: Define a new function with the classic Power Law

power_law <- function(N,alpha)
{
  
  RT = ..... # YOUR CODE HERE 
  
  # Solution
  
  # RT = N^(-alpha)
  
  return(RT)
  
  
}


# We apply the function and simulate Data

params <- expand.grid(N =N, alpha = alpha) # Cross all Steps with learning rates
all <- mdply(params, power_law_new) #apply the function "power_law" to the created data points using mdply

# We plot the created data with ggplot

ggplot(all, aes(N,V1*5, colour=factor(alpha)))+geom_line(linewidth=1)+theme_bw() +
  labs(title = "Power Law of Learning (Heathcote,2000)", colour="alpha") + 
  xlab("Learning Trials") + ylab("Learning Time") 
