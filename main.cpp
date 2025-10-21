#include <iostream>
#include <iomanip>

using namespace std;

// ============ CLASE BASE ABSTRACTA ============
template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}
    virtual ~MatrizBase() {}
    
    // Métodos virtuales puros
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual T getElemento(int i, int j) const = 0;
    
    // Getters
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
    
    // Sobrecarga del operador +
    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return sumar(otra);
    }
};

// ============ MATRIZ DINÁMICA ============
template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T** _datos;
    
    void asignarMemoria() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = T();
            }
        }
    }
    
    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; i++) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }

public:
    // Constructor
    MatrizDinamica(int filas, int columnas) : MatrizBase<T>(filas, columnas), _datos(nullptr) {
        asignarMemoria();
    }
    
    // Destructor
    ~MatrizDinamica() {
        cout << "Liberando memoria de Matriz Dinamica..." << endl;
        liberarMemoria();
    }
    
    // Constructor de copia (Deep Copy)
    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas), _datos(nullptr) {
        asignarMemoria();
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }
    
    // Operador de asignación (Deep Copy)
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if (this != &otra) {
            liberarMemoria();
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            asignarMemoria();
            for (int i = 0; i < this->_filas; i++) {
                for (int j = 0; j < this->_columnas; j++) {
                    _datos[i][j] = otra._datos[i][j];
                }
            }
        }
        return *this;
    }
    
    // Constructor de movimiento
    MatrizDinamica(MatrizDinamica<T>&& otra) noexcept : MatrizBase<T>(otra._filas, otra._columnas), _datos(otra._datos) {
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }
    
    // Operador de asignación de movimiento
    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& otra) noexcept {
        if (this != &otra) {
            liberarMemoria();
            this->_filas = otra._filas;
            this->_columnas = otra._columnas;
            _datos = otra._datos;
            otra._datos = nullptr;
            otra._filas = 0;
            otra._columnas = 0;
        }
        return *this;
    }
    
    void cargarValores() override {
        cout << "Ingrese los valores para la matriz (" << this->_filas << "x" << this->_columnas << "):" << endl;
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                cout << "Elemento [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }
    
    void setElemento(int i, int j, T valor) {
        if (i >= 0 && i < this->_filas && j >= 0 && j < this->_columnas) {
            _datos[i][j] = valor;
        }
    }
    
    T getElemento(int i, int j) const override {
        if (i >= 0 && i < this->_filas && j >= 0 && j < this->_columnas) {
            return _datos[i][j];
        }
        return T();
    }
    
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Las dimensiones no coinciden para la suma." << endl;
            return nullptr;
        }
        
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
        
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                T suma = _datos[i][j] + otra.getElemento(i, j);
                resultado->setElemento(i, j, suma);
            }
        }
        
        return resultado;
    }
    
    void imprimir() const override {
        for (int i = 0; i < this->_filas; i++) {
            cout << "| ";
            for (int j = 0; j < this->_columnas; j++) {
                cout << fixed << setprecision(1) << _datos[i][j];
                if (j < this->_columnas - 1) cout << " | ";
            }
            cout << " |" << endl;
        }
    }
};

// ============ MATRIZ ESTÁTICA ============
template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                _datos[i][j] = T();
            }
        }
    }
    
    void cargarValores() override {
        cout << "Ingrese los valores para la matriz (" << M << "x" << N << "):" << endl;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                cout << "Elemento [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }
    
    void setElemento(int i, int j, T valor) {
        if (i >= 0 && i < M && j >= 0 && j < N) {
            _datos[i][j] = valor;
        }
    }
    
    T getElemento(int i, int j) const override {
        if (i >= 0 && i < M && j >= 0 && j < N) {
            return _datos[i][j];
        }
        return T();
    }
    
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (M != otra.getFilas() || N != otra.getColumnas()) {
            cout << "Error: Las dimensiones no coinciden para la suma." << endl;
            return nullptr;
        }
        
        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();
        
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                T suma = _datos[i][j] + otra.getElemento(i, j);
                resultado->setElemento(i, j, suma);
            }
        }
        
        return resultado;
    }
    
    void imprimir() const override {
        for (int i = 0; i < M; i++) {
            cout << "| ";
            for (int j = 0; j < N; j++) {
                cout << fixed << setprecision(1) << _datos[i][j];
                if (j < N - 1) cout << " | ";
            }
            cout << " |" << endl;
        }
    }
};

// ============ PROGRAMA PRINCIPAL ============
int main() {
    cout << "--- Sistema generico de Algebra Lineal ---" << endl << endl;
    cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << endl << endl;
    
    // 1. Creación de Matriz Dinámica A
    cout << "// 1. Creacion de Matriz Dinamica A (3x2)..." << endl;
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    
    // Cargando valores predefinidos manualmente
    MatrizDinamica<float>* A_din = dynamic_cast<MatrizDinamica<float>*>(A);
    A_din->setElemento(0, 0, 1.5);
    A_din->setElemento(0, 1, 2.0);
    A_din->setElemento(1, 0, 0.0);
    A_din->setElemento(1, 1, 1.0);
    A_din->setElemento(2, 0, 4.5);
    A_din->setElemento(2, 1, 3.0);
    
    cout << "A =" << endl;
    A->imprimir();
    cout << endl;
    
    // 2. Creación de Matriz Estática B
    cout << "// 2. Creacion de Matriz Estatica B (3x2)..." << endl;
    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    
    MatrizEstatica<float, 3, 2>* B_est = dynamic_cast<MatrizEstatica<float, 3, 2>*>(B);
    B_est->setElemento(0, 0, 0.5);
    B_est->setElemento(0, 1, 1.0);
    B_est->setElemento(1, 0, 2.0);
    B_est->setElemento(1, 1, 3.0);
    B_est->setElemento(2, 0, 1.0);
    B_est->setElemento(2, 1, 1.0);
    
    cout << "B =" << endl;
    B->imprimir();
    cout << endl;
    
    // 3. Operación Polimórfica (Suma usando operador +)
    cout << "// 3. Operacion Polimorfica (Suma)" << endl;
    cout << "SUMANDO: Matriz C = A + B ..." << endl;
    cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)" << endl << endl;
    
    // Usando el operador + sobrecargado
    MatrizBase<float>* C = *A + *B;
    
    if (C != nullptr) {
        cout << "Matriz Resultado C (3x2, Tipo FLOAT):" << endl;
        C->imprimir();
        cout << endl;
    }
    
    // 4. Limpieza de memoria
    cout << ">> Demostracion de Limpieza de Memoria <<" << endl;
    
    if (C != nullptr) {
        cout << "Llamando al destructor de C..." << endl;
        delete C;
    }
    
    cout << "Llamando al destructor de A..." << endl;
    delete A;
    
    cout << "Llamando al destructor de B..." << endl;
    delete B;
    
    cout << "Sistema cerrado." << endl << endl;
    
    // ============================================
    // DEMOSTRACIÓN CON TIPO INT
    // ============================================
    cout << ">> Demostracion de Genericidad (Tipo INT) <<" << endl << endl;
    
    cout << "// Creacion de Matrices Dinamicas con tipo INT" << endl;
    MatrizBase<int>* M1 = new MatrizDinamica<int>(2, 3);
    
    MatrizDinamica<int>* M1_din = dynamic_cast<MatrizDinamica<int>*>(M1);
    M1_din->setElemento(0, 0, 5);
    M1_din->setElemento(0, 1, 10);
    M1_din->setElemento(0, 2, 15);
    M1_din->setElemento(1, 0, 20);
    M1_din->setElemento(1, 1, 25);
    M1_din->setElemento(1, 2, 30);
    
    cout << "M1 =" << endl;
    M1->imprimir();
    cout << endl;
    
    MatrizBase<int>* M2 = new MatrizDinamica<int>(2, 3);
    
    MatrizDinamica<int>* M2_din = dynamic_cast<MatrizDinamica<int>*>(M2);
    M2_din->setElemento(0, 0, 1);
    M2_din->setElemento(0, 1, 2);
    M2_din->setElemento(0, 2, 3);
    M2_din->setElemento(1, 0, 4);
    M2_din->setElemento(1, 1, 5);
    M2_din->setElemento(1, 2, 6);
    
    cout << "M2 =" << endl;
    M2->imprimir();
    cout << endl;
    
    cout << "SUMANDO: M3 = M1 + M2 ..." << endl;
    MatrizBase<int>* M3 = *M1 + *M2;
    
    if (M3 != nullptr) {
        cout << "M3 =" << endl;
        M3->imprimir();
        cout << endl;
    }
    
    cout << "Liberando memoria INT..." << endl;
    delete M3;
    delete M1;
    delete M2;
    
    cout << "Programa finalizado correctamente." << endl;
    
    return 0;
}