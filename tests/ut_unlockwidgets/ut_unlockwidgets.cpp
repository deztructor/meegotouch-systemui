/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system-ui.
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
#include "ut_unlockwidgets.h"
#include <debug.h>
#include <MTheme>
#include <MApplication>
#include <unlockwidgets.h>
#include <QGraphicsSceneMouseEvent>

int   argc = 1;
char *argv[] = {
    (char *) "./ut_unlockwidgets",
    NULL };
const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

MApplication    *m_App;
MWindow         *m_Window;

void ut_unlockwidgets::initTestCase ()
{
    m_App = new MApplication(argc, argv);

    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
    MTheme::loadCSS (styleDir + "unlockscreen.css");
}

void ut_unlockwidgets::cleanupTestCase ()
{
    delete m_App;
}

void ut_unlockwidgets::init ()
{

}

void ut_unlockwidgets::cleanup ()
{

}

void ut_unlockwidgets::test_unlock_header ()
{
    UnlockHeader *header = new UnlockHeader;

    // Test whether the grab start correctly...
    QSignalSpy spy (header, SIGNAL (activateArea (bool)));

    // Simulate a click event
    //
    QGraphicsSceneMouseEvent press (QEvent::GraphicsSceneMousePress);
    press.ignore ();
    press.setButton (Qt::LeftButton);
    press.setScenePos (header->geometry ().topRight ());
    header->mousePressEvent (&press);
    // an activateArea (true) should come 

    QTest::qWait(100);

    QCOMPARE(spy.count (), 1);
    QList<QVariant> arguments = spy.takeFirst ();
    QVERIFY(arguments.at (0).toBool () == true);

    delete header;
}

void ut_unlockwidgets::test_unlock_area ()
{
    UnlockArea  *area = new UnlockArea;

    delete area;
}

void ut_unlockwidgets::test_unlock_missedevents ()
{
    UnlockNotifications *missedEvents =
        new UnlockNotifications;


    delete missedEvents;
}

QTEST_APPLESS_MAIN (ut_unlockwidgets)

