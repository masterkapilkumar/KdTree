#include <iostream>
#include <queue>
#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;
 
double l2(vector<double> point1, vector<double> point2){

    double sum = 0;
    for(int i=0; i<point1.size(); i++) {
        double temp = point1[i]-point2[i];
        sum += (temp*temp);
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

struct tree_node{
    vector<double> point;
    tree_node *left;
    tree_node *right;
    int level;
    double partition_point;
    double min_point;
    double max_point;

    tree_node(){
        left = NULL;
        right = NULL;
    }
};

int cur_dim = 0;
int net_dim = 2;
int k;


bool colsort(const vector<double>& a,const vector<double>& b) {
    
    return a[cur_dim] < b[cur_dim];
}

class my_comparator {
    public:
    int operator() (const node& p1, const node& p2) {

        return tie(p1.dist, p1.point) < tie(p2.dist, p2.point);
    }
};


void grow(struct tree_node* root, vector<vector<double>> data, int level){

    if(data.size()==1){

        root->point = data[0];
        // cerr << data[0][0] << " " << data[0][1] << endl;

    }else{

        cur_dim = level;
        sort(data.begin(), data.end(), colsort);

        vector<vector<double>> leftdata(data.begin(), data.begin() + data.size()/2);
        vector<vector<double>> rightdata(data.begin() + data.size()/2, data.end());

        root->level = level;
        root->partition_point = leftdata[leftdata.size()-1][level];
        root->min_point = leftdata[0][level];
        root->max_point = rightdata[rightdata.size()-1][level];
        // cerr << root->partition_point << endl;
        root->left = new tree_node();
        root->right = new tree_node();
        grow(root->left, leftdata, (level+1)%net_dim);
        grow(root->right, rightdata, (level+1)%net_dim);

    }

}

void fill_pq(priority_queue<node, vector<node>, my_comparator> &pq, tree_node* root, vector<double> &qpoint){

    if(root->left==NULL && root->right==NULL){
        
        node point(l2(root->point,qpoint), root->point);
        if (pq.size()<k)
        {
            pq.push(point);
        }else if(pq.top().dist > point.dist){
            pq.pop();
            pq.push(point);
        }

        return;
    }

    int split_dim = root->level;
    // cerr << split_dim << endl;
    double mbr = qpoint[split_dim];
    double root_part = root->partition_point;
    double max_part = root->max_point;
    double min_part =  root->min_point;

    if(pq.size()<k){

        if(mbr<root_part){
            if(root->left !=NULL){
                fill_pq(pq, root->left, qpoint);
            }            
            if(root->right !=NULL){
                fill_pq(pq, root->right, qpoint);
            }                 
        }else{
            if(root->right !=NULL){
                fill_pq(pq, root->right, qpoint);
            }                     
            if(root->left !=NULL){
                fill_pq(pq, root->left, qpoint);
            }                        
        }
        return;

    }

     if(mbr<root_part){

        if(min_part-mbr > pq.top().dist){

            // cerr << "benefit1" << endl;
            return;

        }

        if(root_part-mbr > pq.top().dist){

            // cerr << "benefit2" << endl;

            if(root->left !=NULL){
                fill_pq(pq, root->left, qpoint);
            }
            
        }else{

            if(root->left !=NULL){
                fill_pq(pq, root->left, qpoint);
            }            
            if(root->right !=NULL){
                fill_pq(pq, root->right, qpoint);
            }              
       
        }
     }else{


        if(mbr-max_part > pq.top().dist){

            // cerr << "benefit3" << endl;
            return;

        }

        if(mbr-root_part > pq.top().dist){
            // cerr << "benefit4" << endl;

            if(root->right !=NULL){
                fill_pq(pq, root->right, qpoint);
            }   
        }else{

            if(root->right !=NULL){
                fill_pq(pq, root->right, qpoint);
            }                     
            if(root->left !=NULL){
                fill_pq(pq, root->left, qpoint);
            }                        
            
        }        
     }

}


int main (int argc, char** argv) {

    //input data
    int D, N1, N2;
    vector<vector<double>> data;
    
    ifstream fin(argv[1]);
    fin>>D>>N1;

    net_dim = D;

    ofstream fout("results.txt");

    double temp;
    for(int i=0; i<N1; i++) {
        vector<double> point;

        for(int j=0; j<D; j++) {
            fin>>temp;
            point.push_back(temp);
        }
        data.push_back(point);
    }
    
    fin.close();

    tree_node* root = new tree_node();

    grow(root, data, 0);

    string query_file;
    cout<<0<<endl;
    
    cin>>query_file;
    // query_file = "query_dim20.txt";
    
    cin>>k;
    // k = 20;

    //input queries
    fin.open(query_file);
    fin>>D>>N2;
	
	// double second_dist = 0;
	// double hundredth_dist = 0;

    for(int i=0; i<N2; i++) {
        vector<double> query_point;
        
        for(int j=0; j<D; j++) {
            fin>>temp;
            query_point.push_back(temp);
        }

        priority_queue<node, vector<node>, my_comparator> pq;

        fill_pq(pq, root ,query_point);

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
            for(double d: n.point) {
                fout<<d<<" ";
            }
            fout << "\n";
        }

    }
	
	// cout<<"Second closest dist: "<<second_dist<<endl;
	// cout<<"Hundredth closest dist: "<<hundredth_dist<<endl;

    cout<<1<<endl;
    fin.close();
    fout.close();
    
    return 0;
}