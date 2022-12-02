# performs Max-Pooling and Average-Pooling on image


# Import useful libraries
import numpy as np
import imageio
from IPython.display import display, Image
import matplotlib.pyplot as plt


# # Convert image to array, print out the shape of array, and print out the entire array
img_matrix = imageio.imread('image.jpg')
img_matrix2 = imageio.imread('image.jpg')
print('\n\nSize of array:', img_matrix.shape)
print('Image mmatrix form:\n', img_matrix)
print('Original Image:')
Image('image.jpg')

######## Convolution with Laplacian Filter ##################

# create the lapalacian kernel
kernel = np.array([[0,1,0],[1,-4,1],[0,1,0]])

# conduct convolution 
def convolve2D(img_matrix, kernel, strides, padding=0):
  xShape = img_matrix.shape[0] 
  yShape =  img_matrix.shape[1]

  xOut = int(((xShape - kernel.shape[0]  + 2 * padding) / strides) + 1)
  yOut = int(((yShape - kernel.shape[1]  + 2 * padding) /strides) + 1)

  output = np.zeros((xOut, yOut))

  x=0
  y=0
  imagePadded = img_matrix
  for i in range(yShape):
    if i % strides==0 and i <= xShape - 3:
       if i != 0: y = y+1
       for j in range(xShape):
            if j% strides==0 and j<= yShape -3: 
              x = x+1
              if x>= output.shape[0]:
                x =0
              if y >= yOut: y=0
              #print(i ,' ' , j)
              curr = np.array([    [img_matrix[x][y], img_matrix[x][y+1],img_matrix[x][y+2]],    [img_matrix[x+1][y], img_matrix[x+1][y+1], img_matrix[x+1][y+2]],    [img_matrix[x+2][y],img_matrix[x+2][y+1],img_matrix[x+2][y+2]]   ])
              # assigning 
              output[x, y] = (kernel * curr).sum()
  

  return output

output = (convolve2D(img_matrix, kernel,1,0))


# PRINT OUT convolution result for first ten rows.
print('First ten rows: \n')
for i in range(10):
  print(output[i,:])
print('\n')
# PRINT OUT the shape of the convolution result.
print('Shape of output: ', output.shape)
print('\n')
# DISPLAY convolution result as image with matplotlib. (Don't worry about the value <0 or >255. Scaling process will be conducted in imshow function to make sure valid display.)
plt.imshow(output)

######## Convolution with Laplacian Filter ##################


output = (convolve2D(img_matrix, kernel,2,0))

# PRINT OUT convolution result for first ten rows.
print('First ten rows: \n')
for i in range(10):
  print(output[i,:])
print('\n')
# PRINT OUT the shape of the convolution result.
print('Shape of output: ', output.shape)
print('\n')
# DISPLAY convolution result as image with matplotlib. (Don't worry about the value <0 or >255. Scaling process will be conducted in imshow function to make sure valid display.)
plt.imshow(output)

######## MaxPooling with the setting of 2X2 ##################
mask = np.array([[3,-1], [-3,1]])

# maxPooling function
def maxPool(img_matrix, mask):

  # get size of original input and use it to find dimensions of output
  xShape = img_matrix.shape[0] 
  yShape =  img_matrix.shape[1]
  xOut = (int)(xShape/ mask.shape[0])
  yOut = (int)(yShape/mask.shape[1] )

  # size of output
  output = np.zeros((xOut, yOut))

  for i in range(yShape):
    if i%2==0 and i!=30:
       for j in range(xShape):
            if j%2==0 and j!=30: 
              if i != 0:
                x = (int)(i/2)
              else:
                 x = 0
              if j != 0:
                y = (int)(j/2)
              else:
                y = 0
              # assigning the max value found in the pooling size and assigning it to its corresponding output location
              output[x][y] = max(img_matrix[i][j], img_matrix[i+1][j],img_matrix[i][j+1],img_matrix[i+1][j+1])

  return output

  
# call maxPool
output3 = (maxPool(img_matrix,mask))


# PRINT OUT convolution result for first ten rows.
print('First ten rows: \n')
for i in range(10):
  print(output3[i,:])
print('\n')
# PRINT OUT the shape of the convolution result.
print('Shape of output: ', output3.shape)
print('\n')
# DISPLAY convolution result as image with matplotlib. (Don't worry about the value <0 or >255. Scaling process will be conducted in imshow function to make sure valid display.)
plt.imshow(output3)

