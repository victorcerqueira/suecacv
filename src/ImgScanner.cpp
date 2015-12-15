#include "imgScanner.h"
#include "opencv2\opencv.hpp"

ImgScanner::ImgScanner(cv::Mat* frame){
	this->frame = frame;
	cards = vector<Card>();
}

Card ImgScanner::detectSingleCard(cv::Mat* f){
	Card c = Card(f);

	cv::Mat mask = applyMask(*f);

	vector<vector<cv::Point> > contours = findContours(mask);

	if(!contours.empty()){

		vector<cv::Point2f> corners = getCorners(contours[0]);
		
		if(corners.size() != 0){
			cv::Point2f center = corners[corners.size()-1];
			corners.pop_back();

			c.setPos(corners);
			c.setCenter(center);
			c.outline();
		}
	}
	return c;
}

void ImgScanner::detectCards(){

	cards.clear();

	cv::Mat mask = applyMask(*frame);

	vector<vector<cv::Point> > contours = findContours(mask);

	int size = min(NUM_CARDS,(int)contours.size());

	for(int i=0; i<size; i++){
		Card c = Card(frame);

		vector<cv::Point2f> corners = getCorners(contours[i]);
		
		if(corners.size() != 0){
			cv::Point2f center = corners[corners.size()-1];
			corners.pop_back();

			c.setPos(corners);
			c.setCenter(center);
			cards.push_back(c);
		}
	}
	setCardsPos();
	
}

cv::Mat ImgScanner::applyMask(cv::Mat frame){
	cv::Mat mask;
	cvtColor(frame, mask, cv::COLOR_BGR2GRAY);
	threshold( mask, mask, 120, 255,CV_THRESH_BINARY);
	return mask;
}

bool ImgScanner::greaterArea (vector<cv::Point> i,vector<cv::Point> j) {
	return (contourArea(i)<contourArea(j));
}

vector<vector<cv::Point> > ImgScanner::findContours(cv::Mat mask){
	static vector<vector<cv::Point> > contours;
    static vector<cv::Vec4i> hierarchy;

	cv::findContours( mask, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	sort(contours.begin(),contours.end(),greaterArea);

	hierarchy.clear();
	return contours;
}

bool ImgScanner::sortCorners(vector<cv::Point2f>& corners, cv::Point2f center){
    vector<cv::Point2f> top = vector<cv::Point2f>();
	vector<cv::Point2f> bot = vector<cv::Point2f>();

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }

	if(top.size()!=2 || bot.size()!=2) return false;

    cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
    cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
    cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

	if(norm(tr-tl) > norm(bl - tl)){ //if the card is horizontal, flip it
		cv::Point2f temp;
		temp = tl;
		tl = tr;
		tr = br;
		br = bl;
		bl = temp;
	}

    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);

	return true;
}

vector<cv::Point2f> ImgScanner::getCorners(vector<cv::Point> countour){
	vector<cv::Point2f> approx = vector<cv::Point2f>();
	double d=0;
	do
	{
		d++;
		approxPolyDP(countour,approx,d,true);
	}
	while (approx.size()>4);

	if(approx.size() != 4) return vector<cv::Point2f>();

	cv::Point2f center(0,0);
	for (int i = 0; i < approx.size(); i++)
		center += approx[i];

	center *= (1. / approx.size());

	if(sortCorners(approx, center)){
		approx.push_back(center);
		return approx;
	}
	else
		return vector<cv::Point2f>();
}

void ImgScanner::outlineCards(){
	for(int i=0; i<cards.size(); i++){
		cards.at(i).outline();
	}
}

void ImgScanner::setCardsPos(){
	cv::Point2f north = cv::Point2f(frame->cols/2,0);
	cv::Point2f south = cv::Point2f(frame->cols/2,frame->rows);
	cv::Point2f east = cv::Point2f(frame->cols/2,frame->rows/2);
	cv::Point2f west = cv::Point2f(0,frame->rows/2);

	if(cards.size()<4) return;

	vector<Card> temp_cards = cards;

	// set north
	int north_pos = 0;
	double distance = norm(temp_cards[0].getCenter() - north);
	for(int i=1; i<cards.size(); i++){
		double temp = norm(temp_cards[i].getCenter() - north);
		if(temp < distance){
			distance = temp;
			north_pos = i;
		}
	}
	cards[north_pos].setQuadrant(NORTH);

	// set south
	int south_pos = -1;
	distance = numeric_limits<double>::max();
	for(int i=0; i<cards.size(); i++){
		double temp = norm(temp_cards[i].getCenter() - south);
		if(i!=north_pos && temp < distance){
			distance = temp;
			south_pos = i;
		}
	}
	cards[south_pos].setQuadrant(SOUTH);

	// set east
	int east_pos = -1;
	distance = numeric_limits<double>::max();
	for(int i=0; i<cards.size(); i++){
		double temp = norm(temp_cards[i].getCenter() - east);
		if(i!=north_pos && i!=south_pos && temp < distance){
			distance = temp;
			east_pos = i;
		}
	}
	cards[east_pos].setQuadrant(EAST);

	// set west
	int west_pos = -1;
	distance = numeric_limits<double>::max();
	for(int i=0; i<cards.size(); i++){
		double temp = norm(temp_cards[i].getCenter() - west);
		if(i!=north_pos && i!=south_pos && i!=east_pos && temp < distance){
			distance = temp;
			west_pos = i;
		}
	}
	cards[west_pos].setQuadrant(WEST);

}