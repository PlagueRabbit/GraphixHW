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
        //벡터의 크기는 각 성분의 제곱들을 더하고 루트씌운 값
    }

    VECTOR Add(VECTOR v) {
        //더하는 피연산자 벡터를 매개변수로 가져와서 더하도록 함
        VECTOR result;//더한 결과는 벡터
        result.x = x + v.x;
        result.y = y + v.y;
        result.z = z + v.z;
        return result;
    }

    float InnerProduct(VECTOR v) {
        //벡터의 내적은 두 벡터의 각 같은 축 성분끼리의 곱
        return x * v.x + y * v.y + z * v.z;
    }

    VECTOR CrossProduct(VECTOR v) {
        //벡터의 외적은 𝒏=𝒂×𝒃=((𝒂_𝒚 𝒃_𝒛−𝒂_𝒛 𝒃_𝒚 ),(𝒂_𝒛 𝒃_𝒙−𝒂_𝒙 𝒃_𝒛 ),(𝒂_𝒙 𝒃_𝒚−𝒂_𝒚 𝒃_𝒙 ))
        VECTOR result;
        result.x = y * v.z - z * v.y;
        result.y = z * v.x - x * v.z;
        result.z = x * v.y - y * v.x;
        return result;
    }
};
class MATRIX {
public:
    //행렬의 원소들을 vector 자료구조를 사용하여 다룬다. 원소는 정수
    //2차원 행렬일것이므로 vector < vector <float> > 이런식으로 선언
    vector<vector<float>> elements;
    //행렬 생성자. vector의 resize함수를 써서 행 크기만큼의 공간을 선언하고 빈공간엔 vector을 열 개수만큼 넣음
    MATRIX(int rows, int cols) {
        elements.resize(rows, vector<float>(cols, 0.0));
    }
    //행렬의 값을 set하는 함수
    void SetElement(int row, int col, float value) {
        elements[row][col] = value;
    }

    MATRIX Add(MATRIX m) {
        //this는 현재 행렬이고, 행렬의 행 크기와 열 크기를 vector의 원소 갯수로 구해서 넣는다.
        int thisRows = elements.size();
        int thisCols = elements[0].size();
        //m은 피연산 행렬이다. this와 m행렬끼리 더할거다.
        int mRows = m.elements.size();
        int mCols = m.elements[0].size();

        //행렬의 덧셈은 같은 크기의 행렬끼리만 가능하므로 예외처리
        if (thisRows != mRows || thisCols != mCols) {
            throw "행렬 덧셈이 불가능합니다.";
        }

        MATRIX result(thisRows, thisCols);
        //2중 for문으로 결과행렬에 더한 값을 넣기
        for (int i = 0; i < thisRows; i++) {
            for (int j = 0; j < thisCols; j++) {
                result.elements[i][j] = elements[i][j] + m.elements[i][j];
            }
        }
        return result;
    }

    MATRIX Multiply(MATRIX m) {
        //add에서 설명했으므로 생략
        int thisRows = elements.size();
        int thisCols = elements[0].size();
        int mRows = m.elements.size();
        int mCols = m.elements[0].size();

        //행렬의 곱셈이 성립하려면 첫 행렬의 행과 두번째 행렬의 열의 크기가 같아야됨. 예외처리
        if (thisCols != mRows) {
            throw "행렬 곱셈이 불가능합니다.";
        }

        MATRIX result(thisRows, mCols);
        //행렬의 곱은 행 곱하기 열들의 합
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
        //전치행렬은 행렬의 행과 열을 전치해서 원소의 내용을 바꿔치기한다. 만약 정방행렬이면 대각선원소는 그대로
        for (int i = 0; i < thisRows; i++) {
            for (int j = 0; j < thisCols; j++) {
                result.elements[j][i] = elements[i][j];
            }
        }
        return result;
    }
};

int main() {
    //ifstream과 ofstream을 이용해 파일입출력
    ifstream readfile("input.txt");
    ofstream writefile("output_20191729.txt");
    writefile.precision(1);
    writefile.setf(ios_base:: fixed, ios_base:: floatfield);
    if (readfile.is_open() && writefile.is_open()) {
        string target;
        while (readfile >> target) {
            //대상을 읽고 vector일경우와 matrix일 경우 분기를 다르게 설정
            if (target == "vector") {
                string operation;//연산자
                float x1, y1, z1, x2, y2, z2;//두 벡터의 원소들
                readfile >> operation;
                readfile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
                VECTOR v1 = {x1, y1, z1};
                VECTOR v2 = {x2, y2, z2};

                //연산자 따라서 분기 다르게.
                if (operation == "mag") {//mag면 벡터의 크기
                    writefile << "scalar "<<endl<< v1.Magnitude() << endl;
                } else if (operation == "add") {//add는 벡터의 합
                    VECTOR result = v1.Add(v2);
                    writefile << "vector "<<endl<< result.x << " " << result.y << " " << result.z << endl;
                } else if (operation == "inner") {//inner은 벡터의 내적
                    float result = v1.InnerProduct(v2);
                    writefile << "scalar "<<endl<<result << endl;
                } else if (operation == "cross") {//cross는 벡터의 외적
                    VECTOR result = v1.CrossProduct(v2);
                    writefile << "vector "<<endl<<result.x << " " << result.y << " " << result.z << endl;
                }
            }



             else if (target == "matrix") {
                string operation;
                int rows1,cols1,rows2,cols2;

                readfile>>operation>>rows1>>cols1;//첫 연산자와, 첫 행렬의 열과 크기까지만 일단 읽음
                MATRIX m1(rows1, cols1);//첫번째 행렬 선언
                if (operation == "add") {//add면 행렬의 합
                    //2중 for문으로 1행은 열의 갯수만큼 읽도록
                    for (int i = 0; i < rows1; i++) {
                        for (int j = 0; j < cols1; j++) {
                            float value;
                            readfile >> value;//ifstream으로 행렬의 원소를 읽어옴
                            m1.SetElement(i, j, value);//읽은걸 각 행렬의 (i,j)번째 원소에 set
                    }
                }
                    //2번째 행렬의 크기를 읽기
                    readfile>>rows2>>cols2;
                    MATRIX m2(rows2, cols2);
                    for (int i = 0; i < rows2; i++) {
                        for (int j = 0; j < cols2; j++) {
                            float value;
                            readfile >> value;
                            m2.SetElement(i, j, value);
                        }
                    }
                    //2번째 행렬을 매개변수로 해서 행렬클래스의 add함수 사용하여 더하고 그 결과 벡터를 리턴
                    MATRIX result = m1.Add(m2);
                    // 결과 파일에 쓰기
                    writefile << "matrix" << endl<<rows1<<" "<<cols1<<endl;
                    for (int i = 0; i < rows1; i++) {
                        for (int j = 0; j < cols1; j++) {
                            writefile << result.elements[i][j] << " ";
                            }
                        writefile << endl;
                }
                    }
            else if (operation == "mult") {//mult는 행렬의 곱
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
        else cout<<"not matched!";//없는 연산자 쓸 경우

            }
        }
        readfile.close();
        writefile.close();
    } else {
        cout << "파일을 열 수 없습니다." << endl;
    }

    return 0;
}

