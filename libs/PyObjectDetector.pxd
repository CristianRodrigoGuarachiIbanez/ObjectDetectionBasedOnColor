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

cdef extern from "opencv2/core/types.hpp" namespace "cv":

    cdef cppclass Scalar[T]:
        Scalar() except +
        Scalar(T v0) except +
        Scalar(T v0, T v1, T v2=0, T v3=0) except +
    cdef cppclass Point_[T]:
        Point_()  except +
        Point_(T x, T y)  except +
    ctypedef Point_[int] Point
    cdef cppclass Rect_[T]:
        Rect_() except +
        Rect_(T _x, T _y, T _width, T _height) except +
        Point br
        Point tl
    ctypedef Rect_[int] Rect


cdef extern from "../objectDetector.h" namespace "img":
    cdef cppclass COORDINATES:
        int tlx
        int tly
        int brx
        int bry
    cdef cppclass Object:
        pass
    cdef cppclass ObjectElements:
        Object object
        Point position;
        Rect rect;
        ObjectElements() except +
        ObjectElements(Object object, Rect rect, int x, int y) except +

    cdef cppclass BlackWhiteObjectDetector:
        vector[ObjectElements] OBJECTS;
        Mat background,img;

        BlackWhiteObjectDetector(Mat&image, int binary) except +
        BlackWhiteObjectDetector(Mat&img, int low, int high) except +
        void findContours(int low, int high, Object object);
        vector[COORDINATES] getObjectCoordinates()

        Mat selectImage(Mat&img, int binary);
        void duplicateMat(Mat&target);
        void getObject( Scalar low, Scalar high, Object object);
        void drawObject(Mat&background);