#include "bitmatrix.h"
#include <fstream>

int main()
{
	ofstream myfile1;
	myfile1.open("testdata/ratiotest.txt");
	myfile1<<"#RATIO TEST\n"<<"#rows/columns	BruteForce	BFT	MSTmethod	MSTT	MST4partitions	MST4\n";

	double rows,columns;
	double ratio;
	int totalNumberofElements=1024*1024;
	for(int i=256;i<totalNumberofElements/4;i*=2){
		rows=totalNumberofElements/i;
		columns=i;
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
		ratio=rows/columns;
		myfile1<<ratio<<"	";
		auto start = std::chrono::system_clock::now();
		resultvector=matrix01.mult(operandVector,myfile1);
		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		myfile1 << elapsed.count() <<"	";

		cout<<7;

		start = std::chrono::system_clock::now();
		matrix01.boostcalcmst();
		cout<<8;
		matrix01.boostcalcalist();
		cout<<9;
		resultvector=matrix01.multmst(operandVector,myfile1);
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		myfile1 << elapsed.count() <<"	";

		start = std::chrono::system_clock::now();
		if(i>256)
			resultvector=matrix01.multmstpartitioned(operandVector,myfile1,4);
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		myfile1 << elapsed.count() <<"	";
		myfile1<<endl;
		cout<<"rows: "<<rows<<" columns: "<<columns<<endl;
	}
	myfile1.close();
	return 0;
}
