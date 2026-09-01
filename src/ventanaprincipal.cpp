#include "ventanaprincipal.h"
#include "recibo.h"
#include "pdfgenerator.h"

#include <QStackedWidget>
#include <QLabel>
#include <QFormLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
#include <QTableWidget>
#include <QHeaderView>
#include <QStandardPaths>
#include <QDir>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QWidget(parent),
    settings("MiAplicacion", "GeneradorRecibos")
{
    setupUi();

    numeroRecibo = settings.value("ultimoNumero", 1).toInt();

    connect(irAFormularioButton, &QPushButton::clicked, this, [this](){
             stackedWidget -> setCurrentIndex(1);
    });


    connect(generarButton, &QPushButton::clicked,
            this, &VentanaPrincipal::onGenerarReciboClicked);

    connect(configButton, &QPushButton::clicked, this, [this](){
        abrirConfiguracion();
    });

    connect(registroPagosButton,&QPushButton::clicked, this, [this](){
        stackedWidget->setCurrentIndex(2);
        cargarInformePagos();
    });

}
void VentanaPrincipal::setupUi()
{
    resize(800, 600);
    setWindowTitle("Generador de Recibos");

    stackedWidget = new QStackedWidget(this);

    //***************//
    //Barra de tareas//
    //***************//

    QHBoxLayout *topBarLayout = new QHBoxLayout();

    configButton = new QPushButton("Configuración");
    configButton -> setFixedWidth(120);

    topBarLayout->addWidget(configButton);
    topBarLayout->addStretch();
    //***************//
    //-----Menu------//
    //***************//

    menuPage = new QWidget();
    QVBoxLayout *menuLayout = new QVBoxLayout(menuPage);

    irAFormularioButton = new QPushButton("Generar nuevo recibo", menuPage);
    irAFormularioButton -> setFixedHeight(40);

    registroPagosButton = new QPushButton("Ver Informe de Pagos", menuPage);
    registroPagosButton -> setFixedHeight(40);

    menuLayout->addStretch();
    menuLayout->addWidget(irAFormularioButton);
    menuLayout->addWidget(registroPagosButton);
    menuLayout->addStretch();
    // menuPage->setLayout(menuLayout);

    //***************//
    //--Formulario---//
    //***************//

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
    QPushButton *volverDesdeForm = new QPushButton("Volver al menú", formularioPage);
    configurarBotonVolver(volverDesdeForm);


    QFormLayout *formLayout = new QFormLayout(formularioPage);
    formLayout->addRow("Cliente:", clienteEdit);
    formLayout->addRow("CUIT:", cuitEdit);
    formLayout->addRow("Fecha:", fechaEdit);
    formLayout->addRow("Concepto:", conceptoEdit);
    formLayout->addRow("Importe:", importeEdit);
    formLayout->addRow(generarButton);
    formLayout->addRow(volverDesdeForm);

    //***************//
    //Informe de Pagos//
    //***************//

    informePage = new QWidget();
    QVBoxLayout *informeLayout = new QVBoxLayout(informePage);

    tablaInformes = new QTableWidget(informePage);

    QStringList cabeceras = {
        "Cliente", "CUIT", "Ene",
        "Feb", "Mar", "Abr", "May",
        "Jun", "Jul", "Ago", "Sep",
        "Oct", "Nov", "Dic"
    };
    
    tablaInformes->setColumnCount(cabeceras.size());
    tablaInformes->setHorizontalHeaderLabels(cabeceras);
    
    tablaInformes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tablaInformes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    for (int i=2; i < 14; i++){
        tablaInformes->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    QPushButton *volverDesdeInforme = new QPushButton("Volver al menú", informePage);
    configurarBotonVolver(volverDesdeInforme);

    informeLayout->addWidget(tablaInformes, 1);
    informeLayout->addWidget(volverDesdeInforme, 0);

    //Páginas del stacked widget

    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(formularioPage);
    stackedWidget->addWidget(informePage);

    stackedWidget->setCurrentIndex(0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topBarLayout);
    mainLayout->addWidget(stackedWidget);

    setLayout(mainLayout);
}

void VentanaPrincipal::configurarBotonVolver(QPushButton *boton)
{
    // Conecta cualquier botón que le pases para que cambie al menú (índice 0)
    connect(boton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
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

    database.registrarPago(recibo.getCuit(), recibo.getFecha().month(), recibo.getFecha().year());

    QString carpetaDocumentos = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString carpetaDestino = carpetaDocumentos + "/MisRecibos";
    QDir().mkpath(carpetaDestino);

    QString nombreArchivo = QString("recibo_%1.pdf").arg(recibo.getNumero(), 6, 10, QChar('0'));
    QString ruta = carpetaDestino + "/" + nombreArchivo;

    if (!PdfGenerator::generar(recibo,ruta)){
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

void VentanaPrincipal::cargarInformePagos()
{
    if (!tablaInformes) return;

    tablaInformes->clearContents();
    tablaInformes->setRowCount(0);

    QList<QPair<QString, QString>> clientes = database.obtenerClientes();
    int anioActual = QDate::currentDate().year();

    for (int i = 0; i < clientes.size(); ++i) {
        tablaInformes->insertRow(i);

        QString cliente = clientes[i].first;
        QString cuit = clientes[i].second;

        // Columna 0: Cliente
        tablaInformes->setItem(i, 0, new QTableWidgetItem(cliente));

        // Columna 1: CUIT
        tablaInformes->setItem(i, 1, new QTableWidgetItem(cuit));

        // 1. Obtener la lista de meses pagados (1 al 12) desde la base de datos para este CUIT y Año
        QList<int> mesesPagados = database.obtenerMesesPagados(cuit, anioActual);

        // Columnas 2 a 13 equivalen a los meses 1 a 12 (Enero = columna 2, Febrero = columna 3, etc.)
        for (int mes = 1; mes <= 12; ++mes) {
            int columna = mes + 1; // Mapeo: mes 1 -> col 2, mes 12 -> col 13

            // 2. Verificar si el mes está pagado
            bool estaPagado = mesesPagados.contains(mes);
            QString texto = estaPagado ? "X" : "-";

            QTableWidgetItem *itemMes = new QTableWidgetItem(texto);
            itemMes->setTextAlignment(Qt::AlignCenter);

            // 3. Destacar la "X" visualmente
            if (estaPagado) {
                itemMes->setForeground(Qt::darkGreen);
                QFont font = itemMes->font();
                font.setBold(true);
                itemMes->setFont(font);
            }

            tablaInformes->setItem(i, columna, itemMes);
        }
    }
}
