#ifndef VENTANACONFIGURACION_H
#define VENTANACONFIGURACION_H

#include <QWidget>

namespace Ui {
class VentanaConfiguracion;
}

class VentanaConfiguracion : public QWidget
{
    Q_OBJECT

public:
    explicit VentanaConfiguracion(QWidget *parent = 0);
    ~VentanaConfiguracion();

    Ui::VentanaConfiguracion *ui;

    void aplicarValoresIniciales();

};

#endif // VENTANACONFIGURACION_H
