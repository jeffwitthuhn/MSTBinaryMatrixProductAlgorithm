#include "bitmatrix.h"
#include <fstream>

int main()
{
	ofstream myfile1;
	myfile1.open("testdata/elementstest2.txt");
	myfile1<<"#ofelements TEST\n"<<"#ofelements	BruteForce	MSTmethod	MST4partitions\n";

	double rows,columns;
	double ratio;
	int MaxNumberofElements=4500;
	int start;
	cin>>start;
	for(int i=start;i<=start;i+=100){
		rows=columns=i;
		cout<<1;
		Bitmatrix matrix01(rows,columns);
		cout<<2;
		Doublematrix operandVector(columns,1);//initialize the operand vector
		cout<<3;
		Doublematrix resultvector(rows,1);//initialize the  result vector
		cout<<4;
		matrix01.randomize();
		cout<<5;
		operandVector.randomize(1,50);
		cout<<6;
		ratio=i*i;
		myfile1<<ratio<<"	";
		resultvector=matrix01.mult(operandVector,myfile1);
		cout<<7;
		matrix01.boostcalcmst();
		cout<<8;
		matrix01.boostcalcalist();
		cout<<9;
		resultvector=matrix01.multmst(operandVector,myfile1);
		resultvector=matrix01.multmstpartitioned(operandVector,myfile1,4);
		myfile1<<endl;
		cout<<"rows: "<<rows<<" columns: "<<columns<<endl;
	}
	myfile1.close();
	return 0;
}
