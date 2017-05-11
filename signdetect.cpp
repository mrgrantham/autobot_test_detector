#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static void help()
{
    cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
            "This classifier can recognize many kinds of rigid objects, once the appropriate classifier is trained.\n"
            "Program has been modified for use with CMPE297-08 GROUP 5 - Fall 2016\n"
            "Usage:\n"
            "./stopdetect [--stop-cascade=<cascade_path> classifier used for detecting stop signs.]\n"
               "   [--cross-cascade=<cascade_path>] classifier used for detecting pedestrian crossing signs.\n"
               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>]\n"
               // "   [--try-flip]\n"
               "   [filename|camera_index]\n\n"
            "see facedetect.cmd for one call:\n"
            "\tUsing OpenCV version " << CV_VERSION << "\n" << endl;
}

void detectAndDraw( Mat& img, CascadeClassifier& stopCascade,
                    CascadeClassifier& crossCascade,
                    double scale);

string stopCascadeName;
string crossCascadeName;

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame, image;
    string inputName;
    bool tryflip;
    CascadeClassifier stopCascade, crossCascade;
    double scale;

    cv::CommandLineParser parser(argc, argv,
        "{help h||}"
        "{stop-cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|}"
        "{cross-cascade|../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}"
        "{scale|1|}"
        "{@filename||}"
    );
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    stopCascadeName = parser.get<string>("stop-cascade");
    crossCascadeName = parser.get<string>("cross-cascade");
    scale = parser.get<double>("scale");
    if (scale < 1)
        scale = 1;
    inputName = parser.get<string>("@filename");
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }
    if ( !crossCascade.load( crossCascadeName ) ) 
    {
        cerr << "ERROR: Could not load crosswalk classifier cascade" << endl;
        help();
        return -1;
    }
    if( !stopCascade.load( stopCascadeName ) )
    {
        cerr << "ERROR: Could not load stop sign classifier cascade" << endl;
        help();
        return -1;
    }
    if( inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1) )
    {
        int c = inputName.empty() ? 0 : inputName[0] - '0';
        if(!capture.open(c))
            cout << "Capture from camera #" <<  c << " didn't work" << endl;
    }

    if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;

        for(;;)
        {
            capture >> frame;
            if( frame.empty() )
                break;

            Mat frame1 = frame.clone();
            detectAndDraw( frame1, stopCascade, crossCascade, scale);

            int c = waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
    }
    return 0;
}

void detectAndDraw( Mat& img, CascadeClassifier& stopCascade,
                    CascadeClassifier& crossCascade,
                    double scale)
{
    double t = 0;
    vector<Rect> crossSigns, stopSigns;
    static Scalar stopColor = Scalar(0, 0, 255);
    static Scalar crossColor = Scalar(0, 255, 255);
    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)getTickCount();
    stopCascade.detectMultiScale( smallImg, stopSigns,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
    crossCascade.detectMultiScale( smallImg, crossSigns,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
    t = (double)getTickCount() - t;
    printf( "detection time = %g ms\n", t*1000/getTickFrequency());

    // mark cross walk signs in the frame
    for ( size_t i = 0; i < crossSigns.size(); i++ )
    {
        Rect r = crossSigns[i];
        Point center;
        int radius;
        Point textOrg;

        textOrg.x = r.x;
        textOrg.y = r.y;
        center.x = cvRound((r.x + r.width*0.5)*scale);
        center.y = cvRound((r.y + r.height*0.5)*scale);
        radius = cvRound((r.width + r.height)*0.25*scale);
        circle( img, center, radius, crossColor, 3, 8, 0 );
        putText( img, "CROSSING SIGN", textOrg, FONT_HERSHEY_SIMPLEX, 1.0, crossColor, 2);
    }
    // mark stop signs in the frame
    for ( size_t i = 0; i < stopSigns.size(); i++ )
    {
        Rect r = stopSigns[i];
        Point center;
        int radius;

        Point textOrg;
        textOrg.x = r.x;
        textOrg.y = r.y;
        center.x = cvRound((r.x + r.width*0.5)*scale);
        center.y = cvRound((r.y + r.height*0.5)*scale);
        radius = cvRound((r.width + r.height)*0.25*scale);
        circle( img, center, radius, stopColor, 3, 8, 0 );
        putText( img, "STOP SIGN", textOrg, FONT_HERSHEY_SIMPLEX, 1.0, stopColor, 2);
    }
    imshow( "result", img );
}