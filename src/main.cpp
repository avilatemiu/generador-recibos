#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include "recibo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget ventana;
    ventana.resize(800, 600);
    ventana.setWindowTitle("Generador de Recibos");

    QLabel *clienteLabel = new QLabel("Cliente: ");
    QLineEdit *clienteEdit = new QLineEdit();

    QLabel *cuitLabel = new QLabel("CUIT: ");
    QLineEdit *cuitEdit = new QLineEdit();

    QLabel *fechaLabel = new QLabel("Fecha: ");
    QDateEdit *fechaEdit = new QDateEdit();
    fechaEdit -> setCalendarPopup(true);
    fechaEdit -> setDate(QDate::currentDate());

    QLabel *conceptoLabel = new QLabel("Concepto: ");
    QLineEdit *conceptoEdit = new QLineEdit();

    QLabel *importeLabel = new QLabel("Impoete: ");
    QDoubleSpinBox *importeEdit = new QDoubleSpinBox();
    importeEdit -> setMaximum(99999999.99);
    importeEdit -> setDecimals(2);
    importeEdit -> setPrefix("$ ");

    QPushButton *generarButton = new QPushButton("Generar recibo");

    QFormLayout *layout = new QFormLayout();
    layout->addRow(clienteLabel, clienteEdit);
    layout->addRow(cuitLabel, cuitEdit);
    layout->addRow(fechaLabel, fechaEdit);
    layout->addRow(conceptoLabel, conceptoEdit);
    layout->addRow(importeLabel, importeEdit);
    layout->addRow(generarButton);

    //Esto es una señal y el [&ventana]() un lambda de cpp
    QObject::connect(generarButton,
                     &QPushButton::clicked,
                     &ventana, [&ventana, clienteEdit, cuitEdit,
                      fechaEdit, conceptoEdit, importeEdit]() {

                         Recibo recibo;

                         recibo.setCliente(clienteEdit->text());
                         recibo.setCuit(cuitEdit->text());
                         recibo.setFecha(fechaEdit->date());
                         recibo.setConcepto(conceptoEdit->text());
                         recibo.setImporte(importeEdit->value());

                         QString mensaje = QString(
                             "Cliente : %1\n"
                             "CUIT: %2\n"
                             "Fecha: %3\n"
                             "Concepto: %4\n"
                             "Importe: %5")
                                               .arg(
                                                   recibo.getCliente(),
                                                   recibo.getCuit(),
                                                   recibo.getFecha().toString("dd/MM/yyyy"),
                                                   recibo.getConcepto()
                                                   ).arg(
                                                   recibo.getImporte(), 0, 'f', 2
                                                   );

                         QMessageBox::information(
                             &ventana,
                             "Datos del recibo",
                             mensaje
                             );
                        }
                     );

    ventana.setLayout(layout);

    ventana.show();

    return app.exec();
}

