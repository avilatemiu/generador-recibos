#ifndef RECIBO_H
#define RECIBO_H

#include <QString>
#include <QDate>

class Recibo
{
public:
    Recibo();

    QString getCliente() const;
    void setCliente(const QString &cliente);

    QString getCuit() const;
    void setCuit(const QString &cuit);

    QDate getFecha() const;
    void setFecha(const QDate &fecha);

    QString getConcepto() const;
    void setConcepto(const QString &concepto);

    double getImporte() const;
    void setImporte(double importe);

private:
    QString cliente;
    QString cuit;
    QDate fecha;
    QString concepto;
    double importe;
};

#endif // RECIBO_H
