import time, pulp as p

# Take inputs

M = eval(input().strip()) # M months
D = list(map(eval, input().strip().split())) # demand in each month
# start employees, hiring cost, firing cost
E, Hcost, Fcost = map(eval, input().strip().split())
S, C = map(eval, input().strip().split()) # Salary, carpets per month
# max overtime carpets, overtime cost per carpet
OTC, OTPrice = map(eval, input().strip().split()) 
W = eval(input().strip()) # warehouse cost per carpet

start = time.time()
# Formulate ILP

prob = p.LpProblem("production-planning", p.LpMinimize)
e = list(p.LpVariable.dicts("E", [i for i in range(M)],
                                cat='Integer', lowBound=0).values()) # no of employees in month i+1
x = list(p.LpVariable.dicts("X", [i for i in range(M)],
                                cat='Integer', lowBound=0).values()) # no of carpets in month i+1
o = list(p.LpVariable.dicts("O", [i for i in range(M)],
                                cat='Integer', lowBound=0).values()) # no of overtime carpets
h = list(p.LpVariable.dicts("H", [i for i in range(M+1)],
                                cat='Integer', lowBound=0).values()) # no hired at beginning of month i+1
f = list(p.LpVariable.dicts("F", [i for i in range(M+1)],
                                cat='Integer', lowBound=0).values()) # no fired at beginning of month i+1
s = list(p.LpVariable.dicts("S", [i for i in range(M)],
                                cat='Integer', lowBound=0).values()) # carpets stored at end of month i+1

prob += S*sum(e) + Hcost*sum(h) + Fcost*sum(f) + W*sum(s[:-1]) + OTPrice*sum(o) # cost (objective)

for i in range(M):
    prob += x[i] == C*e[i]+o[i]
    prob += o[i] <= OTC*e[i]
    if i==0:
        prob += e[0] == E + h[0]-f[0]
        prob += s[0] == x[0]-D[0]
    else:
        prob += e[i] == e[i-1]+h[i]-f[i]
        prob += s[i] == s[i-1]+x[i]-D[i]

prob += e[M-1]+h[M]-f[M] == E

prob.solve(p.apis.PULP_CBC_CMD(msg=0))
end = time.time()
print(int(prob.objective.value()), "{:.4f}".format(end-start))