#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
//Ball.cpp
#include "objectDetector.h"


cv::Scalar armLow = cv::Scalar(3, 3, 3);
cv::Scalar armHigh = cv::Scalar(12, 12, 12);
int handLow = 25;
int handHigh = 30;
int objectLow = 30;
int objectHigh = 35;


int main(){
     cv::Mat target = cv::imread("/home/cristian//PycharmProjects/tEDRAM/tEDRAM2/training_data/binocular_imgs/binocular_img_left71.png"); //cv::IMREAD_COLOR
     std::vector<img::COORDINATES> objects;
     //Binary image
    img::BlackWhiteObjectDetector BWImage(target, 0);
    //BWImage.findCountours(armLow, armHigh, img::Color::ARM);
    BWImage.findContours(handLow, handHigh, img::Object::HAND);
    BWImage.findContours(objectLow, objectHigh, img::Object::OBJECT);
    cv::imshow("hsv", BWImage.img);
    //cv::imshow("binary", binaryMat);
    //cv::imwrite("./img_test2_r.png", BWImage.background);
    objects = BWImage.getObjectCoordinates();
    std::cout<< objects.size()<<std::endl;
    for(int i =0;i<objects.size();i++){
        //std::cout<< "coordinates -> " << objects[i].rect.tl().x<< " "<< objects[i].rect.tl().y << objects[i].rect.br()<<std::endl;
        std::cout<< objects[i].tlx <<" "<< objects[i].tly <<objects[i].brx <<" "<< objects[i].bry<<std::endl;
    }
    cv::imshow("contours", BWImage.background);
    cv::waitKey(0);
    return 0;
}