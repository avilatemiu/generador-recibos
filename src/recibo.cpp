#include "recibo.h"

Recibo::Recibo()
    : importe(0.0)
{
}

QString Recibo::getCliente() const
{
    return cliente;
}

void Recibo::setCliente(const QString &cliente)
{
    this->cliente = cliente;
}

QString Recibo::getCuit() const
{
    return cuit;
}

void Recibo::setCuit(const QString &cuit)
{
    this->cuit = cuit;
}

QDate Recibo::getFecha() const
{
    return fecha;
}

void Recibo::setFecha(const QDate &fecha)
{
    this->fecha = fecha;
}

QString Recibo::getConcepto() const
{
    return concepto;
}

void Recibo::setConcepto(const QString &concepto)
{
    this->concepto = concepto;
}

double Recibo::getImporte() const
{
    return importe;
}

void Recibo::setImporte(double importe)
{
    this->importe = importe;
}
