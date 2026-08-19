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
    pdf.setPageOrientation(QPageLayout::Portrait);
    pdf.setResolution(300);

    QPainter painter(&pdf);

    if (!painter.isActive()){
        return false;
    }

    const int ancho = pdf.width();
    const int alto = pdf.height();

    //Márgenes

    const int margen = 150;

    //Borde del recibo
    painter.drawRect(
        margen,
        margen,
        ancho - (margen * 2),
        alto - (margen *2)
        );

    //Título
    QFont tituloFont("Times New Roman", 24, QFont::Bold);
    painter.setFont(tituloFont);

    painter.drawText(
        ancho - 1800,
        margen + 500,
        1400,
        400,
        Qt::AlignRight,
        "RECIBO"
        );

    //Número de recibo
    QFont normalFont("Times New Roman", 12);
    painter.setFont(normalFont);

    QString numero = QString("Nº %1")
                         .arg(recibo.getNumero(), 8, 10, QChar('0'));

    painter.drawText(
        ancho - 2500,
        margen + 1000,
        2100,
        300,
        Qt::AlignRight,
        numero
        );

    // Fecha
    QString fecha = QString("FECHA: %1")
                        .arg(recibo.getFecha().toString("dd/MM/yyyy"));

    painter.drawText(
        ancho - 3000,
        margen + 1500,
        2600,
        300,
        Qt::AlignRight,
        fecha
        );

    // Texto principal
    int y = margen + 2500;

    painter.drawText(
        margen + 300,
        y,
        ancho - (margen * 2) - 600,
        400,
        Qt::AlignLeft,
        QString("Recibí de %1").arg(recibo.getCliente())
        );

    y += 900;

    painter.drawText(
        margen + 300,
        y,
        ancho - (margen * 2) - 600,
        400,
        Qt::AlignLeft,
        QString("la cantidad de pesos $%1")
            .arg(recibo.getImporte(), 0, 'f', 2)
        );

    y += 900;

    painter.drawText(
        margen + 300,
        y,
        ancho - (margen * 2) - 600,
        400,
        Qt::AlignLeft,
        QString("en concepto de %1").arg(recibo.getConcepto())
        );

    // Importe
    y += 1200;

    painter.drawText(
        margen + 300,
        y,
        700,
        400,
        Qt::AlignLeft,
        "Son: $"
        );

    painter.drawRect(
        margen + 1000,
        y - 50,
        ancho - margen - 1300,
        500
        );

    painter.drawText(
        margen + 1150,
        y,
        ancho - margen - 1600,
        400,
        Qt::AlignLeft,
        QString("%1").arg(recibo.getImporte(), 0, 'f', 2)
        );

    // Firma
    int firmaY = alto - margen - 1000;

    painter.drawLine(
        ancho - 2500,
        firmaY,
        ancho - margen - 300,
        firmaY
        );

    painter.drawText(
        ancho - 2500,
        firmaY + 100,
        2200,
        400,
        Qt::AlignCenter,
        "Firma"
        );

    painter.end();

    return true;

}
