#include "ventanaprincipal.h"
#include "recibo.h"
#include "pdfgenerator.h"

#include <QLabel>
#include <QFormLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QWidget(parent),
    settings("MiAplicacion", "GeneradorRecibos")
{
    setupUi();

    numeroRecibo = settings.value("ultimoNumero", 1).toInt();

    connect(generarButton, &QPushButton::clicked,
            this, &VentanaPrincipal::onGenerarReciboClicked);

    // connect(verPeriodo, &QPushButton::clicked, [](){
    //     qDebug() << "Botón presionado!";
    // });
}

void VentanaPrincipal::setupUi()
{
    resize(800, 600);
    setWindowTitle("Generador de Recibos");

    clienteEdit = new QLineEdit(this);
    cuitEdit = new QLineEdit(this);

    fechaEdit = new QDateEdit(this);
    fechaEdit->setCalendarPopup(true);
    fechaEdit->setDate(QDate::currentDate());

    conceptoEdit = new QLineEdit(this);

    importeEdit = new QDoubleSpinBox(this);
    importeEdit->setMaximum(99999999.99);
    importeEdit->setDecimals(2);
    importeEdit->setPrefix("$ ");

    generarButton = new QPushButton("Generar recibo", this);

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow("Cliente:", clienteEdit);
    layout->addRow("CUIT:", cuitEdit);
    layout->addRow("Fecha:", fechaEdit);
    layout->addRow("Concepto:", conceptoEdit);
    layout->addRow("Importe:", importeEdit);
    layout->addRow(generarButton);

    // verPeriodo = new QPushButton("Ver períodos", this);
    // verPeriodo -> move (100,100);

    setLayout(layout);
}

void VentanaPrincipal::onGenerarReciboClicked()
{
    Recibo recibo;
    recibo.setNumero(numeroRecibo);
    recibo.setCliente(clienteEdit->text());
    recibo.setCuit(cuitEdit->text());
    recibo.setFecha(fechaEdit->date());
    recibo.setConcepto(conceptoEdit->text());
    recibo.setImporte(importeEdit->value());

    if (!recibo.reciboValido()) {
        QMessageBox::warning(this, "Datos inválidos",
                             "Completá todos los campos correctamente.");
        return;
    }

    if (!database.guardarCliente(recibo.getCliente(), recibo.getCuit())) {
        QMessageBox::warning(this, "Error",
                             "No se pudo guardar el cliente en la base de datos.");
        return;
    }

    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar recibo",
        QString("recibo_%1.pdf").arg(recibo.getNumero(), 6, 10, QChar('0')),
        "Archivos PDF (*.pdf)"
        );

    if (ruta.isEmpty()) {
        return;
    }

    if (!ruta.endsWith(".pdf", Qt::CaseInsensitive)) {
        ruta += ".pdf";
    }

    if (!PdfGenerator::generar(recibo, ruta)) {
        QMessageBox::critical(this, "Error", "No se pudo generar el PDF.");
        return;
    }

    QMessageBox::information(
        this,
        "Recibo generado",
        QString("El recibo fue generado correctamente en:\n%1").arg(ruta)
        );

    numeroRecibo++;
    settings.setValue("ultimoNumero", numeroRecibo);
}
