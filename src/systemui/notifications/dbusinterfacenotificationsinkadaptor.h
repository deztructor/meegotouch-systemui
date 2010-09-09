/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp dbusinterfacenotificationsink.xml -a dbusinterfacenotificationsinkadaptor -c DBusInterfaceNotificationSinkAdaptor -l DBusInterfaceNotificationSink -i dbusinterfacenotificationsink.h -i metatypedeclarations.h
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DBUSINTERFACENOTIFICATIONSINKADAPTOR_H_1283267570
#define DBUSINTERFACENOTIFICATIONSINKADAPTOR_H_1283267570

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "dbusinterfacenotificationsink.h"
#include "metatypedeclarations.h"
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.meego.core.MNotificationManager
 */
class DBusInterfaceNotificationSinkAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.meego.core.MNotificationManager")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.meego.core.MNotificationManager\">\n"
"    <method name=\"registerSink\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"service\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"path\"/>\n"
"    </method>\n"
"    <method name=\"unregisterSink\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"service\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"path\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    DBusInterfaceNotificationSinkAdaptor(DBusInterfaceNotificationSink *parent);
    virtual ~DBusInterfaceNotificationSinkAdaptor();

    inline DBusInterfaceNotificationSink *parent() const
    { return static_cast<DBusInterfaceNotificationSink *>(QObject::parent()); }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void registerSink(const QString &service, const QString &path);
    void unregisterSink(const QString &service, const QString &path);
Q_SIGNALS: // SIGNALS
};

#endif
