#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

class ErrorNoFile {
    string str = "ErrorNoFile";
public:
    ErrorNoFile(string s) : str(s) {}
    void Message() {
        cout << "ErrorNoFile " << str << endl;
    }
};

class ErrorAlgorytm {
public:
    ErrorAlgorytm() {}
    void Message() {
        cout << "Error in function: switching to Algorithm 2." << endl;
    }
};

string MyDetermineFile(double& x) {
    if (fabs(x) <= 1) return "dat_X_1_1.dat";
    else if (x < -1) {
        x = 1 / x;
        return "dat_X_00_1.dat";
    }
    else if (x > 1) {
        x = 1 / x;
        return "dat_X_1_00.dat";
    }
}

double MyT(double x) throw (ErrorNoFile) {
    try {
        string filename = MyDetermineFile(x);
        ifstream is(filename);
        if (!is) throw ErrorNoFile(filename);

        double xi, Ti, xi1, Ti1, result = 0;
        is >> xi1 >> Ti1;
        if (xi1 == x) result = Ti1;
        else {
            while (!is.eof()) {
                xi = xi1;
                Ti = Ti1;
                is >> xi1 >> Ti1;
                if (xi < x && x < xi1) {
                    result = Ti + (Ti1 - Ti) * (x - xi) / (xi1 - xi);
                    break;
                }
                if (xi1 == x) {
                    result = Ti1;
                    break;
                }
            }
        }
        is.close();
        return result;
    }
    catch (ErrorNoFile& e) {
        throw;
    }
}

double MyU(double x) throw (ErrorNoFile) {
    try {
        string filename = MyDetermineFile(x);
        ifstream is(filename);
        if (!is) throw ErrorNoFile(filename);

        double xi, Ui, xi1, Ui1, result = 0;
        is >> xi1 >> Ui1;
        if (xi1 == x) {
            result = Ui1;
        }
        else {
            while (!is.eof()) {
                xi = xi1;
                Ui = Ui1;
                is >> xi1 >> Ui1;
                if (xi < x && x < xi1) {
                    result = Ui + (Ui1 - Ui) * (x - xi) / (xi1 - xi);
                    break;
                }
                if (xi1 == x) {
                    result = Ui1;
                    break;
                }
            }
        }
        is.close();
        return result;
    }
    catch (ErrorNoFile& e) {
        throw;
    }
}

double MyGold(double x, double y) throw (ErrorAlgorytm) {
    if (x > y && y != 0) {
        return x / y;
    }
    else if (x < y && x != 0) {
        return y / x;
    }
    else {
        throw ErrorAlgorytm();
    }
}

double MyGold1(double x, double y) {
    if (x > y && fabs(y) > 0.1) return x / y;
    else if (x <= y && fabs(x) > 0.1) return y / x;
    else if (x < y && fabs(x) > 0.1) return 0.15;
    else if (y == 0) return 0.1;
}

double MyGlr(double x, double y) throw (ErrorAlgorytm) {
    double exp = sqrt(x * x + y * y - 4);
    if (fabs(x) < 1) return x;
    else if (fabs(x) >= 1 && fabs(y) < 1) return y;
    else if (fabs(x) >= 1 && fabs(y) >= 1 && exp > 0.1) return y / exp;
    else throw ErrorAlgorytm();
}

double MyGlr1(double x, double y) {
    if (fabs(x) < 1) return x;
    else return y;
}

double MySrz(double x, double y, double z) {
    if (x > y) return MyT(x) + MyU(z) - MyT(y);
    else return MyT(y) + MyU(y) - MyU(z);
}

double MyGrs(double x, double y) {
    double p1 = 0.1389 * MySrz(x + y, MyGold(x, y), MyGlr(x, x * y));
    double p2 = 1.8389 * MySrz(x - y, MyGold(y, x / 5), MyGlr(5 * x, x * y));
    double p3 = 0.83 * MySrz(x - 0.9, MyGlr(y, x / 5), MyGold(5 * y, y));
    return p1 + p2 + p3;
}

double MyGrs1(double x, double y) {
    double p1 = 0.14 * MySrz(x + y, MyGold(x, y), MyGlr1(x, x * y));
    double p2 = 1.83 * MySrz(x - y, MyGold(y, x / 5), MyGlr1(4 * x, x * y));
    double p3 = 0.83 * MySrz(x, MyGlr1(y, x / 4), MyGold(4 * y, y));
    return p1 + p2 + p3;
}

double MyFun(double x, double y, double z) throw (ErrorAlgorytm, ErrorNoFile) {
    double temp;
    try {
        temp = x * x * MyGrs(y, z) + y * y * MyGrs(x, z) + 0.33 * x * y * MyGrs(x, z);
    }
    catch (ErrorAlgorytm& e) {
        temp = x * MyGrs1(x, y) + y * MyGrs1(y, z) + z * MyGrs1(z, x);
    }
    catch (ErrorNoFile& e) {
        temp = 1.3498 * z + 2.2362 * y - 2.348 * x * y;
    }
    return temp;
}

int main() {
    double x, y, z, result;
    cout << "Enter x, y, z values: ";
    cin >> x >> y >> z;
    try {
        result = MyFun(x, y, z);
        cout << "Result: " << result << endl;
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}
