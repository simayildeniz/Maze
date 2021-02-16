# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include "randGen.h"

using namespace std;

ofstream outFile;
int K,M,N;
int mazeID;
int entryX, entryY, exitX, exitY;

template <class Object>
class Stack
{
public:
	Stack();
	Stack(const Stack & rhs);
	~Stack();

	bool isEmpty() const;
	void makeEmpty(); // for destructor

	void pop();
	void push(const Object & rhs);
	const Object & top() const;
	Object topAndPop( );
	bool inStack(const Object & temp);

	void reverseStack();
	Object FindObject(const Object & obj);

	const Stack & operator= (const Stack & rhs);

private:
	struct ListNode
	{
		Object element;
		ListNode * next;

		ListNode ( const Object & theElement, ListNode * n = NULL)
			:element (theElement), next(n) {}
	};

	ListNode * topOfStack; // list itself is the stack

};

//taken from lecture notes of 2nd week friday lecture recording:
template <class Object>
Stack<Object>::Stack()
{
	topOfStack = NULL;
}

template<class Object>
bool Stack<Object>::isEmpty() const
{
	return topOfStack == NULL;
}

template<class Object>
const Object & Stack<Object>::top() const
{
	if (isEmpty())
	{
		cout << "Stack empty [ top() ] " << endl;
	}
	return topOfStack->element;
}

template<class Object>
void Stack<Object>::pop() 
{
	if (isEmpty())
	{
		cout << "Stack empty [ pop() ]  "<< endl;
	}
	ListNode *oldTop = topOfStack;
	topOfStack = topOfStack->next;
	delete oldTop;
}

template<class Object>
void Stack<Object>::push( const Object & x)
{
	topOfStack = new ListNode( x, topOfStack );
}

// make the stack logically empty
template<class Object>
void Stack<Object>::makeEmpty()
{
	while (!isEmpty())
	{
		pop();
	}
}

template <class Object>
Object Stack<Object>::topAndPop()
{
	Object topItem= top();
	pop();
	return topItem;
}

template <class Object>
bool Stack<Object>::inStack(const Object & temp)
{
	ListNode *iterator = (*this).topOfStack;
	while (iterator != NULL)
	{
		if (iterator->element == temp)
		{
			return true;
		}
		iterator = iterator->next;
	}
	return false;
}

//deep copy 
template<class Object>
const Stack<Object> & Stack<Object>::operator=(const Stack<Object> & rhs )
{
	if (this != &rhs)
	{
		makeEmpty();
		if (rhs.isEmpty())
		{
			return *this;
		}
		ListNode * rptr = rhs.topOfStack;
		ListNode * ptr = new ListNode( rptr->element );
		topOfStack = ptr;

		for (rptr = rptr->next; rptr != NULL; rptr = rptr->next)
		{
			ptr = ptr->next = new ListNode( rptr -> element );
		}
	}
	return *this;
}

//copy constructor
template<class Object>
Stack<Object>::Stack( const Stack<Object> & rhs )
{
	topOfStack = NULL;
	*this = rhs; //deep copy
}

template<class Object>
Stack<Object>::~Stack()
{
	makeEmpty();
}

// reverses the stack for printing operations
template<class Object>
void Stack<Object>::reverseStack()
{
	Stack<Object> temp;
	while (!isEmpty())
	{
		Object tempTop = topAndPop();
		temp.push(tempTop);
	}
	(*this) = temp;
}

//checks whether the cell is inside the stack, hence if it is visited before in the path construction
template<class Object>
Object Stack<Object>::FindObject(const Object & obj)
{
	ListNode * loopVar;
	loopVar = topOfStack;
	while (loopVar != NULL)
	{
		if (loopVar->element == obj)
		{
			return loopVar->element;
		}
		loopVar = loopVar->next;
	}
	//if nothing is returned:
	cout << "Object not in stack!" << endl;
	return obj;
}

struct Cell
{
	int x_coord, y_coord;
	bool rightVisited;    /////all set to true initially. 4 walls.
	bool leftVisited;
	bool upVisited;
	bool downVisited;
	bool visitedInPath;  // set to false initially, can be changed while path construction
	bool gezdimMi;

	/*Cell()
	{
	rightVisited = true;
	leftVisited = true;
	upVisited = true;
	downVisited = true;
	visitedInPath = false;
	}*/

	Cell (){}

	Cell(int x_coord, int y_coord) : x_coord(x_coord) , y_coord(y_coord)
	{
		//cout <<"asd" << endl;
		//cout << x_coord<<endl;
		//cout <<this->x_coord<<endl;
		rightVisited = true;
		leftVisited = true;
		upVisited = true;
		downVisited = true;
		visitedInPath = false;
		gezdimMi = false;

	}

	const Cell & operator=(const Cell & rhs)
	{
		//deep copy
		x_coord = rhs.x_coord;
		y_coord = rhs.y_coord;
		rightVisited = rhs.rightVisited;    
		leftVisited = rhs.leftVisited;
		upVisited = rhs.upVisited;
		downVisited = rhs.downVisited;
		visitedInPath = rhs.visitedInPath;
		return *this;
	}

	bool operator == (const Cell & rhs)
	{
		bool check = ((rhs.x_coord == x_coord) && (rhs.y_coord == y_coord));
		if (check == true)
		{
			return check;
		}
		else
		{
			return (!check);
		}
	}

	void breakWall (Cell & n)
	{
		int colChek = y_coord; 
		int rowChek = x_coord;

		int colBoundary = -1;
		int rowBoundary = -1;

		if ((colChek - n.y_coord) == 1)
		{
			n.rightVisited = false;
			leftVisited = false;
		}

		else if ((colChek - n.y_coord) == colBoundary)
		{
			n.leftVisited = false;
			rightVisited = false;
		}

		else if ((rowChek - n.x_coord) == 1)
		{
			n.upVisited = false;
			downVisited = false;
		}
		else if ((rowChek - n.x_coord) == rowBoundary)
		{
			n.downVisited = false;
			upVisited = false;
		}
	}

};

class Maze
{
private:

	int ID;
	Cell * currentCell;
	Cell * currentInPath;
	vector<Cell* > tumKomsular;

	int totalRow;
	int totalCol;

	vector<vector<Cell>> labirent; //to store what is inside each row of the maze
	//vector<vector<Cell>> labirent(totalRow,vector<Cell> totalCols);

public:
	//constructor:
	Maze(int id, int rowSayisi, int colSayisi);

	// for maze construction:
	Stack<Cell *> labirentGenerator;

	// for path construction:
	Stack<Cell*> pathGenerator;

	//constructs a random maze:
	void constructMaze(const int & row, const int & col);

	//chooses a random integer number which denotes to a specified wall -right,left,up,down- to break; called inside of constructMaze()
	Cell * chooseRandom();

	//called inside of chooseRandom, stores all the possible locations where a wall can be broken
	vector<Cell* > detectNeighbors(int & row, int & col);

	//friend class to display the content
	//It is friend since I want to reach private members of maze class:
	void createTXTpart1(const int & row, const int & col);

	Cell* nereyeGitsem();
	Stack<Cell*> pathDrawer();

	void createTXTpart2(Stack<Cell*> & path);
};

//constructor with parameters:
Maze::Maze(int id,int rowSayisi, int colSayisi)
{
	ID = id;
	totalRow = rowSayisi;
	totalCol = colSayisi;

	currentInPath = nullptr;

	//describe each cell:
	for (int row = 0; row < totalRow; row++)
	{
		vector<Cell>thisRow; //each row will be filled with column values, hence each will have size of totalCol

		for (int column = 0; column < totalCol; column++)
		{
			//create a cell with coordinates row,col:
			//Cell(xcoord,ycoord) constructor written + 
			Cell here(row,column);
			thisRow.push_back(here);
			//cout << "thisRow  " << thisRow[column].y_coord << endl;
			//cout << here.x_coord << " "<<here.y_coord << "labirent ici "<< endl;
		}
		//thisRow is filled, now push this into labirent deposu in order to finish maze creation:
		this->labirent.push_back(thisRow);

		//now, you have to empty the vector of thisRow for other maze constructions:
		for (int i = 0; i < thisRow.size(); i++)
		{
			thisRow.pop_back();
		}

	}
	// as stated in the hw document, current cell is going to be at 0,0:
	//cout << labirent.size()<< endl;
	//cout << labirent[0][0].x_coord << "hello" << labirent[0][0].y_coord << endl << endl <<endl <<endl;
	this->currentCell = &labirent[0][0];

	//this->currentInPath = &labirent[entryX][entryY];
}

Cell * Maze::chooseRandom()
{
	vector<Cell* > currentKomsular;
	currentKomsular = (*this).detectNeighbors((*this).totalRow,(*this).totalCol);

	int howManyKomsu = currentKomsular.size();
	if (howManyKomsu <= 0)
	{
		return nullptr;
	}

	//it is certain that currentKomsular has elements
	RandGen rand;
	int random = rand.RandInt(1,howManyKomsu);

	return currentKomsular[(random - 1)];

}

vector<Cell* > Maze::detectNeighbors( int & row, int & col)
{
	row = (*(*this).currentCell).x_coord; 
	col = (*(*this).currentCell).y_coord; 

	vector<Cell* > Komsular;

	// left
	if (!((*(*this).currentCell).y_coord - 1 == -1))
	{
		if ((*this).labirent[(*(*this).currentCell).x_coord][(*(*this).currentCell).y_coord - 1].visitedInPath == false)
		{
			Komsular.push_back(&(*this).labirent[(*(*this).currentCell).x_coord][(*(*this).currentCell).y_coord - 1]);
		}
	}

	//down
	if (!((*(*this).currentCell).x_coord - 1 == -1))
	{
		if ((*this).labirent[(*(*this).currentCell).x_coord - 1][(*(*this).currentCell).y_coord].visitedInPath == false)
		{
			Komsular.push_back(&(*this).labirent[(*(*this).currentCell).x_coord - 1][(*(*this).currentCell).y_coord]);
		}
	}

	//up
	if (((*(*this).currentCell).x_coord + 1 < M))
	{
		if ((*this).labirent[(*(*this).currentCell).x_coord + 1][(*(*this).currentCell).y_coord].visitedInPath == false)
		{
			Komsular.push_back(&(*this).labirent[(*(*this).currentCell).x_coord + 1][(*(*this).currentCell).y_coord]);
		}
	}

	//right
	if (((*(*this).currentCell).y_coord + 1 < N))
	{
		//error in the below if, vector out of range
		if ((*this).labirent[(*(*this).currentCell).x_coord][(*(*this).currentCell).y_coord + 1].visitedInPath == false)
		{
			(Komsular.push_back(&labirent[(*(*this).currentCell).x_coord][(*(*this).currentCell).y_coord + 1]));
		}
	}

	return Komsular;
}

void Maze::constructMaze(const int & row, const int & col)
{
	//Initially push current:
	(*currentCell).visitedInPath = true;
	labirentGenerator.push(currentCell);

	while (labirentGenerator.isEmpty() == false) // labirent generator is the stack
	{
		Cell * wheretoGO  = chooseRandom();
		if (wheretoGO == nullptr)
		{
			(*this).currentCell = labirentGenerator.top();
			labirentGenerator.pop();
		}
		else if (wheretoGO != nullptr)
		{

			// wheretoGO is visited in the path now:
			(*wheretoGO).visitedInPath = true;
			// push to labirent generator:
			labirentGenerator.push(wheretoGO);

			/*

			///////remove the walls of current cell so that you can move to wheretoGO:
			//right
			if ((currentCell->y_coord) - (wheretoGO->y_coord) < 0)
			{
			wheretoGO->leftVisited = false;
			currentCell->rightVisited = false;
			}
			//left
			else if ((currentCell->y_coord) - (wheretoGO->y_coord) == 1)
			{
			wheretoGO->rightVisited = false;
			currentCell->leftVisited = false;
			}
			//up
			else if ((currentCell->x_coord) - (wheretoGO->x_coord) < 0)
			{
			wheretoGO->downVisited = false;
			currentCell->upVisited = false;
			}
			//down
			else if ((currentCell->x_coord) - (wheretoGO->x_coord) == 1)
			{
			wheretoGO->upVisited = false;
			currentCell->downVisited = false;
			}
			*/
			(*currentCell).breakWall(*wheretoGO);

			//update the current:
			(*this).currentCell = wheretoGO;
		}
	}


	return;
}

void Maze::createTXTpart1(const int & row, const int & col)
{
	//OBJECTION UPDATE: I was creating .txt on top of each other, I fixed this using a static int variable:
	static int idx = 1; outFile.open("maze_" + to_string(idx) + ".txt");  idx ++; outFile << row << " " << col << endl;
	//the column and row indexes will be vice versa !!!
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			outFile << "x="  << (*this).labirent[r][c].y_coord << " y=" << (*this).labirent[r][c].x_coord 
				<< " l=" << (*this).labirent[r][c].leftVisited << " r=" << (*this).labirent[r][c].rightVisited << " u=" << (*this).labirent[r][c].upVisited << " d=" << (*this).labirent[r][c].downVisited 
				<< endl;
		}
	}
	outFile.close();
}

Cell* Maze::nereyeGitsem()
{
	Cell* suraya;

	//up
	if ((*this).currentInPath->upVisited == false)
	{
		if ((*this).labirent[(*this).currentInPath->x_coord + 1][(*this).currentInPath->y_coord].gezdimMi == false)
		{
			suraya = &(*this).labirent[(*this).currentInPath->x_coord + 1][(*this).currentInPath->y_coord];
			return suraya;
		}
		//else // bu else girerse diger komsulara bakamaz
		//{
		//	//(*this).labirent[(*this).currentInPath->x_coord + 1][(*this).currentInPath->y_coord].gezdimMi = true;
		//	suraya = pathGenerator.top();
		//	pathGenerator.pop();
		//	return suraya;
		//}
	}
	//left	
	if ((*this).currentInPath->leftVisited == false)
	{
		if ((*this).labirent[(*this).currentInPath->x_coord ][(*this).currentInPath->y_coord - 1].gezdimMi == false)
		{
			suraya = &(*this).labirent[(*this).currentInPath->x_coord ][(*this).currentInPath->y_coord - 1];
			return suraya;
		}
		//else
		//{
		//	//(*this).labirent[(*this).currentInPath->x_coord ][(*this).currentInPath->y_coord - 1].gezdimMi = true;
		//	suraya = pathGenerator.top();
		//	pathGenerator.pop();
		//	return suraya;
		//}
	}
	//down
	if ((*this).currentInPath->downVisited == false)
	{
		if ((*this).labirent[(*this).currentInPath->x_coord - 1][(*this).currentInPath->y_coord ].gezdimMi == false)
		{
			suraya = &(*this).labirent[(*this).currentInPath->x_coord - 1][(*this).currentInPath->y_coord ];
			return suraya;
		}
		//else
		//{
		//	//(*this).labirent[(*this).currentInPath->x_coord - 1][(*this).currentInPath->y_coord ].gezdimMi = false;
		//	suraya = pathGenerator.top();
		//	pathGenerator.pop();
		//	return suraya;
		//}
	}

	//right
	if ((*this).currentInPath->rightVisited == false)
	{
		if ((*this).labirent[(*this).currentInPath->x_coord ][(*this).currentInPath->y_coord + 1].gezdimMi == false)
		{
			suraya = &(*this).labirent[(*this).currentInPath->x_coord ][(*this).currentInPath->y_coord + 1];
			return suraya;
		}
		//else
		//{
		//	//(*this).labirent[(*this).currentInPath->x_coord ][(*this).currentInPath->y_coord + 1].gezdimMi = true;
		//	suraya = pathGenerator.top();
		//	pathGenerator.pop();
		//	return suraya;
		//}
	}

	
	pathGenerator.pop();

	suraya = pathGenerator.top();
	
	return suraya;


}

Stack<Cell*> Maze::pathDrawer()
{
	(*this).currentInPath = &labirent[entryY][entryX]; 
	(*this).currentInPath->x_coord = entryY; (*this).currentInPath->y_coord = entryX; //OBJECTION UPDATE: I was misplacing these two variables that caused debug error vector out of range, I swapped the values it is okay now.
	(*this).currentInPath->gezdimMi = true;
	(*this).pathGenerator.push(currentInPath);
	
	while ( !(pathGenerator.top() == &labirent[exitY][exitX]) )
	{
		Cell* suraya;
		suraya = (*this).nereyeGitsem();

		if (suraya == nullptr)
		{
			pathGenerator.pop();
			(*this).currentInPath = pathGenerator.top();
		}
		else if (suraya != nullptr )
		{
			
			if (suraya != pathGenerator.top()) { pathGenerator.push(suraya); }
			suraya->gezdimMi = true;
			(*this).currentInPath = suraya;
		}

	}
	createTXTpart2(pathGenerator);
	return pathGenerator;
}

void Maze::createTXTpart2(Stack<Cell*> & path)
{
	//in order to reverse the path:
	Stack<Cell* > temp;
	while (!path.isEmpty())
	{
		temp.push(path.top());
		path.pop();
	}

	ofstream of;
	of.open("maze_" + to_string((*this).ID) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt");
	while (temp.isEmpty() == false)
	{
		string a = to_string(temp.top()->y_coord);
		string b = to_string(temp.top()->x_coord);
		of << "x = " << a << " y= " << b << endl;
		temp.pop();
	}

	of.close();


}

int main()
{
	vector<Maze> mazeDepo; 

	cout << "Enter the number of mazes: ";
	cin >> K;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> M >> N;
	cout << "All mazes are generated. " << endl;
	cout <<endl;

	int row = M, col = N;
	int loopVar = 0;

	while (loopVar < K) //construct maze K many times
	{
		Maze temp((loopVar + 1),M,N); //id will start from 1
		temp.constructMaze(M,N); //Maze constructed

		//create .txt file
		//display the maze: 
		temp.createTXTpart1(M,N);
		mazeDepo.push_back(temp);
		loopVar++;
	}
	//all mazes are generated


	cout << "Enter a maze ID between 1 to "<< K << " inclusive to find a path: ";
	cin >> mazeID;

	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entryX >> entryY;

	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exitX >> exitY;

	Maze mazetoTrace = mazeDepo[mazeID - 1];
	//mazetoTrace.currentInPath->x_coord = entryX;
	Stack< Cell*> fin = mazetoTrace.pathDrawer();

	//system("pause");
	return 0;
}