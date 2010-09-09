/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "dbusinterfacenotificationsink.h"
#include "dbusinterfacenotificationsinkadaptor.h"
#include "dbusinterfacenotificationsinkproxy.h"

DBusInterfaceNotificationSink::DBusInterfaceNotificationSink()
{
    new DBusInterfaceNotificationSinkAdaptor(this);
}

DBusInterfaceNotificationSink::~DBusInterfaceNotificationSink()
{
}

void DBusInterfaceNotificationSink::registerSink(const QString &service, const QString &path)
{
    proxies.insert(QPair<QString, QString>(service, path),
                   QSharedPointer<DBusInterfaceNotificationSinkProxy>(new DBusInterfaceNotificationSinkProxy(service, path, QDBusConnection::sessionBus())));
}

void DBusInterfaceNotificationSink::unregisterSink(const QString &service, const QString &path)
{
    proxies.remove(QPair<QString, QString>(service, path));
}

void DBusInterfaceNotificationSink::addNotification(const Notification &notification)
{
    foreach (QSharedPointer<DBusInterfaceNotificationSinkProxy> proxy, proxies) {
        proxy->addNotification(notification);
    }
}

void DBusInterfaceNotificationSink::removeNotification(uint notificationId)
{
    foreach (QSharedPointer<DBusInterfaceNotificationSinkProxy> proxy, proxies) {
        proxy->removeNotification(notificationId);
    }
}

void DBusInterfaceNotificationSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    foreach (QSharedPointer<DBusInterfaceNotificationSinkProxy> proxy, proxies) {
        proxy->addGroup(groupId, parameters);
    }
}

void DBusInterfaceNotificationSink::removeGroup(uint groupId)
{
    foreach (QSharedPointer<DBusInterfaceNotificationSinkProxy> proxy, proxies) {
        proxy->removeGroup(groupId);
    }
}
