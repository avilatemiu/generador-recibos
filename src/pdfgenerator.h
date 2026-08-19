#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include "recibo.h"

#include <QString>

class PdfGenerator
{
public:
    static bool generar(const Recibo &recibo, const QString &ruta);
};

#endif
