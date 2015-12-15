#ifndef CARD_H
#define CARD_H

#include "opencv2/opencv.hpp"

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

using namespace std;

class Card {
	private:
		cv::Mat* frame;
		vector<cv::Point2f> corners;
		cv::Point2f center;
		cv::Mat cardImg;
		int quadrant;	//0-north, 1-south, 2-east, 3-west
		void printQuadrant();
	public:
		Card(cv::Mat* frame);
		void setPos(vector<cv::Point2f> corners);
		vector<cv::Point2f> getCorners();
		void outline();
		void setQuadrant(int q);
		void setCenter(cv::Point2f p);
		cv::Point2f getCenter();
		bool isImgSet();
		cv::Mat getImg();
};

#endif