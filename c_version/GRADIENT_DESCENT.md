Gradient Descent Algorithm Comparison
This document outlines the core differences between the standard Gradient Descent methods (Batch, Stochastic, Mini-Batch) and the advanced adaptive optimizer, Adam (Adaptive Moment Estimation), which was implemented in the Canvas file.

Core Differences by Algorithm
The primary distinction among the first three algorithms is the number of training examples used to calculate the gradient before a weight update occurs.

Algorithm

Batch Size (B)

Weight Updates per Epoch

Convergence Path

Primary Feature

Batch Gradient Descent (BGD)

m (All data)

1

Smooth and direct

Guaranteed convergence (for convex functions), but slow.

Stochastic Gradient Descent (SGD)

1 (Single example)

m (Very frequent)

Noisy, high variance

Fastest initial updates; can escape shallow local minima.

Mini-Batch Gradient Descent (MBGD)

32 to 256 (e.g., 64)

m/B

Moderately noisy

Industry standard; optimal balance of speed and stability.

Adam

32 to 256 (Adaptive)

m/B

Efficient and stable

Adaptive learning rate per weight; incorporates momentum.

Why Adam Stands Out (Adaptive Optimization)
Adam is significantly more complex than standard gradient descent methods because it is an adaptive learning rate optimizer. It doesn't use a single fixed learning rate (η) for all weights; instead, it dynamically calculates a unique learning rate for every individual weight (w 
j
​
 ).

1. Momentum (First Moment: m 
t
​
 )
Adam maintains an exponentially decaying average of past gradients, often referred to as the first moment (m 
t
​
 ). This mechanism is known as Momentum.

Impact: It helps the optimizer accelerate movement along directions of consistent gradient and dampens oscillations in directions where the gradient frequently changes sign.

2. Adaptive Learning Rate Scaling (Second Moment: v 
t
​
 )
Adam also maintains an exponentially decaying average of the squared past gradients, known as the second moment (v 
t
​
 ). This mechanism is similar to RMSprop.

Impact: It effectively scales the learning rate inversely proportional to the magnitude of the recent gradients for that specific weight.

Weights with small, rare updates get a larger effective learning rate.

Weights with large, frequent updates get a smaller effective learning rate.

The Adam Update Formula
The core weight update for Adam (after bias correction) is:

w 
t
​
 ←w 
t−1
​
 − 
v
^
  
t
​
 
​
 +ϵ
α
​
 ⋅ 
m
^
  
t
​
 
Where:

α is the base learning rate (ALPHA_RATE).

m
^
  
t
​
  is the bias-corrected momentum (first moment).

v
^
  
t
​
  is the bias-corrected squared gradient average (second moment).

ϵ is the stability term (EPSILON).

This formula shows that the step size is effectively controlled by the momentum ( 
m
^
  
t
​
 ) and scaled by the history of squared gradients ( 
v
^
  
t
​
 ), giving it superior performance in most machine learning tasks.
