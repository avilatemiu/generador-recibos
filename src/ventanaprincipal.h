#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSettings>
#include "database.h"

class VentanaPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = nullptr);

private slots:
    void onGenerarReciboClicked();

private:

    void setupUi();

    //Contenedor de pantallas
    QStackedWidget *stackedWidget;

    //Componente del menú
    QWidget *menuPage;
    QPushButton *irAFormularioButton;

    //Componente del formulario
    QWidget *formularioPage;
    QLineEdit *clienteEdit;
    QLineEdit *cuitEdit;
    QDateEdit *fechaEdit;
    QLineEdit *conceptoEdit;
    QDoubleSpinBox *importeEdit;
    QPushButton *generarButton;
    QPushButton *volverMenuButton;
    // QPushButton *verPeriodo;

    Database database;
    QSettings settings;
    int numeroRecibo;
};

#endif // VENTANAPRINCIPAL_H
