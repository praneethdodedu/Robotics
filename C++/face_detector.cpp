//This program will detect faces from the webcam stream
#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<iostream>
#include <std_msgs/String.h>
#include <sstream>

using namespace std;
using namespace cv;

//Face detection function
int detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);
string casecadeName, nestedCascadeName;

int main(int argc, char** argv) {

  ros::init(argc, argv, "face_detector_bot");
  ros::NodeHandle nh;
ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("face_detector_bot", 1000);
ros::Rate loop_rate(10);


  cout << "Started" << endl; 
  VideoCapture capture;
  Mat frame, image;
  CascadeClassifier cascade, nestedCascade;
  double scale = 1;
  nestedCascade.load("./haarcascade_eye_tree_eyeglasses.xml");
  cascade.load("./haarcascade_frontalcatface.xml");
  //capture.open(0);
  capture.open(-1);

if(!nestedCascade.load("./haarcascade_eye_tree_eyeglasses.xml")){
	cout << "haarcascade_eye_tree_eyeglasses Load issue" << endl;
}

if(!cascade.load("./haarcascade_frontalcatface.xml")){
	cout << "haarcascade_frontalcatface.xml Load issue" << endl;
}


  if(capture.isOpened()){
    cout << "Face Detection Started" << endl;
  

  while(1){
    capture >> frame;
    
    if(frame.empty())
      break;
    
    Mat frame1 = frame.clone();
    int detectedfaces = detectAndDraw(frame1, cascade, nestedCascade, scale);
    if(detectedfaces > 0)
    {
   	std_msgs::String msg;
   
       	std::stringstream ss;
       	ss << detectedfaces << " faces detected ";
      	 msg.data = ss.str();
   
      	ROS_INFO("%s", msg.data.c_str());
   
    
      	chatter_pub.publish(msg);
    }

    char c = (char)waitKey(30);
    
    //Press q to exit from window
    if(c == 27 || c == 'q' || c == 'Q')
      break;
    
  
  }
  }
  else 
  cout << "Could not open camera" << endl; 
  
  return 0;
}

int detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale){

int detectedfaces = 0;
  vector<Rect> faces, faces2;
  Mat gray, smallImg, finalImg, finalGray;
  cvtColor(img, gray, COLOR_BGR2GRAY); //Convert to gray scale...
  double fx = 1/scale;

  resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
  equalizeHist(smallImg, smallImg);
  //Detecting faces using cascade classifier
  cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30,30));

detectedfaces = faces.size();
  //Draw circles around faces
  for(size_t i =0 ; i < faces.size() ; i++){
	cout << "detectAndDraw -  draw cicles" << endl;
    Rect r = faces[i];
    Mat smallImgROI;
    vector<Rect> nestedObjects;
    Point center;
    Scalar color = Scalar(255, 0, 0); //Colour for  drawing tool
    int radius;
    
    double aspect_ratio = (double)r.width/r.height;
cout << "detectAndDraw - aspect_ratio" << endl;
    if(0.75 < aspect_ratio && aspect_ratio < 1.3){
      center.x = cvRound((r.x + r.width * 0.5) * scale);
      center.y = cvRound((r.y + r.height * 0.5) * scale);
      radius = cvRound((r.width + r.height) * 0.25 * scale);
      circle(img, center, radius, color, 3, 8, 0);
cout << "detectAndDraw - aspect_ratio if" << endl;
    }
    else {
cout << "detectAndDraw - aspect_ratio else" << endl;
    rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)), cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)), color, 3, 8, 0);    
    }
    cout << "detectAndDraw - aspect_ratio continue" << endl;
    if(nestedCascade.empty())
        continue;
    smallImgROI = smallImg(r);

cout << "detectAndDraw - smallImgROI" << endl;

    //Detection of eyes
    nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

   //Drawing Circles around eyes
   for(size_t j = 0 ; j < nestedObjects.size() ; j++){
    Rect nr = nestedObjects[j];
    center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
    center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
    radius = cvRound((nr.width + nr.height)*0.25*scale);
    circle(img, center, radius, color, 3, 8, 0);
  }    
  
  resize(img, finalImg, Size(), 0.5, 0.5, INTER_LINEAR);
  cvtColor(finalImg, finalGray, COLOR_BGR2GRAY); //Convert to gray scale...
  imshow("Face Detection", finalGray);



}

return detectedfaces;

}
