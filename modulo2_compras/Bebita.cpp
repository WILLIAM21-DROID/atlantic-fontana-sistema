#include <iostream>   
#include <iomanip>    
#include <string>     
using namespace std;


// datos que no cambian

const double IGV = 0.18;   
const int MAX = 50;        


//  donde se guardan los datos

// --- Proveedores ---
string nombres[MAX];        
string rucs[MAX];           
int totalProveedores = 0;   

// --Ordenes de compra ---
int proveedorDeOrden[MAX];  
string productos[MAX];      
int cantidades[MAX];        
double precios[MAX];        
double subtotales[MAX];     
double igvs[MAX];           
double totales[MAX];        
int totalOrdenes = 0;       

// -Requerimientos (lo que pide la obra) 
string reqObra[MAX];       
string reqMaterial[MAX];    
int reqCantidad[MAX];      
bool reqAtendido[MAX];      
int totalRequerimientos = 0;   

//  revisar el RUC

bool rucEsValido(string ruc) {
    
    if (ruc.size() != 11) {
        return false;
    }
  
    for (int i = 0; i < 11; i++) {
        
        if (ruc[i] < '0' || ruc[i] > '9') {
            return false;
        }
    }
    return true;   
}


//  opcion 1 - registrar proveedor

void registrarProveedor() {
  
    if (totalProveedores == MAX) {
        cout << "No hay espacio para mas proveedores." << endl;
        return;
    }

    string nombre;
    string ruc;

    cout << "Nombre del proveedor (sin espacios): ";
    cin >> nombre;
    cout << "RUC (11 numeros): ";
    cin >> ruc;

    
    if (rucEsValido(ruc) == false) {
        cout << "Error de datos: el RUC debe tener 11 numeros." << endl;
        return;
    }

    
    nombres[totalProveedores] = nombre;
    rucs[totalProveedores] = ruc;
    totalProveedores = totalProveedores + 1;

    cout << "Proveedor registrado con el numero " << totalProveedores << endl;
}

//  opcion 2 - mostrar proveedores

void mostrarProveedores() {
    if (totalProveedores == 0) {
        cout << "No hay proveedores registrados." << endl;
        return;
    }

    cout << "--- PROVEEDORES ---" << endl;
    
    for (int i = 0; i < totalProveedores; i++) {
        
        cout << (i + 1) << ". " << nombres[i] << " (RUC " << rucs[i] << ")" << endl;
    }
}


//  mostrar una orden de compra

void mostrarOrden(int i) {
    int prov = proveedorDeOrden[i];  

    cout << "\n===== ORDEN DE COMPRA N. " << (i + 1) << " =====" << endl;
    cout << "Proveedor  : " << nombres[prov] << endl;
    cout << "Producto   : " << productos[i] << endl;
    cout << "Cantidad   : " << cantidades[i] << endl;
    cout << "Precio un. : S/ " << precios[i] << endl;
    cout << "Subtotal   : S/ " << subtotales[i] << endl;
    cout << "IGV (18%)  : S/ " << igvs[i] << endl;
    cout << "TOTAL      : S/ " << totales[i] << endl;
}


//  calcular y guardar una orden de compra

void guardarOrden(int prov, string producto, int cantidad, double precio) {
    //  Calculos 
    double subtotal = cantidad * precio;   
    double igv = subtotal * IGV;           
    double total = subtotal + igv;         

    //  Guardamos todo en el casillero que sigue 
    int pos = totalOrdenes;
    proveedorDeOrden[pos] = prov;
    productos[pos] = producto;
    cantidades[pos] = cantidad;
    precios[pos] = precio;
    subtotales[pos] = subtotal;
    igvs[pos] = igv;
    totales[pos] = total;
    totalOrdenes = totalOrdenes + 1;

    mostrarOrden(pos);
}


//  opcion 3 - crear orden de compra 

void crearOrden() {
    
    if (totalProveedores == 0) {
        cout << "Primero registre un proveedor." << endl;
        return;
    }
    
    if (totalOrdenes == MAX) {
        cout << "No hay espacio para mas ordenes." << endl;
        return;
    }

    mostrarProveedores();

    int numero;        
    string producto;
    int cantidad;
    double precio;

    
    cout << "Numero del proveedor: ";
    cin >> numero;
   
    if (cin.fail()) {
        cin.clear();               
        cin.ignore(1000, '\n');   
        cout << "Error de datos: escriba un numero." << endl;
        return;
    }
    if (numero < 1 || numero > totalProveedores) {
        cout << "Error de datos: ese proveedor no existe." << endl;
        return;
    }

    //  Pedimos el producto 
    cout << "Producto (sin espacios): ";
    cin >> producto;

    //  Pedimos la cantidad y el precio 
    cout << "Cantidad: ";
    cin >> cantidad;
    cout << "Precio unitario: ";
    cin >> precio;

    // Si en cantidad o precio escribieron letras
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error de datos: cantidad y precio deben ser numeros." << endl;
        return;
    }

    // No se puede comprar 0 ni numeros negativos
    if (cantidad <= 0 || precio <= 0) {
        cout << "Error de datos: cantidad y precio deben ser mayores a cero." << endl;
        return;
    }

    
    guardarOrden(numero - 1, producto, cantidad, precio);
}
 

//  opcion 4 - historial y total gastado

void mostrarHistorial() {
    if (totalOrdenes == 0) {
        cout << "No hay ordenes registradas." << endl;
        return;
    }

    double gastoTotal = 0;  

    for (int i = 0; i < totalOrdenes; i++) {
        mostrarOrden(i);
        gastoTotal = gastoTotal + totales[i];  
    }

    cout << "\nTotal gastado en compras: S/ " << gastoTotal << endl;
}


//  opcion 5 - registrar requerimiento

void registrarRequerimiento() {
    if (totalRequerimientos == MAX) {
        cout << "No hay espacio para mas requerimientos." << endl;
        return;
    }

    string obra;
    string material;
    int cantidad;

    cout << "Obra que hace el pedido (sin espacios): ";
    cin >> obra;
    cout << "Material (sin espacios): ";
    cin >> material;
    cout << "Cantidad: ";
    cin >> cantidad;

    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error de datos: la cantidad debe ser un numero." << endl;
        return;
    }
    if (cantidad <= 0) {
        cout << "Error de datos: la cantidad debe ser mayor a cero." << endl;
        return;
    }

    
    reqObra[totalRequerimientos] = obra;
    reqMaterial[totalRequerimientos] = material;
    reqCantidad[totalRequerimientos] = cantidad;
    reqAtendido[totalRequerimientos] = false;   
    totalRequerimientos = totalRequerimientos + 1;

    cout << "Requerimiento registrado con el numero " << totalRequerimientos << endl;
}


//  opcion 6 - mostrar requerimientos

void mostrarRequerimientos() {
    if (totalRequerimientos == 0) {
        cout << "No hay requerimientos registrados." << endl;
        return;
    }

    cout << "--- REQUERIMIENTOS ---" << endl;
    for (int i = 0; i < totalRequerimientos; i++) {
        cout << (i + 1) << ". Obra: " << reqObra[i]
             << " | " << reqMaterial[i] << " x " << reqCantidad[i];
       
        if (reqAtendido[i] == true) {
            cout << " | ATENDIDO" << endl;
        } else {
            cout << " | PENDIENTE" << endl;
        }
    }
}


// Cuenta cuantos requerimientos siguen pendientes

int contarPendientes() {
    int pendientes = 0;
    for (int i = 0; i < totalRequerimientos; i++) {
        if (reqAtendido[i] == false) {
            pendientes = pendientes + 1;
        }
    }
    return pendientes;
}


// opcion 7 - atender un requerimiento

void atenderRequerimiento() {
    if (contarPendientes() == 0) {
        cout << "No hay requerimientos pendientes." << endl;
        return;
    }
    if (totalProveedores == 0) {
        cout << "Primero registre un proveedor." << endl;
        return;
    }
    if (totalOrdenes == MAX) {
        cout << "No hay espacio para mas ordenes." << endl;
        return;
    }

    mostrarRequerimientos();

    int numReq;
    int numProv;
    double precio;

   
    cout << "Numero del requerimiento a atender: ";
    cin >> numReq;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error de datos: escriba un numero." << endl;
        return;
    }
    if (numReq < 1 || numReq > totalRequerimientos) {
        cout << "Error de datos: ese requerimiento no existe." << endl;
        return;
    }
    if (reqAtendido[numReq - 1] == true) {
        cout << "Ese requerimiento ya fue atendido." << endl;
        return;
    }

    //  Elegimos el proveedor 
    mostrarProveedores();
    cout << "Numero del proveedor: ";
    cin >> numProv;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error de datos: escriba un numero." << endl;
        return;
    }
    if (numProv < 1 || numProv > totalProveedores) {
        cout << "Error de datos: ese proveedor no existe." << endl;
        return;
    }

    //  Escribimos el precio que dio el proveedor 
    cout << "Precio unitario: ";
    cin >> precio;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error de datos: el precio debe ser un numero." << endl;
        return;
    }
    if (precio <= 0) {
        cout << "Error de datos: el precio debe ser mayor a cero." << endl;
        return;
    }

    // Creamos la orden con los datos del requerimiento
    guardarOrden(numProv - 1, reqMaterial[numReq - 1], reqCantidad[numReq - 1], precio);

    // Marcamos el requerimiento como atendido
    reqAtendido[numReq - 1] = true;
    cout << "Requerimiento " << numReq << " marcado como ATENDIDO." << endl;
}


//  opcion 8 - buscar proveedor

void buscarProveedor() {
    if (totalProveedores == 0) {
        cout << "No hay proveedores registrados." << endl;
        return;
    }

    string texto;
    cout << "Escriba el nombre o el RUC a buscar: ";
    cin >> texto;

    int encontrados = 0;   

    for (int i = 0; i < totalProveedores; i++) {
        
        if (nombres[i] == texto || rucs[i] == texto) {
            cout << "\nProveedor " << (i + 1) << ": " << nombres[i]
                 << " (RUC " << rucs[i] << ")" << endl;

            
            int ordenesDelProveedor = 0;
            double gastado = 0;
            for (int j = 0; j < totalOrdenes; j++) {
                if (proveedorDeOrden[j] == i) {
                    ordenesDelProveedor = ordenesDelProveedor + 1;
                    gastado = gastado + totales[j];
                }
            }
            cout << "Ordenes: " << ordenesDelProveedor
                 << " | Total comprado: S/ " << gastado << endl;

            encontrados = encontrados + 1;
        }
    }

    // Si ninguno coincidio, avisamos
    if (encontrados == 0) {
        cout << "No se encontro ningun proveedor con ese dato." << endl;
    }
}


//  opcion 9 - reporte de compras

void mostrarReporte() {
    if (totalOrdenes == 0) {
        cout << "No hay ordenes para hacer el reporte." << endl;
        return;
    }

    cout << "\n===== REPORTE DE COMPRAS =====" << endl;

    //  cuanto se compro a cada proveedor 
    cout << "\nCompras por proveedor:" << endl;
    for (int i = 0; i < totalProveedores; i++) {
        double suma = 0;      
        int cantidad = 0;     
        for (int j = 0; j < totalOrdenes; j++) {
            if (proveedorDeOrden[j] == i) {
                suma = suma + totales[j];
                cantidad = cantidad + 1;
            }
        }
        cout << (i + 1) << ". " << nombres[i] << ": " << cantidad
             << " ordenes, S/ " << suma << endl;
    }

    //  Parte 2: totales generales y orden mas cara 
    double totalGeneral = 0;
    double igvAcumulado = 0;
    int posMasCara = 0;   

    for (int j = 0; j < totalOrdenes; j++) {
        totalGeneral = totalGeneral + totales[j];
        igvAcumulado = igvAcumulado + igvs[j];
       
        if (totales[j] > totales[posMasCara]) {
            posMasCara = j;
        }
    }

    cout << "\nTotal general comprado : S/ " << totalGeneral << endl;
    cout << "IGV acumulado          : S/ " << igvAcumulado << endl;
    cout << "Orden mas cara         : N. " << (posMasCara + 1) << " ("
         << productos[posMasCara] << ", " << nombres[proveedorDeOrden[posMasCara]]
         << ") S/ " << totales[posMasCara] << endl;
}


//  programa principal

void moduloCompras() {
    
    cout << fixed << setprecision(2);

    int opcion = -1;

    while (opcion != 0) {
        cout << "\n=== COMPRAS Y PROVEEDORES ===" << endl;
        cout << "1. Registrar proveedor" << endl;
        cout << "2. Ver proveedores" << endl;
        cout << "3. Crear orden de compra" << endl;
        cout << "4. Ver historial de ordenes" << endl;
        cout << "5. Registrar requerimiento" << endl;
        cout << "6. Ver requerimientos" << endl;
        cout << "7. Atender requerimiento (crear orden)" << endl;
        cout << "8. Buscar proveedor" << endl;
        cout << "9. Reporte de compras" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "Opcion: ";
        cin >> opcion;

       
        if (cin.fail()) {
            if (cin.eof()) {
                break;   
            }
            cin.clear();
            cin.ignore(1000, '\n');
            opcion = -1;
        }

        if (opcion == 1) {
            registrarProveedor();
        } else if (opcion == 2) {
            mostrarProveedores();
        } else if (opcion == 3) {
            crearOrden();
        } else if (opcion == 4) {
            mostrarHistorial();
        } else if (opcion == 5) {
            registrarRequerimiento();
        } else if (opcion == 6) {
            mostrarRequerimientos();
        } else if (opcion == 7) {
            atenderRequerimiento();
        } else if (opcion == 8) {
            buscarProveedor();
        } else if (opcion == 9) {
            mostrarReporte();
        } else if (opcion == 0) {
            cout << "Hasta luego REY. " << endl;
        } else {
            cout << "Opcion invalida REY " << endl;
        }
    }

    
}
