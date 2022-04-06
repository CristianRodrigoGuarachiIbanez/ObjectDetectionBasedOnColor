# distutils: language = c++
from libc.string cimport memcpy
from cython cimport boundscheck, wraparound, cdivision
from libcpp.utility cimport pair
from libs.imageDataAnalyser cimport *
from libs.PyObjectDetector cimport *
from numpy import ascontiguousarray, asarray, uint8
from numpy cimport uint8_t, ndarray

ctypedef enum Object:
    HAND
    ARM
    OBJECT
ctypedef struct Coordinates:
    int tlx
    int tly
    int brx
    int bry
cdef class PyImageAnalyser:
    cdef:
     vector[unordered_map[int , int ]] leftImages;
     vector[unordered_map[int , int ]] rightImages;
     Mat image
     ImageDataAnalyser * _analyser
     int sizeData
     vector[vector[COORDINATES]] leftObjectCoordinates
     vector[vector[COORDINATES]] rightObjectCoordinates

    def __cinit__(self, const char * path, uchar [:,:,:] left_image, uchar [:,:,:] right_image ):
        self.sizeData = 266
        self._analyser = new ImageDataAnalyser()
        self.convertNumpyToMatArrays(left_image, right_image)

    def __deallocate__(self):
        del self._analyser

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef vector[unordered_map[int, int]] getPixelValuesLeftImages(self):
        return self.leftImages

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef vector[unordered_map[int, int]] getPixelValuesRightImages(self):
        return self.rightImages

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef void convertNumpyToMatArrays(self, uchar [:,:,:] &left_image, uchar [:,:,:] &right_image ):
        cdef:
            int i, reserve_1, reserve_2, reserve_3, lowHand, lowObject, highHand, highObject
            Mat left_img, right_img
            ImageDataAnalyser * dataAnalyser
            BlackWhiteObjectDetector * leftObjectDetector
            BlackWhiteObjectDetector * rightObjectDetector
            Object object1, object2
        lowHand = 25
        highHand = 30
        lowObject =30
        highObject= 35
        for i in range(left_image.shape[0]):
            left_img = self.np2Mat2D(left_image[i,:,:])
            right_img = self.np2Mat2D(right_image[i,:,:])

            dataAnalyser = new ImageDataAnalyser( left_img, right_img)
            self.leftImages.push_back(dataAnalyser.getPixelValuesLeftImage())
            self.rightImages.push_back(dataAnalyser.getPixelValuesRightImage())

            leftObjectDetector = new BlackWhiteObjectDetector(left_img, 0)
            object1 = HAND
            leftObjectDetector.findContours(lowHand, highHand, object1 )
            self.leftObjectCoordinates.push_back(leftObjectDetector.getObjectCoordinates())

            rightObjectDetector = new BlackWhiteObjectDetector(right_img, 0)
            object2 = OBJECT
            rightObjectDetector.findContours(lowObject, highObject, object2)
            self.rightObjectCoordinates.push_back(rightObjectDetector.getObjectCoordinates()) # tl() and br()

            del dataAnalyser
            del leftObjectDetector
            del rightObjectDetector

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef inline Mat np2Mat2D(self, uchar[:,:]&image ):
        cdef ndarray[uint8_t, ndim=2, mode ='c'] np_buff = ascontiguousarray(image, dtype=uint8)
        cdef unsigned int* im_buff = <unsigned int*> np_buff.data
        cdef int r = image.shape[0]
        cdef int c = image.shape[1]
        cdef Mat m
        m.create(r, c, CV_8UC1)
        memcpy(m.data, im_buff, r*c)
        return m

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef inline map[int, set] PycalculateMeanPerPixelValue(self, vector[unordered_map[int, int]] imgs):
        return self._analyser.calculateMeanPerPixelValue(imgs)

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef inline map[int,set] PycalculateMeanPerImage(self, vector[unordered_map[int,int]] imgs):
        return self._analyser.calculateMeanPerImage(imgs)

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef void printvector(self):
        cdef int i
        print("size -> ", self.leftImages.size())
        for i in range(self.leftImages.size()):
            print("unordered map {}: left images -> {} right image -> {}".format(i,self.leftImages[i], self.rightImages[i]))

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef inline list getDataFromObjectElements(self, vector[COORDINATES]&objects):
        outputs = []
        cdef:
            int i, tlx, tly, brx, bry
            size_t size
        size = objects.size()
        #print("size of the coordinates-> ", size)
        for i in range(size):
            tlx = objects[i].tlx
            tly = objects[i].tly
            brx = objects[i].brx
            bry = objects[i].bry
            outputs.append((tlx, tly))
            outputs.append((brx, bry))
        #objects.clear()
        return outputs

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef dict getCoordinates(self, bint left ):
        left_coordinate_data ={}
        right_coordinate_data ={}
        cdef:
            int i, j
            size_t left_size, right_size
        if(left is True):
            left_size =  self.leftObjectCoordinates.size()
            #print("size->", left_size)
            if(left_size==0):
                left_coordinate_data[0] = 0
                return left_coordinate_data
            else:
                for i in range(left_size):
                    left_coordinate_data[i] = self.getDataFromObjectElements(self.leftObjectCoordinates[i])
                #self.leftObjectCoordinates.clear()
                return left_coordinate_data
        else:
            right_size = self.rightObjectCoordinates.size()
            #print("size->", left_size)
            if(right_size==0):
                right_coordinate_data[0] = 0
                return right_coordinate_data
            else:
                for j in range(right_size):
                    right_coordinate_data[j] = self.getDataFromObjectElements(self.rightObjectCoordinates[j])
                #self.rightObjectCoordinates.clear()
                return right_coordinate_data

    @boundscheck(True)
    @wraparound(True)
    @cdivision(True)
    cdef list getPixelImageValues(self,bint left):
        image_data = {}
        pixel_data = {}
        cdef:
            int i, j
            size_t image_size, pixel_data_size
            set valuesPerPixel, valuesPerImage
            map[int,set]  DataPerPixel,  imageData
        if(left is True):
            dataPerPixel = self.PycalculateMeanPerPixelValue(self.leftImages)
            imageData = self.PycalculateMeanPerImage(self.leftImages)
            pixel_data_size = dataPerPixel.size()
            image_size = imageData.size()
        else:
            dataPerPixel = self.PycalculateMeanPerPixelValue(self.rightImages)
            imageData = self.PycalculateMeanPerImage(self.rightImages)
            pixel_data_size = dataPerPixel.size()
            image_size = imageData.size()

        dataPixel = []
        for i in range(pixel_data_size):
            valuesPerPixel = dataPerPixel.at(i)
            dataPixel.append(valuesPerPixel.mean)
            dataPixel.append(valuesPerPixel.stdev)
            dataPixel.append(valuesPerPixel.max)
            dataPixel.append(valuesPerPixel.min)
            pixel_data[i] = dataPixel[:]
            dataPixel.clear()
        dataImage = []
        for j in range(image_size):
            valuesPerImage = imageData[j]
            dataImage.append(valuesPerImage.mean)
            dataImage.append(valuesPerImage.stdev)
            dataImage.append(valuesPerImage.max)
            dataImage.append(valuesPerImage.min)
            image_data[j] = dataImage[:]
            dataImage.clear()

        return [image_data, pixel_data]

    def get_pixel_values(self, left =True):
        return self.getPixelImageValues(left)
    def get_coordinates(self, left = True):
        return self.getCoordinates(left)