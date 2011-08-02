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
#include "ut_batterybusinesslogic.h"

#include "batterybusinesslogic.h"
#include "lowbatterynotifier_stub.h"

#ifdef HAVE_QMSYSTEM
#include "qmled_stub.h"
#include "qmbattery_stub.h"
#include "qmdevicemode_stub.h"
#include "qmdisplaystate_stub.h"
#endif

#include <MNotification>
#include <MFeedback>

/******************************************************************************
 * The stub for MFeedback class.
 */
static QString nameOfLastFeedback;

MFeedback::MFeedback (
		const QString &name, 
		QObject *parent) :
	d_ptr (0)
{
    Q_UNUSED (parent);

    nameOfLastFeedback = name;
}

void
MFeedback::play () const 
{
}

/*
 * TODO:
 *  - test the feedback playing also
 *  [XXX: we need to know first, how to play those correctly...]
 */

QList<MNotification*> gMNotificationPublish;
bool MNotification::publish ()
{
    gMNotificationPublish.append(this);
    return false;
}

QList<QString> gMNotificationRemoveEventType;
bool MNotification::remove ()
{
    gMNotificationRemoveEventType.append(eventType());
    return false;
}

void QTimer::start()
{
    id = 1;
}

void Ut_BatteryBusinessLogic::initTestCase()
{
    /* Add testsuite initialization here */
}

void
Ut_BatteryBusinessLogic::cleanupTestCase ()
{
    /* Add testsuite deinitialization here */
}


void
Ut_BatteryBusinessLogic::init ()
{
    /* Testcase initialization... */
    m_logic = new BatteryBusinessLogic;
}

void
Ut_BatteryBusinessLogic::cleanup ()
{
    /* Testcase deinitialization... */
    delete m_logic;
    m_logic = NULL;
    gMNotificationPublish.clear();
    gMNotificationRemoveEventType.clear();
    gLowBatteryNotifierStub->stubReset();
}

void
Ut_BatteryBusinessLogic::testInitBattery ()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    /* Set some ret. values for stubs */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState> (
        "getChargingState", MeeGo::QmBattery::StateNotCharging);
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::BatteryState> (
        "getBatteryState", MeeGo::QmBattery::StateOK);

    /* no notification should be shown,
     * and battery-charging pattern should be deactivated:
     */
    m_logic->initBattery ();
    /* wait for signals... if any... */
    QTest::qWait (10);

    QCOMPARE (spy.count (), 0);
    QVERIFY (gQmLEDStub->stubLastCallTo ("deactivate").parameter<QString>(0)
             == QString ("PatternBatteryCharging"));
#endif
}

void
Ut_BatteryBusinessLogic::testLowBatteryAlert ()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    m_logic->lowBatteryAlert ();

    QTest::qWait (10);

    QCOMPARE (spy.count (), 1);

    QList<QVariant> arguments = spy.takeFirst ();

    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.lowbattery");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_lowbatt"));
    QVERIFY (arguments.at (2).toString () == "");
#endif
}

void
Ut_BatteryBusinessLogic::testBatteryStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    gQmBatteryStub->stubReset ();
    gQmLEDStub->stubReset ();

    /* StateFull */
    spy.clear ();
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateFull);

    QTest::qWait (10);

    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.chargingcomplete");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_charcomp"));
    QVERIFY (arguments.at (2).toString () == "");
    QVERIFY (gQmLEDStub->stubLastCallTo ("activate").parameter<QString>(0)
             == QString ("PatternBatteryFull"));

    /* StateOK */
    spy.clear ();
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateOK);

    QTest::qWait (10);
    /* no signals/notifications should come, just silently no-op */
    QCOMPARE (spy.count (), 0);

    /* StateEmpty */
    spy.clear ();
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateEmpty);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.recharge");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_rebatt"));
    QVERIFY (arguments.at (2).toString () == "");

    /* StateError */
    spy.clear ();
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateError);

    QTest::qWait (10);
    /* no signals/notifications should come, just silently no-op */
    QCOMPARE (spy.count (), 0);

    /* StateLow and charging */
    spy.clear ();
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState> (
        "getChargingState", MeeGo::QmBattery::StateCharging);
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateLow);

    QTest::qWait (10);
    /* no signals/notifications should come, because battery is charging... */
    QCOMPARE (spy.count (), 0);

    /* StateLow and not charging */
    spy.clear();
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->setTouchScreenLockActive(true);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);
#endif
}

void
Ut_BatteryBusinessLogic::testChargingStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    gQmBatteryStub->stubReset ();
    gQmLEDStub->stubReset ();

    gQmBatteryStub->stubSetReturnValue (
        "getChargerType", MeeGo::QmBattery::Wall);

    for (int i = 0; i <= 100; i += 5)
    {
      /* StateCharging */
      gQmBatteryStub->stubSetReturnValue<int> ("getRemainingCapacityPct", i);
      m_logic->chargingStateChanged (MeeGo::QmBattery::StateCharging);

      QTest::qWait (1);
      QCOMPARE (spy.count (), 1);
      arguments = spy.takeFirst ();
      QVERIFY (arguments.at (0).toString () == "x-nokia.battery");
      QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_charging"));
      QVERIFY (arguments.at (2).toString () == m_logic->chargingImageId ());
      QVERIFY (gQmLEDStub->stubLastCallTo ("activate").parameter<QString>(0)
               == QString ("PatternBatteryCharging"));
      spy.clear ();
    }

    /* StateNotCharging */
    m_logic->chargingStateChanged (MeeGo::QmBattery::StateNotCharging);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 0);
    QVERIFY (gQmLEDStub->stubLastCallTo ("deactivate").parameter<QString>(0)
             == QString ("PatternBatteryCharging"));
    spy.clear ();

    /* StateChargingFailed */
    m_logic->chargingStateChanged (MeeGo::QmBattery::StateChargingFailed);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.chargingnotstarted");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_repcharger"));
    QVERIFY (arguments.at (2).toString () == "");
    spy.clear ();

    /* Test "not enough power to charge" situation... */
    gQmBatteryStub->stubSetReturnValue (
        "getChargerType", MeeGo::QmBattery::USB_100mA);
    m_logic->chargingStateChanged (MeeGo::QmBattery::StateCharging);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.notenoughpower");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_nopowcharge"));
    QVERIFY (arguments.at (2).toString () == "icon-m-energy-management-insufficient-power");
    spy.clear ();
#endif
}

void
Ut_BatteryBusinessLogic::testBatteryChargerEvent ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    /* Wall charger */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::Wall);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::Wall);

    /* Plug out : charger type = none */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::None);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::None);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    /* Look for the notification: "Disconnect the charger from..." */
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.removecharger");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_remcha"));
    QVERIFY (arguments.at (2).toString () == "");
    spy.clear ();

    /* USB 500mA */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::USB_500mA);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::USB_500mA);

    /* USB 100mA */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::USB_100mA);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::USB_100mA);

    /* Unknown */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::Unknown);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::Unknown);
#endif
}

void
Ut_BatteryBusinessLogic::testPSMStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    /* Entering to power-save mode */
    m_logic->devicePSMStateChanged (MeeGo::QmDeviceMode::PSMStateOn);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.enterpsm");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_ent_psnote"));
    QVERIFY (arguments.at (2).toString () == m_logic->chargingImageId());

    spy.clear ();

    /* Exiting from power-save mode */
    m_logic->devicePSMStateChanged (MeeGo::QmDeviceMode::PSMStateOff);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.exitpsm");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_exit_psnote"));
    QVERIFY (arguments.at (2).toString () == m_logic->chargingImageId());
#endif
}

void
Ut_BatteryBusinessLogic::testLowBatteryNotifierConnection ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    gQmBatteryStub->stubSetReturnValue (
        "getChargerType", MeeGo::QmBattery::USB_500mA);

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);

    /* Simulate battery-state-low change */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->setTouchScreenLockActive(true);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY(m_logic->m_LowBatteryNotifier != NULL);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    /* And should send a low-battery notification */
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);

    /* Simulate now a charging event */
    m_logic->chargingStateChanged (MeeGo::QmBattery::StateCharging);

    /* After this call LowBatteryNotifier should be destroyed */
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);

    /* State OK should stop notifications */
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateOK);
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);

    /* State Full should stop notifications */
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateFull);
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStopsThenNotificationRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    QVERIFY(gMNotificationPublish.count() > 0);
    QCOMPARE(gMNotificationPublish.last()->eventType(), QString("x-nokia.battery"));
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery"));
    QCOMPARE(m_logic->notificationTimer.isActive(), false);
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStopsWhenConnectedToWallChargerThenNotificationRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Wall);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    m_logic->batteryChargerEvent (MeeGo::QmBattery::None);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery"));
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStopsMoreThanNSecondAfterBeingStartedThenNotificationNotRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    m_logic->notificationTimer.stop();
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);
    QCOMPARE(gMNotificationRemoveEventType.count(), 0);
#endif
}

void Ut_BatteryBusinessLogic::testSetTouchScreenLockActive()
{
    m_logic->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 0);

#ifdef HAVE_QMSYSTEM
    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_500mA);
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    m_logic->setTouchScreenLockActive(false);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 2);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), false);

    m_logic->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 3);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
#endif
}

QTEST_MAIN(Ut_BatteryBusinessLogic)
