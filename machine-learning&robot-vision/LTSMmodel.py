import tensorflow as tf
import tensorflow.keras as keras
from keras import layers
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

reset_random_seeds() # randomly set initial data

# Prepare the data here

max_features = 1000
maxlen = 100

(x_train, y_train), (x_val, y_val) = keras.datasets.imdb.load_data(num_words = max_features ) # load IMDB data with specified num_words = 1000; testing set is set to validation set.
print(len(x_train), "Training sequences")
print(len(x_val), "Validation sequences")
x_train = keras.preprocessing.sequence.pad_sequences(x_train,maxlen) # Pad IMDB training data with specified maxlen=100
x_val = keras.preprocessing.sequence.pad_sequences( x_val, maxlen) # Pad IMDB validation data with specified maxlen=100

### Model design with Embedding and LSTM layers ####
inputs = keras.Input(shape=(None,), dtype="int32") # This is an easy way to set an adaptive length for input sequence
x = layers.Embedding(max_features, 8)(inputs) # Embed data in an 8-dimensional vector
x = layers.LSTM(8)(x) # Add 1st layer of LSTM with 8 hidden states (aka units)
outputs = layers.Dense(1, activation= 'sigmoid')(x) # Add a classifier with units=1 and activation="sigmoid"

### Clear cached model to refresh memory and build new model for training ###
keras.backend.clear_session() # Clear cached model
model = keras.Model(inputs, outputs) # Build new keras model
model.summary() # Print out model summary

model.compile("adam", "binary_crossentropy", metrics=["accuracy"]) # Compile built model with "adam", "binary_crossentropy", and metrics=["accuracy"]
model.fit( x_train, y_train, batch_size=64, epochs=10, validation_data=(x_val, y_val)) # Train the compiled model with model.fit()

########################### Scenario 1 ###########################
##################################################################

### Set random seed to ensure deterministic results ###
import os
seed_value = 1
os.environ['PYTHONHASHSEED']=str(seed_value)
def reset_random_seeds():
   tf.random.set_seed(seed_value)
   np.random.seed(seed_value)
   random.seed(seed_value)

reset_random_seeds() # randomly set initial data

max_features =  1000# Only consider the top 1k words
maxlen =  100 #Only consider the first 100 words of each movie review

### Model design with Embedding and LSTM layers ####
inputs = keras.Input(shape=(None,), dtype="int32") # This is an easy way to set an adaptive length for input sequence
x = layers.Embedding(max_features,16 )(inputs) # Embed data in a 16-dimensional vector
x = layers.LSTM(16, return_sequences = True)(x) # Add 1st layer of LSTM with 16 hidden states (aka units); set return_sequences=true.
x = layers.LSTM(16)(x) # Add 2nd layer of LSTM with 16 hidden states (aka units)
outputs = layers.Dense(1, activation = 'sigmoid')(x) # Add a classifier with units=1 and activation="sigmoid"


### Clear cached model to refresh memory and build new model for training ###
keras.backend.clear_session() # Clear cached model
model = keras.Model(inputs, outputs) # Build new keras model
model.summary() # Print out model summary

model.compile("adam", "binary_crossentropy", metrics=["accuracy"]) # Compile built model with "adam", "binary_crossentropy", and metrics=["accuracy"]
model.fit( x_train, y_train, batch_size=64, epochs=10, validation_data=(x_val, y_val)) # Train the compiled model using model.fit() with batch_size=64, epochs=10, and validation_data=(x_val, y_val)

########################### Scenario 2 ###########################
##################################################################

### Set random seed to ensure deterministic results ###
import os
seed_value = 1
os.environ['PYTHONHASHSEED']=str(seed_value)
def reset_random_seeds():
   tf.random.set_seed(seed_value)
   np.random.seed(seed_value)
   random.seed(seed_value)

reset_random_seeds() # randomly set initial data

max_features =  1000 # Only consider the top 1k words
maxlen =  100# Only consider the first 100 words of each movie review

### Model design with Embedding and LSTM layers ####
inputs = keras.Input(shape=(None,), dtype="int32") # This is an easy way to set an adaptive length for input sequence
x = layers.Embedding(max_features,128)(inputs) # Embed data in a 128-dimensional vector
x = layers.LSTM(128, return_sequences = True)(x) # Add 1st layer of LSTM with 128 hidden states (aka units); set return_sequences=true.
x = layers.LSTM(128)(x) # Add 2nd layer of LSTM with 128 hidden states (aka units)
outputs = layers.Dense(1, activation = 'sigmoid')(x) # Add a classifier with units=1 and activation="sigmoid"


### Clear cached model to refresh memory and build new model for training ###
keras.backend.clear_session() # Clear cached model
model = keras.Model(inputs, outputs) # Build new keras model
model.summary() # Print out model summary

model.compile("adam", "binary_crossentropy", metrics=["accuracy"]) # Compile built model with "adam", "binary_crossentropy", and metrics=["accuracy"]
model.fit( x_train, y_train, batch_size=64, epochs=10, validation_data=(x_val, y_val)) # Train the compiled model using model.fit() with batch_size=64, epochs=10, and validation_data=(x_val, y_val)

########################### Scenario 3 ###########################
##################################################################

### Set random seed to ensure deterministic results ###
import os
seed_value = 1
os.environ['PYTHONHASHSEED']=str(seed_value)
def reset_random_seeds():
   tf.random.set_seed(seed_value)
   np.random.seed(seed_value)
   random.seed(seed_value)

reset_random_seeds() # randomly set initial data

max_features =  1000 # Only consider the top 1k words
maxlen = 200# Only consider the first 200 words of each movie review

### Model design with Embedding and LSTM layers ####
inputs = keras.Input(shape=(None,), dtype="int32") # This is an easy way to set an adaptive length for input sequence
x = layers.Embedding(max_features,128 )(inputs) # Embed data in a 128-dimensional vector
x = layers.LSTM(128, return_sequences = True )(x) # Add 1st layer of LSTM with 128 hidden states (aka units); set return_sequences=true.
x = layers.LSTM( 128)(x) # Add 2nd layer of LSTM with 128 hidden states (aka units)
outputs = layers.Dense(1, activation = 'sigmoid' )(x) # Add a classifier with units=1 and activation="sigmoid"

### Clear cached model to refresh memory and build new model for training ###
keras.backend.clear_session() # Clear cached model
model = keras.Model(inputs, outputs) # Build new keras model
model.summary() # Print out model summary

model.compile("adam", "binary_crossentropy", metrics=["accuracy"]) # Compile built model with "adam", "binary_crossentropy", and metrics=["accuracy"]
model.fit( x_train, y_train, batch_size=64, epochs=10, validation_data=(x_val, y_val)) # Train the compiled model using model.fit() with batch_size=64, epochs=10, and validation_data=(x_val, y_val)

