#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class Database
{
public:
    Database();

    bool guardarCliente(const QString &nombre, const QString &cuit);

private:
    bool conectar();
};

#endif
