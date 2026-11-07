#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Lee un numero entero y vuelve a pedirlo si el usuario escribe letras
int Leer_entero(string mensaje){
    int valor;
    cout<<mensaje;
    while(!(cin>>valor)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"Debe ingresar un numero. "<<mensaje;
    }
    return valor;
}

// Guarda un movimiento en el kardex (vectores paralelos de movimientos)
void Registrar_movimiento(vector<int> &mov_material, vector<string> &mov_fecha, vector<string> &mov_tipo,
    vector<string> &mov_detalle, vector<int> &mov_cantidad, vector<int> &mov_saldo,
    int pos, string fecha, string tipo, string detalle, int cantidad, int saldo){
    mov_material.push_back(pos);
    mov_fecha.push_back(fecha);
    mov_tipo.push_back(tipo);
    mov_detalle.push_back(detalle);
    mov_cantidad.push_back(cantidad);
    mov_saldo.push_back(saldo);
}

void Cargar_datos_iniciales(vector<string> &codigos, vector<string> &nombres,
     vector<string> &unidades, vector<int> &obras_material,vector<int> &stock, vector<int> &minimo){
    //Material 01 (Cemento sol)
    codigos.push_back("M001");
    nombres.push_back("Cemento Sol");
    unidades.push_back("bolsa");
    obras_material.push_back(0);        // posicion en el arreglo obras
    stock.push_back(120);
    minimo.push_back(50);
    //Material 02 (Fierros Arequipa )
    codigos.push_back("M002");
    nombres.push_back("Fierros Arequipa");
    unidades.push_back("trenza");
    obras_material.push_back(1);
    stock.push_back(80);
    minimo.push_back(30);
    //Material 03 (Ladrillos San Juan)
    codigos.push_back("M003");
    nombres.push_back("Ladrillos San Juan");
    unidades.push_back("ladrillo");
    obras_material.push_back(2);
    stock.push_back(200);
    minimo.push_back(100);
    //Material 04 (Arena fina)
    codigos.push_back("M004");
    nombres.push_back("Arena fina");
    unidades.push_back("m3");
    obras_material.push_back(3);
    stock.push_back(50);
    minimo.push_back(20);
    //Material 05 (grava)
    codigos.push_back("M005");
    nombres.push_back("Grava");
    unidades.push_back("m3");
    obras_material.push_back(0);
    stock.push_back(15);
    minimo.push_back(30);
    //Material 06 (Madera)
    codigos.push_back("M006");
    nombres.push_back("Madera");
    unidades.push_back("m3");
    obras_material.push_back(1);
    stock.push_back(10);
    minimo.push_back(20);
    //Material 07 (Pintura)
    codigos.push_back("M007");
    nombres.push_back("Pintura");
    unidades.push_back("galon");
    obras_material.push_back(2);
    stock.push_back(25);
    minimo.push_back(10);
    //Material 08 (Clavos)
    codigos.push_back("M008");
    nombres.push_back("Clavos");
    unidades.push_back("kg");
    obras_material.push_back(3);
    stock.push_back(50);
    minimo.push_back(20);
}
int Buscar_indice(const vector<string> &codigos, string codigo_buscado){
    for(int i = 0; i < (int)codigos.size(); i++){
        if(codigos[i] == codigo_buscado){
            return i;
        }
    }
    return -1;
}
void Registrar_material_nuevo(vector<string> &codigos, vector<string> &nombres, vector<string> &unidades, vector<int> &obras_material, vector<int> &stock, vector<int> &minimo,  const string obras[],
    vector<int> &mov_material, vector<string> &mov_fecha, vector<string> &mov_tipo,
    vector<string> &mov_detalle, vector<int> &mov_cantidad, vector<int> &mov_saldo){
    string codigo, nombre, unidad, fecha;
    int obra_elegida, stock_inicial, stock_minimo;

    do{
        cout<<"Codigo (ej. M005): ";
        cin>>codigo;
        if(Buscar_indice(codigos, codigo) != -1){
            cout<<"Ese codigo ya existe."<<endl;
        }
    }while(Buscar_indice(codigos, codigo) != -1);
    cin.ignore();
    cout<<"Nombre: ";
    getline(cin, nombre);

    cout<<"Unidad: ";              // ya no va cin.ignore() aqui: getline no deja el salto de linea
    getline(cin, unidad);

    for(int i = 0; i < 4; i++){
        cout<<i+1<<". "<<obras[i]<<endl;
    }
    do{
        obra_elegida = Leer_entero("Seleccione la obra (1-4): ");
        if(obra_elegida < 1 || obra_elegida > 4){
            cout<<"Opcion invalida."<<endl;
        }
    }while(obra_elegida < 1 || obra_elegida > 4);
    do{
        stock_inicial = Leer_entero("Mencione el Stock inicial: ");
        if(stock_inicial < 0){
            cout<<"El stock inicial no puede ser negativo."<<endl;
        }
    }while(stock_inicial <0);
    do{
        stock_minimo = Leer_entero("Mencione el Stock minimo: ");
        if(stock_minimo < 0){
            cout<<"El stock minimo no puede ser negativo."<<endl;
        }
    }while(stock_minimo < 0);
    cout<<"Fecha de registro (dd/mm/aaaa): ";
    cin>>fecha;
    cout<<"=====GUARDANDO DATOS...====="<<endl;

    codigos.push_back(codigo);
    nombres.push_back(nombre);
    unidades.push_back(unidad);
    obras_material.push_back(obra_elegida - 1);
    stock.push_back(stock_inicial);
    minimo.push_back(stock_minimo);

    // El stock inicial es el primer movimiento del kardex
    Registrar_movimiento(mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo,
        codigos.size() - 1, fecha, "INICIAL", "Saldo inicial", stock_inicial, stock_inicial);

    cout<<"=====MATERIAL REGISTRADO EXITOSAMENTE====="<<endl;
}
void Listar_materiales(const vector<string> &codigos, const vector<string> &nombres, const vector<string> &unidades,
    const vector<int> &obras_material, const vector<int> &stock, const vector<int> &minimo, const string obras[]){
    cout<<left<<setw(10)<<"CODIGO"<<setw(30)<<"NOMBRE"<<setw(15)<<"UNIDAD"<<setw(15)<<"OBRA"<<setw(10)<<"STOCK"<<setw(10)<<"MINIMO"<<endl;
    cout<<"------------------------------------------------------------------------------------"<<endl;
    for(int i = 0; i < (int)codigos.size(); i++){
        cout<<left<<setw(10)<<codigos[i]<<setw(30)<<nombres[i]<<setw(15)<<unidades[i]<<setw(15)<<obras[obras_material[i]]<<setw(10)<<stock[i]<<setw(10)<<minimo[i]<<endl;
    }

}
void Buscar_material_por_codigo(const vector<string> &codigos, const vector<string> &nombres,
    const vector<string> &unidades, const vector<int> &obras_material,
    const vector<int> &stock, const vector<int> &minimo, const string obras[]){
    string codigo;
    cout<<"=====Ingrese el codigo del material a buscar: ";
    cin>>codigo;
    int pos = Buscar_indice(codigos, codigo);

    if(pos == -1){
        cout<<"Material no encontrado."<<endl;
    }else{
        cout<<"Codigo: "<<codigos[pos]<<endl;
        cout<<"Nombre: "<<nombres[pos]<<endl;
        cout<<"Unidad: "<<unidades[pos]<<endl;
        cout<<"Obra: "<<obras[obras_material[pos]]<<endl;
        cout<<"Stock actual: "<<stock[pos]<<endl;
        cout<<"Stock minimo: "<<minimo[pos]<<endl;
        if(stock[pos] < minimo[pos]){
            cout<<"Alerta: Stock por debajo del minimo."<<endl;
        }
    }
}
void Registrar_entrada(const vector<string> &codigos, const vector<string> &nombres, const vector<string> &unidades,
    vector<int> &stock,
    vector<int> &mov_material, vector<string> &mov_fecha, vector<string> &mov_tipo,
    vector<string> &mov_detalle, vector<int> &mov_cantidad, vector<int> &mov_saldo){
    string codigo, fecha, documento;
    int cantidad;

    cout<<"Codigo del material: ";
    cin>>codigo;
    int pos = Buscar_indice(codigos, codigo);
    if(pos == -1){
        cout<<"Material no encontrado."<<endl;
        return;
    }
    cout<<"Material: "<<nombres[pos]<<" | Stock actual: "<<stock[pos]<<" "<<unidades[pos]<<endl;
    cout<<"Fecha (dd/mm/aaaa): ";
    cin>>fecha;
    cin.ignore();
    cout<<"Documento de ingreso (ej. Guia 001-2345): ";
    getline(cin, documento);
    do{
        cantidad = Leer_entero("Cantidad que ingresa: ");
        if(cantidad <= 0){
            cout<<"La cantidad debe ser mayor que cero."<<endl;
        }
    }while(cantidad <= 0);

    stock[pos] = stock[pos] + cantidad;
    Registrar_movimiento(mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo,
        pos, fecha, "ENTRADA", documento, cantidad, stock[pos]);

    cout<<"=====ENTRADA REGISTRADA. Nuevo stock: "<<stock[pos]<<" "<<unidades[pos]<<"====="<<endl;
}
void Registrar_salida(const vector<string> &codigos, const vector<string> &nombres, const vector<string> &unidades,
    const vector<int> &obras_material, vector<int> &stock, const vector<int> &minimo, const string obras[],
    vector<int> &mov_material, vector<string> &mov_fecha, vector<string> &mov_tipo,
    vector<string> &mov_detalle, vector<int> &mov_cantidad, vector<int> &mov_saldo){
    string codigo, fecha;
    int cantidad;

    cout<<"Codigo del material: ";
    cin>>codigo;
    int pos = Buscar_indice(codigos, codigo);
    if(pos == -1){
        cout<<"Material no encontrado."<<endl;
        return;
    }
    if(stock[pos] == 0){
        cout<<"No hay stock de "<<nombres[pos]<<". No se puede despachar."<<endl;
        return;
    }
    cout<<"Material: "<<nombres[pos]<<" | Stock actual: "<<stock[pos]<<" "<<unidades[pos]<<endl;
    cout<<"Se despacha a: "<<obras[obras_material[pos]]<<endl;
    cout<<"Fecha (dd/mm/aaaa): ";
    cin>>fecha;
    do{
        cantidad = Leer_entero("Cantidad que sale: ");
        if(cantidad <= 0){
            cout<<"La cantidad debe ser mayor que cero."<<endl;
        }else if(cantidad > stock[pos]){
            cout<<"No hay suficiente stock. Maximo: "<<stock[pos]<<endl;
        }
    }while(cantidad <= 0 || cantidad > stock[pos]);

    stock[pos] = stock[pos] - cantidad;
    Registrar_movimiento(mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo,
        pos, fecha, "SALIDA", "Despacho a " + obras[obras_material[pos]], cantidad, stock[pos]);

    cout<<"=====SALIDA REGISTRADA. Nuevo stock: "<<stock[pos]<<" "<<unidades[pos]<<"====="<<endl;
    if(stock[pos] < minimo[pos]){
        cout<<"Alerta: el stock quedo por debajo del minimo ("<<minimo[pos]<<")."<<endl;
    }
}
void Ver_kardex_de_un_material(const vector<string> &codigos, const vector<string> &nombres, const vector<string> &unidades,
    const vector<int> &stock,
    const vector<int> &mov_material, const vector<string> &mov_fecha, const vector<string> &mov_tipo,
    const vector<string> &mov_detalle, const vector<int> &mov_cantidad, const vector<int> &mov_saldo){
    string codigo;
    cout<<"Codigo del material: ";
    cin>>codigo;
    int pos = Buscar_indice(codigos, codigo);
    if(pos == -1){
        cout<<"Material no encontrado."<<endl;
        return;
    }
    cout<<"=====KARDEX: "<<codigos[pos]<<" - "<<nombres[pos]<<" ("<<unidades[pos]<<")====="<<endl;
    cout<<left<<setw(12)<<"FECHA"<<setw(10)<<"TIPO"<<setw(28)<<"DETALLE"<<setw(10)<<"ENTRADA"<<setw(10)<<"SALIDA"<<setw(10)<<"SALDO"<<endl;
    cout<<"------------------------------------------------------------------------------"<<endl;
    for(int i = 0; i < (int)mov_material.size(); i++){
        if(mov_material[i] == pos){
            cout<<left<<setw(12)<<mov_fecha[i]<<setw(10)<<mov_tipo[i]<<setw(28)<<mov_detalle[i];
            if(mov_tipo[i] == "SALIDA"){
                cout<<setw(10)<<"-"<<setw(10)<<mov_cantidad[i];
            }else{
                cout<<setw(10)<<mov_cantidad[i]<<setw(10)<<"-";
            }
            cout<<setw(10)<<mov_saldo[i]<<endl;
        }
    }
    cout<<"------------------------------------------------------------------------------"<<endl;
    cout<<"Stock actual: "<<stock[pos]<<" "<<unidades[pos]<<endl;
}
void Ver_alertas_de_stock_minimo(const vector<string> &codigos, const vector<string> &nombres, const vector<string> &unidades,
    const vector<int> &obras_material, const vector<int> &stock, const vector<int> &minimo, const string obras[]){
    int cantidad_alertas = 0;
    cout<<left<<setw(10)<<"CODIGO"<<setw(30)<<"NOMBRE"<<setw(15)<<"OBRA"<<setw(10)<<"STOCK"<<setw(10)<<"MINIMO"<<setw(10)<<"FALTAN"<<endl;
    cout<<"-------------------------------------------------------------------------------------"<<endl;
    for(int i = 0; i < (int)codigos.size(); i++){
        if(stock[i] < minimo[i]){
            cout<<left<<setw(10)<<codigos[i]<<setw(30)<<nombres[i]<<setw(15)<<obras[obras_material[i]]
                <<setw(10)<<stock[i]<<setw(10)<<minimo[i]<<minimo[i] - stock[i]<<" "<<unidades[i]<<endl;
            cantidad_alertas++;
        }
    }
    if(cantidad_alertas == 0){
        cout<<"No hay materiales por debajo del stock minimo."<<endl;
    }else{
        cout<<"Total de materiales en alerta: "<<cantidad_alertas<<endl;
    }
}
void Modificar_stock_minimo(const vector<string> &codigos, const vector<string> &nombres, const vector<string> &unidades,
    const vector<int> &stock, vector<int> &minimo){
    string codigo;
    int nuevo_minimo;
    cout<<"Codigo del material: ";
    cin>>codigo;
    int pos = Buscar_indice(codigos, codigo);
    if(pos == -1){
        cout<<"Material no encontrado."<<endl;
        return;
    }
    cout<<"Material: "<<nombres[pos]<<" | Minimo actual: "<<minimo[pos]<<" "<<unidades[pos]<<endl;
    do{
        nuevo_minimo = Leer_entero("Nuevo stock minimo: ");
        if(nuevo_minimo < 0){
            cout<<"El stock minimo no puede ser negativo."<<endl;
        }
    }while(nuevo_minimo < 0);
    minimo[pos] = nuevo_minimo;
    cout<<"=====STOCK MINIMO ACTUALIZADO====="<<endl;
    if(stock[pos] < minimo[pos]){
        cout<<"Alerta: con el nuevo minimo, este material queda en alerta."<<endl;
    }
}
void Stock_por_obra(const vector<string> &codigos, const vector<string> &nombres, const vector<string> &unidades,
    const vector<int> &obras_material, const vector<int> &stock, const string obras[]){
    for(int o = 0; o < 4; o++){
        int cantidad_materiales = 0;
        cout<<"===== "<<obras[o]<<" ====="<<endl;
        for(int i = 0; i < (int)codigos.size(); i++){
            if(obras_material[i] == o){
                cout<<left<<setw(10)<<codigos[i]<<setw(30)<<nombres[i]<<setw(8)<<stock[i]<<unidades[i]<<endl;
                cantidad_materiales++;
            }
        }
        if(cantidad_materiales == 0){
            cout<<"Sin materiales asignados."<<endl;
        }else{
            cout<<"Materiales en esta obra: "<<cantidad_materiales<<endl;
        }
        cout<<endl;
    }
}

int main(){
    string obras[4] = {"Obra de pavimentacion", "Obra de construccion de edificio", "Obra de carretera", "Obra de remodelacion"};
    vector<string> codigos, nombres, unidades;
    vector<int> obras_material, stock, minimo;
    // Vectores del kardex: un elemento por cada movimiento
    vector<int> mov_material, mov_cantidad, mov_saldo;
    vector<string> mov_fecha, mov_tipo, mov_detalle;
    int Numero_opcion_menu;
    Cargar_datos_iniciales(codigos, nombres, unidades, obras_material, stock, minimo);
    // El stock de los materiales precargados entra al kardex como saldo inicial
    for(int i = 0; i < (int)codigos.size(); i++){
        Registrar_movimiento(mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo,
            i, "01/11/2026", "INICIAL", "Saldo inicial", stock[i], stock[i]);
    }
    do{
    cout<<endl<<"=====ATLANTIC FONTANA -INVENTARIO Y ALMACENAMIENTO====="<<endl;
    cout<<"1. Registrar material nuevo"<<endl;
    cout<<"2. Listar materiales"<<endl;
    cout<<"3. Buscar material por codigo"<<endl;
    cout<<"4. Registrar entrada (ingreso a almacen)"<<endl;
    cout<<"5. Registrar salida (despacho a obra)"<<endl;
    cout<<"6. Ver kardex de un material"<<endl;
    cout<<"7. Ver alertas de stock minimo"<<endl;
    cout<<"8. Modificar stock minimo"<<endl;
    cout<<"9. Stock por obra"<<endl;
    cout<<"10. Salir"<<endl;
    Numero_opcion_menu = Leer_entero("=====Ingrese el numero de la opcion que desea realizar: ");

    switch(Numero_opcion_menu){
        case 1:
            cout<<"Registrar material nuevo"<<endl;
            Registrar_material_nuevo(codigos, nombres, unidades, obras_material, stock, minimo, obras,
                mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo);
            break;
        case 2:
            cout<<"Listar materiales"<<endl;
            Listar_materiales(codigos, nombres, unidades, obras_material, stock, minimo, obras);
            break;
        case 3:
            cout<<"Buscar material por codigo"<<endl;
            Buscar_material_por_codigo(codigos, nombres, unidades, obras_material, stock, minimo, obras);
            break;
        case 4:
            cout<<"Registrar entrada (ingreso a almacen)"<<endl;
            Registrar_entrada(codigos, nombres, unidades, stock,
                mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo);
            break;
        case 5:
            cout<<"Registrar salida (despacho a obra)"<<endl;
            Registrar_salida(codigos, nombres, unidades, obras_material, stock, minimo, obras,
                mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo);
            break;
        case 6:
            cout<<"Ver kardex de un material"<<endl;
            Ver_kardex_de_un_material(codigos, nombres, unidades, stock,
                mov_material, mov_fecha, mov_tipo, mov_detalle, mov_cantidad, mov_saldo);
            break;
        case 7:
            cout<<"Ver alertas de stock minimo"<<endl;
            Ver_alertas_de_stock_minimo(codigos, nombres, unidades, obras_material, stock, minimo, obras);
            break;
        case 8:
            cout<<"Modificar stock minimo"<<endl;
            Modificar_stock_minimo(codigos, nombres, unidades, stock, minimo);
            break;
        case 9:
            cout<<"Stock por obra"<<endl;
            Stock_por_obra(codigos, nombres, unidades, obras_material, stock, obras);
            break;
        case 10:
            cout<<"Saliendo del sistema ..."<<endl;
            break;
        default:
            cout<<"Opcion invalida, por favor intente de nuevo."<<endl;
            break;
    }
    }while(Numero_opcion_menu != 10 );
    return 0;
}
