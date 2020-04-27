#ifndef _INC_forms2
#define _INC_forms2

#include "main.h"

void  Forms::chetv_struct_Generation()
{
	int i, i1, j1, j2, k, n1, Nform, flag1;
	ofstream fout;

	// N - dlina slova
	// N_2 kolichestvo sovpadeniy vo vsem slove (t.e. identity threshold)
	// Nsovp - kolichestvo sovpadeniy v nashey forme (t.e. troyki, chetverki, ...)


	int N = N_glob;
	int N_2 = N_2_glob;
	int Nsovp = Nsovp1_Glob;

	CString qqq1, qqq2;


	//zadanie massiva, hranyaschego formi	
	int NformMax = 10000; //prosto dostatochno bol'shoe chislo
	int** forms; //massiv form; indeksi ot 0 do N-1 mogut bit' 1 ili 0; N - dlina formi
				 //vkluchaya probeli

	forms = new int* [NformMax];
	for (i = 0; i < NformMax; i++)
		forms[i] = new int[N + 1];

	for (i = 0; i < NformMax; i++)
		for (j1 = 0; j1 < N; j1++)
			forms[i][j1] = 0;

	for (i = 0; i < NformMax; i++)
	{
		forms[i][N] = -1;
		forms[i][0] = 1;
	}

	/*
		chtobi sgenerirovat' vse varianti sovpadeniy rassmatrivaem match 2-uh slov dlinoy N
		kak zapis' chisla v dvoichnom kode: "0" - nesovpadenie; "1" - sovpadenie; Izvesno,
		chto pravilo perevoda v dvoichniy kod: X = a0*2**0 + a1*2**1 + ... + ak*2**k,
		ai - koefitsienti, kajdiy iz kotorih raven "1" ili "0", ** - vozvedenie v stepen'.
		Togda X v dvoichnom	kode budet a0a1a2...ak. Max chislo predstavlennoe slovom iz
		N pozitsiy mojet bit' 2**N (t.k. pervaya pozitsiya - 0-stepen').
		Takim obrazom, algoritm generiruet varianti sovpadeniy, prosto berya vse chisla ot 0
		do 2**N	i zapisivaet ih v 2-om kode, a zatem vibiraet formi s nujnim chislom "1"
		(t.e. sovpadeniy, N_2). Potom iz etih variantov vichlenyaet vse varianti form
		zadannoy dlini Nsovp.
	*/

	k = 0;
	Nform = 0;
	for (i = 0; i < NVars_Glob; i++)
	{
		/*if (i % 100 == 0)
			cerr << "\n " << i;*/

		//sravnenie s imeyuschimisya formami
		flag1 = 1;
		for (i1 = 0; i1 < Nform; i1++)
		{
			for (j1 = 0; j1 <= N - forms[i1][N]; j1++)
			{
				if (Vars_Glob[i][j1] == 0) continue;

				flag1 = 0;
				for (j2 = 1; j2 < forms[i1][N]; j2++)
				{
					if (forms[i1][j2] == 0) continue;

					if (Vars_Glob[i][j1 + j2] != 1)
					{
						flag1 = 1;
						break;
					}
				}

				if (flag1 == 0)	break; //odna iz form v sovpadenii uje prisutstvuet,
									   //znachit takoe sovpadenie nam neinteresno.
			}

			if (flag1 == 0)	break;
		}

		if (flag1 == 0)	continue;

		//if there is no existing forms - add a new form (can be optimized) 
		for (j1 = 0; j1 < N; j1++)
		{
			if (Vars_Glob[i][j1] == 0) continue;

			n1 = 1;
			for (j2 = 1; j2 < N; j2++)
			{
				forms[Nform][j2] = Vars_Glob[i][j2 + j1];

				if (forms[Nform][j2] == 1)
					n1++;

				if (n1 == Nsovp)
				{
					forms[Nform][N] = j2 + 1; //zapis' razmera
					Nform++;
					break;
				}
			}

			break;
		}

		if (Nform == NformMax)
			break;
	}

	fout.open("forms", ios::out);

	fout << "Nform1 = " << Nform << "\t" << "form1Size = " << N + 1 << " Nsovp1 = " << Nsovp << " N_2 = " << N_2;

	for (i1 = 0; i1 < Nform; i1++)
	{
		fout << "\n>" << i1 << "\t";
		for (j1 = 0; j1 < N + 1; j1++)
			fout << forms[i1][j1] << " ";
	}

	fout.close();

	//cerr << "\nk = " << Nform;
	//	cin>>i;

		///////////////////copy to forms_Glob
	Nform1_Glob = Nform;
	form1Size = N + 1;

	forms1Glob = new int* [Nform1_Glob];
	for (i = 0; i < Nform1_Glob; i++)
		forms1Glob[i] = new int[form1Size];

	for (i1 = 0; i1 < Nform1_Glob; i1++)
		for (j1 = 0; j1 < form1Size; j1++)
			forms1Glob[i1][j1] = forms[i1][j1];
	///////////////////////////////////////

}

void  Forms::Read_Forms1(std::string fname)
{
	int i, n, k;
	char a, buf[1000];

	ofstream fout;
	ifstream fin;

	fin.open(fname.c_str(), ios::in);
	if (!fin.good()) return;

	cerr << "\nT00";

	FindWord(fin, "Nform1 =");

	cerr << "\nT01";
	fin >> Nform1_Glob;



	FindWord(fin, "form1Size =");
	fin >> form1Size;

	FindWord(fin, "Nsovp1 =");
	fin >> Nsovp1_Glob;

	FindWord(fin, "N_2 =");
	fin >> N_2_glob;

	cerr << "\nT01";

	forms1Glob = new int* [Nform1_Glob];
	for (i = 0; i < Nform1_Glob; i++)
		forms1Glob[i] = new int[form1Size];

	cerr << "\n" << Nform1_Glob << " " << form1Size;
	//		cin>>k;

	for (n = 0; n < Nform1_Glob; n++)
		for (i = 0; i < form1Size; i++)
			forms1Glob[n][i] = 0;

	for (n = 0; n < Nform1_Glob; n++)
	{
		fin.getline(buf, sizeof(buf));

		fin >> a;
		fin >> k;

		for (i = 0; i < form1Size; i++)
			fin >> forms1Glob[n][i];

	}

	fin.close();

	cerr << "\nT03";
}

void Forms::Forms1_anal1()
{
	/*
		Bivaet, pri opredelennih N_2 i Nsovp, chto naydennie formi vstrechayutsya tol'ko
		drug s drugom, t.e. chas iz nih mojno vikinut' iz rassmotreniya. Eto i proveryaet
		dannaya funktsiya
	*/
	int i, i1, ii1, j1, j2, k, n, flag1;

	ofstream fout;

	CString fname;
	char buf[1000];

	cerr << "\nt0";
	Read_Forms1("forms");

	cerr << "\nt1";

	fout.open("forms_c", ios::out);
	fout << "Nform1 = " << Nform1_Glob << "\t" << "form1Size = " << form1Size << " Nsovp1 = " << Nsovp1_Glob
		<< " N_2 = " << N_2_glob;
	for (i1 = 0; i1 < Nform1_Glob; i1++)
	{
		fout << "\n>" << i1 << "\t";
		for (j1 = 0; j1 < form1Size; j1++)
			fout << forms1Glob[i1][j1] << " ";
	}
	fout.close();


	int N = form1Size - 1;
	int N_2 = N_2_glob;

	int* mas3, * mas4;
	mas3 = new int[Nform1_Glob];
	mas4 = new int[Nform1_Glob];

	for (i = 0; i < Nform1_Glob; i++)
	{
		mas3[i] = 0;
		mas4[i] = 0;
	}

	for (i = 0; i < NVars_Glob; i++)
	{
		if (i % 100 == 0)
			cerr << "\n " << i << " anal";

		//sravnenie s imeyuschimisya formami
		for (i1 = 0; i1 < Nform1_Glob; i1++)
			mas3[i1] = 0;

		for (i1 = 0; i1 < Nform1_Glob; i1++)
			for (j1 = 0; j1 <= N - forms1Glob[i1][N]; j1++)
			{
				if (Vars_Glob[i][j1] == 0) continue;

				flag1 = 0;
				for (j2 = 1; j2 < forms1Glob[i1][N]; j2++)
				{
					if (forms1Glob[i1][j2] == 0) continue;

					if (Vars_Glob[i][j1 + j2] != 1)
					{
						flag1 = 1;
						break;
					}
				}

				if (flag1 == 0)
				{
					mas3[i1] = 1;
					break;
				}
			}

		k = 0;
		for (i1 = 0; i1 < Nform1_Glob; i1++)
			k = k + mas3[i1];

		// otmetka formi, kotoraya bila edinstvennoy
		if (k == 1)
			for (i1 = 0; i1 < Nform1_Glob; i1++)
				if (mas3[i1] == 1)
					mas4[i1] = 1;

	}

	fout.open("forms_an1", ios::out);
	fout << "Nform1 = " << Nform1_Glob << "\t" << "form1Size = " << form1Size << " Nsovp1 = " << Nsovp1_Glob;
	for (i1 = 0; i1 < Nform1_Glob; i1++)
	{
		fout << "\n>" << i1 << "\t";
		for (j1 = 0; j1 < form1Size; j1++)
			fout << forms1Glob[i1][j1] << " ";
		fout << "\t" << mas4[i1];
	}
	fout.close();

	//fil'tratsiya lishnih form
	for (ii1 = 0; ii1 < Nform1_Glob; ii1++)
	{
		cerr << "\n " << Nform1_Glob << "  " << ii1 << " anal2";

		if (mas4[ii1] == 1) continue;

		mas4[ii1] = -1;

		//videlenie form, kotorie hot' v odnom sluchae - edinstvennie, esli ubrat' formu ii1	
		for (i = 0; i < NVars_Glob; i++)
		{
			//sravnenie s imeyuschimisya formami
			for (i1 = 0; i1 < Nform1_Glob; i1++)
				mas3[i1] = 0;

			for (i1 = 0; i1 < Nform1_Glob; i1++)
			{
				if (mas4[i1] == -1) // s ii1 formoy ne sravnivaem
					continue;

				for (j1 = 0; j1 <= N - forms1Glob[i1][N]; j1++)
				{
					if (Vars_Glob[i][j1] == 0) continue;

					flag1 = 0;
					for (j2 = 1; j2 < forms1Glob[i1][N]; j2++)
					{
						if (forms1Glob[i1][j2] == 0) continue;

						if (Vars_Glob[i][j1 + j2] != 1)
						{
							flag1 = 1;
							break;
						}
					}

					if (flag1 == 0)
					{
						mas3[i1] = 1;
						break;
					}
				}

				if (flag1 == 0 && mas4[i1] == 1)
				{
					flag1 = 2; //tut est' neudalyaemaya forma - nechego dal'she delat'
					break;
				}

			}

			if (flag1 == 2)
				continue;

			k = 0;
			for (i1 = 0; i1 < Nform1_Glob; i1++)
				k = k + mas3[i1];

			// otmetka formi, kotoraya bila edinstvennoy
			if (k == 1)
				for (i1 = 0; i1 < Nform1_Glob; i1++)
					if (mas3[i1] == 1)
						mas4[i1] = 1;
		}
	}




	fout.open("forms_an2", ios::out);
	fout << "Nform1 = " << Nform1_Glob << "\t" << "form1Size = " << form1Size << " Nsovp1 = " << Nsovp1_Glob;
	for (i1 = 0; i1 < Nform1_Glob; i1++)
	{
		fout << "\n>" << i1 << "\t";
		for (j1 = 0; j1 < form1Size; j1++)
			fout << forms1Glob[i1][j1] << " ";
		fout << "\t" << mas4[i1];
	}
	fout.close();

	int* nums;
	nums = new int[Nform1_Glob];

	k = 0;
	for (i1 = 0; i1 < Nform1_Glob; i1++)
		if (mas4[i1] == 1)
		{
			nums[i1] = k;
			k++;
		}
		else
			nums[i1] = -1;

	sprintf(buf, "forms_Result_%i_%i", N_2, Nsovp1_Glob);
	fname = buf;

	fout.open(fname, ios::out);
	fout << "Nform1 = " << k << "\t" << "form1Size = " << form1Size << " Nsovp1 = " << Nsovp1_Glob
		<< " N_2 = " << N_2;
	n = 0;
	for (i1 = 0; i1 < Nform1_Glob; i1++)
	{
		if (mas4[i1] != 1) continue;
		fout << "\n>" << n << "\t";
		for (j1 = 0; j1 < form1Size; j1++)
			fout << forms1Glob[i1][j1] << " ";
		n++;
	}
	fout.close();

	/////////////////////copy of the decreased mas into glob
	Nform1_Glob = k;

	for (i = 0; i < Nform1_Glob; i++)
		if (nums[i] != -1)
			for (j1 = 0; j1 < form1Size; j1++)
				forms1Glob[nums[i]][j1] = forms1Glob[i][j1];
	///////////////////////////////////

	sprintf(buf, "forms_Result_%i_%i_ch", N_2, Nsovp1_Glob);
	fname = buf;

	fout.open(fname, ios::out);
	fout << "Nform1 = " << Nform1_Glob << "\t" << "form1Size = " << form1Size << " Nsovp1 = " << Nsovp1_Glob
		<< " N_2 = " << N_2;
	n = 0;
	for (i1 = 0; i1 < Nform1_Glob; i1++)
	{
		fout << "\n>" << i1 << "\t";
		for (j1 = 0; j1 < form1Size; j1++)
			fout << forms1Glob[i1][j1] << " ";
	}
	fout.close();

}

#endif  // _INC_forms2