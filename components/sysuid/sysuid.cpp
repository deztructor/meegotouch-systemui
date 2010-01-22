#include <DuiLocale>
#include <DuiTheme>
#include <DuiLocale>
#include <DuiGConfItem>

#include <QDBusConnection>
#include <QDebug>

#include "sysuid.h"
#include "systemuigconf.h"
#include "batterybusinesslogic.h"
#include "batterybusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"
#include "shutdownbusinesslogic.h"

#define TRANSLATION_CATALOG "duicontrolpanel-systemui"

namespace
{
const QString themeDir = "/usr/share/sysuid/themes/";
const QString styleDir = themeDir + "style/";
const QString svgDir = themeDir + "svg/";
}

Sysuid* Sysuid::_sysuid = NULL;

Sysuid::Sysuid () : QObject ()
{
    qDebug () << "starting sysuidaemon";

    _sysuid = this;

    DuiTheme::addPixmapDirectory (svgDir); // or ..(themeDir, true); ?
    DuiTheme::loadCSS (styleDir + "sysuid.css");
    DuiTheme::loadCSS (styleDir + "unlocksliderstyle.css");

    // Load translation of System-UI
    retranslate ();

    systemUIGConf   = new SystemUIGConf (this);
    shutdownLogic   = new ShutdownBusinessLogic (this);
    lockScreenLogic = new LockScreenBusinessLogic (this);
    batteryLogic    = new BatteryBusinessLogic (systemUIGConf, this);

    // D-Bus registration and stuff

    new BatteryBusinessLogicAdaptor (this, batteryLogic);
    new LockScreenBusinessLogicAdaptor (this, lockScreenLogic);

    QDBusConnection bus = QDBusConnection::sessionBus ();
    if (!bus.registerService (dbusService ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus service";
        abort();
    }
    if (!bus.registerObject (dbusPath (), sysuid ())) {
        qCritical () << Q_FUNC_INFO << "failed to register dbus object";
        abort();
    }
}

Sysuid::~Sysuid ()
{
    _sysuid = NULL;
}

Sysuid* Sysuid::sysuid ()
{
    return _sysuid;
}

QString Sysuid::dbusService ()
{
    return QString ("com.nokia.systemui");
}

QString Sysuid::dbusPath ()
{
    return QString ("/");
}

void Sysuid::retranslate ()
{
    DuiGConfItem     languageItem ("/Dui/i18n/Language");
    QString          lang = languageItem.value ().toString ();
    // Create a new duilocale instance with current language
    DuiLocale        locale (lang);

    qDebug () << Q_FUNC_INFO << "with lang" << lang;

    // Install engineering english
    locale.installTrCatalog (TRANSLATION_CATALOG ".qm");
    // Install real translation
    locale.installTrCatalog (TRANSLATION_CATALOG);

    DuiLocale::setDefault (locale);
}
