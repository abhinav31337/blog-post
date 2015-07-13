Mat intrinsic(3,3,CV_64FC1);
Mat intrinsicInverse(3,3,CV_64FC1);
Mat homography(3,3,CV_64FC1);
Mat R(3,3,CV_64FC1);
Mat t=Mat(3,3,CV_64FC1);

bool computePose()
  {
         float h10 = homography.at<float>(0,0);
         float h11 = homography.at<float>(1,0);
         float h12 = homography.at<float>(2,0);
         float h20 = homography.at<float>(0,1);
         float h21 = homography.at<float>(1,1);
         float h22 = homography.at<float>(2,1);
         float h30 = homography.at<float>(0,2);
         float h31 = homography.at<float>(1,2);
         float h32 = homography.at<float>(2,2);
         float r10, r11, r12;
         float r20, r21, r22;
         float r30, r31, r32;
         Mat vT(3,3,CV_32FC1);
         //
         float invC0 = intrinsicInverse.at<float>(0,0);
         float invC1 = intrinsicInverse.at<float>(0,1);
         float invC2 = intrinsicInverse.at<float>(0,2);
         float invC3 = intrinsicInverse.at<float>(1,0);
         float invC4 = intrinsicInverse.at<float>(1,1);
         float invC5 = intrinsicInverse.at<float>(1,2);
         float invC6 = intrinsicInverse.at<float>(2,0);
         float invC7 = intrinsicInverse.at<float>(2,1);
         float invC8= intrinsicInverse.at<float>(2,2);
         //
         float  invH10 = invC0*h10 + invC1*h11 + invC2*h12;
         float  invH11  = invC3*h10 + invC4*h11 + invC5*h12;
         float  invH12  = invC6*h10 + invC7*h11 + invC8*h12;

         float lambda = sqrt( invH10 * invH10 + invH11 * invH11 + invH12 * invH12 );

         if (lambda == 0) return false;

         lambda = 1.0 / lambda;
         invC0 *= lambda;
         invC1 *= lambda;
         invC2 *= lambda;
         invC3 *= lambda;
         invC4 *= lambda;
         invC5 *= lambda;
         invC6 *= lambda;
         invC7 *= lambda;
         invC8 *= lambda;

         // Create normalized R1 & R2:
         r10 = invC0*h10 + invC1*h11 + invC2*h12;
         r11 = invC3*h10 + invC4*h11 + invC5*h12;
         r12 = invC6*h10 + invC7*h11 + invC8*h12;
         //
         r20 = invC0*h20 + invC1*h21 + invC2*h22;
         r21 = invC3*h20 + invC4*h21 + invC5*h22;
         r22 = invC6*h20 + invC7*h21 + invC8*h22;

         // Get R3 orthonormal to R1 and R2:
         r30 = r11 * r22 - r12 * r21;
         r31 = r12 * r20 - r10 * r22;
         r32 = r10 * r21 - r11 * r20;

         // Put the rotation column vectors in the rotation matrix:
         R.at<float>(0,0) = r10;
         R.at<float>(0,1) = r20;
         R.at<float>(0,2) = r30;
         R.at<float>(1,0) = r11;
         R.at<float>(1,1) = r21;
         R.at<float>(1,2) = r31;
         R.at<float>(2,0) = r12;
         R.at<float>(2,1) = r22;
         R.at<float>(2,2) = r32;

         // Calculate Translation Vector T:
         t.at<float>(0,0) = invC0*h30 + invC1*h31 + invC2*h32;
         t.at<float>(0,1) = invC3*h30 + invC4*h31 + invC5*h32;
         t.at<float>(0,2) = invC6*h30 + invC7*h31 + invC8*h32;

         return true;
  }

  void setIntrinsicParams(float fx,float fy,float cx,float cy)
  {
         intrinsic.at<float>(0,0) = fx;
         intrinsic.at<float>(0,1) = 0;
         intrinsic.at<float>(0,2) = cx;
         intrinsic.at<float>(1,0) = 0;
         intrinsic.at<float>(1,1) = fy;
         intrinsic.at<float>(1,2) = cy;
         intrinsic.at<float>(2,0) = 0;
         intrinsic.at<float>(2,1) = 0;
         intrinsic.at<float>(2,2) = 1.0;
         //
         // Create inverse calibration matrix:
         float  tau = fx / fy;
         intrinsicInverse.at<float>(0,0) = 1.0 / (tau*fy);
         intrinsicInverse.at<float>(0,1) = 0.0;
         intrinsicInverse.at<float>(0,2) = -cx / (tau*fy);
         intrinsicInverse.at<float>(1,0) = 0.0;
         intrinsicInverse.at<float>(1,1) = 1.0 / fy;
         intrinsicInverse.at<float>(1,2) = -cy / fy;
         intrinsicInverse.at<float>(2,0) = 0.0;
         intrinsicInverse.at<float>(2,1) = 0.0;
         intrinsicInverse.at<float>(2,2) = 1.0;
  }
