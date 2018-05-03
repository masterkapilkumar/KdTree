#include <iostream>
#include <queue>
#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;
 
void showpq(priority_queue <int> gq)
{
    priority_queue <int> g = gq;
    while (!g.empty())
    {
        cout << '\t' << g.top();
        g.pop();
    }
    cout << '\n';
}
 
double l2(vector<double> point1, vector<double> point2){

    double sum = 0;
    for(int i=0; i<point1.size(); i++) {
        sum += (point1[i]-point2[i])*(point1[i]-point2[i]);
    }
    return sqrt(sum);

}

struct node {
    double dist;
    vector<double> point;

    node(double d, vector<double> v) {
        dist = d;
        point = v;
    }
};

class my_comparator {
    public:
    int operator() (const node& p1, const node& p2) {

        return tie(p1.dist, p1.point) < tie(p2.dist, p2.point);
    }
};

int main (int argc, char** argv) {

    //input data
    int D, N1, N2;
    vector<vector<double> > data;
    ifstream fin(argv[1]);
    ofstream fout("results.txt");
    fin>>D>>N1;
    double temp;
    for(int i=0; i<N1; i++) {
        vector<double> point;
        for(int j=0; j<D; j++) {
            fin>>temp;
            point.push_back(temp);
        }
        data.push_back(point);
    }

    /*
    cout<<"Data: \n";
    for(int i=0; i<N1; i++) {
        for(double d: data[i]) {
            cout<<d<<" ";
        }
        cout << "\n";
        
    }
    */
    fin.close();

    int k;

    cout<<0<<endl;

    string query_file;
    cin>>query_file;
    cin>>k;

    //input queries
    fin.open(query_file);
    fin>>D>>N2;

    //time starts
    // clock_t begin = clock();
	// double second_dist = 0;
	// double hundredth_dist = 0;
    for(int i=0; i<N2; i++) {
        vector<double> query_point;
        for(int j=0; j<D; j++) {
            fin>>temp;
            query_point.push_back(temp);
        }

        //perform knn search
        //sequential scan
        priority_queue<node, vector<node>, my_comparator> pq;

        for(int i=0; i<N1; i++) {
            double dist = l2(query_point, data[i]);
            if(!pq.empty()) {
                if(pq.size()==k && pq.top().dist>dist) {
                    pq.pop();
                    pq.push(node(dist, data[i]));
                }
                else if(pq.size()<k)
                    pq.push(node(dist, data[i]));

            }
            else {
                pq.push(node(dist, data[i]));            
            }
        }

        //order the result
        vector<node> result;
        while (pq.empty() == false) {
            result.push_back(pq.top());
            pq.pop();
        }
        sort(result.begin(), result.end(), my_comparator());

		
		// for(int i=1; i<=100; i++) {
			// if(i==2) {
				// second_dist += result[i-1].dist;
			// }
			// if(i==100) {
				// hundredth_dist += result[i-1].dist;
			// }
		// }
		
		
		
        //print the result
        for(node n: result) {
			// fout<<n.dist<<"** ";
            for(double d: n.point) {
                fout<<d<<" ";
            }
            fout << "\n";
        }
    }
	// cout<<"Second closest dist: "<<second_dist<<endl;
	// cout<<"Hundredth closest dist: "<<hundredth_dist<<endl;

    // clock_t end = clock();
    //time ends
    // double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    // cerr << to_string(elapsed_secs) << endl;

    cout<<1<<endl;

    fin.close();
    fout.close();
    
 
    return 0;
}