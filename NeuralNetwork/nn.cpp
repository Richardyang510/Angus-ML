#include <bits/stdc++.h>

using namespace std;

#define SIZE 1
#define HIDDEN 100
#define OUT 1
#define MAX_INPUT_SIZE 100000

int gen;
int correct = 0;

double in[SIZE];
double hidden[HIDDEN];
double out[OUT];

double w[SIZE][HIDDEN];
double ow[HIDDEN][OUT];

double delta_ow[HIDDEN][OUT];

double delta_hidden_sum[HIDDEN]; // # of hidden units
double delta_weights[SIZE][HIDDEN];

double ans;

double input[MAX_INPUT_SIZE][SIZE + 1];

double out_error = 0;
double out_error_margin = 0;
double out_sum = 0;

double cross(int b) {
    double out = 0;
    for (int i = 0; i < SIZE; i++) {
        out += in[i] * w[i][b];
        //cout << in[i] << " "  << w[i][b] << " ";
    }
    //cout << endl;
    return out;
}

double f(double x) {
    return (1 / (1 + exp(-(x - 1))));
}

double fprime(double x) {
    return f(x) * (1 - f(x));
}

void forward(void) {
    for (int i = 0; i < HIDDEN; ++i) {
        hidden[i] = f(cross(i));
        //cout << cross(i) << " " << hidden[i] << endl;
    }

    for (int i = 0; i < HIDDEN; i++) {
        out_sum += ow[i][0] * hidden[i];
    }
    out[0] = f(out_sum);


    out_error_margin = ((ans - out[0]));

    cout << "generation " << gen << " | result " << out[0] << " | actual " << ans << " | error " << out_error_margin
         << endl;
}

void back(void) {

    double delta_out_sum = fprime(out_sum) * out_error_margin;

    //cout << delta_out_sum << endl;

    for (int i = 0; i < HIDDEN; i++) {
        delta_ow[i][0] = delta_out_sum / hidden[i];
        // ow[0][i] += delta_ow[0][i];
        //cout << delta_ow[i][0] << endl;
    }


    for (int i = 0; i < HIDDEN; i++) {
        delta_hidden_sum[i] = (delta_out_sum / ow[i][0]) * fprime(hidden[i]);
        //cout << delta_hidden_sum[i] << endl;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < HIDDEN; j++) {
            delta_weights[i][j] = delta_hidden_sum[j] / (in[i]);
            w[i][j] += delta_weights[i][j];
            //cout << w[i][j] << endl;
        }
    }

    for (int i = 0; i < HIDDEN; i++) {
        ow[0][i] += delta_ow[0][i];
        //cout << ow[0][i] << endl;
    }

    if (out[0] == ans) correct++;
    gen++;

}

void train_once(int num) {
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < SIZE; j++) {
            in[j] = input[i][j];
        }
        ans = input[i][SIZE];

        //cout << in[0] << " " << in[1] << " " << ans << endl;

        forward();
        back();
    }
}

int main(void) {

    char c = ' ';


    cin >> c;
    if (c == 'g') {

        ofstream offile2;
        offile2.open("weight.dat");
        cout << "Making Weights" << endl;

        offile2 << 0 << endl;

        for (int i = 0; i < HIDDEN; i++) {
            for (int j = 0; j < OUT; j++) {
                offile2 << 1 << endl;
            }
        }

        for (int i = 0; i < HIDDEN; i++) {
            offile2 << ow[0][i] << endl;
        }

        offile2.close();

    } else {

        ifstream infile1;
        infile1.open("weight.dat");
        cout << "Reading Weights" << endl;

        infile1 >> gen;

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < HIDDEN; j++) {
                infile1 >> w[i][j];
            }
        }

        for (int i = 0; i < HIDDEN; i++) {
            infile1 >> ow[0][i];
        }

        infile1.close();
        cout << "Weights Read" << endl;
    }



    // ===============================================================

    cout << "Reading Training " << endl;

    int num = 0;

    ifstream infile;
    infile.open("sin.dat");

    infile >> num;

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < SIZE + 1; j++) {
            //double x = 0;
            infile >> input[i][j];
            //input[i][j] = x;
        }
    }

    //cout << input[0][0] << " " << input[0][1] << endl; // << " " << input[0][2] << " " << input[0][3] << " " << endl;
    // cout << input[1][0] << " " << input[1][1] << endl;

    infile.close();

    cout << "Begin Training" << endl;

    int num_times = 1;

    /*
    for(int i = 0;i < num_times;i++){
        train_once(num);
    }
    */

    while (scanf("%c", &c)) {
        if (c == 'q') break;
        train_once(num);
    }

    cout << "Training Complete" << endl;

    // ================================================================

    if (remove("weight.dat") != 0)
        perror("Error deleting file");
    else
        cout << "File successfully deleted" << endl;

    ofstream outfile2;
    outfile2.open("weight.dat");

    cout << "Writing Weights" << endl;

    outfile2 << gen << endl;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < HIDDEN; j++) {
            outfile2 << w[i][j] << endl;
        }
    }

    for (int i = 0; i < HIDDEN; i++) {
        outfile2 << ow[i][0] << endl;
    }

    outfile2.close();
    cout << "Weights written" << endl;

    cout << "Correct " << correct << endl;

    cout << "Press any key to exit" << endl;
    cin >> c;

    return 0;
}
