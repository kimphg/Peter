# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
from time import sleep
import numpy as np
from math import sqrt
import matplotlib.pyplot as plt
def predict_probability(feature_matrix, coefficients):
    # Take dot product of feature_matrix and coefficients  
    ## YOUR CODE HERE
    dotPro = np.dot(feature_matrix,coefficients)
    
    # Compute P(y_i = +1 | x_i, w) using the link function
    ## YOUR CODE HERE
    predictions = 1./(1 + np.exp(-dotPro))
    
    return predictions
def feature_derivative_with_L2(errors, feature, coefficient, l2_penalty, feature_is_constant): 
    
    # Compute the dot product of errors and feature
    ## YOUR CODE HERE
    derivative = np.dot(errors,feature)

    # add L2 penalty term for any feature that isn't the intercept.
    if not feature_is_constant: 
        ## YOUR CODE HERE
        derivative-=2.*l2_penalty*coefficient
        
    return derivative
def compute_log_likelihood_with_L2(feature_matrix, sentiment, coefficients, l2_penalty):
    indicator = (sentiment==+1)
    scores = np.dot(feature_matrix, coefficients)
    #print(scores)
    lp = np.sum((indicator-1)*scores - np.log(1. + np.exp(-scores))) - l2_penalty*np.sum(coefficients[1:]**2)
    
    return lp
def logistic_regression_with_L2(feature_matrix, sentiment, initial_coefficients, step_size, l2_penalty, max_iter):
    coefficients = np.array(initial_coefficients) # make sure it's a numpy array
    for itr in xrange(max_iter):
        # Predict P(y_i = +1|x_i,w) using your predict_probability() function
        ## YOUR CODE HERE
        predictions = predict_probability(feature_matrix,coefficients)
        
        # Compute indicator value for (y_i = +1)
        indicator = (sentiment==+1)*1
        
        # Compute the errors as indicator - predictions
        errors = indicator - predictions
        for j in xrange(len(coefficients)): # loop over each coefficient
            is_intercept = (j == 0)
            # Recall that feature_matrix[:,j] is the feature column associated with coefficients[j].
            # Compute the derivative for coefficients[j]. Save it in a variable called derivative
            ## YOUR CODE HERE
            derivative = feature_derivative_with_L2(errors,feature_matrix[:,j],coefficients[j],l2_penalty,is_intercept)
            #print(derivative,j)
            # add the step size times the derivative to the current coefficient
            ## YOUR CODE HERE
            coefficients[j]= coefficients[j]+derivative*step_size/(itr/900.+1.)
            #print derivative
        # Checking whether log likelihood is increasing
        if itr <= 15 or (itr <= 100 and itr % 10 == 0) or (itr <= 1000 and itr % 100 == 0) \
        or (itr <= 10000 and itr % 1000 == 0) or itr % 10000 == 0:
            lp = compute_log_likelihood_with_L2(feature_matrix, sentiment, coefficients, l2_penalty)
            print 'iteration %*d: log likelihood of observed labels = %.8f' % \
                (int(np.ceil(np.log10(max_iter))), itr, lp)
            #
    return coefficients
def get_classification_accuracy(feature_matrix, sentiment, coefficients):
    scores = np.dot(feature_matrix, coefficients)
    apply_threshold = np.vectorize(lambda x: 1. if x > 0  else -1.)
    predictions = apply_threshold(scores)
    
    num_correct = ((predictions == sentiment)).sum()
    accuracy = float(num_correct) / (feature_matrix.shape[0])    
    return accuracy
def loadData(fileName):
    dataTxt = np.genfromtxt(fileName,dtype='str')
    dataset = np.loadtxt(dataTxt,dtype='double',delimiter=',')
    return dataset
train_data = loadData('logfileLinearFit.dat')

#train_data = train_data[train_data[:,0]<430.]
#train_data = train_data[train_data[:,1]<1.]
#train_data = train_data[train_data[:,2]<120.]
#train_data = train_data[train_data[:,3]<125.]#125*
#train_data = np.delete(train_data, [0], axis=1)
#train_data = np.delete(train_data, [0], axis=1)
#train_data = np.delete(train_data, [0], axis=1)
#train_data = dataset[:dataset.shape[0]/2,:]
#print(train_data.shape)
#test_data = dataset[dataset.shape[0]/2:,:]
#print(test_data.shape)
feature_matrix = np.c_[np.ones(train_data.shape[0]),train_data[:,:(train_data.shape[1]-1)]]
label_array = train_data[:,-1]
#label_array = (label_array*2)-1
print(train_data.shape)
dataset0 = train_data[label_array==-1]
dataset1 = train_data[label_array== 1]
print(dataset0.shape)
print(dataset1.shape)
#initial_coefficients=np.array([ 0.45844316,-0.00055291,-0.02725333,-0.03192186, 0.02382861])
initial_coefficients = np.zeros(feature_matrix.shape[1])
coefficients_0_penalty = logistic_regression_with_L2(feature_matrix, label_array,
                                                     initial_coefficients,
                                                     step_size=1e-3, l2_penalty=0, max_iter=40001)
                                                     
print (coefficients_0_penalty)
test_data = loadData('logfileLinearFit.dat')
#test_data = test_data[test_data[:,3]<300.]
#test_data = test_data[test_data[:,0]<500.]
#test_data = test_data[test_data[:,1]<1.]
#test_data = test_data[test_data[:,2]<120.]
#test_data = np.delete(test_data, [0], axis=1)
#test_data = np.delete(test_data, [0], axis=1)
#test_data = np.delete(test_data, [0], axis=1)
label_test_data = test_data[:,-1]
feature_matrix_test = np.c_[np.ones(test_data.shape[0]),test_data[:,:(test_data.shape[1]-1)]]
#accTrain = get_classification_accuracy(feature_matrix,label_array,coefficients_0_penalty)
accTest = get_classification_accuracy(feature_matrix_test,label_test_data,coefficients_0_penalty)
#print accTrain,accTest
print accTest
#plt.hist(dataset1[:2500,0],50)
#plt.hist(dataset0[:,0],50)

