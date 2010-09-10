/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#ifndef LOCKSCREENUI_H
#define LOCKSCREENUI_H

#include <MSceneWindow>
#include <MImageWidget>
#include <MWindow>
#include <QBasicTimer>
#include <MOverlay>

class QPointF;
class QPixmap;
class MWidget;
class QGraphicsLinearLayout;
class QGraphicsSceneMouseEvent;
class MGConfItem;
class QShowEvent;

class LockScreenWindow : public MSceneWindow
{
    Q_OBJECT

public:
    LockScreenWindow (MWindow *window, MWidget *locklift, MWidget *lockland);
    ~LockScreenWindow ();

    void resetState ();

    enum dnd_state {
        STATE_NONE = 0,
        STATE_MOVING,
        STATE_MOVING_ACTIVE
    };

signals:
    void unlocked (); 

protected:
    virtual void paint (
            QPainter                       *painter, 
            const QStyleOptionGraphicsItem *option, 
            QWidget                        *widget = 0);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);

private slots:
    void reloadLandscapeBackground ();
    void reloadPortraitBackground ();

    void redraw ();
    void redrawIdle ();

private:
    void updateDnDicon ();

    // The background GConf keys:
    MGConfItem      *m_confBgLandscape;
    MGConfItem      *m_confBgPortrait;
    // And the background images:
    QPixmap          m_bgLandscape;
    QPixmap          m_bgPortrait;
    // Needed to show the DnD icon on the window:
    MWindow         *m_Window;
    // The DnD icon:
    MImageWidget    *m_DnDicon;
    MOverlay         m_DnDoverlay;
    // The DnD state 
    int              m_DnDstate;
    // Needed to determine the current DnD states:
    MWidget         *m_LockLiftArea;
    MWidget         *m_LockLandArea;
    // for frame-dropping
    bool             m_pendingDraw;
    QPointF          m_DnDposition;

#ifdef UNIT_TEST
    friend class Ut_LockScreenUI;
    friend class Ft_LockScreenUI;
#endif
};

class LockScreenUI : public MWindow
{
    Q_OBJECT

public:
    LockScreenUI ();
    virtual ~LockScreenUI();

signals:
    void unlocked ();

public slots:
    void updateDateTime ();
    void reset ();

protected:
    void createContent ();
    virtual void showEvent(QShowEvent *event);

private slots:
    void realize ();
    void sliderUnlocked ();
    void showHideNotifications (bool show);

private:
    bool                   m_Realized;
    QGraphicsLinearLayout *m_policy;
    LockScreenWindow      *m_SceneWindow;

    MWidget         *m_notificationArea;
    MWidget         *m_LockLiftArea;
    MWidget         *m_LockLandArea;
#ifdef UNIT_TEST
    friend class Ut_LockScreenUI;
    friend class Ft_LockScreenUI;
    friend class Ut_LockScreenBusinessLogic;
    friend class Ft_LockScreenBusinessLogic;
#endif
};

class EventEaterUI : public QWidget
{
    Q_OBJECT

public:
    EventEaterUI ();
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    
protected:
    virtual void showEvent(QShowEvent *event);

signals:
    void OneInput ();
#ifdef UNIT_TEST
    friend class Ut_LockScreenUI;
    friend class Ft_LockScreenUI;
    friend class Ut_LockScreenBusinessLogic;
    friend class Ft_LockScreenBusinessLogic;
#endif
};

#endif