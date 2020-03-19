// Bin packing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//Solution to knapsack problem
//


#include <random>
#include <time.h>
#include <iomanip>
#include <iostream>
#include "cppitertools/range.hpp"
#include "gsl/span"

using namespace std;
using namespace iter;
using namespace gsl;

struct Object {
	double netProfit;
	unsigned mass;
	double profitMass;
};

struct BackPack {
	unsigned massMax = 0;
	unsigned nObjects = 0;
	Object* content[1024] = {};
};

struct ObjectList {
	unsigned nObjects = 0;
	Object* content[256] = {};
};

void printBackPack(const BackPack& backPack) {
	cout << "NetProfit Mass" << endl;
	for (Object* object : span(backPack.content).subspan(0, backPack.nObjects)) {
		cout << object->netProfit << "$ " << " " << object->mass << "kg " << endl;
	}
}

void initBackPack(BackPack& backPack) {
	cout << "Please enter the maximum number of mass that the backpack can contain : ";
	cin >> backPack.massMax;
	cout << endl;
}

void addToBackPack(Object* object, BackPack& backPack, unsigned nTimes) {
	for (unsigned i = 0; i < (nTimes); i++) {
		backPack.content[backPack.nObjects] = object;
		backPack.nObjects++;
	}
}

void printObjectList(ObjectList& objectList) {
	cout << "Object List" << endl;
	cout << "NetProfit Mass Profit/Mass" << endl;
	for (Object* object : span(objectList.content).subspan(0, objectList.nObjects)) {
		cout << object->netProfit << "$ " << " " << object->mass << "kg " << object->profitMass << "$/kg "<< endl;
	}
}

void menuInitObjectList() {
	cout << "       Initialisation Backpack Menu       " << endl
		 << "___________________________________________\n" << endl
		<< "1. Generate Random object list             " << endl
		<< "2. Create an object list  yourself         " << endl
		<< "___________________________________________\n" << endl
		<< "Please Entrer your command : ";
}

void randomGenObjectList(ObjectList& objectList) {
	cout << "Enter the maximum mass for the random generator : ";
	unsigned massMax;
	cin >> massMax;
	cout << endl;

	cout << "Enter the maximum net profit per object for the random generator : ";
	double netProfitMax;
	cin >> netProfitMax;
	cout << endl;

	cout << "Enter a number of object in object list to generate less than 200 : ";
	unsigned nbObject;
	cin >> nbObject;
	cout << endl;

	for (unsigned i : range(nbObject)) {
		Object* ptrObject = new Object;
		ptrObject->mass = rand() % (massMax)+1;
		ptrObject->netProfit = netProfitMax * double(rand()) / RAND_MAX;
		objectList.content[objectList.nObjects] = ptrObject;
		objectList.nObjects++;
	}
}

void createObjectList(ObjectList& objectList) {
	cout << "Enter the number of object you want in the object list : ";
	unsigned nObject;
	cin >> nObject;
	cout << endl;

	for (unsigned i : range(nObject)) {
		Object* ptrObject = new Object{};
		cout << "Enter the net profit and mass for the object : ";
		cin >> ptrObject->netProfit >> ptrObject->mass;
		cout << endl;
		objectList.content[objectList.nObjects] = ptrObject;
		objectList.nObjects++;
	}
}

ObjectList initObjectList() {
	ObjectList objectList;
	bool entryOkay = true;

	menuInitObjectList();
	char command;
	cin >> command;
	
	switch (command) {
		case '1':
			randomGenObjectList(objectList);
			break;
		case '2':
			createObjectList(objectList);
			break;
		default:
			cout << endl << endl;
			menuInitObjectList();
			char command;
			cin >> command;
	}
	
	for (Object* ptrObject : span(objectList.content).subspan(0, objectList.nObjects)) {
		ptrObject->profitMass = ptrObject->netProfit / ptrObject->mass;
	}
	return objectList;
}

void putInOrderFromMostValue(ObjectList& objectList) {
	for (unsigned i : range(0, int(objectList.nObjects))) {
		for (unsigned j : range(0, int(objectList.nObjects - 1 - i))) {
			if (objectList.content[j]->profitMass < objectList.content[j + 1]->profitMass) {
				Object* a = objectList.content[j];
				Object* b = objectList.content[j + 1];
				objectList.content[j] = b;
				objectList.content[j + 1] = a;
			}
		}
	}
}

void packBackPackForMaxProfit(BackPack& backPack, ObjectList& objectList) {
	putInOrderFromMostValue(objectList);
	unsigned massRemaining = backPack.massMax;
	unsigned nTimes;
	for (unsigned i = 0; massRemaining > 0; i++) {
		nTimes = massRemaining / objectList.content[i]->mass;
		cout << nTimes << "x " << objectList.content[i]->netProfit << " " << objectList.content[i]->mass << endl;
		addToBackPack(objectList.content[i], backPack, nTimes);
		massRemaining -= objectList.content[i]->mass * nTimes;
	}
}

double profitBackPack(const BackPack& backPack) {
	double profit = 0;
	for (Object* object : span(backPack.content).subspan(0, backPack.nObjects)) {
		profit += object->netProfit;
	}
	return profit;
}

int main()
{
	srand(unsigned(time(NULL)));
	BackPack backPack;
	initBackPack(backPack);
	ObjectList objectList = initObjectList();
	
	//List from wikipedia
	/*Object* test1 = new Object{ 4.0, 12, 4.0/12 };
	Object* test2 = new Object{ 2.0, 1, 2.0 };
	Object* test3 = new Object{ 10.0, 4, 2.5 };
	Object* test4 = new Object{ 1.0, 1, 1.0 };
	Object* test5 = new Object{ 2.0, 2, 1.0 };
	ObjectList objectList = { 5, {test1, test2, test3, test4, test5} };*/ //List from wikipedia

	cout << "\n\n";
	
	printObjectList(objectList);
	
	cout << "\n\n";

	packBackPackForMaxProfit(backPack, objectList);
	
	cout << "\n\n";
	printObjectList(objectList);
	cout << "\n\n";

	cout << endl << "The max profit for this backpack is : " << profitBackPack(backPack) << "$" << endl;
	cout << "The backpack contains : " << endl;
	printBackPack(backPack);
	
}


