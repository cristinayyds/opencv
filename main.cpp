#include  <opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include<algorithm>
#include<math.h>

using namespace std;
using namespace cv;

bool xcmp(Point n1,Point n2)              //前比后小，return true
{
    return n1.x<n2.x;
}

bool ycmp(Point n1,Point n2)
{
    return n1.y<n2.y;
}

int main()
{
    Mat img = imread("examples.png");           //读取模板
    if(img.empty( ))
    {
        cout<<"please ensure the name"<<endl;
        return -1;
    }
resize(img,img,Size(),0.5,0.5);
vector<Mat> templates;             //读取待匹配图像
templates.push_back(imread("number1.png"));
templates.push_back(imread("number2.png"));
templates.push_back(imread("number3.png"));
templates.push_back(imread("number4.png"));
templates.push_back(imread("number5.png"));
templates.push_back(imread("number6.png"));
templates.push_back(imread("number7.png"));

int minSize;
double minValRes=-1;
for(int size=40;size<80;size++)
{
    Mat result;
    Mat temp;
    resize(templates[0],temp,Size(size,size));
    matchTemplate(img,temp,result,TM_SQDIFF_NORMED);
    double minVal,maxVal;
    Point minLoc,maxLoc;
   minMaxLoc(result, &minVal , &maxVal,&minLoc,&maxLoc);
    if(minValRes==-1 || minValRes>minVal)
    {
        minSize=size;
        minValRes=minVal;
    }
}
cout<<"size"<<minSize;
    
    //缩放模板大小

    for(auto p=templates.begin();p!=templates.end();p++)
    {
        resize(*p,*p,Size(minSize,minSize));
    }
    cout<<"(row,col)"<<templates[0].rows<<templates[0].cols<<endl;

    //模板匹配
    vector<Mat>results;
    for(auto p=templates.begin();p!=templates.end();p++)
    {
        Mat Result;
        matchTemplate(img,*p,Result,TM_SQDIFF_NORMED);    //此方法值越小越匹配 
        results.push_back(Result);
   }
//NMS
vector<vector<Point>> final_loc;
for(auto p_results=results.begin();p_results!=results.end();p_results++)
{
    
    Mat result_copy=p_results->clone();
    double thres;
    minMaxLoc(*p_results,&thres);  //minMaxLoc(&scr,&minVal,&maxVal,&minLoc,&maxLoc)
    threshold(*p_results,*p_results,thres*8,0,THRESH_TOZERO_INV);
    vector<Point> transition;
    vector<Point>result_loc;
    findNonZero(*p_results,transition);
    int flag=1;
    for(auto  pt=transition.begin();pt!=transition.end();pt++)
    {
        flag=1;
        for(auto p=result_loc.begin();p!=result_loc.end();p++)
        {
            if(abs(p->x-pt->x)<=minSize/1.5 &&abs(p->y-pt->y)<=minSize/1.5)
            {
                flag=0;
                break;
            }
        }
        if(flag==1)
        {
             result_loc.push_back(*pt);
             circle(result_copy,*pt,5,Scalar(255,255,255),2);
        }
 }
    final_loc.push_back(result_loc);
}
   //得到final_loc中所有点的集合的排列方式
int y_up,y_down,x_left,x_right;
y_up=x_right=0;
y_down=x_left=1000;
for(auto& frame:final_loc)
{
    x_right=max_element(frame.begin(),frame.end(),xcmp)->x >x_right? max_element(frame.begin(),frame.end(),xcmp)->x:x_right;
    y_up=max_element(frame.begin(),frame.end(),ycmp)->y >y_up? max_element(frame.begin(),frame.end(),ycmp)->y:y_up;
    x_left=min_element(frame.begin(),frame.end(),xcmp)->x<x_left? min_element(frame.begin(),frame.end(),xcmp)->x:x_left;
    y_down=min_element(frame.begin(),frame.end(),ycmp)->y<y_down?min_element(frame.begin(),frame.end(),ycmp)->y:y_down;
}
int  xnum,ynum;
xnum=(int)round((x_right-x_left)/(minSize*1.135))+1;
ynum=(int)round((y_up-y_down)/(minSize*1.135))+1;
//建立矩阵储存结果
vector<vector<int>> final_result(ynum,vector<int>(xnum,0))
int id=1;
for(auto&  outer:final_loc)
{
    for(auto& inner:outer)
    {
        final_result[round((inner.y-y_down)/(minSize*1.135))][round((inner.x-x_left)/(minSize*1.135))]=id;
    }
    id++;
}
//展示结果
for(auto& show_row:final_result)
{
    for(auto& show_cols:show_row)
    {
        cout<<show_cols<<" ";
    }
    cout<<endl;

}


   return 0;
}

