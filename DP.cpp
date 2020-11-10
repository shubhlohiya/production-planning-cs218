#include <iostream>
using namespace std;

int main(){

    int m;
    cin>>m;

    int D[m], CDemand = 0;
    for (int i=0; i<m; i++) {
        cin>>D[i];
        CDemand += D[i];
    }
    int E, C, OTC; float Hcost, Fcost, S, OTPrice, W;

    cin>>E>>Hcost>>Fcost>>S>>C>>OTC>>OTPrice>>W;

    int maxE = 0;
    for (int i=0; i<m; i++) {
        maxE = max(max(E, 1 + D[i]/C), maxE);
    }

    float minCost[m+1][CDemand+1][maxE+1] = {0};

    for (int k = m-1; m>=0; m--){
        for(int e = 0, e<=maxE, e++){
            for(int i = 0; i<=Cdemand; i++){

                minCost[k][i][e] = minCost[k+1][i][e]; //just for initialization purposes, since 0 will be incorrect
                //loop for minimum on minCost[k+1][imin][emin], i.e. the following months

                for(int emin = 0, emin<=maxE, emin++){
                    for(int imin = 0; imin<=Cdemand; imin++){
                        
                        //calculate HFcost - Hiring/Firing Costs
                        float HFcost;
                        if (emin>e) // we fire
                            HFcost = Fcost*(emin - e);
                        else //we hire
                            HFcost = Hcost*(e - emin);

                        //calculate Overtime cost here for emin, imin
                        float Overtime;
                        if (C*emin + imin - i - D[k] < 0) //imin = i + C*emin - D[k] + alpha (excess production reqd)
                            Overtime = -(C*emin + imin - i - D[k]) * OTPrice; 

                        //Calculate Salary here
                        float Salary = S * emin;
                        
                        //Storage cost to begin with imin inventory
                        float Storage = W*imin; //or is it i 
                        //!!!!!!!!!!!!!!!!! check
                        
                        minCost[k][i][e] = min(minCost[k][i][e], minCost[k+1][imin][emin] + HFcost + Overtime + Salary + Storage);
                    }
                }


            }
        }
    }
    //note that there is one redundant loop in the end for minCost[0][i][e]. 
    //We can stop the loop, but currently let's keep it for simplicity


    cout<<minCost[0][0][0];

    return 0;
}
