#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database()
{
    conectar();
}

bool Database::conectar()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("clientes.db");

    if (!db.open()){
        qDebug() << "Error al abrir la base de datos:"
                 << db.lastError().text();
        return false;
    }

    return true;
}

bool Database::guardarCliente(const QString &nombre, const QString &cuit)
{
    QSqlQuery query;

    query.prepare(
        "INSERT OR IGNORE INTO clientes (nombre, cuit)"
        "VALUES (:nombre, :cuit)"
        );
    query.bindValue(":nombre", nombre);
    query.bindValue(":cuit", cuit);

    if (!query.exec()) {
        qDebug() << "Erro al guardar cliente:"
                 << query.lastError().text();
                    return false;
    }

    return true;
}
