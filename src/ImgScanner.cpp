#include "imgScanner.h"
#include "opencv2\opencv.hpp"

using namespace cv;

ImgScanner::ImgScanner(Mat* frame){
	this->frame = frame;
	cards = vector<Card>();
}

void ImgScanner::detectCards(){

	cards.clear();

	Mat mask = applyMask(*frame);

	vector<vector<Point> > contours = findContours(mask);

	int size = min(NUM_CARDS,(int)contours.size());

	for(int i=0; i<size; i++){
		Card c = Card(frame);

		vector<Point2f> corners = getCorners(contours[i]);
		
		if(corners.size() != 0){
			Point2f center = corners[corners.size()-1];
			corners.pop_back();

			c.setPos(corners);
			c.setCenter(center);
			cards.push_back(c);
		}
	}
	setCardsPos();
}

Mat ImgScanner::applyMask(Mat frame){
	Mat mask;
	cvtColor(frame, mask, COLOR_BGR2GRAY);
	threshold( mask, mask, 120, 255,CV_THRESH_BINARY);
	return mask;
}

bool ImgScanner::greaterArea (vector<Point> i,vector<Point> j) {
	return (contourArea(i)<contourArea(j));
}

vector<vector<Point> > ImgScanner::findContours(Mat mask){
	static vector<vector<Point> > contours;
    static vector<Vec4i> hierarchy;

	cv::findContours( mask, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	sort(contours.begin(),contours.end(),greaterArea);

	hierarchy.clear();
	return contours;
}

bool ImgScanner::sortCorners(vector<Point2f>& corners, Point2f center){
    vector<Point2f> top = vector<Point2f>();
	vector<Point2f> bot = vector<Point2f>();

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
		Point2f temp;
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

vector<Point2f> ImgScanner::getCorners(vector<Point> countour){
	vector<Point2f> approx = vector<Point2f>();
	double d=0;
	do
	{
		d++;
		approxPolyDP(countour,approx,d,true);
	}
	while (approx.size()>4);

	if(approx.size() != 4) return vector<Point2f>();

	cv::Point2f center(0,0);
	for (int i = 0; i < approx.size(); i++)
		center += approx[i];

	center *= (1. / approx.size());

	if(sortCorners(approx, center)){
		approx.push_back(center);
		return approx;
	}
	else
		return vector<Point2f>();
}

void ImgScanner::outlineCards(){
	for(int i=0; i<cards.size(); i++){
		cards.at(i).outline();
	}
}

void ImgScanner::setCardsPos(){
	Point2f north = Point2f(frame->cols/2,0);
	Point2f south = Point2f(frame->cols/2,frame->rows);
	Point2f east = Point2f(frame->cols/2,frame->rows/2);
	Point2f west = Point2f(0,frame->rows/2);

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