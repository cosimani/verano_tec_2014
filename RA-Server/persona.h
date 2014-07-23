#ifndef PERSONA_H
#define PERSONA_H

#include <QString>

class Persona  {
public:
    Persona()  {

    }

    QString getNombre() const  {  return nombre;  }
    QString getLinkedin() const  {  return urlLinkedin;  }
    QString getMail() const  {  return mail;  }
    void setNombre(QString nombre)  {  this->nombre = nombre;  }
    void setLinkedin(QString urlLinkedin)  {  this->urlLinkedin = urlLinkedin;  }
    void setMail(QString mail)  {  this->mail = mail;  }

private:
    QString nombre;
    QString mail;
    QString urlLinkedin;
};

#endif // PERSONA_H
