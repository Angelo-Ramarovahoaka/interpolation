#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Cinterp {
public:
    Cinterp();
    ~Cinterp(){};
    void display();
    void plot();
    // Méthode pour interpolation linéaire
    void interpLin();
    // Méthode pour interpolation de Lagrange
    float lagrangePol(int i, float x);
    float lagrangeInterpol(float x);
    void interpLagrange();
    // Méthode pour interpolation par spline cubique
    void interpSpline();
    
private:
    vector<float> x_val;
    vector<float> y_val;
    int dim;
};

Cinterp::Cinterp() {
    string tmp;
    ifstream fs("data.txt");

    if (fs.fail()) {
        cout << "Erreur à l'ouverture du fichier" << endl;
    }

    fs >> tmp;
    dim = stoi(tmp);

    for (int i = 0; i < dim; i++) {
        fs >> tmp;
        x_val.push_back(stof(tmp));
        fs >> tmp;
        y_val.push_back(stof(tmp));
    }
}

void Cinterp::interpLin() {
    ofstream fs("interLin.data");
    if (fs.fail()) {
        cout << "Echec creation fichier" << endl;
    } else {
        for (int i = 0; i < x_val.size() - 1; ++i) {
            float x0 = x_val[i];
            float y0 = y_val[i];
            float x1 = x_val[i + 1];
            float y1 = y_val[i + 1];

            // Génération de points interpolés entre x0 et x1
            for (double x = x0; x <= x1; x += 0.1) {
                double y = y0 + (y1 - y0) * (x - x0) / (x1 - x0);
                fs << x << " " << y << endl;
            }
        }
        fs << x_val[dim - 1] << " " << y_val[dim - 1] << endl;
        fs.close();
    }
}

void Cinterp::interpLagrange() {
    ofstream fs("interLagrange.data");
    if (fs.fail()) {
        cout << "Echec creation fichier" << endl;
    } else {
        // Génération de points interpolés entre x0 et x1
        for (float x = x_val[0]; x <= x_val[dim - 1]; x += 0.001) {
            float y = lagrangeInterpol(x);
            fs << x << " " << y << endl;
        }
        fs << x_val[dim - 1] << " " << y_val[dim - 1] << endl;
        fs.close();
    }
}

float Cinterp::lagrangeInterpol(float x) {
    float somme = 0;
    for (int i = 0; i < dim; i++) {
        somme += y_val[i] * lagrangePol(i, x);
    }
    return somme;
}

float Cinterp::lagrangePol(int i, float x) {
    float prod = 1;
    for (int j = 0; j < dim; j++) {
        if (i != j) {
            prod *= (x - x_val[j]) / (x_val[i] - x_val[j]);
        }
    }
    return prod;
}

void Cinterp::interpSpline() {
    int n = dim - 1;
    vector<double> h(n), alpha(n), l(dim), mu(dim), z(dim);
    vector<double> a(dim), b(dim), c(dim), d(dim);

    for (int i = 0; i < dim; i++) {
        a[i] = y_val[i];
    }

    for (int i = 0; i < n; i++) {
        h[i] = x_val[i + 1] - x_val[i];
    }

    for (int i = 1; i < n; i++) {
        alpha[i] = (3 / h[i]) * (a[i + 1] - a[i]) - (3 / h[i - 1]) * (a[i] - a[i - 1]);
    }

    l[0] = 1;
    mu[0] = z[0] = 0;

    for (int i = 1; i < n; i++) {
        l[i] = 2 * (x_val[i + 1] - x_val[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    l[n] = 1;
    z[n] = c[n] = 0;

    for (int j = n - 1; j >= 0; j--) {
        c[j] = z[j] - mu[j] * c[j + 1];
        b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
        d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
    }

    // Écrire les points interpolés dans un fichier
    ofstream fs("interSpline.data");
    if (fs.fail()) {
        cout << "Echec creation fichier" << endl;
    } else {
        for (int i = 0; i < n; i++) {
            for (double x = x_val[i]; x <= x_val[i + 1]; x += 0.1) {
                double dx = x - x_val[i];
                double y = a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
                fs << x << " " << y << endl;
            }
        }
        fs.close();
    }
}

void Cinterp::plot() {
    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe) {
        cerr << "Erreur à l'ouverture de gnuplot" << endl;
        return;
    }

    // Définir le titre du graphe et le tracé
    fprintf(gnuplotPipe, "set title 'Interpolation'\n");
    fprintf(gnuplotPipe, "plot 'interLin.data' with lines title 'Interpolation linéaire',");
    fprintf(gnuplotPipe, "'interLagrange.data' with lines title 'Interpolation de Lagrange' lc 'red',");
    fprintf(gnuplotPipe, "'interSpline.data' with lines title 'Spline cubique' lc 'blue'\n");

    fflush(gnuplotPipe);

    // Fermer le pipe
    pclose(gnuplotPipe);
}

void Cinterp::display() {
    for (int i = 0; i < dim; i++) {
        cout << "x = " << x_val[i] << ", y = " << y_val[i] << endl;
    }
}

int main() {
    Cinterp inter;

    // inter.display();
    inter.interpLin();
    inter.interpLagrange();
    inter.interpSpline();
    inter.plot();

    return 0;
}
