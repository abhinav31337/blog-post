 void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {

          cv_bridge::CvImagePtr cv_ptr;
          try
          {
             cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
          }
          catch (cv_bridge::Exception& e)
          {
                 ROS_ERROR("cv_bridge exception: %s", e.what());
                 return;
          }

         // ImageConverter im;
         vector<vector<Point> > squares;
         compare(cv_ptr->image);
         findWindow(cv_ptr->image,squares);
         drawWindow(cv_ptr->image, squares);
        // imshow(wndname,cv_ptr->image);

         // Output modified video stream
         image_pub_.publish(cv_ptr->toImageMsg());

         int c=  waitKey(30);
         if((char) c == 27 )
         {
                 destroyWindow(wndname);
                 return ;
         }
  }
