#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QWidget>
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

    QLineEdit *clienteEdit;
    QLineEdit *cuitEdit;
    QDateEdit *fechaEdit;
    QLineEdit *conceptoEdit;
    QDoubleSpinBox *importeEdit;
    QPushButton *generarButton;
    // QPushButton *verPeriodo;

    Database database;
    QSettings settings;
    int numeroRecibo;
};

#endif // VENTANAPRINCIPAL_H
