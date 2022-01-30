// C++ Implementation of Quadtree Tree
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h> // time.h file needed to generate random numbers in main() function
using namespace std;
#define NUM_POINTS 1000

// Used to hold details of a point
struct Point
{
	float x;
	float y;
	Point(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
	Point()
	{
		x = 0.0f;
		y = 0.0f;
	}
};

// The objects that we want stored in the quadtree
struct Node
{
	Point pos;
	int index;
	Node(Point _pos, int _index)
	{
		pos = _pos;
		index = _index;
	}
	Node()
	{
		index = -1;
	}
};

// The main quadtree class
class Quadtree
{

	// Contains details of node
	Node *n;

	// Children of this tree
	Quadtree *topLeftTree;
	Quadtree *topRightTree;
	Quadtree *botLeftTree;
	Quadtree *botRightTree;

public:
	// Hold details of the boundary of this node
	Point topLeft;
	Point botRight;
	Quadtree()
	{
		topLeft = Point(0, 0);
		botRight = Point(0, 0);
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
	}
	Quadtree(Point topL, Point botR)
	{
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
		topLeft = topL;
		botRight = botR;
	}
	void insert(Node*);
	void findclosest(Point, float*, int*);
	bool inBoundary(Point);
};

// Insert a node into the quadtree
void Quadtree::insert(Node *node)
{
	if (node == NULL)
		return;

	// Current quad cannot contain it
	if (!inBoundary(node->pos))
		return;

	if (n == NULL)
	{
		n = node;
	}
	else {	

		if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
		{
			// Indicates topLeftTree
			if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
			{
				if (topLeftTree == NULL)
					topLeftTree = new Quadtree(
						Point(topLeft.x, topLeft.y),
						Point((topLeft.x + botRight.x) / 2,
							(topLeft.y + botRight.y) / 2));
				topLeftTree->insert(node);
			}
	
			// Indicates botLeftTree
			else
			{
				if (botLeftTree == NULL)
					botLeftTree = new Quadtree(
						Point(topLeft.x,
							(topLeft.y + botRight.y) / 2),
						Point((topLeft.x + botRight.x) / 2,
							botRight.y));
				botLeftTree->insert(node);
			}
		}
		else
		{
			// Indicates topRightTree
			if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
			{
				if (topRightTree == NULL)
					topRightTree = new Quadtree(
						Point((topLeft.x + botRight.x) / 2,
							topLeft.y),
						Point(botRight.x,
							(topLeft.y + botRight.y) / 2));
				topRightTree->insert(node);
			}
	
			// Indicates botRightTree
			else
			{
				if (botRightTree == NULL)
					botRightTree = new Quadtree(
						Point((topLeft.x + botRight.x) / 2,
							(topLeft.y + botRight.y) / 2),
						Point(botRight.x, botRight.y));
				botRightTree->insert(node);
			}
		}
	}
}
	
// Finds the closest node to the point p in the quadtree
// Returns the closest node's Euclidean distance in bestdist and the index of the
// closest node in bestindex.
// A mostly empty stub is provided below.
void Quadtree::findclosest(Point p, float* bestdist, int* bestindex)
{
	*bestdist = 100000;
	*bestindex = -1;
	float x_distance = 100000;
	float y_distance = 100000;
	// Check if node in quadtree has no children
	if (n == NULL)
	{
		return;
	}
	else
	{
		// Evaluating if Euclidean distance of closest point and point p is less than value of bestdist
		if ( (pow((n->pos.x - p.x), 2.0) + pow((n->pos.y - p.y), 2.0)) <= pow(*bestdist, 2.0))
		{
			// set value of bestdist to the distance between closest point and point p
			*bestdist = sqrt(pow((n->pos.x - p.x), 2.0) + pow((n->pos.y - p.y), 2.0)); 
			*bestindex = n->index;
		}
		if (topLeftTree != NULL) 
		{
			// obtaining the minimum values for horizontal and vertical distances from quadtree boundary to point p
			x_distance = fmin(fabs(p.x - topLeftTree->topLeft.x), fabs(p.x - topLeftTree->botRight.x));
			y_distance = fmin(fabs(p.y - topLeftTree->topLeft.y), fabs(p.y - topLeftTree->botRight.y));
			
			// Evaluate if both distance components are less than bestdit (e.g. at a distance closer to the point being searched)
			if (x_distance <= *bestdist && y_distance <= *bestdist)
			{
				// performs the search for closest point on each subtree (recursive search)
				topLeftTree->findclosest(p, bestdist, bestindex);
			}
		}
		if (topRightTree != NULL)
		{
			x_distance = fmin(fabs(p.x - topRightTree->topLeft.x), fabs(p.x - topRightTree->botRight.x));
			y_distance = fmin(fabs(p.y - topRightTree->topLeft.y), fabs(p.y - topRightTree->botRight.y));
			if (x_distance <= *bestdist && y_distance <= *bestdist)
			{
				topRightTree->findclosest(p, bestdist, bestindex);
			}
		}
		if (botRightTree != NULL)
		{
			x_distance = fmin(fabs(p.x - botRightTree->topLeft.x), fabs(p.x - botRightTree->botRight.x));
			y_distance = fmin(fabs(p.y - botRightTree->topLeft.y), fabs(p.y - botRightTree->botRight.y));
			if (x_distance <= *bestdist && y_distance <= *bestdist)
			{
				botRightTree->findclosest(p, bestdist, bestindex);
			}
		}
		if (botLeftTree != NULL)
		{
			x_distance = fmin(fabs(p.x - botLeftTree->topLeft.x), fabs(p.x - botLeftTree->botRight.x));
			y_distance = fmin(fabs(p.y - botLeftTree->botRight.y), fabs(p.y - botLeftTree->botRight.y));
			if (x_distance <= *bestdist && y_distance <= *bestdist)
			{
				botLeftTree->findclosest(p, bestdist, bestindex);
			}
		}
	}

};

// Check if current quadtree contains the point
bool Quadtree::inBoundary(Point p)
{
	return (p.x >= topLeft.x &&
		p.x <= botRight.x &&
		p.y >= topLeft.y &&
		p.y <= botRight.y);
}

// Driver program
int main()
{
	srand(time(NULL));
	Point p1(0,0);
	Point p2(1000,1000);
	Quadtree Square(p1, p2);
	Point p[NUM_POINTS];

	// Insert randomly generated points into quadtree
	for (int i = 0; i < NUM_POINTS; i++)
	{
		p[i] = Point((float) rand()/RAND_MAX*1000, (float) rand()/RAND_MAX*1000);
		Node* point = new Node(p[i], i);
		Square.insert(point);
	}
	float d = 10000;
	int index = -1;
	Point searchpoint(600,600);

	// Search for closest point to searchpoint in the quadtree with your implementation of the search function
	Square.findclosest(searchpoint,&d, &index);
	float curdist;
	float bestdist = 1000000;
	int bestindex;

	// Find the closest point index and distance by searching through the randomly generated points 
	// sequentially.
	for (int i = 0; i < NUM_POINTS; i++)
	{
		curdist = sqrt((p[i].x-searchpoint.x)*(p[i].x-searchpoint.x)+(p[i].y-searchpoint.y)*(p[i].y-searchpoint.y));
		if (curdist < bestdist)
		{
			bestdist = curdist;
			bestindex = i;
		}
		
	}

	// If your search function worked correctly, then the closest distance and closest point index 
	// should be the same from the search function as from a brute-force search
	if ((fabs(d-bestdist)>0.01) || (index != bestindex))
		cout << "The correct closest point index and distance were not found correctly." << endl;
	else
		cout << "Congratulations! The correct closest point index and distance were found!" << endl;
	return 0;
}
