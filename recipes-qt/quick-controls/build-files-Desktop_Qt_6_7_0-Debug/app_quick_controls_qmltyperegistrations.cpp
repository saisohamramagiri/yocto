/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <windowsan.h>


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_quick_controls()
{
    qmlRegisterTypesAndRevisions<Windowsan>("quick_controls", 1);
    qmlRegisterAnonymousType<QQuickWindow, 254>("quick_controls", 1);
    qmlRegisterAnonymousType<QWindow, 254>("quick_controls", 1);
    qmlRegisterModule("quick_controls", 1, 0);
}

static const QQmlModuleRegistration quickcontrolsRegistration("quick_controls", qml_register_types_quick_controls);
