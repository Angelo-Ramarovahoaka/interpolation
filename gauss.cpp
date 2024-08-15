#include <iostream>
#include <fstream>
using namespace std;

class GaussMatrix {
private:
    double** matrix;
    int size;

public:
    GaussMatrix(int n);
    ~GaussMatrix();
    void setElement(int row, int col, double value);
    void gaussianElimination();
    void displayMatrix();
};
GaussMatrix::GaussMatrix(int n) {
    size = n;
    matrix = new double*[size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size + 1];
    }
}

GaussMatrix::~GaussMatrix() {
    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void GaussMatrix::setElement(int row, int col, double value) {
    matrix[row][col] = value;
}

void GaussMatrix::gaussianElimination() {
    for (int i = 0; i < size; i++) {
        // Find pivot row
        int pivotRow = i;
        for (int j = i + 1; j < size; j++) {
            if (abs(matrix[j][i]) > abs(matrix[pivotRow][i])) {
                pivotRow = j;
            }
        }

        // Swap rows
        double* temp = matrix[i];
        matrix[i] = matrix[pivotRow];
        matrix[pivotRow] = temp;

        // Perform row operations to eliminate variables
        for (int j = i + 1; j < size; j++) {
            double factor = matrix[j][i] / matrix[i][i];
            for (int k = i; k < size + 1; k++) {
                matrix[j][k] -= factor * matrix[i][k];
            }
        }
    }

    // Back substitution to find solution
    double* solution = new double[size];
    for (int i = size - 1; i >= 0; i--) {
        solution[i] = matrix[i][size] / matrix[i][i];
        for (int j = i - 1; j >= 0; j--) {
            matrix[j][size] -= matrix[j][i] * solution[i];
        }
    }

    // Print the solution
    std::cout << "Solution: ";
    for (int i = 0; i < size; i++) {
        std::cout << solution[i] << " ";
    }
    std::cout << std::endl;

    delete[] solution;
}

void GaussMatrix::displayMatrix(){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++)
        {
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}
int main() {
    float data,value;
    ifstream fs("./dataMatrice.txt");
    if (fs.fail())
        std:cout<<"Fichier non trouve";

    fs >> data;
    // std::cout <<"data: " <<data<< std::endl;
    GaussMatrix gauss(data);
    for (int i = 0; i < data; i++) {
        // std::cout<<"i: "<<i<<std::endl;
        for (int j = 0; j < data + 1; j++) {
            fs >> value;
            // std::cout << value << std::endl;
            gauss.setElement(i, j, value);
        }
    }
    gauss.displayMatrix();
    gauss.gaussianElimination();

    return 0;
}