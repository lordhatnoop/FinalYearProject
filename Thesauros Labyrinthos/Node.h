#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>

class Node {
public:
	float fCost;
	float gCost;
	float hCost;

	int index;
	int parentIndex;
	
	void score(float parentGScore, int currentX, int currentY, int goalX, int goalY) {
		// Calculate F score
		float dx, dy;
		//multiple heuristics as tetsing which one worked the best and tetsing their impact on game performance
		//Euclidean distance Heuristic 
		gCost = parentGScore + 1.0f; // G is parent's G + 1
		dx = (float)(goalX - currentX);
		dy = (float)(goalY - currentY);
		hCost = sqrt(dx * dx + dy * dy); // H is Euclidean distance to goal
		fCost = gCost + hCost; // F is just G + H
	};

	bool operator<(const Node& other) {
		return fCost < other.fCost;
	};
};