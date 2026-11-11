/*
 * Course: COEN 2220 - Programming 2
 * Name: [John Badillo Lamourt]
 * Lab: Lab 12 - Project Application (Delivery Report)
 * Description: Filter, sort, and lookup delivery orders using a pointer view
 * Due date: [November 10, 2026]
 */

#include <iostream>
#include <string>

using namespace std;

struct OrderPreview {
    int orderId;
    string orderType;
    string status;
    string building;
    int priority;
};

// Prototipos
void displayOrders(const OrderPreview orders[], int count);
void displayView(OrderPreview* view[], int count);
void swapOrderPointers(OrderPreview*& left, OrderPreview*& right);
bool compareOrders(OrderPreview* a, OrderPreview* b);
int partitionOrders(OrderPreview* view[], int first, int last);
void quickSortOrders(OrderPreview* view[], int first, int last);
int findOrderInView(OrderPreview* view[], int count, int targetId);

int main() {
    const int CAPACITY = 6;

    // 1. Datos base especificados en la Parte E1 del laboratorio
    OrderPreview orders[CAPACITY] = {
        {420, "DELIVERY", "PENDING", "Biblioteca", 1},
        {180, "DELIVERY", "IN_PROGRESS", "Ingenieria", 3},
        {650, "DINE_IN", "PENDING", "Cafeteria-Lunch", 1},
        {310, "DELIVERY", "PENDING", "Biblioteca", 3},
        {205, "DELIVERY", "PENDING", "Biblioteca", 3},
        {900, "DELIVERY", "COMPLETED", "Ingenieria", 2}
    };

    OrderPreview* activeView[CAPACITY];
    int activeCount = 0;

    // Mostrar registros originales antes de cualquier operación
    cout << "=== Original arrival records (Before filtering/sorting) ===" << endl;
    displayOrders(orders, CAPACITY);

    // 2. Filtrar órdenes activas de tipo DELIVERY (PENDING o IN_PROGRESS)
    for (int i = 0; i < CAPACITY; ++i) {
        if (orders[i].orderType == "DELIVERY" && 
            (orders[i].status == "PENDING" || orders[i].status == "IN_PROGRESS")) {
            activeView[activeCount] = &orders[i];
            activeCount++;
        }
    }

    // 3. Ordenar la vista utilizando Quicksort con la regla multirregla
    quickSortOrders(activeView, 0, activeCount - 1);

    cout << "\n=== Active Delivery Report (Sorted by Building, Priority desc, ID asc) ===" << endl;
    displayView(activeView, activeCount);

    // 4. Consulta interactiva por ID (Búsqueda lineal en la vista)
    int targetId;
    cout << "\nEnter an order ID to search in active view: ";
    if (!(cin >> targetId)) {
        cout << "Invalid input." << endl;
        return 1;
    }

    int position = findOrderInView(activeView, activeCount, targetId);
    if (position == -1) {
        cout << "Order ID " << targetId << " not found in active delivery view." << endl;
    } else {
        cout << "Found! Order ID: " << activeView[position]->orderId
             << " | Building: " << activeView[position]->building
             << " | Status: " << activeView[position]->status
             << " | Priority: " << activeView[position]->priority << endl;
    }

    // Comprobación de inmutabilidad: mostrar registros originales otra vez
    cout << "\n=== Original arrival records after report and search (Unchanged) ===" << endl;
    displayOrders(orders, CAPACITY);

    return 0;
}

// Muestra los registros originales
void displayOrders(const OrderPreview orders[], int count) {
    for (int i = 0; i < count; ++i) {
        cout << "ID: " << orders[i].orderId
             << " | Type: " << orders[i].orderType
             << " | Status: " << orders[i].status
             << " | Building: " << orders[i].building
             << " | Priority: " << orders[i].priority << endl;
    }
}

// Muestra la vista de punteros
void displayView(OrderPreview* view[], int count) {
    for (int i = 0; i < count; ++i) {
        cout << "ID: " << view[i]->orderId
             << " | Type: " << view[i]->orderType
             << " | Status: " << view[i]->status
             << " | Building: " << view[i]->building
             << " | Priority: " << view[i]->priority << endl;
    }
}

// Intercambia dos punteros por referencia
void swapOrderPointers(OrderPreview*& left, OrderPreview*& right) {
    OrderPreview* temp = left;
    left = right;
    right = temp;
}

// Criterio de ordenación múltiple:
// 1. Edificio (ascendente alfabético)
// 2. Prioridad (descendente: mayor prioridad primero)
// 3. ID de orden (ascendente para desempatar)
bool compareOrders(OrderPreview* a, OrderPreview* b) {
    if (a->building != b->building) {
        return a->building < b->building; // Edificio ascendente
    }
    if (a->priority != b->priority) {
        return a->priority > b->priority; // Prioridad descendente
    }
    return a->orderId < b->orderId;       // ID ascendente
}

int partitionOrders(OrderPreview* view[], int first, int last) {
    int middle = first + (last - first) / 2;
    swapOrderPointers(view[first], view[middle]);
    
    OrderPreview* pivot = view[first];
    int boundary = first;

    for (int scan = first + 1; scan <= last; ++scan) {
        if (compareOrders(view[scan], pivot)) {
            boundary++;
            swapOrderPointers(view[boundary], view[scan]);
        }
    }
    swapOrderPointers(view[first], view[boundary]);
    return boundary;
}

void quickSortOrders(OrderPreview* view[], int first, int last) {
    if (first >= last) {
        return;
    }
    int pivotIndex = partitionOrders(view, first, last);
    quickSortOrders(view, first, pivotIndex - 1);
    quickSortOrders(view, pivotIndex + 1, last);
}

// Búsqueda lineal en la vista (necesaria porque el reporte está ordenado principalmente por edificio/prioridad y no por ID)
int findOrderInView(OrderPreview* view[], int count, int targetId) {
    for (int i = 0; i < count; ++i) {
        if (view[i]->orderId == targetId) {
            return i;
        }
    }
    return -1;
}