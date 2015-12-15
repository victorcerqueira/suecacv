#ifndef DECKCARD_H
#define DECKCARD_H

#include "opencv2/opencv.hpp"
#include "Utils.h"

using namespace std;

class DeckCard {
	private:
		int num;
		char suit;
		cv::Mat image;
		bool imageSet;
	public:
		DeckCard(int num, char suit);
		void loadImage(cv::Mat img);
		cv::Mat getImage();
		bool hasImageLoaded();
		string getName();
		string getShortName();
};

#endif