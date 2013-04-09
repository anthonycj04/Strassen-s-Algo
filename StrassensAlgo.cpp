#include <iostream>

using namespace std;

template<typename T>
class matrix{
	template<typename U>
	friend ostream& operator<<(ostream& outStream, const matrix<U>& _matrix);
private:
	T **elements;
	int size;

	void initializeElements(int size, T initialValue = (T)0){
		this->size = size;
		elements = new T*[size];
		for (int i = 0; i < size; i++){
			elements[i] = new T[size];
			for (int j = 0; j < size; j++)
				elements[i][j] = initialValue;
		}
	}

	void deleteElements(){
		if (size > 0){
			for (int i = 0; i < size; i++)
				delete [] elements[i];
			delete [] elements;
		}
	}

public:
	matrix<T>& operator=(const matrix<T>& _matrix){
		if (this != &_matrix){
			deleteElements();
			initializeElements(_matrix.getSize());
			for (int i = 0; i < size; i++){
				for (int j = 0; j < size; j++)
					elements[i][j] = _matrix[i][j];
			}
		}
		return *this;
	}

	matrix(int size = 2){
		initializeElements(size);
	}

	matrix(int size, T initialValue){
		initializeElements(size, initialValue);
	}

	matrix(const matrix<T>& _matrix){
		initializeElements(_matrix.getSize());
		*this = _matrix;
	}

	~matrix(){
		deleteElements();
	}

	int getSize() const{
		return size;
	}

	void resize(int size){
		deleteElements();
		initializeElements(size);
	}

	void getSubArrays(matrix<T>& matrix11, matrix<T>& matrix12, matrix<T>& matrix21, matrix<T>& matrix22) const{
		int halfSize = size % 2?size / 2 + 1:size / 2;
		matrix11.resize(halfSize);
		matrix12.resize(halfSize);
		matrix21.resize(halfSize);
		matrix22.resize(halfSize);
		for (int i = 0; i < halfSize; i++){
			for (int j = 0; j < halfSize; j++){
				matrix11[i][j] = elements[i][j];
				matrix12[i][j] = (j + halfSize) < size?elements[i][j + halfSize]:0;
				matrix21[i][j] = (i + halfSize) < size?elements[i + halfSize][j]:0;
				matrix22[i][j] = (i + halfSize) < size && (j + halfSize) < size?elements[i + halfSize][j + halfSize]:0;
			}
		}
	}

	void combineArrays(const matrix<T>& C11, const matrix<T>& C12, const matrix<T>& C21, const matrix<T>& C22){
		int halfSize = size % 2?size / 2 + 1:size / 2;
		for (int i = 0; i < halfSize; i++){
			for (int j = 0; j < halfSize; j++){
				elements[i][j] = C11[i][j];
				if (j + halfSize < size)
					elements[i][j + halfSize] = C12[i][j];
				if (i + halfSize < size)
					elements[i + halfSize][j] = C21[i][j];
				if (j + halfSize < size && i + halfSize < size)
					elements[i + halfSize][j + halfSize] = C22[i][j];
			}
		}
	}

	T* operator[](int index) const{
		return elements[index]; 
	}

	matrix<T>& operator+=(const matrix<T>& _matrix){
		if (this->size != _matrix.getSize())
			cerr << "error!!!" << endl;
		else{
			for (int i = 0; i < size; i++){
				for (int j = 0; j < size; j++){
					elements[i][j] += _matrix[i][j];
				}
			}
		}
	}

	matrix<T>& operator-=(const matrix<T>& _matrix){
		if (this->size != _matrix.getSize())
			cerr << "error!!!" << endl;
		else{
			for (int i = 0; i < size; i++){
				for (int j = 0; j < size; j++){
					elements[i][j] -= _matrix[i][j];
				}
			}
		}
	}

	matrix<T>& operator*=(const matrix<T>& _matrix);
	// matrix<T>& operator*=(const matrix<T>& _matrix){
	// 	if (this->size != _matrix.getSize())
	// 		cerr << "error!!!" << endl;
	// 	else{
	// 		for (int i = 0; i < size; i++){
	// 			for (int j = 0; j < size; j++){
	// 				elements[i][j] -= _matrix[i][j];
	// 			}
	// 		}
	// 	}
	// }

	matrix<T>& operator^=(const matrix<T>& _matrix){
		if (this->size != _matrix.getSize())
			cerr << "error!!!" << endl;
		else{
			matrix<T> tempMatrix(size);
			for (int i = 0; i < size; i++){
				for (int j = 0; j < size; j++){
					for (int k = 0; k < size; k++)
					tempMatrix[i][j] += elements[i][k] * _matrix[k][j];
				}
			}
			*this = tempMatrix;
		}
	}

	const matrix<T> operator+(const matrix<T>& _matrix) const{
		matrix<T> tempMatrix = *this;
		tempMatrix += _matrix;
		return tempMatrix;
	}

	const matrix<T> operator-(const matrix<T>& _matrix) const{
		matrix<T> tempMatrix = *this;
		tempMatrix -= _matrix;
		return tempMatrix;
	}

	const matrix<T> operator*(const matrix<T>& _matrix) const{
		matrix<T> tempMatrix = *this;
		tempMatrix *= _matrix;
		return tempMatrix;
	}

	const matrix<T> operator^(const matrix<T>& _matrix) const{
		matrix<T> tempMatrix = *this;
		tempMatrix ^= _matrix;
		return tempMatrix;
	}
};

template<typename T>
matrix<T>& matrix<T>::operator*=(const matrix<T>& _matrix){
	if (this->size != _matrix.getSize() || size < 1)
		cerr << "error!!!" << endl;
	else{
		if (size == 1)
			elements[0][0] *= _matrix[0][0];
		else if (size == 2){
			T S[10], P[7];
			S[0] = _matrix[0][1] - _matrix[1][1];
			S[1] = elements[0][0] + elements[0][1];
			S[2] = elements[1][0] + elements[1][1];
			S[3] = _matrix[1][0] - _matrix[0][0];
			S[4] = elements[0][0] + elements[1][1];
			S[5] = _matrix[0][0] + _matrix[1][1];
			S[6] = elements[0][1] - elements[1][1];
			S[7] = _matrix[1][0] + _matrix[1][1];
			S[8] = elements[0][0] - elements[1][0];
			S[9] = _matrix[0][0] + _matrix[0][1];

			P[0] = elements[0][0] * S[0];
			P[1] = S[1] * _matrix[1][1];
			P[2] = S[2] * _matrix[0][0];
			P[3] = elements[1][1] * S[3];
			P[4] = S[4] * S[5];
			P[5] = S[6] * S[7];
			P[6] = S[8] * S[9];

			elements[0][0] = P[4] + P[3] - P[1] + P[5];
			elements[0][1] = P[0] + P[1];
			elements[1][0] = P[2] + P[3];
			elements[1][1] = P[4] + P[0] - P[2] - P[6];
		}
		else{
			// matrix<T> S[10], P[7], A[2][2], B[2][2], C[2][2];
			matrix<T> P[7], A[2][2], B[2][2];
			// type:
		// 	0: 11, 1: 12, 2: 21, 3: 22
			getSubArrays(A[0][0], A[0][1], A[1][0], A[1][1]);
			_matrix.getSubArrays(B[0][0], B[0][1], B[1][0], B[1][1]);
			// S[0] = B[0][1] - B[1][1];
			// S[1] = A[0][0] + A[0][1];
			// S[2] = A[1][0] + A[1][1];
			// S[3] = B[1][0] - B[0][0];
			// S[4] = A[0][0] + A[1][1];
			// S[5] = B[0][0] + B[1][1];
			// S[6] = A[0][1] - A[1][1];
			// S[7] = B[1][0] + B[1][1];
			// S[8] = A[0][0] - A[1][0];
			// S[9] = B[0][0] + B[0][1];

			P[0] = A[0][0] * (B[0][1] - B[1][1]);
			P[1] = (A[0][0] + A[0][1]) * B[1][1];
			P[2] = (A[1][0] + A[1][1]) * B[0][0];
			P[3] = A[1][1] * (B[1][0] - B[0][0]);
			P[4] = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
			P[5] = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);
			P[6] = (A[0][0] - A[1][0]) * (B[0][0] + B[0][1]);

			// P[0] = A[0][0] * S[0];
			// P[1] = S[1] * B[1][1];
			// P[2] = S[2] * B[0][0];
			// P[3] = A[1][1] * S[3];
			// P[4] = S[4] * S[5];
			// P[5] = S[6] * S[7];
			// P[6] = S[8] * S[9];

			// C[0][0] = P[4] + P[3] - P[1] + P[5];
			// C[0][1] = P[0] + P[1];
			// C[1][0] = P[2] + P[3];
			// C[1][1] = P[4] + P[0] - P[2] - P[6];
			// combineArrays(C[0][0], C[0][1], C[1][0], C[1][1]);
			combineArrays(P[4] + P[3] - P[1] + P[5],
							P[0] + P[1],
							P[2] + P[3],
							P[4] + P[0] - P[2] - P[6]);
		}
	}
	return *this;
};

template<typename T>
ostream& operator<<(ostream& outStream, const matrix<T>& _matrix){
	for (int i = 0; i < _matrix.size; i++){
		for (int j = 0; j < _matrix.size; j++)
			outStream << _matrix.elements[i][j] << " ";
		outStream << endl;
	}
	outStream << endl;
	return outStream;
}

int main(int argc, char*argv[]){
	// srand(time(NULL));
	// const int n = 10;
	// matrix<int> A(n), B(n);
	// for (int i = 0; i < n; i++){
	// 	for (int j = 0; j < n; j++){
	// 		A[i][j] = rand() % 5;
	// 		B[i][j] = rand() % 5;
	// 	}
	// }
	// cout << "Matrix A\n" << A;
	// cout << "Matrix B\n" << B;
	// cout << "Matrix AxB\n" << A*B;
	// cout << "Matrix A^B\n" << (A^B);
	// return 0;

	// matrix<int> A(200, 1), B(200, 2);
	matrix<int> A(10000, 1), B(10000, 2);
	clock_t start = clock();
	matrix<int> C(A * B);
	// matrix<int> C(A ^ B);
	clock_t finish = clock();
	cout << (double) (finish - start) / CLOCKS_PER_SEC;
	cout << " seconds\n";
	return 0;
}
