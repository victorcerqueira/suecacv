#include <stdio.h>
#include <stdlib.h>
#include "imgScanner.h"

using namespace std;
using namespace cv;

Mat getContours(Mat src);

void processImage(Mat frame);

int main(int argc, char** argv)
{
    VideoCapture cap;
    if(!cap.open(0))
        return 0;

	Mat frame;
	cap >> frame;
	ImgScanner scanner = ImgScanner(&frame);
    for(;;)
    {
          Mat grey,cont;
          cap >> frame;
          if( frame.empty() ) break; // end of video stream

		  scanner.detectCards();
		  scanner.outlineCards();
		  
          imshow("Color", frame);

          if( waitKey(1) == 27 ) break; // stop capturing by pressing ESC

    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}