#include "bitmatrix.h"
#include <fstream>

int main()
{
	typedef adjacency_list<vecS, vecS, undirectedS,VertexNameProperty, EdgeWeightProperty> Graph;


	/*
		//KRYLOV MATRIX CALCULATION
		

	*/
		ofstream myfile;
		int numberOfPartitions=9;

	myfile.open("testdata/9partitions.txt");
	myfile<<"#CALCULATIONS ON A RANDOM 01 square matrix partitioned into vertical columns of bytes\n";
	myfile<<"#ROWS	OPERATIONS(NORMAL)	OPERATIONS(MST) OPERATIONS(3Partitions)\n";				
	for(int i=72;i<1009;i+=72){
		int columns=i;
		
		bool error=false;
		myfile<<i<<"	";
		//cout<<1;
		Doublematrix m2 (columns,1);
		//cout<<2;
		Bitmatrix m1(columns, columns);
		Doublematrix m3(columns,1);
		Doublematrix m4(columns,1);
		Doublematrix m5(columns,1);

		//cout<<"01 matrix:\n";
		m1.randomize();
		//m1.rprint();
		m1.boostcalcmst();
		m1.boostcalcalist();
	//	cout<<"matrix to multiply by: \n";
		m2.randomize(9,20);
		//m2.rprint();
		//cout<<3;
		//cout<<"normal multiplication: \n";
		m3=m1.mult(m2,myfile);
		//m3.rprint();
		//cout<<4;
		
		//cout<<rows<<endl;
		
		m4=m1.multmst(m2,myfile);
		//m4.rprint();
		//cout<<5;
		m5=m1.multmstpartitioned(m2,myfile,numberOfPartitions);pa
		myfile<<endl;
		cout<<i<<endl;


 }
	myfile.close();

			return 0;
}