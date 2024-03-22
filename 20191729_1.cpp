#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class VECTOR {
public:
    float x;
    float y;
    float z;

    float Magnitude() {
        return sqrt(x * x + y * y + z * z);
    }

    VECTOR Add(VECTOR v) {
        VECTOR result;
        result.x = x + v.x;
        result.y = y + v.y;
        result.z = z + v.z;
        return result;
    }

    float InnerProduct(VECTOR v) {
        return x * v.x + y * v.y + z * v.z;
    }

    VECTOR CrossProduct(VECTOR v) {
        VECTOR result;
        result.x = y * v.z - z * v.y;
        result.y = z * v.x - x * v.z;
        result.z = x * v.y - y * v.x;
        return result;
    }
};
class MATRIX {
public:
    vector<vector<float>> elements;

    MATRIX(int rows, int cols) {
        elements.resize(rows, vector<float>(cols, 0.0));
    }

    void SetElement(int row, int col, float value) {
        elements[row][col] = value;
    }

    MATRIX Add(MATRIX m) {
        int thisRows = elements.size();
        int thisCols = elements[0].size();
        int mRows = m.elements.size();
        int mCols = m.elements[0].size();

        if (thisRows != mRows || thisCols != mCols) {
            throw "행렬 덧셈이 불가능합니다.";
        }

        MATRIX result(thisRows, thisCols);
        for (int i = 0; i < thisRows; i++) {
            for (int j = 0; j < thisCols; j++) {
                result.elements[i][j] = elements[i][j] + m.elements[i][j];
            }
        }
        return result;
    }

    MATRIX Multiply(MATRIX m) {
        int thisRows = elements.size();
        int thisCols = elements[0].size();
        int mRows = m.elements.size();
        int mCols = m.elements[0].size();

        if (thisCols != mRows) {
            throw "행렬 곱셈이 불가능합니다.";
        }

        MATRIX result(thisRows, mCols);
        for (int i = 0; i < thisRows; i++) {
            for (int j = 0; j < mCols; j++) {
                for (int k = 0; k < thisCols; k++) {
                    result.elements[i][j] += elements[i][k] * m.elements[k][j];
                }
            }
        }
        return result;
    }

    MATRIX Transpose() {
        int thisRows = elements.size();
        int thisCols = elements[0].size();

        MATRIX result(thisCols, thisRows);
        for (int i = 0; i < thisRows; i++) {
            for (int j = 0; j < thisCols; j++) {
                result.elements[j][i] = elements[i][j];
            }
        }
        return result;
    }
};

int main() {
    ifstream readfile("input.txt");
    ofstream writefile("output_20191729.txt");

    if (readfile.is_open() && writefile.is_open()) {
        string target;
        while (readfile >> target) {
            if (target == "vector") {
                string operation;
                float x1, y1, z1, x2, y2, z2;
                readfile >> operation;
                readfile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
                VECTOR v1 = {x1, y1, z1};
                VECTOR v2 = {x2, y2, z2};

                if (operation == "mag") {
                    writefile << "scalar " << v1.Magnitude() << endl;
                } else if (operation == "add") {
                    VECTOR result = v1.Add(v2);
                    writefile << "vector " << result.x << " " << result.y << " " << result.z << endl;
                } else if (operation == "inner") {
                    float result = v1.InnerProduct(v2);
                    writefile << "scalar " << result << endl;
                } else if (operation == "cross") {
                    VECTOR result = v1.CrossProduct(v2);
                    writefile << "vector " << result.x << " " << result.y << " " << result.z << endl;
                }
            }



             else if (target == "matrix") {
                string operation;
                int rows1,cols1,rows2,cols2;

                readfile>>operation>>rows1>>cols1;
                MATRIX m1(rows1, cols1);
                if (operation == "add") {
                    for (int i = 0; i < rows1; i++) {
                        for (int j = 0; j < cols1; j++) {
                            float value;
                            readfile >> value;
                            m1.SetElement(i, j, value);
                    }
                }
                    readfile>>rows2>>cols2;
                    if(rows1!=rows2||cols1==cols2){
                        cout<<"다른크기 행렬끼리 덧셈불가능";
                        break;
                    }
                    MATRIX m2(rows2, cols2);
                    for (int i = 0; i < rows2; i++) {
                        for (int j = 0; j < cols2; j++) {
                            float value;
                            readfile >> value;
                            m2.SetElement(i, j, value);
                        }
                    }
                    MATRIX result = m1.Add(m2);
                    writefile << "matrix" << endl<<rows1<<" "<<cols1<<endl;
                    for (int i = 0; i < rows1; i++) {
                        for (int j = 0; j < cols1; j++) {
                            writefile << result.elements[i][j] << " ";
                            }
                        writefile << endl;
                }
                    }
            else if (operation == "mult") {
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                    float value;
                    readfile >> value;
                    m1.SetElement(i, j, value);
                        }
                    }
                readfile>>rows2>>cols2;
                MATRIX m2(rows2, cols2);
                for (int i = 0; i < rows2; i++) {
                    for (int j = 0; j < cols2; j++) {
                        float value;
                        readfile >> value;
                        m2.SetElement(i, j, value);
                }
            }
            // 행렬 곱셈 연산 수행
            MATRIX result = m1.Multiply(m2);
            // 결과 파일에 쓰기
            writefile << "matrix" << endl<<rows1<<" "<<cols2<<endl;
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    writefile << result.elements[i][j] << " ";
                }
                    writefile << endl;
            }
        }
            else if (operation == "trans") {
                MATRIX m(rows1, cols1);
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        float value;
                        readfile >> value;
                        m.SetElement(i, j, value);
                    }
                }
                MATRIX result = m.Transpose();
                    // 결과 파일에 쓰기
                writefile << "matrix" << endl<<cols1<<" "<<rows1<<endl;
                for (int i = 0; i < cols1; i++) {
                    for (int j = 0; j < rows1; j++) {
                        writefile << result.elements[i][j] << " ";
                    }
                    writefile << endl;
    }
}
        else cout<<"not matched!";

            }
        }
        readfile.close();
        writefile.close();
    } else {
        cout << "파일을 열 수 없습니다." << endl;
    }

    return 0;
}

