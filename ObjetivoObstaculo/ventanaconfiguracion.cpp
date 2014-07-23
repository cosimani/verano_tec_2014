#include "ventanaconfiguracion.h"
#include "ui_ventanaconfiguracion.h"

VentanaConfiguracion::VentanaConfiguracion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentanaConfiguracion)
{
    ui->setupUi(this);

    connect(ui->sMatizMinRojo, SIGNAL(valueChanged(int)), ui->lValorMatizMinRojo, SLOT(setNum(int)));
    connect(ui->sMatizMaxRojo, SIGNAL(valueChanged(int)), ui->lValorMatizMaxRojo, SLOT(setNum(int)));
    connect(ui->sSaturacionRojo, SIGNAL(valueChanged(int)), ui->lValorSaturacionRojo, SLOT(setNum(int)));
    connect(ui->sBrilloRojo, SIGNAL(valueChanged(int)), ui->lValorBrilloRojo, SLOT(setNum(int)));
    connect(ui->sMatizMinVerde, SIGNAL(valueChanged(int)), ui->lValorMatizMinVerde, SLOT(setNum(int)));
    connect(ui->sMatizMaxVerde, SIGNAL(valueChanged(int)), ui->lValorMatizMaxVerde, SLOT(setNum(int)));
    connect(ui->sSaturacionVerde, SIGNAL(valueChanged(int)), ui->lValorSaturacionVerde, SLOT(setNum(int)));
    connect(ui->sBrilloVerde, SIGNAL(valueChanged(int)), ui->lValorBrilloVerde, SLOT(setNum(int)));

}

VentanaConfiguracion::~VentanaConfiguracion()
{
    delete ui;
}

void VentanaConfiguracion::aplicarValoresIniciales()  {
    ui->sMatizMinRojo->setValue(159);
    ui->sMatizMaxRojo->setValue(180);
    ui->sSaturacionRojo->setValue(172);
    ui->sBrilloRojo->setValue(63);
    ui->sMatizMinVerde->setValue(36);
    ui->sMatizMaxVerde->setValue(76);
    ui->sSaturacionVerde->setValue(69);
    ui->sBrilloVerde->setValue(66);
    ui->cbRojoVerde->setChecked(true);
}
