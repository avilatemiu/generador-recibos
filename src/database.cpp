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

    if (!db.open()) {
        qDebug() << "Error al abrir la base de datos:"
                 << db.lastError().text();
        return false;
    }

    return true;
}

bool Database::guardarCliente(const QString &nombre, const QString &cuit)
{
    QSqlQuery query;

    // Primero buscamos si el CUIT ya existe
    query.prepare(
        "SELECT id FROM clientes "
        "WHERE cuit = :cuit"
        );

    query.bindValue(":cuit", cuit);

    if (!query.exec()) {
        qDebug() << "Error al buscar cliente:"
                 << query.lastError().text();
        return false;
    }

    // Si el CUIT ya existe, actualizamos el nombre
    if (query.next()) {
        int id = query.value(0).toInt();

        query.prepare(
            "UPDATE clientes "
            "SET nombre = :nombre "
            "WHERE id = :id"
            );

        query.bindValue(":nombre", nombre);
        query.bindValue(":id", id);

        if (!query.exec()) {
            qDebug() << "Error al actualizar cliente:"
                     << query.lastError().text();
            return false;
        }

        qDebug() << "Cliente actualizado correctamente";

        return true;
    }

    // Si el CUIT no existe, creamos el cliente
    query.prepare(
        "INSERT INTO clientes (nombre, cuit) "
        "VALUES (:nombre, :cuit)"
        );

    query.bindValue(":nombre", nombre);
    query.bindValue(":cuit", cuit);

    if (!query.exec()) {
        qDebug() << "Error al guardar cliente:"
                 << query.lastError().text();
        return false;
    }

    qDebug() << "Cliente creado correctamente";

    return true;
}
