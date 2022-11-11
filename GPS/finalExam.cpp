#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <iomanip>
using namespace std;

class CMPT135_String
{
private:
	char *buffer; //The dynamic array to store the printable characters and a null terminating character 
public:
	CMPT135_String(); //The buffer is initialized to nullptr value
	CMPT135_String(const char *); //A non-default constructor with a null terminated char array argument
	CMPT135_String(const CMPT135_String &); //Deep copy constructor
	~CMPT135_String();  //Delete any heap memory and assign buffer nullptr value
	CMPT135_String& operator = (const CMPT135_String &); //Memory cleanup and deep copy assignment
	int length() const; //Return the number of printable characters. Return zero if buffer is nullptr
	bool empty() const;  //Return true if length is 0. Otherwise return false
	char& operator [] (const int &) const;  //Assert index and then return the char at the index
	CMPT135_String operator + (const char &) const; //Return *this appended with the argument character
	CMPT135_String& operator += (const char &); //Append the character argument to *this and return it
	bool operator == (const CMPT135_String &) const; //Case sensitive equality comparison
	bool operator != (const CMPT135_String &) const; //Case sensitive not equality comparison
	friend istream& operator >> (istream &, CMPT135_String &); //Implemented for you
	friend ostream& operator << (ostream &, const CMPT135_String &); //Implemented for you
};
bool CMPT135_String::operator != (const CMPT135_String &s) const{
	return !(*this == s);
}
bool CMPT135_String::operator == (const CMPT135_String &s) const {
	int len = this->length();
	if (len != s.length())
		return false;
	else
	{
		for (int i = 0; i < len; i++)
		{
			if (buffer[i] != s[i])
				return false;
		}
		return true;
	}
}
CMPT135_String& CMPT135_String::operator += (const char &s) {
	CMPT135_String A = *this + s;
	this->~CMPT135_String();
	*this = A;
	return *this;
}
CMPT135_String CMPT135_String::operator + (const char &s) const {
	char* temp;
	temp = new char[this->length() + 1];
	for(int i=0; i < this->length(); i++)
		temp[i] = this->buffer[i];
	temp[this->length()] = s;
	temp[this->length()+1] = '\0';
	CMPT135_String A = temp;
	return A;
}
char& CMPT135_String::operator [] (const int &index) const {
	
	assert(index >= 0 && index < this->length());
	return this->buffer[index];
}
bool CMPT135_String::empty() const {
	if (this->length() == 0)
		return true;
	return false;
}

int CMPT135_String::length() const {
	int count = 0;
	if(this->buffer == nullptr)
		return count;
	for(int i=0; this->buffer[i] != '\0'; i++)
		count++;
	return count;
}
CMPT135_String& CMPT135_String::operator = (const CMPT135_String &s) {
	int len = s.length();
	if (len == 0)
		this->buffer = nullptr;

	char* temp = new char[len + 1];
	for (int i = 0; i < len; i++) {
		temp[i] = s.buffer[i];
	}
	temp[len] = '\0';
	this->~CMPT135_String();
	this->buffer = temp;
	
return *this;
}
CMPT135_String::~CMPT135_String() {
	if(this->length() > 0 || this->buffer != nullptr) {
		delete[] this->buffer;
		this->buffer = new char;
		this->buffer = nullptr;
	}
}
CMPT135_String::CMPT135_String(const CMPT135_String &s) { // Deep copy constructor
	int size = s.length();
	char* temp = new char[size + 1];
	int i = 0;
	while(i < size) {
		temp[i] = s.buffer[i];
		i++;
	}
	temp[size] = '\0';
	this->buffer = temp;
}
CMPT135_String::CMPT135_String(const char *s) {
	int count=0;
	if (s==0)
		this->buffer = nullptr;
	else {
		for(int i = 0; s[i] != '\0'; i++)
			count++;
		this->buffer = new char[count+ 1];
		for (int i = 0; i < count; i++)
			buffer[i] = s[i];
		this->buffer[count] = '\0';
	}
}
CMPT135_String::CMPT135_String() {
	this->buffer = nullptr;
}
istream& operator >> (istream &in, CMPT135_String &s)
{
	//This function reads characters input from a keyboard or a file until either a TAB, EOL, or EOF is 
	//reached. The function ignores any leading spaces, TAB, or EOL characters. It is designed 
	//to be able to read a string of characters that may or may not contain spaces without any problem.
	//It is also designed to ignore any trailing spaces.

	//Define some useful constant values
	#define SPACE ' '
	#define TAB '\t'
	#define	EOL	'\n'

	//Delete the old value of s
	s.~CMPT135_String();

	//Skip leading spaces, tabs, and empty lines
	char ch;
	while (!in.eof())
	{
		in.get(ch);
		if (ch == SPACE || ch == TAB || ch == EOL)
			continue;
		break;
	}

	if (in.eof())
		return in;

	//Append ch to s
	if (ch != SPACE && ch != TAB && ch != EOL)
		s += ch;

	//Read characters into s until a TAB or EOL or EOF is reached
	while (!in.eof())
	{
		in.get(ch);
		if (ch == TAB || ch == EOL || in.eof())
			break;
		else
			s += ch;
	}

	//Remove trailing spaces if any
	int trailingSpacesCount = 0;
	for (int i = s.length()-1; i >= 0; i--)
	{
		if (s[i] != SPACE)
			break;
		trailingSpacesCount++;
	}
	if (trailingSpacesCount > 0)
	{
		CMPT135_String temp;
		for (int i = 0; i < s.length()-trailingSpacesCount; i++)
			temp += s[i];
		s = temp;
	}

	return in;
}
ostream& operator << (ostream &out, const CMPT135_String &s) {
	for (int i = 0; i < s.length(); i++)
		out << s[i];
	return out;
}

template <class T>
class SmarterArray
{
private:
	T *A; //The dynamic array to store the elements
	int size; //The number of elements in the array

public:
	//Constructors
	SmarterArray(); //Implemented for you
	SmarterArray(const SmarterArray<T>&); //Copy constructor. Deep copy of the argument

	//Assignment operator
	SmarterArray<T>& operator = (const SmarterArray<T>&); //Memory cleanup and deep copy of the argument

	//Destructor
	~SmarterArray(); //Implemented for you

	//Getters, Setters, operators and other functions
	int getSize() const; //Return the number of elements in the container
	T& operator[](const int&) const; //Assert index and then return the element at the given index
	int find(const T&) const; //Return the index of the first element that is == to the argument. 
								//Return -1 if not found.
	void insert(const int &index, const T&); //Assert index >= 0 && index <= size and then insert the T
										//type argument into the calling object at the index. If index is
										//equal to size, then insert as a last element
	void append(const T&);  //Insert T as a last element
	bool remove(const int&); //If the index is valid, then remove the element at the index argument
							//from the calling object and return true. Otherwise do nothing and return 
							//false. Do not assert the index argument.
	bool operator == (const SmarterArray<T>&) const; //Return true if sizes are equal and elements at the
														//same indexes are equal. Otherwise return false
	
	template <class T1> //Those of you working with xCode, don't use the same template name T. T1 is ok.
	friend ostream& operator << (ostream&, const SmarterArray<T1>&); //Implemented for you
};
template <class T>
bool SmarterArray<T>::operator == (const SmarterArray<T>& B) const{
	if(B.A == nullptr && this->A == nullptr){
		return true;
	}
	if(this->getSize() != B.getSize())
		return false;
	if(this->getSize() == B.getSize()) {
        for (int i = 0; i < this->getSize(); i++){
            if (this->A[i] != B[i])
                return false;
        }
    }
    return true;
}
template <class T>
bool SmarterArray<T>::remove(const int& e){
	int index = e;
	if(index < 0 || index >= this->getSize())
		return false;
    else {
        //First create a temp array whose size is this-size - 1
        T *temp;
        if (this->getSize() > 1)
            temp = new T[this->getSize() - 1];

        //Copy elements of this-A to temp except the element at index
        for (int i = 0; i < index; i++)
            temp[i] = this->A[i];
        for (int i = index+1; i < this->getSize(); i++)
            temp[i-1] = this->A[i];

        //Delete the existing array this->A
		if(this->getSize() > 0)
            delete[] this->A;

        //Make the array this->A to point to temp and decrement the size
        if (this->getSize() > 0)
            this->A = temp;
        else
            this->A = nullptr;
        this->size -= 1;
        return true;  
    }
}
template <class T>
void SmarterArray<T>::append(const T& e) {
	//Create a temp array with size + 1
    T *temp = new T[this->getSize() + 1];

    //Copy elements of this->A to temp
    for (int i = 0; i < this->getSize(); i++) {
        temp[i] = this->A[i];
    }

    //Copy the elements to be appended to temp
    temp[this->getSize()] = e;

    //Delete the existing array this->A
    if (this->getSize() > 0)
        delete[] this->A;

    //Make the array this->A to point to temp and increment the size
	this->A = new T;
	this->A = nullptr;
    this->A = temp;
    this->size += 1;
}

template <class T>
void SmarterArray<T>::insert(const int &index, const T&value){
	assert(index >= 0 && index <= size);
	if (index != this->getSize())
	{
		T *smarr = new T[this->getSize() + 1];
		for(int i=0; i<index; i++)
			smarr[i] = this->A[i];

		smarr[index] = value;

		for(int i=index+1; i<this->getSize() + 1; i++)
			smarr[i] = this->A[i-1];

		if(this->getSize() > 0)
			delete[] this->A;

		this->A = smarr;
		this->size += 1;
	}
	else
		this->append(value);
}
template <class T>
int SmarterArray<T>::find(const T& value) const{
	for (int i = 0; i < this->getSize(); i++)
		if (this->A[i] == value)
			return i;
	return -1;
}
template <class T>
T& SmarterArray<T>::operator[](const int& index) const{
	assert(index >= 0 && index < this->getSize());
	if (this->A == nullptr){
		cout << "A is pointing to a nullptr. Exiting program" << endl;
		abort();
	}
	if (index < 0 || index >= this->getSize()){
        cout << "Error! Index out of bounds. Exiting program..." << endl;
        abort(); // This will crash the program
    }
    return this->A[index];
}
template <class T>
int SmarterArray<T>::getSize() const{
	return this->size;
}
template <class T>
SmarterArray<T>& SmarterArray<T>::operator = (const SmarterArray<T> &s){
	//Check for self assignment
	if (this == &s)
		return *this;
	if (s.A == nullptr){
		this->~SmarterArray();
		return *this;
	}
	//Delete left hand side object memory
	this->~SmarterArray();
	//Copy right hand side into left hand side
	this->size = s.getSize();
	if (this->getSize() > 0){
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); i++){
			this->A[i] = s.A[i]; //Instead of s.A[i] Yonas put s[i] but i disagree, we shall test and see
		}
	}
	return *this;
}
template <class T>
SmarterArray<T>::SmarterArray(const SmarterArray<T>& s) { //Copy constructor
	if(s.A == nullptr){ 
		this->A = nullptr;
		return;
	}
	this->size = s.getSize();
	if (this->getSize() > 0){
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); i++){
			this->A[i] = s[i];
		}
	}
}
template <class T>
SmarterArray<T>::SmarterArray()
{
	this->A = nullptr;
	this->size = 0;
}
template <class T>
SmarterArray<T>::~SmarterArray() {
	if (this->getSize() > 0) {
		delete[] this->A;
		this->A = new T; //Added this myself
		A = nullptr;
		this->size= 0;
	}
}
template <class T1>
ostream& operator << (ostream& out, const SmarterArray<T1>& L)
{
	if (L.getSize() == 0)
		out << "[Empty List]";
	else
	{
		cout << endl;
		for (int i = 0; i < L.getSize()-1; i++)
			out << L[i] << endl;
		out << L[L.getSize()-1] << endl;
	}
	return out;
}

struct Edge
{
	int desVertexIndex; //the index (in the underlying graph) of the destination vertex of this edge
	double cost;//cost of an edge
};

class Vertex
{
private:
	CMPT135_String name; //Name of the city at the vertex
	SmarterArray<Edge> E; //A container to store the edges emanating from this vertex. All the elements of 
	         //E have the same origin vertex which is the *this object. But they have different destination
		     //vertices which are given by the desVertexIndex member variable of each element of the E container

public:
	Vertex(); //Assign name = "N/A" and initialize E to an empty container (default object E)
	Vertex(const CMPT135_String &); //Assign name = the argument and initialize E to an empty container
	CMPT135_String getName() const; //Return the name
	SmarterArray<Edge> getEdgeSet() const; //Return E
	int getEdgeSetSize() const; //Return the size of E
	Edge getEdge(const int & desVertexIndex) const; //Assert an edge whose destination vertex index is 
										        //equal to the argument is found in E. Then return the edge
	double getEdgeCost(const int &desVertexIndex) const; //Assert an edge whose destination vertex index 
					                        //is equal to the argument is found in E. Then return its cost
	void setEdgeCost(const int &desVertexIndex, const double &cost); //Assert an edge whose destination vertex index 
					                        //is equal to the argument is found in E. Then assign its cost the argument
	void appendEdge(const int &desVertexIndex, const double &cost); //Assert there is no element of E 
				//whose destination vertex index and cost are equal to the argument values. Then append
				//a new element whose destination vertex index and cost are initialized with the
				//argument values to E
	friend ostream& operator << (ostream &, const Vertex &); //Implemented for you
};
void Vertex::appendEdge(const int &desVertexIndex, const double &newCost){
	int foo = true;
	for (int i = 0; i < this->E.getSize(); i++) {
		if(this->E[i].cost == newCost && this->E[i].desVertexIndex == desVertexIndex) {
			foo = false; //If the edge is already there then foo = false
		}
	}
	assert(foo!=false); //Assert that foo must be false aka the edge doesn't already exist

	Edge temp;
	temp.cost = newCost;
	temp.desVertexIndex = desVertexIndex;
	this->E.append(temp); //Add the new temp to E
	
}
void Vertex::setEdgeCost(const int &desVertexIndex, const double &newCost){ 
	bool foo = false;
	for (int i = 0; i < this->E.getSize(); i++){
		if (desVertexIndex == E[i].desVertexIndex){
			foo = true;
			this->E[i].cost = newCost; 
		}
	}
}
double Vertex::getEdgeCost(const int &desVertexIndex) const{
	bool foo = false;
	for (int i = 0; i < this->E.getSize(); i++){
		if (desVertexIndex == E[i].desVertexIndex){
			foo = true;
			return this->E[i].cost;
		}
	}
	if(foo == false)
		return -1;
}
Edge Vertex::getEdge(const int & desVertexIndex) const{
	bool foo = false;
	for (int i = 0; i < this->E.getSize(); i++) {
		if (this->E[desVertexIndex].desVertexIndex == this->E[i].desVertexIndex){
			foo = true;
			return E[i];
		}
	}
	assert(!foo);
}
int Vertex::getEdgeSetSize() const{
	int size = this->E.getSize();
	return size;
}
SmarterArray<Edge> Vertex::getEdgeSet() const{
	return this->E;
}
CMPT135_String Vertex::getName() const{
	return this->name;
}
Vertex::Vertex(const CMPT135_String &s){
	this->name = s;
	this->E = SmarterArray<Edge>();
}
Vertex::Vertex(){
	this->name = "N/A";
	this->E = SmarterArray<Edge>();
}

ostream& operator << (ostream &out, const Vertex &vertex)
{
	out << "Name = " << vertex.name << endl;
	out << "Edge Set" << endl;
	for (int i = 0; i < vertex.E.getSize(); i++)
		out << "\t to ---> " << vertex.E[i].desVertexIndex << ", cost = " << vertex.E[i].cost << endl;
	return out;
}

class Graph
{
private:
	SmarterArray<Vertex> V; //A graph is simply a container that contains many vertex objects where each vertex
							//will have all its connectivity information in it.

public:
	Graph();//Construct empty graph (default object V)
	Graph(const char *); //Construct a graph from a text file whose path is given by the argument cstring.
        //The input text file will consist pairs of cities and the cost to go from one to the other as in the 
		//following examples

        //        Vancouver             Port Coquitlam        4.5
        //        Burnaby          Surrey        2.5
        //        UBC					Delta          6.8

        //The pairs of cities and their costs will be separated by one or more SPACE, TAB or EOL characters.
        //It doesn't matter how many spaces, tabs or EOL characters are present. All leading spaces, tabs and 
		//EOL characters will be ignored. Moreover any trailing spaces will be removed.

		//The reading of characters will start at the first non space, tab, or EOL character and it will stop 
		//when either a tab, EOL, or EOF character is read. This means THERE MUST BE AT LEAST ONE TAB OR EOL 
		//character between pairs of city names and cost. Last but not least, there can be as many spaces, TABs,
		//or EOL characters at the end of the file and these do not affect the reading of the input file.
	
		//This means a city name can have a space in it as in "Port Coquitlam" and it will be read correctly.
	
		//Please note that we do not need to worry about all these input format details because our CMPT135_String
		//class is designed to do all the reading details for us as shown in the test program given above.

		//Thus this function should perform the following tasks
		//1.	Construct a non-default file input streaming object fin using the cstring argument file name
		//2.	Assert the file is opened successfully
		//3.	While EOF is not reached do
		//	a.	Read city name. This is the departure city.
		//	b.	Read city name. This is the destination city.
		//	b.	If either the departure city or the destination city is empty CMPT135_String object, then break.
		//	d.	Read the cost.
		//	e.	Append a vertex whose name is the departure city and whose edge set is empty to the graph. 
		//		You must use the appendVertex member function of this class (see below) to append appropriately.
		//	f.	Append a vertex whose name is the destination city and whose edge set is empty to the graph. 
		//		You must use the appendVertex member function of this class to append appropriately.
		//	g.	Append an edge from the departure city to the destination city with a cost read in part (d) above 
		//		to the graph. You must use the appendEdge member function of this class (see below) to append 
		//		appropriately.
		//	h.	Append an edge from the destination city to the departure city with a cost read in part (d) above 
		//		to the graph. You must use the appendEdge member function of this class (see below) to append 
		//		appropriately.
		//4.	Close the input file stream object and you are done.

	SmarterArray<Vertex> getVertexSet() const; //Return V
	int getVertexSetSize() const; //Return the number of elements of V
	Vertex& operator[](const int &index) const; //Assert the index argument and then return the element at index
	int getVertexIndex(const CMPT135_String &) const; //Return the index of an element whose name matches	
													//the argument. If no such element is found, return -1.
													//Assertion should not be performed.
	int getVertexIndex(const Vertex &) const; //Return the index of the element whose name matches the
											//name of the vertex argument. If no such element is found, 
											//return -1. Assertion should not be performed.
	CMPT135_String getRandomVertexName() const; //Pick a vertex at random and return its name
	void appendVertex(const CMPT135_String &); //Append a vertex with the given name and empty E only 
				           //if no vertex with the same name already exists in the graph. If same name 
							//vertex already exists then do nothing and return. Assertion should not be performed.
	void appendVertex(const Vertex &); //Append the argument only if no vertex with the same name already exists 
										//in the graph. If same name vertex already exists then do nothing 
										//and return. Assertion should not be performed.

	void appendEdge(const CMPT135_String &departure, const CMPT135_String &destination, const double &cost); 
//Assert two vertices whose names match the departure and destination arguments exist in the graph.
//If there is already an edge from the departure argument to the destination argument, then
	//Update (modify) its cost to the cost argument.
//Otherwise
	//Append an edge to the vertex whose name matches the departure argument. The destination vertex index of the 
	//edge must be set to the index of the vertex whose name matches destination argument and its cost must be set to
	// the cost argument.
	friend ostream& operator << (ostream &, const Graph &); //Implemented for you
};
void Graph::appendEdge(const CMPT135_String &departure, const CMPT135_String &destination, const double &cost){
	int destinationVertex = getVertexIndex(destination);
	int departureVertex = this->getVertexIndex(departure);
	assert(destinationVertex != -1 && departureVertex != -1);
	
		

		if(V[departureVertex].getEdgeCost(destinationVertex) == -1)
			this->V[departureVertex].appendEdge(destinationVertex, cost);
		else
			this->V[departureVertex].setEdgeCost(destinationVertex, cost);
}
void Graph::appendVertex(const Vertex &s){
	int index = getVertexIndex(s);
	if (index != -1)
		return;
	Vertex temp(s); V.append(temp);
}
void Graph::appendVertex(const CMPT135_String &str){
	int foo = getVertexIndex(str);
	if (foo != -1)
		return;
	Vertex temp;
	temp = str;
	this->V.append(temp);
}
CMPT135_String Graph::getRandomVertexName() const{
	int randy = rand() % this->V.getSize();
	CMPT135_String random;
	random = V[randy].getName(); //This could cause issues because of copy constructor or something
	return random;
}
int Graph::getVertexIndex(const Vertex &V2) const{
	for (int i = 0; i < V.getSize(); i++){
		if(this->V[i].getName() == V2.getName())
			return i;
	} return -1;
}
int Graph::getVertexIndex(const CMPT135_String &string) const{
	for (int i = 0; i < V.getSize(); i++){
		if(this->V[i].getName() == string){
			return i;
		}
	} return -1;
}
Vertex& Graph::operator[](const int &index) const{
	assert(index >= 0);
	assert(index < this->getVertexSetSize());
	return this->V[index]; //Should test this as could fail
}
int Graph::getVertexSetSize() const{
	return this->V.getSize();
}
SmarterArray<Vertex> Graph::getVertexSet() const{
	return this->V;
}
Graph::Graph(const char *file){ 
	ifstream fin(file);
	assert(!(fin.fail())); 
	double cost = 0.0;
	CMPT135_String departure, destination;
	while (!fin.eof()){
		fin >> departure;
		if (departure.empty() == true)  
			break;
		fin >> destination >> cost; 
		
		this->appendVertex(departure); 
		this->appendVertex(destination);
		this->appendEdge(departure, destination, cost);
		this->appendEdge(destination, departure, cost);
	}
fin.close();
}
Graph::Graph(){
	SmarterArray<Vertex> temp;
	V = temp;
}
ostream& operator << (ostream &out, const Graph &g)
{
	const int CITY_NAME_WIDTH = 25;
	out << endl;
	out << "The graph has " << g.getVertexSetSize() << " vertices." << endl;
	out << "These vertices are" << endl;
	for (int i = 0; i < g.getVertexSetSize(); i++)
	{
		Vertex v = g.V[i];
		out << "Vertex at index " << i << " = " << v.getName() << endl;
	}
	out << endl;
	out << "Each vertex together with its edge set looks like as follows" << endl;
	for (int i = 0; i < g.getVertexSetSize(); i++)
	{
		Vertex v = g.V[i];
		out << v << endl;
	}
	out << endl;
	out << "The graph connectivities are as follows..." << endl;
	out.setf(ios::fixed | ios::left);	//Left aligned fixed decimal places formatting
	for (int i = 0; i < g.getVertexSetSize(); i++)
	{
		Vertex depVertex = g[i];
		SmarterArray<Edge> E = depVertex.getEdgeSet();
		for (int j = 0; j < E.getSize(); j++)
		{
			int desVertexIndex = E[j].desVertexIndex;
			Vertex desVertex = g[desVertexIndex];
			out << depVertex.getName() << setw(CITY_NAME_WIDTH - depVertex.getName().length()) << " ";
			out << desVertex.getName() << setw(CITY_NAME_WIDTH - desVertex.getName().length()) << " ";
			out << setprecision(2) << E[j].cost << endl;
		}
	}
	out.unsetf(ios::fixed | ios::left);	//Removing formatting
	cout.precision(0);					//Resetting the decimal places to default
	return out;
}

class Path
{
private:
	SmarterArray<int> P; //The indices (singular index) the vertices along the path
public:
	Path(); //Construct an empty path. Default object P.
	int length() const; //Return the number of vertices along the path (the number of elements of P)
	int find(const int &vertexIndex) const; //Return the index of an element of P such that P[index] == vertexIndex. 
											//If no element satisfies the condition, then return -1
											//Do not perform assertion operation.
	double computePathCost(const Graph &) const; //Compute the sum of the costs of edges along this path 
		//given the underlying Graph argument. Remember that the path object stores only vertex indices. Thus 
		//you need the underlying graph argument to determine the vertex objects in the graph that correspond to  
		//the indices. Then you will be able to find the edges that connect the vertices which will enable you to 
		//get the costs of the edges. The sum of the costs of these edges is returned from this function. If 
		//during the computation of the path cost, you find that there is no any edge in the underlying graph
		//that connects successive elements in P, then it means your path is an invalid path and you
		//need to abort your application.

		//For example, if the Path object contains P = [3, 8, 6, 4, 9] then this function will return the 
		//cost(from vertex whose index in G is 3 to the vertex whose index in G is 8) +
		//cost(from vertex whose index in G is 8 to the vertex whose index in G is 6) +
		//cost(from vertex whose index in G is 6 to the vertex whose index in G is 4) +
		//cost(from vertex whose index in G is 4 to the vertex whose index in G is 9)

	int& operator [](const int &index) const; //Assert index is valid in P and then return P[index]
	void insert(const int &index, const int &vertexIndex); //Assert the condition index >= 0 && 
											//index <= the length and then insert the vertexIndex argument 
											//at the specified index. If index is equal to the length, then 
											//perform append.
	void append(const int &vertexIndex); //Insert the argument as a last element.
	void remove(const int &index); //Assert the index argument and then remove the element P[index]
	SmarterArray<CMPT135_String> getPathNamesList(const Graph &) const; //Implemented for you
	friend ostream& operator << (ostream &, const Path &); //Implemented for you.
}; 
double Path::computePathCost(const Graph & g) const{
	double total = 0;
	for(int i=0; i<P.getSize()-1; i++){
		if(P[i] != -1 && P[i+1] != -1)
			total += g[P[i]].getEdgeCost(P[i+1]);
		else{
			cout << "Path is invalid, need to abort application. Crashing program..." << endl;
			abort();
		}
	}
	return total;
}
int Path::find(const int &vertexIndex) const{
	return this->P.find(vertexIndex);
}
int Path::length() const{
	return this->P.getSize();
}
Path::Path(){
	this->P = SmarterArray<int>();
}
int &Path::operator[] (const int &index) const{
	assert(index >= 0 && index < P.getSize());
	return P[index];
}
void Path::append(const int &vertexIndex){
	P.append(vertexIndex);
}
void Path::insert(const int &index, const int &vertexIndex){
	assert(index >= 0 && index <= this->length());
	if(index == this->length())
		P.append(vertexIndex);
	else
		P.insert(index,vertexIndex);
}
void Path::remove(const int &index){
	assert(index >= 0 && index < P.getSize());
	P.remove(index);
}
SmarterArray<CMPT135_String> Path::getPathNamesList(const Graph &g) const
{
	SmarterArray<CMPT135_String> path;
	for (int i = 0; i < this->length(); i++)
	{
		int vertexIndex = (*this)[i];
		path.append(g[vertexIndex].getName());
	}
	return path;
}
ostream& operator << (ostream &out, const Path &path)
{
	out << "[";
	if (path.length() > 0)
	{
		for (int i = 0; i < path.length()-1; i++)
			out << path[i] << " -> ";
		out << path[path.length()-1];
	}
	out << "]";
	return out;
}
Path computeMinCostPath(const Graph &g, const CMPT135_String &departure, const CMPT135_String &destination, int &pathCount, Path &currentPath){
	currentPath = Path();
	assert(g.getVertexSetSize() > 0);

	int depVertexIndex = g.getVertexIndex(departure);
	int desVertexIndex = g.getVertexIndex(destination);

	assert(depVertexIndex != -1 && desVertexIndex != -1);
	if (depVertexIndex == desVertexIndex){
		Path minCostPath = currentPath;
		minCostPath.append(desVertexIndex);
		cout << "Path found: " << minCostPath << " with cost " << minCostPath.computePathCost(g) << endl;
		return minCostPath;
	}

	else if (currentPath.find(depVertexIndex) == true){
		Path minCostPath = currentPath;
		return minCostPath;
	}
	else{
		Vertex depVertex = g[depVertexIndex];

		SmarterArray<Edge> E = depVertex.getEdgeSet();
		Path minCostPath;
		currentPath.append(depVertexIndex);

		for (int i = 0; i < E.getSize(); i++){
			CMPT135_String nextVertexName = g[i].getName();
			if (currentPath.find(i) !=-1)
				continue;
			Path candidatePath = computeMinCostPath(g, nextVertexName, destination, pathCount, currentPath);
			if (candidatePath.length() == 0)
				continue;
			if (candidatePath[candidatePath.length() - 1] != desVertexIndex)
				continue;
			if (minCostPath.length() == 0){
				minCostPath = candidatePath;
				continue;
			}
			if (minCostPath.computePathCost(g) > candidatePath.computePathCost(g)){
				minCostPath = candidatePath;
				continue;
			}
		}
		currentPath.remove(currentPath.length() - 1);
		return minCostPath;
	}
}
int main() {
	srand(time(0));
	Graph g("ConnectivityMap2.txt");
	cout << "Graph constructed successfully." << endl;
	cout << g << endl;

	CMPT135_String departure, destination;
	int pathCount;

	for (int i = 0; i < 10; i++)
	{
		cout << endl << "Test #" << i+1 << endl;
		departure = g.getRandomVertexName();
		destination = g.getRandomVertexName();
		cout << "Computing shortest path from " << departure << " to " << destination << endl;
		pathCount = 0;
		Path temp; //This will be useful to avoid compiler error when passing default argument to a function by reference
		Path minCostPath = computeMinCostPath(g, departure, destination, pathCount, temp);
		cout << "There were " << pathCount << " possible paths found." << endl;
		cout << "The minimum cost path is: " << minCostPath << " Cost = " << minCostPath.computePathCost(g) << endl;
		cout << "The cities along the minimum cost path are " << endl;
		cout << "[";
		if (minCostPath.length() > 0)
		{
			SmarterArray<CMPT135_String> p = minCostPath.getPathNamesList(g);
			for (int i = 0; i < p.getSize()-1; i++)
				cout << p[i] << " -> ";
			cout << p[p.getSize()-1];
		}
		cout << "]" << endl << endl;
	}

	system("Pause");
	return 0;
}

