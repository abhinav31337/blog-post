# Autonomous Ingress Task
This repository contains all the things needed for the autonomous ingressing task in gazebo simulator enviroment.
Here You can see two scriptes named Window_Ingress.cpp and Navigation.cpp .


***********************************************************************************************************************

####Window_Ingress.cpp : 
This is the Image Processing code in which the ROS image feed of the camera,mounted on the quadrotor in gazebo, is converted to the useful OpenCV Mat image format using *cv_bridge*. ROS passes around images in its own *sensor_msgs/Image message* format, but we will want to use images in conjunction with OpenCV. *cv_bridge* is a ROS library that provides an interface between ROS and OpenCV. CvBridge can be found in the *cv_bridge* package in the *vision_opencv* stack.

 After getting the image feed in openCV format, the various image processing techniqiues are applied to detect the window in the surrounding. Different constraints and restrictions are applied to remove the distorted contours.Various other things are used in order to filter out the false detectives in the surrounding.The following functions are used for their different purposes.


In this code, there is one class ImageConverter is written and its object is created in main().
the differnt functions which are used in this code are described in short below--

* **ImageConverter()**                          : Constructor of the class. It will be called when the object of the image will be formed.Initializes different publishers and subscribers of ROS.Creates the window on start-up.

* **~ImageConverter()**                         : Destructor of the class.Destroys the window on the shutdown.
                 
* **findWindow()**                              : Takes image and the vector of vector of points (vectors of contours) as argument.Image will be the one which would be processed and the vector in which it will save the relavent contours.It detects all the relavent contours ,filter out the false detective and saves contours in a vector squares.
It then calculates the relative angle between the camera plane and reference window plane using homographic estimation and publishes these angles and the distance between the centroid of the contour and the reference center on a ROS_TOPIC
"/chatter_float" which is created in the constructor.

* **DrawWindow()**                              : Takes image on which it will draw the contours and the vector of contours from which most relavent one will be drawn.

* **setIntrinsicParam()**                       :It takes Intrinsic parameter of the camera fx,fy focal lengths and cx,cy camera image center and generates camera Intrinsic and Inverse Intrinsic Matrix.

* **computePose()**                             :Using the Homography and Inverse Intrinsic Matrics it calculates the R,rotation and t,translation matrix.

* **angle()**                                   :finds a cosine of angle between vectors from pt0->pt1 and from pt0->pt2.

* **houghlines()**                              :Find Hough probabilstic transform of the canny output image to filter out the undesired lines. 

* **compare()**                                 :Compare the histograms of the reference image to the current video frame image using histogram comparison and generates Bhattacharyya distance.

* **arrowedLine()**                             : Draw arrowed line from pt1 to pt2.

* **imageCb()**                                 :Callback function of image transport subscriber. Whenever data comes on specified ROS topic for the ROS image it will be called.It converts the ROS image to openCV mat image and call findWindow and drawWindow member functions for processing on that image.

***************************************************************************************************************
####Navigation.cpp : 
This is the navigation script Which subscribes the angles and the *x_difference* on the *chatter_float*  topic and according to these angles it gives the left/right and yaw commands to the quad to navigate it.Once quad reaches the center position at which the *x_difference* will be less than some threshold,it gives commands to move towards the window.

Different functions which are used in this code are described below -

* **chatterFloatCallback()**                   :This is the callback function of the topic *chatter_float*.Whenever data will be published on this topic it will be called.According to the *angle_x* and *x_difference* it tells quad what to do.

* **local_pos()**                              :This is the callback function of the topic  */user_name/iris/vehicle_local_position*.It gives the local position x,y,z of the quad.

* **local_yaw()**                              :This is the callback function of the topic */user_name/iris/vehicle_attitude*
It gives the yaw of the quad.

********************************************************************************************************************
