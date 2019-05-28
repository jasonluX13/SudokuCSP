#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct coords{
    int row;
    int col;
};

int size = 9; //9x9 grid
void updateNumBlanks();
int numBlank(vector<vector<vector<int>>> &vect);
void readFile(ifstream& inputfile);
int forwardChecking(vector<vector<vector<int>>> &vect);
int rowConstraints(vector<vector<vector<int>>> &vect, int row, int col);
int colConstraints(vector<vector<vector<int>>> &vect, int row, int col);
int blockConstraints(vector<vector<vector<int>>> &vect, int row, int col);
int remove(vector<vector<vector<int>>> &vect,int row, int col, int num);
int backtrackSearch(vector<vector<vector<int>>> &vect);
coords mrv(vector<vector<vector<int>>> &vect);
void printGrid(const vector<vector<vector<int>>> &grid1, ofstream& outputfile);



vector<int> domain = {1,2,3,4,5,6,7,8,9};
vector<vector<vector<int>>> grid(9,vector<vector<int>>(9, vector<int>(9))); //rows,columns,constraints
vector<vector<vector<int>>> temp1(9,vector<vector<int>>(9, vector<int>(9))); 
int numBlanks = 81;

int main(int argc, char* argv[]){
    if(argc < 3){
        cerr << "Usage: " << argv[0] << " <inputfile> <outputfile> \n";
        return 1;
    }
    ifstream myfile(argv[1]);
    readFile(myfile);
    
    ofstream outputfile(argv[2]);
    forwardChecking(grid);

    backtrackSearch(grid);
    printGrid(temp1,outputfile);
    
    
}

void updateNumBlanks(){
    int blanks = 0;
    for(int row = 0; row < grid.size(); row++){
        for(int col = 0; col < grid[row].size(); col++){
            if(grid[row][col].size() > 1){
                blanks++;
            }
        }
    }
    numBlanks = blanks;
}
int numBlank(vector<vector<vector<int>>> &vect){
    int blanks = 0;
    for(int row = 0; row < vect.size(); row++){
        for(int col = 0; col < vect[row].size(); col++){
            if(vect[row][col].size() > 1){
                blanks++;
            }
        }
    }
    return blanks;
}
void readFile(ifstream& inputfile){
    int a;
    int count = 0;
    while (inputfile >> a){
        if(a == 0){ //blank position
            grid[count/9][count%9] = {1,2,3,4,5,6,7,8,9};
        }else{
            vector<int> v1;
            v1.push_back(a);
            grid[count/9][count%9] = v1;
          
        }
 
        count++;
    }
}

int forwardChecking(vector<vector<vector<int>>> &vect){
    int numFound = 0;
    for(int row = 0; row<vect.size(); row++){
        for(int col = 0; col < vect[row].size(); col++){
            //check row contraints
            numFound += rowConstraints(vect,row,col);
            //check column constraints
            numFound += colConstraints(vect,row,col);
            //check block constraints
            numFound += blockConstraints(vect,row,col);
        }
    }
    
    updateNumBlanks();
    if(numFound>0){ //run forward checking until no more constraints can be eliminated
        forwardChecking(vect);
    }
    return numFound;
}

int backtrackSearch(vector<vector<vector<int>>> &vect){
    int numBlank1 = numBlank(vect);
    coords min_coords = mrv(vect);
    if(min_coords.row ==10){
        if(numBlank1 == 0){
            //printGrid(vect);
            temp1 = vect;
            return 1;
        }
        return 0;
    }
    int row = min_coords.row;
    int col = min_coords.col;

    for(int i = 0; i < vect[min_coords.row][min_coords.col].size();i++){ //iterate through domain values
        vector<vector<vector<int>>> temp(9,vector<vector<int>>(9, vector<int>(9)));
        temp = vect;
        int selected = temp[row][col][i]; //select the next domain value
        //Remove all other values from constraint list
        vector<int> v1;
        v1.push_back(selected);
        temp[row][col] = v1;
        
        if(forwardChecking(temp) != 0){ //Perform forward checking
            numBlank1 = numBlank(temp);
            
            backtrackSearch(temp);
        }
        
    }
    return 0;
}

coords mrv(vector<vector<vector<int>>> &vect){
    coords min_coords;
    int min_size = 10;
    min_coords.row = 10;
    min_coords.col = 10;
    for(int row = 0; row<vect.size(); row++){
        for(int col = 0; col < vect[row].size(); col++){
            if(vect[row][col].size() > 1 && vect[row][col].size() < min_size){
                min_size = vect[row][col].size();
                min_coords.row = row;
                min_coords.col = col;
            }
        }
    }
    return min_coords; //coords are 10,10 if no min values were found
}
int rowConstraints(vector<vector<vector<int>>> &vect, int row, int col){//check row contraints
    int numFound = 0;
    for(int i = 0; i < vect[row].size(); i++){//iterate through row
        if(i != col){
            if(vect[row][i].size() == 1){ //check if number of constraints is 1
                numFound += remove(vect,row,col,vect[row][i][0]); //remove value from constraints
            }
        }
    }
    return numFound;
}

int colConstraints(vector<vector<vector<int>>> &vect, int row, int col){
    int numFound = 0;
    for(int i = 0; i < vect.size(); i++){//iterate through column
        if(i != row){
            if(vect[i][col].size() == 1){ //check if number of constraints is 1
                numFound += remove(vect,row,col,vect[i][col][0]); //remove value from constraints
            }
        }
    }
    return numFound;
}

int blockConstraints(vector<vector<vector<int>>> &vect, int row, int col){
    int numFound = 0;
    //Find coordinates of upperleft corner of block this tile belongs to
    int startX = (row/3) * 3; 
    int startY = (col/3) * 3;
    for(int x = startX; x < startX+3; x++ ){ //Iterates through the block
        for(int y = startY; y < startY+3; y++){
            if(x!=row && y!=col){
                if(vect[x][y].size() == 1){//check if number of constraints is 1
                    numFound += remove(vect,row,col,vect[x][y][0]); //remove value from constraints
                }
            }
        }
    }
    return numFound;
}
int remove(vector<vector<vector<int>>> &vect,int row, int col, int num){ // remove a number from constraint list
    vector<int>::iterator it;
    if(vect[row][col].size() == 0){
        return 0;
    }
    
    if(vect[row][col].size() == 1){
        if(vect[row][col][0] == num){
            return 0;
        }
    }
    for(auto it = vect[row][col].begin(); it != vect[row][col].end(); it++){
        
        if(*it == num){
            vect[row][col].erase(it);
            return 1;
        }
    }
    return 0;
}


void printGrid(const vector<vector<vector<int>>> &grid1,ofstream& outputfile){
    for(int row = 0; row < grid1.size(); row++){
        
        for(int col = 0; col < grid1[row].size(); col++){
            
            for(int constraint = 0; constraint < grid1[row][col].size(); constraint++){
                outputfile << grid1[row][col][constraint] << " ";
            }
            
        }
        outputfile << "\r\n";
    }
}

