/*
 * ut_pluginlist.h
 *
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#ifndef UT_PLUGINLIST_H
#define UT_PLUGINLIST_H

#include <QtTest/QtTest>
#include <QObject>
#include <DuiStatusIndicatorMenuPluginInterface>

class DuiApplication;
class DuiApplicationWindow;
class DuiApplicationPage;
class PluginList;

class TestPlugin : public QObject, DuiStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiStatusIndicatorMenuPluginInterface)

public:
    virtual DuiWidget *constructWidget(DuiStatusIndicatorMenuInterface &);
};

class Ut_PluginList : public QObject
{
    Q_OBJECT

public:
    static bool duiApplicationIfProxyLaunchCalled;
    static bool applicationWindowMinimized;
    static bool applicationPageShown;
    static QStringList loadedPlugins;

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test initialization
    void testInitialization();
    // Test returning from a subpage
    void testShowStatusIndicatorMenu();
    // Test hiding the application
    void testHideStatusIndicatorMenu();
    // Test clicking the settings button
    void testSettingsButtonClicked();
    // Test that the notification area shows/hides based on available notifications
    void testNotificationAreaVisibility();

signals:
    void settingsButtonClicked();

private:
    // DuiApplication
    DuiApplication *app;
    // DuiApplicationWindow for testing
    DuiApplicationWindow *applicationWindow;
    // DuiApplicationPage for testing
    DuiApplicationPage *applicationPage;
    // The object being tested
    PluginList *pluginList;
};

#endif