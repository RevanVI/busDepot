#ifndef CREATEMESSAGE_H
#define CREATEMESSAGE_H

/*
 * types
 * 1 - ошибка при обращении к БД
 *
*/

#include <QMessageBox>
#include <QVariant>

QMessageBox* createMessage(const QString& sender, qint32 type, QVariant addInfo = 0);

#endif // CREATEMESSAGE_H
