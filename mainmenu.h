#ifndef GAMEAREA_H
#define GAMEAREA_H

#include "gamescene.h"
#include "textgraphicsitem.h"
#include "textanimation.h"
#include "audiodecoder.h"
#include "checkboxgraphicsitem.h"
#include "slidergraphicsitem.h"
#include "switchgraphicsitem.h"
#include "textlanganimation.h"
#include "autosound.h"
#include "songselectmenu.h"
#include <QGraphicsView>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsItem>
#include <QList>


class GameArea : public QGraphicsView
{
    Q_OBJECT
private:
    GameScene    *scene;
    AudioDecoder *decoder;

    void initTextItem(QGraphicsSimpleTextItem *, const QString&, int, int, bool is_hover_accepted = true);

public:
    GameArea();
private slots:
    void createMainMenu();
    void createSettingsMenu();
    void createSongSelectMenu();

    void connectMainMenu();
    void connectSettingsMenu();

    void setAntialiasing(bool);
    void setVolume(int);
    void setLanguage(QString);

    void reloadHint();
    void mouseHoverSound();
    void mouseClickSound();
};

#endif
