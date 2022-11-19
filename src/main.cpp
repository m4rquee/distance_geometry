#include "gurobi_c++.h"
#include <cmath>
#include <sstream>

using namespace std;

string itos(int i) {
    stringstream s;
    s << i;
    return s.str();
}

void read_graph(int &n, int &m, double (**E)[3]) {
    cin >> n >> m;
    *E = new double[m][3];
    for (int i = 0; i < m; i++) cin >> (*E)[i][0] >> (*E)[i][1] >> (*E)[i][2];
}

void method() {
    // Read the graph edges data: -----------------------------------------
    int n, m;
    double(*E)[3];
    read_graph(n, m, &E);

    auto X = new GRBVar[n], Y = new GRBVar[n], Z = new GRBVar[n];// coordinate for each vertex
    auto dist_sqr = new GRBVar[m];                               // length² of each edge

    double time_limit = 180;// 3 min
    try {
        auto *env = new GRBEnv();
        env->set(GRB_DoubleParam_TimeLimit, time_limit);

        GRBModel model = GRBModel(*env);
        model.set(GRB_StringAttr_ModelName, "Molecular Distance Geometry");
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
        model.set(GRB_IntParam_NonConvex, 2);

        // Focus primarily on feasibility of the relaxation:
        // model.set(GRB_IntParam_MIPFocus, GRB_MIPFOCUS_FEASIBILITY);
        // model.set(GRB_IntParam_Cuts, GRB_CUTS_AGGRESSIVE);
        // model.set(GRB_IntParam_Presolve, GRB_PRESOLVE_AGGRESSIVE);

        // Variable creation: _____________________________________________________________________
        GRBQuadExpr obj = 0;
        for (int e = 0; e < m; e++) {
            dist_sqr[e] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "d2_" + itos(e));
            obj += (dist_sqr[e] - E[e][2]) * (dist_sqr[e] - E[e][2]);
        }
        model.setObjective(obj);

        for (int i = 0; i < n; i++) {
            X[i] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x_" + itos(i));
            Y[i] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "y_" + itos(i));
            Z[i] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "z_" + itos(i));
        }
        model.update();// run update to use model inserted variables

        // Constraint creation: ___________________________________________________________________
        // Fix an edge at the origin:
        int u = (int) E[0][0], v = (int) E[0][1];// endpoints of the edge 0
        double d = sqrt(E[0][2]);
        model.addConstr(X[u] == 0);
        model.addConstr(Y[u] == 0);
        model.addConstr(Z[u] == 0);
        model.addConstr(X[v] == d);
        model.addConstr(Y[v] == 0);
        model.addConstr(Z[v] == 0);

        // Relate the distance variables to the actual vertex distances:
        for (int e = 0; e < m; e++) {
            int u = (int) E[e][0], v = (int) E[e][1];// endpoints of the edge e
            auto vtx_dist_sqr = (X[u] - X[v]) * (X[u] - X[v]) + (Y[u] - Y[v]) * (Y[u] - Y[v]) +
                                (Z[u] - Z[v]) * (Z[u] - Z[v]);
            model.addQConstr(dist_sqr[e] == vtx_dist_sqr);
        }

        model.update();// run update before optimize
        model.optimize();
        if (model.get(GRB_IntAttr_SolCount) == 0) throw GRBException("Could not obtain a solution!", -1);

        cout << "Vertex coordinates:" << endl;
        for (int i = 0; i < n; i++)
            cout << i + 1 << "th vtx - (" << X[i].get(GRB_DoubleAttr_X) << "; " << Y[i].get(GRB_DoubleAttr_X)
                 << "; " << Z[i].get(GRB_DoubleAttr_X) << ")" << endl;
        cout << "- Disimilarity Cost " << model.getObjective().getValue() << endl;
    } catch (GRBException &e) {
        cout << "Error number: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) { cout << "Error during callback" << endl; }
    delete[] X;
    delete[] Y;
    delete[] Z;
    delete[] dist_sqr;
}

int main() {
    method();
    return 0;
}
