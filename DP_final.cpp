#include <chrono>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

double prodplanning(int m, int d[], int e, double hcost, double fcost, double s, int c, int otc, double otprice, double w)
{ //Here, the variables are termed the same as that in the HW5 question
	int CDemand = 0;//cumulative demand of carpets from 0 through m-1 months
	for (int i=m-1; i>=0; i--)
		CDemand = CDemand + d[i];

	long int alpha = 0;
	if ((CDemand%c)==0) alpha = (CDemand/c);
	else alpha = (CDemand/c + 1);
	int maxE = max(e, alpha);

	//Calculating maximum inventory required
	int maxInv = max(CDemand, maxE*(otc + c)); 


	//This is the table that stores the min cost as a function of the month (0 to m),
	//inventory (0 to maxInv) and beginning with employees in the respective month (0 to maxE)
	vector<vector<vector<double>>> minCost; 
	minCost.resize(m+1);
	for(int i=m; i>=0; i--)
	{
		minCost[i].resize(maxInv+1);
		for(int j=maxInv; j>=0; j--)
		{
			minCost[i][j].resize(maxE+1);
		}

	}

	//minCost[i][j][k] is the minimum cost of production from month i to M, 
	//given that we have j carpets in the inventory in
	//the month i and k employees at the beginning of the month i 
	for(int i=0; i<m+1; i++)
	{
		for(int j=0; j<=maxInv; j++)
		{
			for(int k=0; k<=maxE; k++)
			{
				if (i==m){
					if(k<=e)
					{
						minCost[i][j][k] = hcost*(e-k); //Arises from the base case
						//minCost of production for last month m if we used k less than e employees in previous month
						//then we hire some employees to make number of employees equal to e
					}
					else
					{
						minCost[i][j][k] = fcost*(k-e); //Arises from the base case
						//minCost of production for last month m if we used k greater than e employees in previous month
						//then we hire some employees to make number of employees equal to e
					}
				}
				else
				{
					minCost[i][j][k] = DBL_MAX; //set it to max possible value 
				}
			}
		}
	}


	//Table has been initialised
	for(int i=m-1; i>=0; i--) //moving backwards in time
	{
		for(int j=0; j<=maxInv; j++)
		{
			for(int k=0; k<=maxE; k++)
			{
				for(int next_k = 0; next_k <= maxE; next_k++) //next_k is the number of employees at start of next month
				{
					for(int next_j = 0; next_j <= maxInv; next_j++) //next_j is the carpets stored in the next month
					{
						int overtime;
						overtime = next_j + d[i] - c*next_k - j; //"overtime" is number of overtime carpets produced for the current month
						if (overtime >= 0 && overtime <= next_k*otc)
						{
							double hire, fire, overtimeCost, salary, inventory;// hiring, firing, overtime carpet production costs, 
							//salaries and inventory storage costs repectively for month i

							overtimeCost = overtime*otprice; //Total overtime cost
							salary = s*next_k; //Total salary to be paid
							inventory = w*j; //Total inventory cost

							//hire/fire decision
							if(next_k>k)
							{
								hire = hcost*(next_k-k);
								fire = 0;
							}
							else
							{
								hire = 0;
								fire = fcost*(k-next_k);
							}

							minCost[i][j][k] = min(minCost[i][j][k], hire + fire + overtime + salary + inventory + minCost[i+1][next_j][next_k]);
							//The reccurence realtion uses the values of minimum cost for i+1 to M months to find the cost for i to M months
							//The number of employees, inventory held designated for i+1 month are given by next_j and next_k
							//Using the above given relations, we can find the additional costs associated with it
							// We they take the minimum of all those
						}
					}
				}
			}
		}
	}
	//Table is complete
	return minCost[0][0][e]; //We need the minimum cost of production for months 0 to M such that we have 0 carpets in the inventory
	//while starting and e employees at the beginning of the month
}

int main()
{
	int m;
	cin >> m;

	int d[m] = {0};
	
	double hcost, fcost, s, otprice, w;
	int e, c, otc;
	
	for(int i=0; i<m; i++)
		cin>>d[i];

	cin >> e >> hcost >> fcost >> s >> c >> otc >> otprice >> w;

	auto start = high_resolution_clock::now();
	double cost = prodplanning(m, d, e, hcost, fcost, s, c, otc, otprice, w);
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	//Time analysis : (All variables used have same meaning as that used in the code) 
	//Resizing of table takes time proportional to m*maxInv*maxE
	//Initialisation of table with base cases takes time = O(m*maxInv*maxE)
	//Formulation of table takes time = O(m*(maxInv^2)*(maxE^2))
	//This is because the calculation of one value of table requires time = O(maxInv*maxE) and there are (m+1)*(maxInv+1)*(maxE+1) elements in the table
	//Therefore total time is O(m*maxInv^2*maxE^2)
	//We know, maxInv = CDemand and maxE = CDemand/c for large demands and less initial employees
	//So then time = O(m*CDemand^4/c^2)
	//If initial employees are large maxE = e and maxInv = e*(c+otc)
	//So then time = O(m*e^4*(c+otc)^2)

	cout << cost << " " << (double)duration.count()/1000000;
}