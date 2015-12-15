#include "card.h"


Card::Card(cv::Mat* frame){
	this->frame = frame;
	quadrant = -1;
	cardImg = cv::Mat();
}

bool Card::isImgSet(){
	return cardImg.empty();
}

void Card::setPos(vector<cv::Point2f> corners){
	this->corners = corners;

	cardImg = cv::Mat::zeros(300, 220, CV_8UC3);  //TODO default img size, solve this
	vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(cardImg.cols, 0));
	quad_pts.push_back(cv::Point2f(cardImg.cols, cardImg.rows));
	quad_pts.push_back(cv::Point2f(0, cardImg.rows));

	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);

	cv::warpPerspective(*frame, cardImg, transmtx, cardImg.size());
}

vector<cv::Point2f> Card::getCorners(){
	return corners;
}

void Card::outline(){
	cv::Scalar green = cv::Scalar(0,255,0);
	line(*frame,corners[0],corners[1],green,1,8);
	line(*frame,corners[1],corners[2],green,1,8);
	line(*frame,corners[2],corners[3],green,1,8);
	line(*frame,corners[3],corners[0],green,1,8);
	printQuadrant();
}

void Card::setQuadrant(int q){
	quadrant = q;
}

void Card::setCenter(cv::Point2f p){
	center = p;
}

cv::Point2f Card::getCenter(){
	return center;
}

void Card::printQuadrant(){
	if(quadrant < 0) return;
	string name = "";

	if(quadrant == NORTH) name = "NORTH";
	else if (quadrant == SOUTH) name = "SOUTH";
	else if (quadrant == EAST) name = "EAST";
	else if (quadrant == WEST) name = "WEST";

	putText(*frame,name,center,cv::FONT_HERSHEY_SIMPLEX,2,cv::Scalar(255,0,0));
}

cv::Mat Card::getImg(){
	return cardImg;
}