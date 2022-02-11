#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("examples.png");
    Mat template1 = imread("number1.1.png");
    if(img.empty( ) || template1.empty( ))
    {
        cout<<"please ensure the name"<<endl;
        return -1;
    }

    Mat result1;
    matchTemplate(img, template1 ,result1,TM_SQDIFF_NORMED);
   cout<<"result:"<<result1.cols<<","<<result1.rows<<endl;
   cout<<"img:cols,rows"<<img.cols<<","<<img.rows<<endl;
   cout<<"template"<<template1.cols<<","<<template1.rows<<endl;

    double maxVal,minVal;
    Point maxLoc, minLoc;
    minMaxLoc(result1, &minVal , &maxVal,&minLoc,&maxLoc);
    cout<<"最小点"<<"("<<minLoc.x<<","<<minLoc.y<<")"<<endl;
    cout<<"max:"<<maxVal<<"min"<<minVal<<endl;
    rectangle(result1,cv::Rect(minLoc.x,minLoc.y,template1.cols,template1.rows),Scalar(0,0,255),2);
  
     for( int i=minLoc.x;i<minLoc.x+template1.cols;i++)    //去掉第一个匹配的框
    {
       for(int j=minLoc.y;j<minLoc.y+template1.rows;j++)
       {
           result1.at<float>(i,j)=1;
       }
    }

    double maxVal_1,minVal_1;                        //取得最大和最小的点和值
    Point maxLoc_1,minLoc_1;
    minMaxLoc(result1, &minVal_1, &maxVal_1,&minLoc_1,&maxLoc_1);
    cout<<"第二个最小的点"<<"("<<minLoc_1.x<<","<<minLoc_1.y<<")"<<endl;
    cout<<"第二个最小的值"<<minVal_1<<endl;
    rectangle(result1,cv::Rect(minLoc_1.x,minLoc_1.y,template1.cols,template1.rows),Scalar(0,0,255),2);
  
   namedWindow("结果图形",WINDOW_NORMAL);
    namedWindow("结果",WINDOW_NORMAL);
    imshow("结果",img);
    imshow("模板图形",template1);
    imshow("结果图形",result1);
    waitKey(0);
   return 0;
}

