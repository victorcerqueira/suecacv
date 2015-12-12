#ifndef CARD_H
#define CARD_H

#include "opencv2/opencv.hpp"

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

using namespace cv;
using namespace std;

class Card {
	private:
		Mat* frame;
		vector<Point2f> corners;
		Point2f center;
		Mat cardImg;
		int quadrant;	//0-north, 1-south, 2-east, 3-west
		void printQuadrant();
	public:
		Card(Mat* frame);
		void setPos(vector<Point2f> corners);
		vector<Point2f> getCorners();
		void outline();
		void setQuadrant(int q);
		void setCenter(Point2f p);
		Point2f getCenter();
};

#endif