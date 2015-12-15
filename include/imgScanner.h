#ifndef IMGSCANNER_H
#define IMGSCANNER_H

#define NUM_CARDS 4

#include "card.h"

class ImgScanner {
	private:
		cv::Mat* frame;
		vector<Card> cards;
		static cv::Mat applyMask(cv::Mat frame);
		static vector<vector<cv::Point> > findContours(cv::Mat mask);
		static bool greaterArea (vector<cv::Point> i,vector<cv::Point> j);
		static bool sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center);
		static vector<cv::Point2f> getCorners(vector<cv::Point> countour);
		void setCardsPos();

	public:
		ImgScanner(cv::Mat* frame);
		void detectCards();
		void outlineCards();
		static Card detectSingleCard(cv::Mat* f);
};

#endif