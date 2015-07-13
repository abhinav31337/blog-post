 float base_test[4];
 
 float minimum()
  {
        float min=base_test[0];
        for(int i=0;i<NUM;i++)
            if(base_test[i]<min)
                min=base_test[i];
        return min;
  }


float compare(const Mat& image)
  {

        Mat src_base[4], hsv_base[4];
        Mat hsv_test;

     ///****reading different base images for histogram matching****///

        src_base[0] = imread( "n3.png");
        src_base[1] = imread( "n2.png");
        src_base[2] = imread( "n1.png");
        src_base[3] = imread( "n5.png");

     /// Convert to HSV
        for(int i=0;i<NUM;i++)
            cvtColor( src_base[i], hsv_base[i], CV_BGR2HSV );
        cvtColor( image, hsv_test, CV_BGR2HSV );

     /// Using 50 bins for hue and 60 for saturation
        int h_bins = 50; int s_bins = 60;
        int histSize[] = { h_bins, s_bins };

     // hue varies from 0 to 179, saturation from 0 to 255
        float h_ranges[] = { 0, 180 };
        float s_ranges[] = { 0, 256 };

        const float* ranges[] = { h_ranges, s_ranges };

     // Use the o-th and 1-st channels
        int channels[] = { 0, 1 };

     /// Histograms
        MatND hist_base[4];
        MatND hist_test;


     /// Calculate the histograms for the HSV images///
        calcHist( &hsv_test, 1, channels, Mat(), hist_test, 2, histSize, ranges, true, false );
        normalize( hist_test, hist_test, 0, 1, NORM_MINMAX, -1, Mat() );

        for(int i=0;i<NUM;i++)
        {
            calcHist( &hsv_base[i], 1, channels, Mat(), hist_base[i], 2, histSize, ranges, true, false );
            normalize( hist_base[i], hist_base[i], 0, 1, NORM_MINMAX, -1, Mat() );

     /// Apply the histogram comparison methods///
            base_test[i] = compareHist( hist_base[i], hist_test,3 );
        }

        compare_minimum=minimum();
    }
