 void findWindow(Mat& image, vector<vector<Point> >& squares )
  {

        squares.clear();

        Mat pyr, timg,image2, gray0(image.size(), CV_8U),gray2;
        Mat blurred;

        ///Blur the image for reducing noise///
        GaussianBlur(image,blurred,Size(9,9),7,7 );

        /// down-scale and upscale the image to filter out more noise///
        pyrDown(blurred, pyr, Size(blurred.cols/2, blurred.rows/2));
        pyrUp(pyr, timg, blurred.size());
        vector<vector<Point> > contours;
        vector< Vec4i > hierarchy;



     /// find contour in every color plane of the image
        for( int c = 0; c < 3; c++ )
         {

                int ch[] = {c, 0};
                ///mixes different channel to get one gray-image///
                mixChannels(&blurred, 1, &gray0, 1, ch, 1);

                ///equilize the intensity distribution to intensify the main object///
                equalizeHist(gray0,gray0);

                ///Detecting Edges using Canny ///
                Canny(gray0, gray1,CANNY_THRESHOLD_1 , CANNY_THRESHOLD_2, 3);

                ///Hough transform the image to extract the desired edges///
                houghlines(0,0);

                /// dilate canny output to remove potential holes between edge segments
                Mat element = getStructuringElement(MORPH_RECT, Size(2,2) , Point(-1,-1));
                dilate(gray1,gray1,element,Point(0,0),1);

                ///finding all contour in the image
                findContours(gray1, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
                vector<Point> approx;

                 /// test each contour
                 for( size_t i = 0; i < contours.size(); i++ )
                 {
                     Mat hull;

                     /// approximate contour with accuracy proportional to the contour perimeter
                     ///Approximating the convex hull of the detected contour
                     convexHull(Mat(contours[i]), hull,false);
                     approxPolyDP(hull, approx, arcLength(Mat(contours[i]), true)*0.02, true);
                     Rect rect=boundingRect(approx);

                     float aspect_ratio=(rect.width/(float)rect.height);
                     float area = fabs(contourArea(Mat(approx)));
                     convexHull(Mat(approx),hull);
                     float hullArea = contourArea(hull);
                     float solidity=area/float(hullArea);

                     if( approx.size() == 4 &&
                         fabs(contourArea(Mat(approx))) >5000  &&  fabs(contourArea(Mat(approx))) < 100000&&
                         isContourConvex(Mat(approx)) && solidity ==1 &&aspect_ratio>=0.8 && aspect_ratio <= 2
                             &&  compare_minimum<0.1)
                     {

                         float maxCosine = 0;
                         int flag=1;
                         for( int j = 2; j < 5; j++ )
                         {
                             // find the maximum cosine of the angle between joint edges
                             float cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                             maxCosine = MAX(maxCosine, cosine);
                             if(cosine<0.1)
                                 flag++;
                         }

                         /// if cosines of all angles are small
                         /// (all angles are ~90 degree) then write quandrange
                            /// vertices to resultant sequence
                         if( (maxCosine < 0.1 && flag==4)  )
                         {
                             squares.push_back(approx);
                             //cout<<"***********************"<<endl;
                         }
                     }

                 }
         }
  }
