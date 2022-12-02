#Basic RNN network

### Import Libraries ###

import tensorflow as tf
import tensorflow.keras as keras
import tensorflow.keras.layers 
import random
import numpy as np

### Set random seed to ensure deterministic results
import os
seed_value = 1
os.environ['PYTHONHASHSEED']=str(seed_value)
def reset_random_seeds():
   tf.random.set_seed(seed_value)
   np.random.seed(seed_value)
   random.seed(seed_value)
reset_random_seeds()

### Prepare and Preprocess Data Here ###
from pandas import read_csv

### Design a Function to Prepare Observation Sequences and Corresponding Labels ###

def create_dataset(dataset, look_back=12):
  dataX, dataY = [], []
  for i in range(len(dataset)-look_back-1):
    a = dataset[i:(i+look_back), 0]
    dataX.append(a)
    dataY.append(dataset[i + look_back, 0])
  return np.array(dataX), np.array(dataY)


### Train and Test Data Loading with float32 type ####
dataframe_train = read_csv('train.txt', usecols=[1], engine='python') # Read train.txt 
dataset_train = dataframe_train.values
dataset_train = dataset_train.astype('float32') # Specify the data type to 'float32'

dataframe_test = read_csv('test.txt', usecols=[1], engine='python') # Read train.txt 
dataset_test = dataframe_train.values
dataset_test = dataset_train.astype('float32') # Specify the data type to 'float32'



### Scale Training and Test Data to [0, 1] ###
from sklearn.preprocessing import MinMaxScaler
scaler = MinMaxScaler(feature_range=(0, 1)) # specify the scaler

train = scaler.fit_transform(dataset_train) # fit the scaler to the training data
test = scaler.transform(dataset_test) # fit the scaler to the test data


### Training and Test Data Split ###
trainX, trainY = create_dataset(train, look_back=12)
testX, testY =   create_dataset(test, look_back = 12)

### Training and Test Data Reshape (to fit RNN input) ###
trainX = np.reshape(trainX, (trainX.shape[0], 1, trainX.shape[1]))
testX = np.reshape(testX, (testX.shape[0], 1, testX.shape[1]))

### Build the RNN Model ###
import keras
from keras.models import Sequential

keras.backend.clear_session()
model = Sequential() # Declare Sequential class and assign it to variable "model"
model.add(keras.layers.SimpleRNN(4)) # Add a simple RNN layer with unit_size=4 in the model 
model.add(keras.layers.Dense(units=1)) # Add a following Dense layer with units=1 in the model 

### Compile the RNN Model  ###

opt = keras.optimizers.Adam(learning_rate=0.005)
model.compile(opt, loss = 'mse') # model compiled with mean_squared_error loss and adam optimizer #CHANGE FROM ...

### Train the RNN Model  ###

model.fit(trainX, trainY, epochs = 1000, batch_size=10, verbose=2) # model fit with epoch=1000, batch_size=10; verbose=2 is optional.
model.summary() # print out model structure with model.summary()

### Make Predictions ###
trainPredict = model.predict(trainX)
testPredict = model.predict(testX)

### Scale Back Predictions ###

trainPredict = scaler.inverse_transform(trainPredict) # scale train prediction back with scaler.inverse_transform()
trainY = scaler.inverse_transform([trainY])  # scale train labels back with scaler.inverse_transform()

testPredict = scaler.inverse_transform(testPredict) # scale test prediction back with scaler.inverse_transform()
testY = scaler.inverse_transform([testY]) # scale test labels back with scaler.inverse_transform()

### Calculate Root Mean Squared Error (RMSE) ###
import math
from sklearn.metrics import mean_squared_error # Import mean_squared_error from sklearn.metrics

trainScore = math.sqrt(mean_squared_error(trainY[0], trainPredict[:,0]))
testScore = math.sqrt(mean_squared_error(testY[0], testPredict[:,0])) 
print('Train Score: %.2f RMSE' % (trainScore))
print('Test Score: %.2f RMSE' % (testScore))

### Plot Observation Data and Prediction Results with TEST dataset ###

import matplotlib.pyplot as plt
plt.plot(testY[0]) # Plot Observations in Test Set
plt.plot(testPredict) # Plot Predictions in Test Set

plt.show()
