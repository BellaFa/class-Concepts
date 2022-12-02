# When given an image of a piece of paper on a plain surface, it find the corners computes a transformation
# matrix and fits the paper to the dimension of the screen. 


# perform warping
def  warpImage(im,H,limit_coordinates):
  
  # the values that can take y
  i = np.linspace(0, im.shape[0]-1, im.shape[0]).astype(int)
  # the values that can take x
  j = np.linspace(0, im.shape[1]-1, im.shape[1]).astype(int)
  # combination of values that can take x, and y in a grid
  yv, xv = np.meshgrid(i, j)

  # nimber of points on the grid
  numpoints = xv.ravel().shape[0]

  # extract the color channels
  r = im[yv.ravel(),xv.ravel(),0]
  g = im[yv.ravel(),xv.ravel(),1]
  b = im[yv.ravel(),xv.ravel(),2]

  # all the values of x coordinates as a row
  x_cord = xv.ravel().reshape((1,numpoints))
  # all the values of y coordinates as a row
  y_cord = yv.ravel().reshape((1,numpoints))

  # define the homogeneous coordinates
  grid = np.concatenate((x_cord, y_cord, np.ones((1,numpoints))),axis=0)
  # transform the grid (your canvas) in the new coordinated determined by the homography defined below 
  newgrid = np.matmul(H,grid)

  # Convert from homogeneous to heterogeneous coordinates
  newgridHetero = newgrid[:2,:]   # the x, and y
  thethirdcoordinate = newgrid[2,:]

  # TO DO : complete the conversion. remember x=X/w          
  newgridHetero[0,:]= newgridHetero[0,:]/thethirdcoordinate
  newgridHetero[1,:]= newgridHetero[1,:]/thethirdcoordinate

  # define your new empty canvas (it will have -1 if is not defined)
  warped = -np.ones((1001,1001,3)).astype(np.int16)

  # Raw approximation. Considering just the integer values 
  newgrid2= newgridHetero.round().astype(np.int64)
  
  # only consider points that are in your area of interest (0 to 1000)
  validsX=np.bitwise_and(newgrid2[0,:]>=limit_coordinates[0],     newgrid2[0,:]<limit_coordinates[1])
  validsY=np.bitwise_and(newgrid2[1,:]>=limit_coordinates[2],     newgrid2[1,:]<limit_coordinates[3])
  valids = np.bitwise_and(validsX,validsY)

  # assign color to the pixels that are valids # TO DO: ASSIGN the remaining colors 
  warped[newgrid2[1,valids], newgrid2[0,valids],0]   =r[valids]
  warped[newgrid2[1,valids], newgrid2[0,valids],1]   =g[valids]
  warped[newgrid2[1,valids], newgrid2[0,valids],2]   =b[valids]

  return warped


# Find the transformation that maps points A, B, C, D  to a new set of points
def computeH(im1_pts,im2_pts):
  im1_pts = im1_pts.reshape((4,2))
  h = np.zeros((4,2),dtype = np.float32)

  add = im1_pts.sum(1)
  h[0] = im1_pts[np.argmin(add)]
  h[2] = im1_pts[np.argmax(add)]

  diff = np.diff(im1_pts,axis=1)
  h[1] = im1_pts[np.argmin(diff)]
  h[3] = im1_pts[np.argmax(diff)]

  pts = np.float32([[0,0],[800,0],[800,800],[0,800]]) # 800
  t = cv2.getPerspectiveTransform(h,pts)

  return t

# filter the image so it can be better understood when looking for corners
def filter(image):
  gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
  blur = cv2.GaussianBlur(gray,(5,5),1)
  image = cv2.Canny(image,200,200)
  return image

# Find the corners of the paper in the image
def getCorners(im):
  target = 0
  contours,hierarchy=cv2.findContours(im,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)  
  contours=sorted(contours,key=cv2.contourArea,reverse=True)
  for c in contours:
      p=cv2.arcLength(c,True)
      approx=cv2.approxPolyDP(c,0.02*p,True)
      if len(approx)==4:
          target=approx
          break
  return target





# First Example
print('\033[92m' +  '\033[1m' + 'First Example:' + '\033[0m' +  '\n')
image1 = imread("https://thumbs.dreamstime.com/b/white-paper-black-background-17358038.jpg")
image1 = imread("https://www.britevisualproducts.com/assets/img/products/large/dc-35.large.jpg")
show = cv2.resize(image1,(650,400))

# filter image
im = filter(image1)
# find corners 
corners = getCorners(im)
#corners = np.array([[98,38],[665,35],[102,444],[673,438]])

# get Homogeny matrix
op = computeH(corners,corners)


# warp image function
dst = warpImage(image1,op,np.array([0,800,0,800]))
dst=cv2.warpPerspective(image1,op,(800,800))

print('Corners:\n',corners)
print('\nTransformation Matrix:\n', op)
cv2_imshow(show)
dst = cv2.resize(dst,(500,500))
cv2_imshow(dst)



# Second Example
print('\033[92m' +  '\033[1m' + 'Second Example:' + '\033[0m' +  '\n')
image2 = imread("https://st2.depositphotos.com/1177973/6137/i/950/depositphotos_61370509-stock-photo-paper-sheet-on-table-close.jpg")
show = cv2.resize(image2,(650,400))

# filter image
im2 = filter(image2)
# find corners 
corners = getCorners(im2)
corners = np.array([[132,157],[564,46],[439,621],[940,445]])
print(image2.shape)
# get Homogeny matrix
op = computeH(corners,corners)


# warp image function
dst = warpImage(image2,op,np.array([0,800,0,800]))
dst=cv2.warpPerspective(image2,op,(800,800))

print('Corners:\n',corners)
print('\nTransformation Matrix:\n', op)
cv2_imshow(show)
dst = cv2.resize(dst,(500,500))
cv2_imshow(dst)

