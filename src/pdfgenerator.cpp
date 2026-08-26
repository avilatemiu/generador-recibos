#include "pdfgenerator.h"

#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QFont>

bool PdfGenerator::generar(const Recibo &recibo, const QString &ruta)
{
    QPdfWriter pdf(ruta);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);

    if (!painter.isActive()) {
        return false;
    }

    QFont font("Times New Roman", 12);
    painter.setFont(font);

    int y = 1000;

    painter.drawText(1000, y, "RECIBO");
    y += 150;

    painter.drawText(1000, y, QString("Numero: %1")
                                  .arg(recibo.getNumero()));
    y += 150;

    painter.drawText(1000, y, QString("Fecha: %1")
                                  .arg(recibo.getFecha().toString("dd/MM/yyyy")));
    y += 150;

    painter.drawText(1000, y, QString("Recibí de %1 el monto de $%2")
                                  .arg(recibo.getCliente(),
                                        QString::number(recibo.getImporte(), 'f', 2)));

    y += 150;

    painter.drawText(1000, y, QString("En concepto de %1")
                                  .arg(recibo.getConcepto()));



    painter.end();

    return true;
}
