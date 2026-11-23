#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>
#include <cmath>

using namespace std;

// ==============================================================================
// ENUMERACIONES Y ESTRUCTURAS DE DATOS
// ==============================================================================

// Estado de alerta de la partida de obra
enum class EstadoAlerta {
    DENTRO_PRESUPUESTO,
    RIESGO_MODERADO,
    ALERTA_SOBRECOSTO,
    CRITICO_EXCEDIDO
};

// Estructura para registrar eventos del historial de modificaciones
struct RegistroHistorial {
    string fecha;
    double gastoAnterior;
    double gastoNuevo;
    double avanceAnterior;
    double avanceNuevo;
    string observacion;
};

// Estructura principal de una Partida de Obra
struct PartidaObra {
    int codigoPartida;
    string nombreObra;
    string ubicacion;
    string ingenieroResponsable;
    string tipoPartida;
    double presupuestoAsignado;
    double gastoEjecutado;
    double avanceFisicoReal; 
    string fechaCreacion;
    string fechaUltimaModificacion;
    vector<RegistroHistorial> historial;
};

// ==============================================================================
// CLASE: ValidadorEntrada
// Maneja la lectura segura y validacion estricta de entradas del usuario.
// ==============================================================================
class ValidadorEntrada {
public:
    static void limpiarBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static double leerDouble(const string& mensaje, double minVal, double maxVal) {
        double valor;
        while (true) {
            cout << mensaje;
            if (cin >> valor) {
                if (valor >= minVal && valor <= maxVal) {
                    limpiarBuffer();
                    return valor;
                } else {
                    cout << " [ERROR] El valor debe estar entre " << minVal << " y " << maxVal << ". Reintente.\n";
                }
            } else {
                cout << " [ERROR] Entrada no valida. Por favor ingrese un numero valido.\n";
                limpiarBuffer();
            }
        }
    }

    static int leerEntero(const string& mensaje, int minVal, int maxVal) {
        int valor;
        while (true) {
            cout << mensaje;
            if (cin >> valor) {
                if (valor >= minVal && valor <= maxVal) {
                    limpiarBuffer();
                    return valor;
                } else {
                    cout << " [ERROR] El codigo o opcion debe estar entre " << minVal << " y " << maxVal << ". Reintente.\n";
                }
            } else {
                cout << " [ERROR] Entrada no valida. Debe ingresar un numero entero.\n";
                limpiarBuffer();
            }
        }
    }

    static string leerTextoNoVacio(const string& mensaje) {
        string texto;
        while (true) {
            cout << mensaje;
            getline(cin, texto);
            size_t primer = texto.find_first_not_of(" \t");
            if (primer != string::npos) {
                return texto;
            }
            cout << " [ERROR] Este campo no puede quedar vacio. Reintente.\n";
        }
    }
};

// ==============================================================================
// CLASE: CalculadorControlObra
// Contiene las formulas matematicas y algoritmos de diagnostico del proyecto.
// ==============================================================================
class CalculadorControlObra {
public:
    // Calcula el porcentaje de consumo financiero: (Gasto / Presupuesto) * 100
    static double calcularPorcentajeConsumo(double gasto, double presupuesto) {
        if (presupuesto <= 0.0) return 0.0;
        return (gasto / presupuesto) * 100.0;
    }

    // Determina el estado de alerta comparando consumo vs avance fisico
    static EstadoAlerta evaluarEstado(double consumoPorcentaje, double avanceFisicoPorcentaje) {
        double diferencia = consumoPorcentaje - avanceFisicoPorcentaje;

        if (consumoPorcentaje > 100.0) {
            return EstadoAlerta::CRITICO_EXCEDIDO;
        } else if (diferencia > 10.0) {
            return EstadoAlerta::ALERTA_SOBRECOSTO;
        } else if (diferencia > 0.0) {
            return EstadoAlerta::RIESGO_MODERADO;
        } else {
            return EstadoAlerta::DENTRO_PRESUPUESTO;
        }
    }

    // Convierte el estado de alerta a una cadena de texto para presentacion
    static string obtenerTextoEstado(EstadoAlerta estado) {
        switch (estado) {
            case EstadoAlerta::DENTRO_PRESUPUESTO:
                return "Dentro de Presupuesto";
            case EstadoAlerta::RIESGO_MODERADO:
                return "Riesgo Moderado";
            case EstadoAlerta::ALERTA_SOBRECOSTO:
                return "ALERTA: Sobrecosto";
            case EstadoAlerta::CRITICO_EXCEDIDO:
                return "CRITICO: Presupuesto Excedido";
            default:
                return "Desconocido";
        }
    }
};

// ==============================================================================
// CLASE: GestorControlObra
// Administra el inventario de partidas de obra y operaciones del modulo.
// ==============================================================================
class GestorControlObra {
private:
    vector<PartidaObra> listaPartidas;

    // Busca el indice de una partida por su codigo unico
    int buscarIndicePorCodigo(int codigo) const {
        for (size_t i = 0; i < listaPartidas.size(); ++i) {
            if (listaPartidas[i].codigoPartida == codigo) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

public:
    GestorControlObra() {
        cargarDatosIniciales();
    }

    // Carga partidas base para realizar pruebas funcionales de inmediato
    void cargarDatosIniciales() {
        PartidaObra p1 = {
            101, "Cimentacion y Excavacion", "Proyecto Fontana Arequipa", 
            "Ing. Carlos Mendoza", "Vialidad y Cimientos",
            50000.0, 42000.0, 70.0, "01/05/2026", "10/05/2026", {}
        };
        PartidaObra p2 = {
            102, "Estructuras de Concreto Armado", "Proyecto Multifamiliar Lima", 
            "Ing. Maria Gomez", "Estructura",
            120000.0, 115000.0, 85.0, "05/05/2026", "12/05/2026", {}
        };
        PartidaObra p3 = {
            103, "Instalaciones Sanitarias y Redes", "Sede Central Trujillo", 
            "Ing. Roberto Ruiz", "Sanitaria",
            35000.0, 15000.0, 30.0, "08/05/2026", "14/05/2026", {}
        };
        PartidaObra p4 = {
            104, "Red Electrica Subterranea", "Proyecto Vial Fontana Sur", 
            "Ing. Ana Torres", "Electrica",
            45000.0, 48000.0, 60.0, "10/05/2026", "15/05/2026", {}
        };
        PartidaObra p5 = {
            105, "Acabados y Fachada Exterior", "Proyecto Multifamiliar Lima", 
            "Ing. Luis Silva", "Acabados",
            80000.0, 20000.0, 25.0, "11/05/2026", "16/05/2026", {}
        };

        listaPartidas.push_back(p1);
        listaPartidas.push_back(p2);
        listaPartidas.push_back(p3);
        listaPartidas.push_back(p4);
        listaPartidas.push_back(p5);
    }

    // Permite al usuario registrar una nueva partida de obra
    void registrarNuevaPartida() {
        cout << "\n=================================================================\n";
        cout << "            ATLANTIC FONTANA - REGISTRAR NUEVA PARTIDA           \n";
        cout << "=================================================================\n";

        int codigo;
        while (true) {
            codigo = ValidadorEntrada::leerEntero("Ingrese codigo unico de partida (1 - 99999): ", 1, 99999);
            if (buscarIndicePorCodigo(codigo) != -1) {
                cout << " [ERROR] El codigo " << codigo << " ya pertenece a otra partida. Pruebe con otro.\n";
            } else {
                break;
            }
        }

        string nombre = ValidadorEntrada::leerTextoNoVacio("Ingrese nombre o descripcion de la partida: ");
        string ubicacion = ValidadorEntrada::leerTextoNoVacio("Ingrese ubicacion u obra asignada: ");
        string responsable = ValidadorEntrada::leerTextoNoVacio("Ingrese nombre del Ingeniero Responsable: ");
        string tipo = ValidadorEntrada::leerTextoNoVacio("Ingrese el tipo de partida (ej. Estructura, Sanitaria): ");

        double presupuesto = ValidadorEntrada::leerDouble("Ingrese presupuesto asignado (S/.): ", 0.01, 50000000.0);
        double gasto = ValidadorEntrada::leerDouble("Ingrese gasto actual ejecutado (S/.): ", 0.0, 50000000.0);
        double avance = ValidadorEntrada::leerDouble("Ingrese el porcentaje de avance fisico (0 - 100 %): ", 0.0, 100.0);
        string fecha = ValidadorEntrada::leerTextoNoVacio("Ingrese fecha de registro (DD/MM/AAAA): ");

        PartidaObra nueva;
        nueva.codigoPartida = codigo;
        nueva.nombreObra = nombre;
        nueva.ubicacion = ubicacion;
        nueva.ingenieroResponsable = responsable;
        nueva.tipoPartida = tipo;
        nueva.presupuestoAsignado = presupuesto;
        nueva.gastoEjecutado = gasto;
        nueva.avanceFisicoReal = avance;
        nueva.fechaCreacion = fecha;
        nueva.fechaUltimaModificacion = fecha;

        listaPartidas.push_back(nueva);

        cout << "\n[EXITO] La partida de obra " << codigo << " - '" << nombre << "' fue registrada correctamente.\n";
    }

    // Actualiza gastos y avances fisicos de una partida existente guardando historial
    void actualizarAvanceYGastos() {
        if (listaPartidas.empty()) {
            cout << "\n[AVISO] No hay partidas registradas en el sistema.\n";
            return;
        }

        cout << "\n=================================================================\n";
        cout << "          ATLANTIC FONTANA - ACTUALIZAR DATOS DE PARTIDA         \n";
        cout << "=================================================================\n";

        int codigo = ValidadorEntrada::leerEntero("Ingrese el codigo de la partida a modificar: ", 1, 99999);
        int idx = buscarIndicePorCodigo(codigo);

        if (idx == -1) {
            cout << "\n[ERROR] No existe ninguna partida con el codigo " << codigo << ".\n";
            return;
        }

        PartidaObra& p = listaPartidas[idx];

        cout << "\n--- INFORMACION ACTUAL DE LA PARTIDA ---\n";
        cout << "Codigo      : " << p.codigoPartida << "\n";
        cout << "Nombre      : " << p.nombreObra << "\n";
        cout << "Presupuesto : S/. " << fixed << setprecision(2) << p.presupuestoAsignado << "\n";
        cout << "Gasto actual: S/. " << p.gastoEjecutado << "\n";
        cout << "Avance actual: " << p.avanceFisicoReal << " %\n";

        cout << "\n--- INGRESO DE NUEVOS VALORES ---\n";
        double nuevoGasto = ValidadorEntrada::leerDouble("Ingrese el nuevo gasto ejecutado (S/.): ", 0.0, 50000000.0);
        double nuevoAvance = ValidadorEntrada::leerDouble("Ingrese el nuevo avance fisico (0 - 100 %): ", 0.0, 100.0);
        string fecha = ValidadorEntrada::leerTextoNoVacio("Ingrese la fecha de actualizacion (DD/MM/AAAA): ");
        string obs = ValidadorEntrada::leerTextoNoVacio("Ingrese una observacion del ajuste realizado: ");

        RegistroHistorial hist;
        hist.fecha = fecha;
        hist.gastoAnterior = p.gastoEjecutado;
        hist.gastoNuevo = nuevoGasto;
        hist.avanceAnterior = p.avanceFisicoReal;
        hist.avanceNuevo = nuevoAvance;
        hist.observacion = obs;

        p.historial.push_back(hist);

        p.gastoEjecutado = nuevoGasto;
        p.avanceFisicoReal = nuevoAvance;
        p.fechaUltimaModificacion = fecha;

        cout << "\n[EXITO] Datos actualizados correctamente e historial guardado para la partida " << codigo << ".\n";
    }

    // Genera la tabla principal del reporte de control de obra y alertas
    void generarReporteGeneral() const {
        if (listaPartidas.empty()) {
            cout << "\n[AVISO] No hay partidas registradas para reportar.\n";
            return;
        }

        cout << "\n=================================================================================================================\n";
        cout << "                           REPORTE GENERAL DE CONTROL DE OBRA - ATLANTIC FONTANA                                 \n";
        cout << "=================================================================================================================\n";
        cout << left << setw(8)  << "Codigo"
             << setw(25) << "Descripcion"
             << setw(14) << "Presup.(S/.)"
             << setw(14) << "Gasto(S/.)"
             << setw(10) << "Avance %"
             << setw(11) << "Consumo %"
             << setw(25) << "Estado de Alerta" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------\n";

        double sumaPresupuesto = 0.0;
        double sumaGasto = 0.0;
        int contadorAlertas = 0;

        for (const auto& p : listaPartidas) {
            double consumo = CalculadorControlObra::calcularPorcentajeConsumo(p.gastoEjecutado, p.presupuestoAsignado);
            EstadoAlerta estado = CalculadorControlObra::evaluarEstado(consumo, p.avanceFisicoReal);
            string txtEstado = CalculadorControlObra::obtenerTextoEstado(estado);

            if (estado == EstadoAlerta::ALERTA_SOBRECOSTO || estado == EstadoAlerta::CRITICO_EXCEDIDO) {
                contadorAlertas++;
            }

            sumaPresupuesto += p.presupuestoAsignado;
            sumaGasto += p.gastoEjecutado;

            cout << left << setw(8)  << p.codigoPartida
                 << setw(25) << (p.nombreObra.length() > 23 ? p.nombreObra.substr(0, 22) + "..." : p.nombreObra)
                 << setw(14) << fixed << setprecision(2) << p.presupuestoAsignado
                 << setw(14) << p.gastoEjecutado
                 << setw(10) << p.avanceFisicoReal
                 << setw(11) << consumo
                 << setw(25) << txtEstado << endl;
        }

        cout << "-----------------------------------------------------------------------------------------------------------------\n";
        cout << "RESUMEN EJECUTIVO GLOBAL:\n";
        cout << " - Total Presupuestado : S/. " << fixed << setprecision(2) << sumaPresupuesto << "\n";
        cout << " - Total Gastado       : S/. " << sumaGasto << "\n";

        double consumoGlobal = CalculadorControlObra::calcularPorcentajeConsumo(sumaGasto, sumaPresupuesto);
        cout << " - Consumo Global Obra : " << consumoGlobal << " %\n";
        cout << " - Partidas en Alerta  : " << contadorAlertas << " de " << listaPartidas.size() << " partidas evaluadas.\n";
        cout << "=================================================================================================================\n";
    }

    // Muestra unicamente las partidas con discrepancias entre gasto y avance
    void mostrarFiltroAlertas() const {
        if (listaPartidas.empty()) {
            cout << "\n[AVISO] No hay partidas en el sistema.\n";
            return;
        }

        cout << "\n================================================================================\n";
        cout << "               REPORTES FILTRADOS: PARTIDAS CON RIESGO O SOBRECOSTO             \n";
        cout << "================================================================================\n";

        int encontradas = 0;
        for (const auto& p : listaPartidas) {
            double consumo = CalculadorControlObra::calcularPorcentajeConsumo(p.gastoEjecutado, p.presupuestoAsignado);
            EstadoAlerta estado = CalculadorControlObra::evaluarEstado(consumo, p.avanceFisicoReal);

            if (estado != EstadoAlerta::DENTRO_PRESUPUESTO) {
                encontradas++;
                cout << " > Codigo Partida : " << p.codigoPartida << "\n";
                cout << "   Obra o Partida : " << p.nombreObra << " (" << p.ubicacion << ")\n";
                cout << "   Responsable    : " << p.ingenieroResponsable << "\n";
                cout << "   Presupuesto    : S/. " << fixed << setprecision(2) << p.presupuestoAsignado 
                     << " | Gasto Real: S/. " << p.gastoEjecutado << "\n";
                cout << "   Avance Fisico  : " << p.avanceFisicoReal << " % | Consumo Financiero: " << consumo << " %\n";
                cout << "   ESTADO         : " << CalculadorControlObra::obtenerTextoEstado(estado) << "\n";
                cout << "--------------------------------------------------------------------------------\n";
            }
        }

        if (encontradas == 0) {
            cout << "\n[EXCELENTE] Todas las partidas estan marchando dentro del presupuesto proyectado.\n";
        } else {
            cout << "\nSe identificaron un total de " << encontradas << " partida(s) que requieren intervencion.\n";
        }
        cout << "================================================================================\n";
    }

    // Consulta detallada de una partida de obra especifica
    void consultarPartidaPorCodigo() const {
        if (listaPartidas.empty()) {
            cout << "\n[AVISO] No hay partidas registradas.\n";
            return;
        }

        int codigo = ValidadorEntrada::leerEntero("Ingrese el codigo de la partida a consultar: ", 1, 99999);
        int idx = buscarIndicePorCodigo(codigo);

        if (idx == -1) {
            cout << "\n[ERROR] No se encontro la partida con codigo " << codigo << ".\n";
            return;
        }

        const PartidaObra& p = listaPartidas[idx];
        double consumo = CalculadorControlObra::calcularPorcentajeConsumo(p.gastoEjecutado, p.presupuestoAsignado);
        EstadoAlerta estado = CalculadorControlObra::evaluarEstado(consumo, p.avanceFisicoReal);

        cout << "\n=================================================================\n";
        cout << "           FICHA DE CONTROL INDIVIDUAL DE PARTIDA DE OBRA        \n";
        cout << "=================================================================\n";
        cout << " Codigo Partida      : " << p.codigoPartida << "\n";
        cout << " Nombre de la Obra   : " << p.nombreObra << "\n";
        cout << " Ubicacion o Sede    : " << p.ubicacion << "\n";
        cout << " Ing. Responsable    : " << p.ingenieroResponsable << "\n";
        cout << " Tipo o Categoria    : " << p.tipoPartida << "\n";
        cout << " Presupuesto (S/.)   : S/. " << fixed << setprecision(2) << p.presupuestoAsignado << "\n";
        cout << " Gasto Ejecutado     : S/. " << p.gastoEjecutado << "\n";
        cout << " Saldo Disponible    : S/. " << (p.presupuestoAsignado - p.gastoEjecutado) << "\n";
        cout << " Avance Fisico Real  : " << p.avanceFisicoReal << " %\n";
        cout << " Consumo Financiero  : " << consumo << " %\n";
        cout << " Estado Diagnostico  : " << CalculadorControlObra::obtenerTextoEstado(estado) << "\n";
        cout << " Fecha Registro      : " << p.fechaCreacion << "\n";
        cout << " Ultima Modificacion : " << p.fechaUltimaModificacion << "\n";

        cout << "\n--- HISTORIAL DE REVISIONES Y CAMBIOS (" << p.historial.size() << ") ---\n";
        if (p.historial.empty()) {
            cout << " No se han registrado cambios previos en esta partida.\n";
        } else {
            for (size_t i = 0; i < p.historial.size(); ++i) {
                const auto& h = p.historial[i];
                cout << " [" << (i + 1) << "] Fecha: " << h.fecha << "\n";
                cout << "     Gasto: S/. " << h.gastoAnterior << " -> S/. " << h.gastoNuevo << "\n";
                cout << "     Avance: " << h.avanceAnterior << "% -> " << h.avanceNuevo << "%\n";
                cout << "     Obs: " << h.observacion << "\n";
            }
        }
        cout << "=================================================================\n";
    }

    // Reajusta el presupuesto total asignado a una partida
    void reajustarPresupuestoPartida() {
        if (listaPartidas.empty()) {
            cout << "\n[AVISO] No hay partidas registradas en el sistema.\n";
            return;
        }

        int codigo = ValidadorEntrada::leerEntero("Ingrese el codigo de la partida para reajuste presupuestal: ", 1, 99999);
        int idx = buscarIndicePorCodigo(codigo);

        if (idx == -1) {
            cout << "\n[ERROR] Codigo de partida no encontrado.\n";
            return;
        }

        PartidaObra& p = listaPartidas[idx];

        cout << "\n--- REAJUSTE PRESUPUESTAL ---\n";
        cout << "Partida: " << p.nombreObra << "\n";
        cout << "Presupuesto actual: S/. " << fixed << setprecision(2) << p.presupuestoAsignado << "\n";

        double nuevoPresupuesto = ValidadorEntrada::leerDouble("Ingrese nuevo presupuesto autorizado (S/.): ", 0.01, 50000000.0);
        string obs = ValidadorEntrada::leerTextoNoVacio("Motivo de adenda o reajuste: ");
        string fecha = ValidadorEntrada::leerTextoNoVacio("Fecha de aprobacion (DD/MM/AAAA): ");

        double tempGasto = p.gastoEjecutado;
        double tempAvance = p.avanceFisicoReal;

        p.presupuestoAsignado = nuevoPresupuesto;
        p.fechaUltimaModificacion = fecha;

        RegistroHistorial hist = {fecha, tempGasto, tempGasto, tempAvance, tempAvance, "Reajuste de Presupuesto: " + obs};
        p.historial.push_back(hist);

        cout << "\n[EXITO] Presupuesto reajustado correctamente.\n";
    }

    // Muestra estadisticas avanzadas del proyecto
    void mostrarEstadisticasAvanzadas() const {
        if (listaPartidas.empty()) {
            cout << "\n[AVISO] Sin datos para calcular estadisticas.\n";
            return;
        }

        double maxDesviacion = -999999.0;
        string obraMayorSobrecosto = "Ninguna";

        double minDesviacion = 999999.0;
        string obraMasEficiente = "Ninguna";

        for (const auto& p : listaPartidas) {
            double consumo = CalculadorControlObra::calcularPorcentajeConsumo(p.gastoEjecutado, p.presupuestoAsignado);
            double dif = consumo - p.avanceFisicoReal;

            if (dif > maxDesviacion) {
                maxDesviacion = dif;
                obraMayorSobrecosto = p.nombreObra;
            }
            if (dif < minDesviacion) {
                minDesviacion = dif;
                obraMasEficiente = p.nombreObra;
            }
        }

        cout << "\n=================================================================\n";
        cout << "        INDICADORES DE RENDIMIENTO (KPIs) - CONTROL DE OBRA      \n";
        cout << "=================================================================\n";
        cout << " > Partida mas eficiente (menor gasto o avance) : " << obraMasEficiente << "\n";
        cout << " > Partida con mayor sobrecosto o desviacion    : " << obraMayorSobrecosto << "\n";
        cout << " > Desviacion critica maxima detectada          : " << fixed << setprecision(2) << maxDesviacion << " %\n";
        cout << "=================================================================\n";
    }
};

// ==============================================================================
// CLASE: MenuModuloControlObra
// Presenta la interfaz visual por consola para interactuar con el sistema.
// ==============================================================================
class MenuModuloControlObra {
private:
    GestorControlObra gestor;

public:
    void desplegarMenu() {
        int opcion = 0;
        do {
            cout << "\n=================================================================\n";
            cout << "       ATLANTIC FONTANA CONSTRUCTORA - SISTEMA DE CONTROL       \n";
            cout << "                     MODULO 3: CONTROL DE OBRA                   \n";
            cout << "=================================================================\n";
            cout << " 1. Registrar nueva partida de obra                             \n";
            cout << " 2. Actualizar avance fisico y gasto ejecutado                  \n";
            cout << " 3. Generar reporte general de control y alertas                \n";
            cout << " 4. Filtrar y mostrar partidas con sobrecosto o riesgo          \n";
            cout << " 5. Consultar ficha individual e historial por codigo           \n";
            cout << " 6. Reajustar presupuesto autorizado de partida                \n";
            cout << " 7. Ver indicadores de rendimiento y KPIs                       \n";
            cout << " 8. Salir del programa                                          \n";
            cout << "-----------------------------------------------------------------\n";

            opcion = ValidadorEntrada::leerEntero("Seleccione una opcion del menu (1 - 8): ", 1, 8);

            switch (opcion) {
                case 1:
                    gestor.registrarNuevaPartida();
                    break;
                case 2:
                    gestor.actualizarAvanceYGastos();
                    break;
                case 3:
                    gestor.generarReporteGeneral();
                    break;
                case 4:
                    gestor.mostrarFiltroAlertas();
                    break;
                case 5:
                    gestor.consultarPartidaPorCodigo();
                    break;
                case 6:
                    gestor.reajustarPresupuestoPartida();
                    break;
                case 7:
                    gestor.mostrarEstadisticasAvanzadas();
                    break;
                case 8:
                    cout << "\n=================================================================\n";
                    cout << "   ¡Gracias por utilizar el Sistema de Control de Obra!          \n";
                    cout << "   Atlantic Fontana Constructora - Trabajo Final USIL            \n";
                    cout << "=================================================================\n";
                    break;
                default:
                    cout << "\n [ERROR] Opcion no implementada.\n";
            }
        } while (opcion != 8);
    }
};

// ==============================================================================
// FUNCION PRINCIPAL
// ==============================================================================
void moduloControlObra() {
    cout << fixed << setprecision(2);
    static MenuModuloControlObra interfaz;   // static para no perder las partidas
    interfaz.desplegarMenu();
}
