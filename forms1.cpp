#ifndef _INC_forms1
#define _INC_forms1

#include "main.h"

bool Forms::FindWord(ifstream &fin, std::string aaa)
{
	char a1, a2;
	int i;
	char buf[1000];

	cerr << "\nT_ " << aaa[0];
	
	while (!fin.eof())
	{
//		fin.get(buf, sizeof(buf), aaa[0]);
		for (i=0; i<aaa.length(); i++)
		{
			fin.get(a1);
//			cerr << "\nT!_ " << a1;
//			cin >> a2;
			if (a1 == aaa[i]) continue;
			else
				break;
		}
		
		if (i == aaa.length()) return(1);

	}
	return (0);
}

double Forms::degInt(int n, double q)
{
	double a=1.0;
	for (int i=0; i<n; i++)
		a = a * q;
	return (a);
}

void  Forms::create_mas1()
{
	int i,j,i1,j1,j2,n,k,n1,Nform,flag1;
	ofstream fout;

	// N - dlina slova
	// N_2 kolichestvo sovpadeniy vo vsem slove (t.e. identity threshold)
	// Nsovp - kolichestvo sovpadeniy v nashey forme (t.e. troyki, chetverki, ...)

	int N = N_glob;
	int N_2 = N_2_glob;	

	int *mas1, *mas2;
	mas1 = new int[N];
	mas2 = new int[N];

	//zagatovka stepeney 2 dlya skorosti scheta
	for (i = 0; i<N; i++)
		mas1[i] = degInt(i,2);
	
	
	double d1,d2,d3;

	d1 = 1.0;
	n = N - N_2;
	for (i=0; i<n; i++)
		d1 = d1*(N - i)/double(n - i);

	int Nmas = int(d1)+1;
//	cerr<<"Nmas = "<<Nmas;
//	cin>>i;

	NVars_Glob = 0; 

	Vars_Glob = new bool *[Nmas];
	for (i = 0; i<Nmas; i++)
		Vars_Glob[i] = new bool[N];

	for (i = 0; i<Nmas; i++)
		for (j = 0; j < N; j++)
			Vars_Glob[i][j] = 0;


	for (i = 0; i<degInt(N,2); i++)
	{
		//generatsiya form
		n1 = 0;
		n = i;
		for (i1 = N-1; i1>=0; i1--)
		{
			mas2[i1] = int (n/mas1[i1]); //i1-pozitsiya v dvoichnoy zapisi
			n1 += mas2[i1]; //scitaet skol'ko "1", t.e. sovpadeniy
			n = n - (mas2[i1] * mas1[i1]);
		}

		//fil'tratsiya
		if (n1 != N_2) continue;
		if (mas2[0] != 1) continue; 

		for (j = 0; j < N; j++)
			Vars_Glob[NVars_Glob][j] = mas2[j];

		NVars_Glob++;

		/*if (NVars_Glob%100 == 0)
			cerr<<"\n "<<NVars_Glob;*/

		if (NVars_Glob > Nmas)
		{
			cerr<<"\n NVars_Glob > Nmas";
			cin>>i;
			return;
		}
	}


	VarsDescr_Glob = new int[NVars_Glob];


	fout.open("tavnits", ios::out);
	
	fout<<"NVars_Glob = "<<NVars_Glob<<"\t"<<"Nmas = "<<Nmas;
	
	for (i = 0; i<NVars_Glob; i++)	
	{
		
		fout<<"\n";
		for (j = 0; j<N; j++)
			fout<<Vars_Glob[i][j];
		
	}
	
	fout.close();

}

#endif  // _INC_forms1