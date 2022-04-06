#distutils: language = c++
from libcpp.unordered_map cimport unordered_map
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp.utility cimport pair
cimport numpy as np
import numpy as np

ctypedef unsigned char uchar


# For cv::Mat usage
cdef extern from "opencv2/core/core.hpp":
  cdef int  CV_WINDOW_AUTOSIZE
  cdef int CV_8UC3
  cdef int CV_8UC1
  cdef int CV_32FC1
  cdef int CV_8U
  cdef int CV_32F

cdef extern from "opencv2/core/core.hpp" namespace "cv":
  cdef cppclass Mat:
    Mat() except +
    void create(int, int, int)
    void* data
    int rows
    int cols
    int channels()
    int depth()
    size_t elemSize()

# For Buffer usage
cdef extern from "Python.h":
    ctypedef struct PyObject
    object PyMemoryView_FromBuffer(Py_buffer *view)
    int PyBuffer_FillInfo(Py_buffer *view, PyObject *obj, void *buf, Py_ssize_t len, int readonly, int infoflags)
    enum:
        PyBUF_FULL_RO



cdef extern from "../imageDataAnalysis.h" namespace "images":
    cdef cppclass set:
        float mean
        float stdev
        float max
        float min
    cdef cppclass ImageDataAnalyser:
        ImageDataAnalyser() except +
        ImageDataAnalyser(Mat&leftImage, Mat&rightImage) except +
        unordered_map[int, int] getPixelValuesLeftImage()
        unordered_map[int, int] getPixelValuesRightImage()

        pair[float, float] calculateTheMean(vector[unordered_map[int, int]]&imagePixelValues);
        map[int, set] calculateMeanPerImage(vector[unordered_map[int, int]]&imagePixelValues);
        map[int, set] calculateMeanPerPixelValue(vector[unordered_map[int, int]]&imagePixelValues);

        #private functions
        void initMapValues(map[int,int] &maps, size_t size)
        void initIMG(map[int,int]&img,vector[unordered_map[int,int]]&imagePixelValues);
        void transferPixelValues(map[int, int] &img, unordered_map[int,int] &imgPixel);
        void extractPixelValuesLeftImage(Mat&img)
        void extractPixelValuesRightImage(Mat&img)

        float standardDeviation(vector[map[int,int]] &data, float &mean, int &col);
        float standardDeviationPerImage(map[int,int] &data, float &mean);


        #class variables
        vector[Mat] channels
        unordered_map[int, int] pixelValuesLeftImage;
        unordered_map[int, int] pixelValuesRightImage;

        #for die Implementierung in Cython dort lassen -> entweder mit linke oder rechter Bilder füllen
        vector[unordered_map[int,int]] imagePixelValues;
        vector[map[int,int]]IMGS;




        Mat b_hist, g_hist, r_hist;
        int histSize
        bint uniform, accumulate