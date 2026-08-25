#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class Database
{
public:
    Database();

    bool guardarCliente(const QString &nombre, const QString &cuit);
    QList<QPair<QString, QString>> obtenerClientes();

private:
    bool conectar();
    bool crearTablas();
};

#endif
