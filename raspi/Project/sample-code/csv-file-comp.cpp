#include <stdio.h>
#include <string.h>
#include <errno.h>
#include<iostream>
#include <wiringSerial.h>
#include<bits/stdc++.h> 

using namespace std;

int main (int argc, char **argv)
{

    fstream fin1; 
    fin1.open("lidar-data.csv", ios::in);

    fstream fin2; 
    fin2.open("lidar-data-0-1.csv", ios::in); 
  
    vector<string> row1; 
    vector<string> row2; 
    string word1, temp1, word2, temp2; 
    double sum=0;
  
    while (fin1 >> temp1) { 
  
        row1.clear();  

	while (getline(fin1, word1, '\n')) { 
		row1.push_back(word1.substr(word1.find(",") + 1)); 
	}  
    } 

    while (fin2 >> temp2) { 
  
        row2.clear();  

	while (getline(fin2, word2, '\n')) { 
		row2.push_back(word2.substr(word2.find(",") + 1)); 
	}  
    } 

    for(int i=1; i<row2.size();i++) {
	stringstream sFirst(row1[i]); 
	int firstData = 0; 
	sFirst >> firstData; 
	stringstream sSecond(row2[i]); 
	int secondData = 0; 
	sSecond >> secondData; 
	if(firstData > 1000 || secondData > 1000)
	  continue;

	cout << "firstData :" << firstData  << " secondData :" << secondData << " pow : " << pow((firstData-secondData), 2) << endl;
	sum = sum + pow((firstData-secondData), 2);
    }
	cout << sum << endl;
	cout << sqrt(sum) << endl;




}
