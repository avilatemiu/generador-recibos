#include "ventanaprincipal.h"
#include "recibo.h"
#include "pdfgenerator.h"

#include <QStackedWidget>
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

    connect(irAFormularioButton, &QPushButton::clicked, this, [this](){
             stackedWidget -> setCurrentIndex(1);
    });

    connect(volverMenuButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(generarButton, &QPushButton::clicked,
            this, &VentanaPrincipal::onGenerarReciboClicked);

    connect(configButton, &QPushButton::clicked, this, [this](){
        abrirConfiguracion();
    });

}
void VentanaPrincipal::setupUi()
{
    resize(800, 600);
    setWindowTitle("Generador de Recibos");

    stackedWidget = new QStackedWidget(this);

    //Barra de tareas

    QHBoxLayout *topBarLayout = new QHBoxLayout();

    configButton = new QPushButton("Configuración");
    configButton -> setFixedWidth(120);

    topBarLayout->addWidget(configButton);
    topBarLayout->addStretch();

    //Menu

    menuPage = new QWidget();
    QVBoxLayout *menuLayout = new QVBoxLayout(menuPage);

    irAFormularioButton = new QPushButton("Generar nuevo recibo", menuPage);
    irAFormularioButton->setFixedHeight(40);

    menuLayout->addStretch();
    menuLayout->addWidget(irAFormularioButton);
    menuLayout->addStretch();
    //menuPage->setLayout(menuLayout);

    //Formulario
    formularioPage = new QWidget();

    clienteEdit = new QLineEdit(formularioPage);
    cuitEdit = new QLineEdit(formularioPage);

    fechaEdit = new QDateEdit(formularioPage);
    fechaEdit->setCalendarPopup(true);
    fechaEdit->setDate(QDate::currentDate());

    conceptoEdit = new QLineEdit(formularioPage);

    importeEdit = new QDoubleSpinBox(formularioPage);
    importeEdit->setMaximum(99999999.99);
    importeEdit->setDecimals(2);
    importeEdit->setPrefix("$ ");

    generarButton = new QPushButton("Generar recibo", formularioPage);
    volverMenuButton = new QPushButton("Volver al menú", formularioPage);

    QFormLayout *formLayout = new QFormLayout(formularioPage);
    formLayout->addRow("Cliente:", clienteEdit);
    formLayout->addRow("CUIT:", cuitEdit);
    formLayout->addRow("Fecha:", fechaEdit);
    formLayout->addRow("Concepto:", conceptoEdit);
    formLayout->addRow("Importe:", importeEdit);
    formLayout->addRow(generarButton);
    formLayout->addRow(volverMenuButton);

    // verPeriodo = new QPushButton("Ver períodos", this);
    // verPeriodo -> move (100,100);

    //Páginas del stacked widget

    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(formularioPage);

    stackedWidget->setCurrentIndex(0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topBarLayout);
    mainLayout->addWidget(stackedWidget);

    setLayout(mainLayout);
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

void VentanaPrincipal::abrirConfiguracion()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Configuración");
    dialog.resize(300,200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(new QLabel("Opciones de configuaricón del sistema...", &dialog));

    QPushButton *cerrarBtn = new QPushButton("Aceptar", &dialog);
    connect(cerrarBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(cerrarBtn);

    dialog.exec();
}
