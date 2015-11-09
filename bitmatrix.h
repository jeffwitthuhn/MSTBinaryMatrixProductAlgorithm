//This is the main class, very long,
//I pretty much put everything into his one class.
//The algorithms that I used were tested for correctness 
//and the test data is valid.
//Even though the code is messy/inefficient
//we counted the operations rather than the time,
//the time can be optimized further. 

//rough draft..

//BITMATRIX class

#ifndef BITMATRIX
#define BITMATRIX
#include <iostream>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>
#include <chrono> 
#include <vector>
#include <cassert>
#include "doublematrix.h"
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/config.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <functional>
#include <fstream>
using namespace std;
using namespace boost;

//GLOBAL adjacency list, I Hope to find
//A better solution soon.
//SEE struct detect_loops
static vector<vector<int>> alist;
//boost/multiarray might increase performance

//GLOBAL for operation count between partitions, dont have time
//for a better solution at the moment,too tired.
static int partitionoperationcount;
//TYPEDEFS TO USE WITH BOOST GRAPH LIBRARY
typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef property<vertex_name_t, int> VertexNameProperty;
typedef adjacency_list<vecS, vecS, undirectedS,VertexNameProperty, EdgeWeightProperty> Graph;
typedef graph_traits<Graph>::vertex_descriptor bVertex;
typedef graph_traits<Graph>::edge_descriptor bEdge;
typedef graph_traits<Graph>::adjacency_iterator Tadjit;
typedef std::pair<Tadjit, Tadjit> viterator;



//SETTING RANDOM SEED FOR RANDOMIZING MATRICIES
unsigned seed=std::chrono::system_clock::now().time_since_epoch().count();
boost::random::mt19937 gen (seed);

//returns a random 0 or 1
//used to randomize 0,1 matrices 
int random01() {

    boost::random::uniform_int_distribution<> dist(0, 1);
 
    return dist(gen);
}
//returns a random int  0 to max
//used to randomize vectors, can switch to double. 
int random0tomax(int max) {

    boost::random::uniform_int_distribution<> dist(0, max);
 
    return dist(gen);
}


class Bitmatrix{
private:


//GRAPH STUFF/declarations 
vector<bVertex> bVertecies;
vector<bEdge> bEdges;
property_map<Graph, edge_weight_t>::type HammingDistance = get(edge_weight, g);
property_map<Graph, vertex_name_t>::type vertexname = get(vertex_name, g);
Graph g,mst;

////
//for partitioning the bitmatrix
vector<Bitmatrix> partitions;

////
//number of row/columns in the bitmatrix `
int rows;
int columns;
//TRUE IF THE MST HAS BEEN CALCULATED, FALSE OTHERWISE
bool mstcalculated; 
bool partitionmstcalculated;

//v is the most important member of this class,
//having a 1 letter name didnt seem so bad at the
//time but now it's really confusing,
//v is the bitmatrix itself, a vector of bitsets

//so bear with this for now and just remember that 
//v is the main member and 
//is a set of bitsets, pretty much a 2d array. 
std::vector<boost::dynamic_bitset<> > v;

//VISITOR CLASS FOR boost Depth first search
//DISCOVER_VERTEX is called at every vertex visited
//NOTE: COULD NOT SAVE ANYTHING INSIDE OF HERE AFTER
// Depth_firsT_Search was called, alterations of 
//the members of the class instance did not take effect.
//probably something conceptual I was missing, to be 
//learned later, but my quick solution to get it working
//was to just make the alist global. 
struct detect_loops : public boost::dfs_visitor<>
{
	
  detect_loops(int rows){
  	alist.resize(rows);
  }
  void discover_vertex(bVertex v, const Graph& g) {
     Tadjit iter,iterend;
     tie(iter, iterend)=adjacent_vertices(v,g);
     for(;iter!=iterend;++iter){
     	alist[v].push_back(*iter);
 

     }
	  return;

  }
  
};
//sadly everything is public
public:
Bitmatrix(){
	rows=0;
	columns=0;
}
//constructor
Bitmatrix(int rows, int columns){
	this->rows=rows;
	this->columns=columns;
	mstcalculated=false;
	partitionmstcalculated=false;

	v.resize(rows);
	for(int i=0;i<rows;i++)
		v[i].resize(columns);

}
//used to reinitialize after constructed
void initialize(int rows, int columns){
	this->rows=rows;
	this->columns=columns;
	this->mstcalculated=false;
	this->partitionmstcalculated=false;

	v.resize(rows);
	for(int i=0; i<rows; i++)
		v[i].resize(columns);

}
//sets all elements to 0
void setzero(){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=1;

	v[1][2]=0;

}
//sets all elements to 1
void setone(){
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=1;
}
//randomizes the 01 matrix according to rate.
//the greater the rate, the more dense with 1's the matrix is. 
void randomizedense(int rate){
	mstcalculated=false;
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++){
			int randomnum=random0tomax(rate);
			if(randomnum)
				v[i][j]=1;
			else 
				v[i][j]=0;
		}

}
//randomizes the 01 matrix according to the rate paramater
//the greater the rate, the more sparse the matrix is. 
void randomizesparse(int rate){
	mstcalculated=false;
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++){
			int randomnum=random0tomax(rate);
			if(!randomnum)
				v[i][j]=1;
			else 
				v[i][j]=0;
		}

}
//sets the specified element
void setelement(int row, int column, int value){
	v[row][column]=value;
	return;
}
//randomizs the 01 matrix, uniform chance of each
void randomize(){
	mstcalculated=false;
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			v[i][j]=random01();

}

//outputs the 01 matrix to cout
void rprint(){
	for(int i=0;i<rows;i++){
		for(int j=0;j<columns;j++)
			cout<<v[i][j];
		cout<<std::endl;
	}
}
//gives the hamming distance of each row to every other row
void printHDgraph(){
	//boost::dynamic_bitset<> temp;
	for(int i=1;i<rows;i++){
		for(int j=0;j<i;j++){
			std::cout<<(v[i]^v[j]).count();			
			//yay this works^^^^^
			//temp=(v[i]^v[j]);
			//std::cout<<temp.count();
		}
		std::cout<<endl;


	}
}
//returns the hamming distance of row i and row j
int getHD(int i, int j){
	//	cout<<"g";

	return ((v[i]^v[j]).count());
}

//returns the specified element value, (row,column)
int getelement(int r, int c){
	return v[r][c];
}
//returns a bitset that is 1 where the two rows differ
boost::dynamic_bitset<> rowXORrow(int index1, int index2){
	return v[index1]^v[index2];
}
//returns the number of row and columns
int getcolumns(){return columns;}
int getrows(){return rows;}

//BOOST STUFF FOR MST
//returns the graph
Graph getGraph(){
	return g;
}
//creates a complete graph that connects all rows to eachother
void fillgraph(){
	bVertecies.resize(rows);
	bEdges.resize(rows*rows);

	//ADD ALL VERTECIES TO THE GRAPH
	for(int i=0;i<rows;i++){
		bVertecies[i]=add_vertex(g);
		vertexname[bVertecies[i]]=i;

	}

	//ADD ALL POSSIBLE EDGES TO THE GRAPH
	int edgecounter=0;
	for(int i=0;i<rows;i++)
		for (int j=i+1;j<rows;j++){
			bEdges[edgecounter]=(add_edge(bVertecies[i],bVertecies[j],g)).first;
			HammingDistance[bEdges[edgecounter]]=getHD(i,j);
			edgecounter++;
	}
	//graph should be filled now with all vertecies
	//and all possible edges in g




}
//calculates the minimum spanning tree using BOOST implementation of
//prims algorithm.
void boostcalcmst(){
	fillgraph();
	std::vector < graph_traits < Graph >::vertex_descriptor > p(num_vertices(g));
	prim_minimum_spanning_tree(g, &p[0]);
	for(int i=0;i<rows;i++){
		bVertecies[i]=add_vertex(mst);
		vertexname[bVertecies[i]]=i;

	}
	for (std::size_t i = 0; i != p.size(); ++i){
		if(p[i]!=i)
			bEdges[i]=(add_edge(bVertecies[p[i]],bVertecies[i],mst)).first;

	}
	mstcalculated=true;

}

//after using the BOOST implementation of prim's algorithm
//I took a shortcut and instead of working with the graph:mst
//I just converted mst to a vector<vector> so I could work with it
//quick in the multiplication

//first call boostcalcmst
//after this is ran, alist will contain the MST
void boostcalcalist(){
	  //alist is global.... so have to maintain it, needs better solution
	  alist.clear();
	  //intializes the visitor 
	  detect_loops vis(rows);
	  //traverses the graph, visiting each vertex and constructs
	  //the MST in alist
	  depth_first_search(mst, visitor(vis));
}


//multiplies the bitmatrix with a doublematrix and outputs 
//the number of oprations to myfile
Doublematrix multmst(Doublematrix d,ostream& myfile){
	//assert(columns==d.getrows());
	int operationcount=0;
	if(!mstcalculated)
		boostcalcmst();
	Doublematrix temp(rows,1);
	//cout<<1;
	double sum=0;
	//traverse the tree DFS or BFS
	//calcfirst row  as rootthen follow tree off
	int start=minpop();
	for(int i=0;i<columns;i++){
		if(v[start][i])
			sum+=d.getelement(i,0);
	}
	temp.setelement(start,0,sum);
	//cout<<2;
//	cout<<"sum of start row is: "<< sum<<endl;
	boost::dynamic_bitset<> marker;
	//cout<<3;
	vector<vector<int>> tempadjlist=alist;
	//cout<<4;
	vector<vector<int>> queues;
	vector<int> workingonqueue;
	
	queues.resize(rows);
	marker.resize(rows);
	//alist[start].back = last element
	//mark last element marker[^]=1 when visited
	//alist[start].pop_back ==delete last element
/*
ALGORITHM

***NOTE: USING THIS AS A STACK, NOT A QUEUE....so replace language accordingly)

CALCULATE START ROW
WORK ON START AND QUEUE ALL VERTECIES THAT ARE USED WITH START IN (THINGS TO WORK ON QUEUE)
MARK START
WHiLE THERE ARE THINGS TO WORK ON:
	DEQUEUE (a) FROM THINGS TO WORK ON
	work on a and queue all unmarked vertecies that are used with a in things to work on.
	mark a
END WHILE
*/
	workingonqueue.push_back(start);
	//cout<<5;
	do{
		int position=workingonqueue.back();
		//cout<<"position is: "<<position<<endl;
		//cout<<"workingonqueue is: "<<workingonqueue.size()<<endl;
		workingonqueue.pop_back();
		//cout<<6;
		//calculate all unmarked adjacent vertecies to position
		//while the temp adjlist is not empty
		boost::dynamic_bitset<> differmatrix;
		differmatrix.resize(columns);
		while(!tempadjlist[position].empty()){
		//if vertecy at the end is not marked
		int last=tempadjlist[position].back();
		//cout<<7;

			if(!marker[last]){
				//   work on the vertecy at the end
					sum=temp.getelement(position,0);
					//find where v[last] and v[position] differ
					differmatrix=v[last]^v[position];
					//<<"differmatrix is: "<<differmatrix<<endl;

					//in the all of the places that they differ:
						//if v[position][i]==1, subtract d[i] from sum
						//if v[position][i]==0, add [i] to sum
						for(int i=0;i<columns;i++)
						if(differmatrix[i]) {//they differ in this spot
								if (v[position][i]){
									//cout<< sum<< " - "<<d.getelement(i,0)<<endl;
									sum-=d.getelement(i,0);
									operationcount++;
								}
								else {
									//cout<< sum<< " + "<<d.getelement(i,0)<<endl;
									operationcount++;

									sum+=d.getelement(i,0);
								}
							}
					//assign sum to temp
					temp.setelement(last, 0, sum);
					//cout<<"setting next row to: "<<sum<<endl;
					//enqueue it to working on queue
				workingonqueue.push_back(last);
			}

		//delete end from tempadjlist
		tempadjlist[position].pop_back();

	
		}


		//mark position
		marker[position]=1;

	}
	while(!workingonqueue.empty());

	myfile<<operationcount<<"	";
	return temp;

}

Doublematrix mult(Doublematrix d, ostream& myfile){

	//cout<<endl<<"start"<<endl;
	assert(columns==d.getrows());
	int operationcount=0;
	Doublematrix temp(rows,1);
	double sum;
	for(int i=0;i<rows;i++){
		sum=0;
		//cout<<1<<endl;
		for(int j=0;j<columns;j++){
		//cout<<2<<endl;
			if(v[i][j]){
				sum+=d.getelement(j,0);
				operationcount++;
			}

			}
		//cout<<3<<endl;

		temp.setelement(i,0,sum);
		//cout<<temp.getelement(i,0)<<endl;

		//cout<<4<<endl;


	}
	myfile<< operationcount<<"	";
	//cout<<5<<endl;


	return temp;


}
Doublematrix operator *= (Doublematrix d){
	assert(columns==d.getrows());


}	


int minpop(){
	//returns the minimum index
	int minpop=v[0].count();
	int minindex=0;
	int temp;
	for(int i=1;i<rows;i++){
		temp=v[i].count();
		if(temp<minpop){
			minindex=i;
			minpop=temp;
		}
	}
	return minindex;
		
}
void partition(int numberOfPartitions){
	//fills partition vector, splits this.v into numberOfPartitions vertical strips

	//(1) initialize partitions
	assert(columns%numberOfPartitions==0);
	int partitionsize=columns/numberOfPartitions;
	assert(partitionsize%8==0);
	partitions.resize(numberOfPartitions);
	for(int i=0; i<numberOfPartitions;i++)
		partitions[i].initialize(rows, partitionsize);
	//(2) fill each partition
	for(int i=0; i<numberOfPartitions;i++)
		for(int j=0;j<rows;j++)
			for (int k=partitionsize*i,m=0;m<partitionsize;m++,k++)
				partitions[i].setelement(j,m,getelement(j,k));
	


}
void calcMSTofPartitions(int numberOfPartitions){
	partition(numberOfPartitions);
	partitionmstcalculated=true;
	for(int i=0; i<numberOfPartitions;i++){
		partitions[i].boostcalcmst();
	}

}
Doublematrix multmstpartitioned(Doublematrix d,ostream& myfile, int numberOfPartitions){
	int partitionsize=columns/numberOfPartitions;
	if(!partitionmstcalculated)
		calcMSTofPartitions(numberOfPartitions);
	vector<Doublematrix> temps;
	temps.resize(numberOfPartitions);
	for(int i=0; i<numberOfPartitions;i++){
		temps[i].initialize(rows,1);
	}
	partitionoperationcount=0;
	for(int m=0; m<numberOfPartitions;m++){
		partitions[m].boostcalcalist();
		//partitions[m].rprint();
			//cout<<1;
			double sum=0;
			//traverse the tree DFS or BFS
			//calcfirst row  as rootthen follow tree off
			int start=partitions[m].minpop();
			for(int i=0,n=partitionsize*m;i<partitionsize;n++,i++){
				//cout<<endl<<partitions[m].getelement(start,i)<<' '<<i;


				if(partitions[m].getelement(start,i))
					sum+=d.getelement(n,0);
			}
			//cout<<sum<<endl;
			temps[m].setelement(start,0,sum);
			//cout<<2;
		//	cout<<"sum of start row is: "<< sum<<endl;
			boost::dynamic_bitset<> marker;
			//cout<<3;
			vector<vector<int>> tempadjlist=alist;
			//cout<<4;
			vector<vector<int>> queues;
			vector<int> workingonqueue;
			
			queues.resize(rows);
			marker.resize(rows);
			//alist[start].back = last element
			//mark last element marker[^]=1 when visited
			//alist[start].pop_back ==delete last element
		/*
		
	***NOTE: USING THIS AS A STACK, NOT A QUEUE....so replace language accordingly)
		ALGORITHM

		CALCULATE START ROW
		WORK ON START AND QUEUE ALL VERTECIES THAT ARE USED WITH START IN (THINGS TO WORK ON QUEUE)
		MARK START
		WHiLE THERE ARE THINGS TO WORK ON:
			DEQUEUE (a) FROM THINGS TO WORK ON
			work on a and queue all unmarked vertecies that are used with a in things to work on.
			mark a
		END WHILE
		*/
			workingonqueue.push_back(start);
			//cout<<5555555555<<endl;
			do{
				int position=workingonqueue.back();
				//cout<<m<<"position is: "<<position<<endl;
				//cout<<"workingonqueue is: "<<workingonqueue.size()<<endl;
				workingonqueue.pop_back();
				//cout<<666;
				//calculate all unmarked adjacent vertecies to position
				//while the temp adjlist is not empty
				boost::dynamic_bitset<> differmatrix;
				differmatrix.resize(partitionsize);
				while(!tempadjlist[position].empty()){
				//if vertecy at the end is not marked
				int last=tempadjlist[position].back();
			//	cout<<7;

					if(!marker[last]){
						//   work on the vertecy at the end
							sum=temps[m].getelement(position,0);
							//find where v[last] and v[position] differ
							differmatrix=partitions[m].rowXORrow(last,position); //v[last]^v[position];
							//<<"differmatrix is: "<<differmatrix<<endl;
						//	cout<<endl<<sum;
							//in the all of the places that they differ:
								//if v[position][i]==1, subtract d[i] from sum
								//if v[position][i]==0, add [i] to sum
								for(int i=0,n=partitionsize*m;i<partitionsize;n++,i++)
								if(differmatrix[i]) {//they differ in this spot
										if (partitions[m].getelement(position,i)){//v[position][i]){
										//	cout<< sum<< " - "<<d.getelement(n,0)<<endl;
											sum-=d.getelement(n,0);
											partitionoperationcount++;
										}
										else {
										//	cout<< sum<< " + "<<d.getelement(n,0)<<endl;
											partitionoperationcount++;

											sum+=d.getelement(n,0);
										}
									//		cout<<endl<<sum;

									}
							//assign sum to temp
							temps[m].setelement(last, 0, sum);
							//cout<<"setting next row to: "<<sum<<endl;
							//enqueue it to working on queue
						workingonqueue.push_back(last);
					}

				//delete end from tempadjlist
				tempadjlist[position].pop_back();

			
				}


				//mark position
				marker[position]=1;

			}
			while(!workingonqueue.empty());
		



	}
	//cout<<endl<<endl;
	//for(int i=0; i<numberOfPartitions;i++){
		//temps[i].rprint();cout<<endl;
	//}
	Doublematrix temp(rows,1);
	temp.addto(temps,numberOfPartitions);
	myfile<<partitionoperationcount<<"	";
	return temp;

}


};


#endif


//