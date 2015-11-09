#include "bitmatrix.h"
#include <fstream>

int main()
{
	ofstream myfile1;
	myfile1.open("testdata/partitions2.txt");
	myfile1<<"partitions TEST\n"<<"partitions	BruteForce	MSTmethod	MST4partitions\n";

	double rows=1024;
	double columns=rows;
	double ratio;
	int MaxNumberofElements=1024*1024;
	int start=1;
	int sparsity;

	for(int i=start;i<1024;i*=2){
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
		ratio=i;
		myfile1<<ratio<<"	";
		resultvector=matrix01.mult(operandVector,myfile1);
		resultvector=matrix01.multmstpartitioned(operandVector,myfile1,i);
		myfile1<<endl;
		cout<<"rows: "<<rows<<" columns: "<<columns<<endl;
	}
	myfile1.close();
	return 0;
}
