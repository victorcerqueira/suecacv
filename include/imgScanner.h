#ifndef IMGSCANNER_H
#define IMGSCANNER_H

#define NUM_CARDS 4

#include "card.h"


class ImgScanner {
	private:
		Mat* frame;
		vector<Card> cards;
		static Mat applyMask(Mat frame);
		static vector<vector<Point> > findContours(Mat mask);
		static bool greaterArea (vector<Point> i,vector<Point> j);
		static bool sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center);
		static vector<Point2f> getCorners(vector<Point> countour);
		void setCardsPos();

	public:
		ImgScanner(Mat* frame);
		void detectCards();
		void outlineCards();
};

#endif