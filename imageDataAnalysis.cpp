

#include "imageDataAnalysis.h"

namespace images{
ImageDataAnalyser::ImageDataAnalyser(){
    std::cout<<"second construct initialised"<<std::endl;
}
ImageDataAnalyser::ImageDataAnalyser(cv::Mat &leftImage, cv::Mat&rightImage){

  extractPixelValuesLeftImage(leftImage);
  extractPixelValuesRightImage(rightImage);
}

ImageDataAnalyser::~ImageDataAnalyser(){

}

cv::Mat ImageDataAnalyser::loadImage(const char* path){
    cv::Mat src = imread(path , cv::IMREAD_GRAYSCALE );
    if(!src.data){
        std::cout<< "image not loaded"<<std::endl;
    }
    return src;
}
void ImageDataAnalyser::extractPixelValuesLeftImage(cv::Mat&img){
        //std::map<std::string, uchar> pixelValues;
        //initMapValues(pixelValues, 256);
        for(int j=0;j<img.rows;j++) {
            for (int i=0;i<img.cols;i++){
                pixelValuesLeftImage[(int)img.at<uchar>(j,i)]++;
                //pixelValues.insert(std::pair<std::string, uchar>(index,img.at<uchar>(i,j)));
            }
        }
}

void ImageDataAnalyser::extractPixelValuesRightImage(cv::Mat&img){
        //std::map<std::string, uchar> pixelValues;

        //initMapValues(pixelValues, 256);
        for(int j=0;j<img.rows;j++) {
            for (int i=0;i<img.cols;i++){
                pixelValuesRightImage[(int)img.at<uchar>(j,i)]++;
                //pixelValues.insert(std::pair<std::string, uchar>(index,img.at<uchar>(i,j)));
            }
        }
}
std::unordered_map<int,int> ImageDataAnalyser::getPixelValuesLeftImage(){

    return pixelValuesLeftImage;
}
std::unordered_map<int, int> ImageDataAnalyser::getPixelValuesRightImage(){

    return pixelValuesRightImage;
}

std::pair<float,float> ImageDataAnalyser::calculateTheMean(std::vector<std::unordered_map<int, int>>&imagePixelValues){
    set perImage, perPixel;
    float sum_img = 0.0, sum_values=0.0;
    std::map<int, images::set> meanPerImage = calculateMeanPerImage(imagePixelValues);
    std::map<int, images::set> meanPerPixelValue = calculateMeanPerPixelValue(imagePixelValues);

    for(int i=0;i<meanPerImage.size();i++){
        perImage = meanPerImage.at(i);
        sum_img += perImage.mean;
    }
    float mean_img = sum_img/meanPerImage.size();
    for(int j=0;j<meanPerPixelValue.size();j++){
        perPixel = meanPerPixelValue.at(j);
        sum_values += perPixel.mean;
    }
    float meanPerValues = sum_values/meanPerPixelValue.size();

    return std::make_pair(mean_img, meanPerValues);
}
std::map<int, images::set> ImageDataAnalyser::calculateMeanPerImage(std::vector<std::unordered_map<int, int>>&imagePixelValues){
    if(imagePixelValues.size() == 0){
        std::cout<< "the vector with unordered maps is empty, please fill it up"<< std::endl;
    }
    std::map<int,int> img;
    initIMG(img, imagePixelValues);
    std::map<int,images::set>valuesPerImage;

    float value,max_value, min_value, max, min;
    for(int row = 0; row<IMGS.size();++row){
        max_value =0.0;
        min_value=0.0;
        min=0.0;
        max=0.0;
        for(auto & item:IMGS[row]){
            value = (float)item.second;
            if(max <value){
                max_value = (float)item.first;
                max = value;
            }
            if(min>value){
                min_value = (float)item.first;
                min=value;
            }
        }
        valuesPerImage[row].mean=max_value;
        valuesPerImage[row].stdev = min_value;
        //std::cout << "stdev per image-> "<<valuesPerImage[row].stdev<<std::endl;
        valuesPerImage[row].max = max;
        valuesPerImage[row].min = min;

    }
    return valuesPerImage;
}
std::map<int, images::set> ImageDataAnalyser::calculateMeanPerPixelValue(std::vector<std::unordered_map<int,int>>&imagePixelValues){
     if(imagePixelValues.size() == 0){
        std::cout<< "the vector with unordered maps is empty, please fill it up"<< std::endl;
    }
    std::map<int,int> img;
    initIMG(img, imagePixelValues);
    std::map<int, images::set> pixelValues;
    float sum, value, stdev, min, max, mean=0.0;
    for(int col = 0; col<histSize;col++){
        value =0.0;
        sum = 0.0;
        stdev=0.0;
        min = 1.0;
        max =0.0;
        for(int row =0; row<IMGS.size();row++){
            //std::cout<< " values ->" <<(float)IMGS[row].at(col)<<std::endl;
            value = (float)IMGS[row].at(col);
            sum += value;
            if(max < value){
                max = value;
            }
            if(min>value){
                min = value;
            }
        }
        mean = sum/IMGS.size();
        stdev = standardDeviation(IMGS, mean, col);
        pixelValues[col].mean = mean;
        pixelValues[col].stdev = stdev;
        pixelValues[col].max = max;
        pixelValues[col].min = min;

    }
    return pixelValues;

}

float ImageDataAnalyser::standardDeviation(std::vector<std::map<int,int>> &data, float &mean, int &col){
    float stdev =0.0;
    for(int i =0; i<data.size();i++){
        stdev+=std::pow(((float)data[i].at(col)-mean), 2);
        //std::cout << "stdev-> "<<stdev<<std::endl;
    }
    return std::sqrt(stdev/data.size());
}
float ImageDataAnalyser::standardDeviationPerImage(std::map<int,int> &data, float &mean){
    float stdev =0.0;
    for(auto &item:data){
        stdev+=std::pow(((float)item.second-mean), 2);
        //std::cout << "stdev per image-> "<<stdev<<std::endl;
    }
    return std::sqrt(stdev/data.size());
}
void ImageDataAnalyser::testprint(std::vector<std::unordered_map<int, int>>imagePixelValues){
    std::map<int,int> img;
    initIMG(img, imagePixelValues);
    for(int k = 0; k<IMGS.size();k++){
        for(auto&item:IMGS[k]){
        //std::cout << " key ->" << item.first<< " values ->" << item.second << std::endl;
        }
    }
}
void ImageDataAnalyser::initIMG(std::map<int,int> &img,std::vector<std::unordered_map<int,int>>&imagePixelValues){
    IMGS.clear();
     for(int k = 0; k < imagePixelValues.size();++k){
        img.clear();
        initMapValues(img, this->histSize);
        transferPixelValues(img, imagePixelValues[k]);
        IMGS.push_back(img);

    }
}
void ImageDataAnalyser::transferPixelValues(std::map<int, int> &img, std::unordered_map<int,int> &imgPixel){
        //img -> 255 mit NULLs
        std::map<int,int>::iterator i;
        int res =0;
           for(i =img.begin(); i!=img.end();i++){
                for(auto&iter:imgPixel){
                    res = i->first;;
                    if(res==iter.first){
                        i->second=iter.second;
                        //std::cout<< "rest -> "<< i->first<< " second -> "<< i->second<<std::endl;
                }
            }

        }
   }
void ImageDataAnalyser::initMapValues(std::map<int, int> &maps, size_t size){
    for(int i =0;i<size;i++){
        maps.insert(std::pair<int,int>(i, 0));
    }
}
void ImageDataAnalyser::histogram(cv::Mat&img, int hist_w, int hist_h ){

     if( img.empty() )
    {
        std::cout<<" image is empty"<<std::endl;
    }

    float range[] = { 0, 256 };
    const float * histRange[] = {range};
    // create a new image to display the histogram


    if(img.channels() ==3){
        std::cout<<img.channels()<<std::endl;

        cv::split( img, channels);

        cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
        int bin_w = cvRound( (double) hist_w/this->histSize );


        cv::calcHist( &channels[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
        cv::calcHist( &channels[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
        cv::calcHist( &channels[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );

        // normalize the histogram

        cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
       for( int i = 1; i < histSize; i++ ){
            cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ), cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
            cv::Scalar( 255, 0, 0), 2, 8, 0  );
            cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ), cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
            cv::Scalar( 0, 255, 0), 2, 8, 0  );
            cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ), cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
            cv::Scalar( 0, 0, 255), 2, 8, 0  );
       }
        cv::imshow("Source image", img );
        cv::imshow("calcHist Demo", histImage );
        cv::waitKey();


    }else if(img.channels()<3){

        //std::map<std::string,uchar> values = extractPixelValues(img, std::to_string(1));
        //std::map<std::string,uchar>::iterator itr;
        extractPixelValuesLeftImage(img);
        for(auto it:this->pixelValuesLeftImage){
            std::cout << " pixel value -> "<< (int) it.first<< " frequency -> "<< (int) it.second<<std::endl;
        }
        std::cout<<img.channels()<<std::endl;
        cv::Mat histImage( hist_h, hist_w, CV_8UC1, cv::Scalar( 0,0,0) );
        int bin_w = cvRound( (double) hist_w/this->histSize );
        cv::calcHist( &img, 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
        cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        for( int i = 1; i < histSize; i++ ){
            cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ), cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
            cv::Scalar( 255, 0, 0), 2, 8, 0  );
        }
        cv::imshow("Source image", img );
        cv::imshow("calcHist Demo", histImage );
        cv::waitKey();
    }
}
}

int main(){
    const char * path = "/home/cristian/PycharmProjects/tEDRAM/tEDRAM2/training_data/binocular_imgs/binocular_img_left1.png";
    const char * path_r = "/home/cristian/PycharmProjects/tEDRAM/tEDRAM2/training_data/binocular_imgs/binocular_img_right1.png";
    cv::Mat img_l = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::Mat img_r = cv::imread(path_r, cv::IMREAD_GRAYSCALE);

    cv::Mat img1 = cv::Mat::zeros(cv::Size(120,160),CV_8UC1);
    cv::Mat img2 = cv::Mat::zeros(cv::Size(120,160), CV_8UC1);

    std::vector<std::unordered_map<int,int>> test;
    images::ImageDataAnalyser analyser(img_l, img_r);
    test.push_back(analyser.getPixelValuesLeftImage());
    images::ImageDataAnalyser manager;
    //manager.testprint(test);
    std::map<int,images::set> means = manager.calculateMeanPerPixelValue(test);
    std::map<int, images::set> meanPerImage = manager.calculateMeanPerImage(test);
    /*
    images::set tmp, tmp2;
    for(int i = 0; i<means.size();i++){
        tmp = means[i];
        std::cout<<"mean -> "<< tmp.mean << " stdev -> " << tmp.stdev<< " min -> "<< tmp.min<< " max -> "<<tmp.max<<std::endl;

    }

    for(int j = 0; j<meanPerImage.size();j++){
        tmp2 = meanPerImage[j];
        std::cout<<"mean img -> "<< tmp2.mean << " stdev img -> " << tmp2.stdev<< " min img -> "<< tmp2.min<< " max img -> "<<tmp2.max<<std::endl;

    }*/
}

