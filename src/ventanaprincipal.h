#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSettings>
#include <QTableWidget>
#include "database.h"


class VentanaPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = nullptr);

private slots:
    void onGenerarReciboClicked();
    void configurarBotonVolver(QPushButton *boton);
    void cargarMesesDelCliente(int row, int column);

private:

    void setupUi();

    //Contenedor de pantallas
    QStackedWidget *stackedWidget;

    //Componente del menú
    QWidget *menuPage;
    QPushButton *irAFormularioButton;
    QPushButton *registroPagosButton;

    //Componente del formulario
    QWidget *formularioPage;
    QLineEdit *clienteEdit;
    QLineEdit *cuitEdit;
    QDateEdit *fechaEdit;
    QLineEdit *conceptoEdit;
    QDoubleSpinBox *importeEdit;
    QPushButton *generarButton;

    //Configuración
    QPushButton *configButton;

    void abrirConfiguracion();

    //Ver Informe
    QWidget *informePage;
    QTableWidget *tablaClientes;
    QTableWidget *tablaMeses;
    void cargarClientesEnTabla();
    void inicializarTablaMeses();



    Database database;
    QSettings settings;
    int numeroRecibo;
};

#endif // VENTANAPRINCIPAL_H
