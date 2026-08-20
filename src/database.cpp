#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

Database::Database()
{
    conectar();
}

bool Database::conectar()
{
    QString directorioEjecutable =
        QCoreApplication::applicationDirPath();

    QDir directorio(directorioEjecutable);

    if (!directorio.cdUp()) {
        qDebug() << "Error al acceder al directorio del proyecto";
        return false;
    }

    QString ruta = directorio.filePath("clientes.db");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(ruta);

    if (!db.open()) {
        qDebug() << "Error al abrir la base de datos:"
                 << db.lastError().text();
        return false;
    }

    qDebug() << "Base de datos:" << ruta;

    return true;
}

bool Database::crearTablas()
{
    QSqlQuery query;

    query.prepare(
        "CREATE TABLE IF NOT EXISTS clientes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre TEXT NOT NULL, "
        "cuit TEXT NOT NULL UNIQUE"
        ")"
        );

    if (!query.exec()) {
        qDebug() << "Error al crear tabla clientes:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::guardarCliente(const QString &nombre, const QString &cuit)
{
    // Buscar si el CUIT ya existe
    QSqlQuery buscar;

    if (!buscar.prepare(
            "SELECT id FROM clientes "
            "WHERE cuit = :cuit"
            )) {

        qDebug() << "Error en prepare:"
                 << buscar.lastError().text();

        return false;
    }

    buscar.bindValue(":cuit", cuit);

    buscar.addBindValue(cuit);

    qDebug() << "SQL:" << buscar.lastQuery();
    qDebug() << "Bound values:" << buscar.boundValues();
    qDebug() << "Cuit:" << cuit;

    if (!buscar.exec()) {
        qDebug() << "Error al buscar cliente:"
                 << buscar.lastError().text();
        return false;
    }

    // Si existe, actualizar
    if (buscar.next()) {
        int id = buscar.value(0).toInt();

        QSqlQuery actualizar;

        actualizar.prepare(
            "UPDATE clientes "
            "SET nombre = :nombre "
            "WHERE id = :id"
            );

        actualizar.bindValue(":nombre", nombre);
        actualizar.bindValue(":id", id);

        if (!actualizar.exec()) {
            qDebug() << "Error al actualizar cliente:"
                     << actualizar.lastError().text();
            return false;
        }

        qDebug() << "Cliente actualizado correctamente";

        return true;
    }

    // Si no existe, insertar
    QSqlQuery insertar;

    insertar.prepare(
        "INSERT INTO clientes (nombre, cuit) "
        "VALUES (:nombre, :cuit)"
        );

    insertar.bindValue(":nombre", nombre);
    insertar.bindValue(":cuit", cuit);

    if (!insertar.exec()) {
        qDebug() << "Error al guardar cliente:"
                 << insertar.lastError().text();
        return false;
    }

    qDebug() << "Cliente creado correctamente";

    return true;
}
