#include "bitmatrix.h"
#include <fstream>

int main()
{
	typedef adjacency_list<vecS, vecS, undirectedS,VertexNameProperty, EdgeWeightProperty> Graph;


	/*
		//KRYLOV MATRIX CALCULATION
		

	*/
	ofstream myfile;
	myfile.open("Random1to3.txt");
	int rows;//=11;
	int incrementation;
	int maxrows;
	cout<<"enter: startrows incrementation maxrows : ";
	cin>>rows>>incrementation>>maxrows;
	//columns=rows;
	//mector_vectrix
	//mectrixvectrix_mector
	//	g++ -pg RandomSquare.cpp -O3 -std=c++11 -I C:\boost_1_57_0  -o Rsq
	//g++ -pg Random3to1.cpp -O3 -std=c++11 -I C:\boost_1_57_0  -o R1to3
	//cout<<88;
	myfile<<"#CALCULATIONS ON A RANDOM 3to1 MATRIX\n";
	myfile<<"#ROWS--OPERATIONS(NORMAL)--OPERATIONS(MST)\n";
	for(int i=rows;i<maxrows;i+=incrementation, rows+=incrementation){
		cout<<rows<<endl;
		int columns=rows*3;
		bool error=false;
		myfile<<rows<<" ";
		Doublematrix m2 (columns,1);
		Bitmatrix m1(rows, columns);

		Doublematrix m3(rows,1);

		Doublematrix m4(rows,1);

		//cout<<"01 matrix:\n";
		m1.randomize();

		//m1.rprint();
		m1.boostcalcmst();

		m1.boostcalcalist();

		//cout<<"matrix to multiply by: \n";
		m2.randomize(9,20);

		//m2.rprint();

		//cout<<"normal multiplication: \n";
	//	cout<<"mult:"<<endl;
		m3=m1.mult(m2,myfile);

		//m3.rprint();

		
		//cout<<rows<<endl;
		//cout<<9;
	//	cout<<"multmst:"<<endl;

		m4=m1.multmst(m2,myfile);

		//m4.rprint();

		myfile<<endl;


 }
myfile.close();

}