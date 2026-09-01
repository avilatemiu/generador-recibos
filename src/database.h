#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class Database
{
public:
    Database();

    bool guardarCliente(const QString &nombre, const QString &cuit);
    QList<QPair<QString, QString>> obtenerClientes();

    bool registrarPago(const QString &cuit, int mes, int anio);
    QList<int> obtenerMesesPagados(const QString &cuit, int anio);

private:
    bool conectar();
    bool crearTablas();
};

#endif
