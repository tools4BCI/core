#ifndef TCTIMESTAMP_CPP 
#define TCTIMESTAMP_CPP 

#include "TCTimestamp.hpp" 
#include <time.h> 
#include <stdio.h> 
#include <string.h> 

TCTimestamp::TCTimestamp(void) {
	this->Unset();
}

TCTimestamp::~TCTimestamp(void) {
}

void TCTimestamp::Unset(void) {
	timerclear(&this->timestamp);
}

bool TCTimestamp::IsSet(void) {
	return(timerisset(&this->timestamp) != 0);
}

void TCTimestamp::Tic(void) {
	gettimeofday(&this->timestamp, NULL);
}

double TCTimestamp::Toc(TCTimeval* timestamp) {
	TCTimeval toc;
	gettimeofday(&toc, NULL);

	return double((toc.tv_sec  - timestamp->tv_sec)*1000000 + 
			toc.tv_usec - timestamp->tv_usec)/1000;
}
		
double TCTimestamp::Toc(void) {
	return this->Toc(&this->timestamp);
}

void TCTimestamp::Get(std::string* timestamp) const{
	char cache[256];
	sprintf(cache, "%ld,%ld", 
			this->timestamp.tv_sec, 
			this->timestamp.tv_usec);

	timestamp->clear();
	timestamp->assign(cache);
}

bool TCTimestamp::Set(const std::string& timestamp) {
	int status = sscanf(timestamp.c_str(), "%ld,%ld", 
			&this->timestamp.tv_sec, 
			&this->timestamp.tv_usec);
	return(status == 2);
}

void TCTimestamp::Set(const TCTimeval* timestamp) {
	this->timestamp.tv_sec = timestamp->tv_sec;
	this->timestamp.tv_usec = timestamp->tv_usec;
}

#endif
