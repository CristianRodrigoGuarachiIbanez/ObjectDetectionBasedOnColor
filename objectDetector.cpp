#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "objectDetector.h"


namespace img{

    ObjectElements::ObjectElements(){
    }

    ObjectElements::ObjectElements(Object object, cv::Rect rect, int x, int y){
        this->object = object;
        this->rect = rect;
        this->position = cv::Point(x, y);
    }
    // ---------------------------      ----------------------------
    BlackWhiteObjectDetector::BlackWhiteObjectDetector(cv::Mat&img, int lowLimit, int highLimit){;
        this->img=img;
        this->duplicateMat(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
        this->findContours(lowLimit, highLimit, Object::ARM);
    }
    BlackWhiteObjectDetector::BlackWhiteObjectDetector(cv::Mat&image, int binary){
       this->duplicateMat(image);
       this->img = selectImage(image, binary);
    }
    std::vector<COORDINATES> BlackWhiteObjectDetector::getObjectCoordinates(){
        std::vector<COORDINATES> OBJ;
        COORDINATES coordinates;
        size_t size = OBJECTS.size();
        if(size==0){
            coordinates.tlx = 0;
            coordinates.tly = 0;
            coordinates.brx = 0;
            coordinates.bry = 0;
           OBJ.push_back(coordinates);
        }
        for(unsigned int i =0; i<size; i++){
            coordinates.tlx = OBJECTS[i].rect.tl().x;
            coordinates.tly = OBJECTS[i].rect.tl().y;
            coordinates.brx = OBJECTS[i].rect.br().x;
            coordinates.bry = OBJECTS[i].rect.br().y;
            OBJ.push_back(coordinates);
        }
        return OBJ;
    }
    cv::Mat BlackWhiteObjectDetector::selectImage(cv::Mat&target, int binary){
        if(binary!=0){
            cv::Mat binaryMat(target.size(), CV_8UC1);
            cv::Mat inverted_binary;
             //Apply thresholding
            cv::threshold(target, binaryMat, 110, 255, cv::THRESH_BINARY);
            cv::bitwise_not(binaryMat, inverted_binary);
            cv::waitKey(0);
            return inverted_binary;
        }else if(binary ==2){
            cv::cvtColor(target, target, cv::COLOR_BGR2HSV);
            return target;
        }else if(binary==0){
           return target;
        }
    }
    void BlackWhiteObjectDetector::findContours( int lowLimit, int highLimit, Object object){
        this->low = cv::Scalar(lowLimit,lowLimit,lowLimit);
        this->high = cv::Scalar(highLimit,highLimit,highLimit);
        this->getObject(low, high, object);
        this->drawObject(background);
    }
    void BlackWhiteObjectDetector::duplicateMat(cv::Mat&target){
        target.copyTo(this->background);
    }
    void BlackWhiteObjectDetector::getObject(cv::Scalar low, cv::Scalar high, Object object) {
        cv::Mat mask;
        cv::inRange(img, low, high, mask);
        //cv::transform(img, mask, cv::Matx13f(1,1,1));
        //cv::imshow("MASK -> ",mask);
        //cv::imwrite("./bw_img_left.png", mask);
        //cv::waitKey(0);
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for (size_t i = 0; i < contours.size(); i++)
        {
            cv::Rect boundRect = boundingRect(contours[i]);
            //std::cout<< "contours -> " << contours[i]<<" AREA -> "<< boundRect.area() << " WIDTH -> "<< boundRect.width<< " HEIGHT -> "<< boundRect.height <<std::endl;
            if (boundRect.area() > 150 && (boundRect.width < 50 || boundRect.height <50)) {
                this->OBJECTS.emplace_back(object, boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2);
            }
        }
    }
    void BlackWhiteObjectDetector::drawObject(cv::Mat&background) {
        //std::cout<<"SIZE -> "<<object.size()<<std::endl;
        for (size_t i = 0; i < OBJECTS.size(); i++) {
            switch (OBJECTS[i].object) {
            case ARM:
                //std::cout<< "RECT BLACK -> "<< object[i].rect.tl() << " "<< object[i].rect.br() <<std::endl;
                rectangle(background, OBJECTS[i].rect.tl(), OBJECTS[i].rect.br(), CV_RGB(255, 0, 0), 4);
                break;
            case HAND:
                //std::cout<< "RECT WHITE -> "<< object[i].rect.tl()<<std::endl;
                rectangle(background, OBJECTS[i].rect.tl(), OBJECTS[i].rect.br(), CV_RGB(0, 255, 0), 4);
                break;
            case OBJECT:
                rectangle(background, OBJECTS[i].rect.tl(), OBJECTS[i].rect.br(), CV_RGB(0, 255,0), 4);
            }
        }
    }
}