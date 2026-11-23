#include <iostream>
#include <limits>
using namespace std;

// Funciones que estan en los otros 3 archivos
void moduloInventario();
void moduloCompras();
void moduloControlObra();

int main() {
    int opcion = -1;
    do {
        cout << "\n==================================================\n";
        cout << "   ATLANTIC FONTANA CONSTRUCTORA - PANEL PRINCIPAL\n";
        cout << "==================================================\n";
        cout << "1. Inventario y almacen\n";
        cout << "2. Compras y proveedores\n";
        cout << "3. Control de obra y reportes\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            if (cin.eof()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
        }

        switch (opcion) {
            case 1: moduloInventario();  break;
            case 2: moduloCompras();     break;
            case 3: moduloControlObra(); break;
            case 0: cout << "Saliendo del sistema...\n"; break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
    return 0;
}
