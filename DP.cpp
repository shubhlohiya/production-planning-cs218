#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <climits>
using namespace std;
using namespace std::chrono;
using ll = long long;

ll getcost(int M, int D[], int E, ll Hcost, ll Fcost,
 			ll S, int C, int OTC, ll OTPrice, ll W){
	// Naming of parameters is as described in the Problem Statement
	int total_demand = 0; // sum of demand over m months
	for(int i=0; i<M; i++)
		total_demand += D[i];
	// safe measure of max possible required employees
	int e_max = max(E, (int)ceil(total_demand/C));
	// safe measure of max possible inventory in storage (all produce)
	int inventory_max = e_max*(C+OTC);

	// Solution Space: All possible combinations of employees and inventory
	// at the beginning of each month for M months

	// Creating cost table of size (M+1, inventory_max+1, e_max+1)
	vector<ll> temp1(e_max+1, LLONG_MAX);
	vector<vector<ll> > temp2(inventory_max+1, temp1);
	vector<vector<vector<ll> > > cost(M+1, temp2);
	// cost[i][j][k] is the minimum production cost from month i to M with
	// j carpets in storage and k employees at the beginning of month i

	// adjust for the hiring/firing cost at end of M months to get E employees
	for(int j=0; j<=inventory_max; j++)
		for(int k=0; k<=e_max; k++)
			cost[M][j][k] = (k<=E)? Hcost*(E-k) : Fcost*(k-E);

	// Fill table
	for(int i=M-1; i>=0; i--){
		for(int j=0; j<=inventory_max; j++){
			if(i==0 && j!=0)
				continue;
			for(int k=0; k<=e_max; k++){
				if(i==0 && k!=E)
					continue;
				for(int j_next=0; j_next<=inventory_max; j_next++){
					for(int k_next=0; k_next<=e_max; k_next++){
						//required overtime carpets in month i
						int o = D[i] - (j-j_next + C*k_next);
						if(o<0 || o > k_next*OTC)
							continue;
						// hiring, firing, overtime, salary and inventory 
						// storage costs for month i
						ll hc, fc, oc, salary, ic;						
						if(k_next>=k){
							hc = Hcost*(k_next-k);
							fc = 0;
						}
						else{
							fc = Fcost*(k-k_next);
							hc = 0;
						}
						oc = o*OTPrice;
						salary = k_next*S;
						ic = j*W;
						// Recurrence
						cost[i][j][k] = min(cost[i][j][k], 
							hc+fc+oc+salary+ic+cost[i+1][j_next][k_next]);
						// The recurrence relates minimum production cost for
						// months i...M to that of months (i+1)...M by minimizing
						// over all possible combinations of next month j and k 
						// (inventory and employees) after adding the corresponding
						// hiring, firing, overtime, salary and storage costs
					}
				}
			}
		}
	}
	// min production cost for M months starting with E employees and 0 inventory
	return cost[0][0][E];

	// Time Analysis:
	// Clearly filling the DP table is the most expensive operation and will
	// determine the total time complexity
	// Total entries we update: (M-1)*(e_max)*(inventory_max)
	// Time to update each entry: (e_max)*(inventory_max)
	// as we are minimizing over all possible combinations from month i+1
	// Time complexity of the Algorithm: O(M*(e_max^2)*(inventory_max^2))
	// where e_max and inventory_max are defined as above
}

int main(){
	int M;
	cin>>M;
	int D[M];
	for(int i=0; i<M; i++)
		cin>>D[i];
	int E,C,OTC;
	ll Hcost, Fcost, S, OTPrice, W;
	cin>>E;
	cin>>Hcost>>Fcost;
	cin>>S;
	cin>>C;
	cin>>OTC;
	cin>>OTPrice;
	cin>>W;
	auto start = high_resolution_clock::now();
	ll min_cost = getcost(M, D, E, Hcost, Fcost, S, C, OTC, OTPrice, W);
	auto end = high_resolution_clock::now();
	cout<<min_cost<<" ";
	cout<<((double)duration_cast<microseconds>(end-start).count())/1e6;
}