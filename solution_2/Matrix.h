

class Matrix {
public:
	~Matrix();

	int* arr = 0;
	int rows;
	int cols;

	void init(int _rows, int _cols);
	int get(int x, int y);
	void set(int x, int y, int value);
	void add(int x, int y, int value);

	void print();


};