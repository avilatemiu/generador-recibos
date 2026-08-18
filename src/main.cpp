#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include "recibo.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QSettings settings("MiAplicacion","GeneradorRecibos");

    QApplication app(argc, argv);

    Database database;

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

    QLabel *importeLabel = new QLabel("Importe: ");
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

    int numeroRecibo = settings.value("ultimoNumero", 1).toInt();

    //Esto es una señal y el [&ventana]() un lambda de cpp
    QObject::connect(generarButton,
                     &QPushButton::clicked,
                     &ventana, [&ventana, &numeroRecibo, &settings, &database, clienteEdit, cuitEdit,
                      fechaEdit, conceptoEdit, importeEdit]() {

                         Recibo recibo;

                         recibo.setNumero(numeroRecibo);
                         recibo.setCliente(clienteEdit->text());
                         recibo.setCuit(cuitEdit->text());
                         recibo.setFecha(fechaEdit->date());
                         recibo.setConcepto(conceptoEdit->text());
                         recibo.setImporte(importeEdit->value());

                         if (!recibo.reciboValido()){
                             QMessageBox::warning(
                                 &ventana,
                                 "Datos inválidos",
                                 "Completá todos los campos correctamente."
                                 );
                             return;
                         }

                         if (!database.guardarCliente(recibo.getCliente(), recibo.getCuit())){
                             QMessageBox::warning(
                                 &ventana,
                                 "Error",
                                 "No se pudo guardar el cliente en la base de datos.");
                             return;
                         }

                         QString mensaje = QString(
                             "Recibo N.°%1\n"
                             "Fecha: %2\n"
                             "Recibí de %3 con CUIT %4 el monto de $ %6\n"
                             "En concepto de %5")
                                               .arg(recibo.getNumero(), 6, 10, QChar('0'))
                                               .arg(
                                                   recibo.getFecha().toString("dd/MM/yyyy"),
                                                   recibo.getCliente(),
                                                   recibo.getCuit(),
                                                   recibo.getConcepto()
                                                   ).arg(
                                                   recibo.getImporte(),0,'f',2
                                                   );

                         QMessageBox::information(
                             &ventana,
                             "Datos del recibo",
                             mensaje
                             );

                         numeroRecibo++;
                         settings.setValue("ultimoNumero", numeroRecibo);

                        }
                     );

    ventana.setLayout(layout);

    ventana.show();

    return app.exec();
}

