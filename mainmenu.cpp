#include "gamearea.h"

GameArea::GameArea()
{
    scene = new GameScene(QRectF(0, 0, QApplication::desktop() -> availableGeometry().size().width() - 10,    // -10, чтобы не появился гор. скролл
                                       QApplication::desktop() -> availableGeometry().size().height() + 37)); // + 37 чтобы уместилась картинка
    createMainMenu();
    setScene(scene);
    decoder = new AudioDecoder(this);
}

void GameArea::createMainMenu()
{
    scene -> clear();

    TextGraphicsItem *play_text_item     = new TextGraphicsItem;
    TextGraphicsItem *settings_text_item = new TextGraphicsItem;
    TextGraphicsItem *quit_text_item     = new TextGraphicsItem;

    initTextItem(quit_text_item,     tr("Выход"),     20, scene -> height() - 116);
    initTextItem(settings_text_item, tr("Настройки"), 20, quit_text_item     -> y() - 78);
    initTextItem(play_text_item,     tr("Играть"),    20, settings_text_item -> y() - 78);

    scene -> addItem(play_text_item);
    scene -> addItem(settings_text_item);
    scene -> addItem(quit_text_item);
    scene -> setBackgroundBrush(QBrush(GameSettings::backgroundPixmap().scaled(scene -> width(), scene -> height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    TextAnimation *play_text_item_animation     = new TextAnimation(play_text_item);
    TextAnimation *settings_text_item_animation = new TextAnimation(settings_text_item);
    TextAnimation *quit_text_item_animation     = new TextAnimation(quit_text_item);

    play_text_item_animation     -> start();
    settings_text_item_animation -> start();
    quit_text_item_animation     -> start();

    connect(play_text_item_animation,   SIGNAL(destroyed()), SLOT(connectMainMenu()));
    connect(play_text_item,             SIGNAL(playSound()), SLOT(mouseHoverSound()));
    connect(settings_text_item,         SIGNAL(playSound()), SLOT(mouseHoverSound()));
    connect(quit_text_item,             SIGNAL(playSound()), SLOT(mouseHoverSound()));
    connect(play_text_item,             SIGNAL(clicked()),   SLOT(mouseClickSound()));
    connect(settings_text_item,         SIGNAL(clicked()),   SLOT(mouseClickSound()));
    connect(quit_text_item,             SIGNAL(clicked()),   SLOT(mouseClickSound()));
}

void GameArea::initTextItem(QGraphicsSimpleTextItem *item, const QString &item_text, int x_pos, int y_pos, bool is_hover_accepted)
{
    item -> setAcceptHoverEvents(is_hover_accepted);
    item -> setFont(GameSettings::currentFont());
    item -> setPen(QPen(GameSettings::penBrush(), 1));
    item -> setBrush(GameSettings::penBrush());
    item -> setText(item_text);
    item -> setPos(x_pos, y_pos);
}

void GameArea::createSettingsMenu()
{
    scene -> clear();

    TextGraphicsItem *volume_text_item       = new TextGraphicsItem;
    TextGraphicsItem *antialiasing_text_item = new TextGraphicsItem;
    TextGraphicsItem *language_text_item     = new TextGraphicsItem;
    TextGraphicsItem *return_text_item       = new TextGraphicsItem;

    initTextItem(return_text_item,       tr("Назад"),       20, scene -> height() - 116);
    initTextItem(language_text_item,     tr("Язык"),        20, return_text_item ->       y() - 78, false);
    initTextItem(antialiasing_text_item, tr("Сглаживание"), 20, language_text_item ->     y() - 78, false);
    initTextItem(volume_text_item,       tr("Громкость"),   20, antialiasing_text_item -> y() - 78, false);

    volume_text_item       -> setAcceptedMouseButtons(0);
    antialiasing_text_item -> setAcceptedMouseButtons(0);
    language_text_item     -> setAcceptedMouseButtons(0);

    static QFontMetrics fm(QFont(GameSettings::currentFont()));

    CheckboxGraphicsItem *antialiasing_checkbox_item = new CheckboxGraphicsItem(fm.width(tr("Сглаживание")) + 60,
                                                                                antialiasing_text_item -> y() + (fm.height() * 26 / 100),
                                                                                fm.height() - (fm.height() * 30 / 100));

    SliderGraphicsItem *volume_slider_item   = new SliderGraphicsItem(fm.width(tr("Сглаживание")) + 60,
                                                                      volume_text_item -> y() + (fm.height() * 26 / 100), 150);

    SwitchGraphicsItem *language_switch_item = new SwitchGraphicsItem(fm.width(tr("Сглаживание")) + 60, language_text_item -> y());

    antialiasing_checkbox_item -> setAcceptedMouseButtons(Qt::LeftButton);
    language_switch_item       -> setAcceptedMouseButtons(Qt::LeftButton);
    volume_slider_item         -> setAcceptedMouseButtons(Qt::LeftButton);

    scene -> addItem(volume_text_item);
    scene -> addItem(antialiasing_text_item);
    scene -> addItem(language_text_item);
    scene -> addItem(return_text_item);
    scene -> addItem(antialiasing_checkbox_item);
    scene -> addItem(volume_slider_item);
    scene -> addItem(language_switch_item);

    TextAnimation *volume_text_item_animation           = new TextAnimation(volume_text_item);
    TextAnimation *antialiasing_text_item_animation     = new TextAnimation(antialiasing_text_item);
    TextAnimation *language_text_item_animation         = new TextAnimation(language_text_item);
    TextAnimation *return_text_item_animation           = new TextAnimation(return_text_item);
    TextAnimation *volume_slider_item_animation         = new TextAnimation(volume_slider_item);
    TextAnimation *language_switch_item_animation       = new TextAnimation(language_switch_item);
    TextAnimation *antialiasing_checkbox_item_animation = new TextAnimation(antialiasing_checkbox_item);

    volume_text_item_animation           -> start();
    antialiasing_text_item_animation     -> start();
    language_text_item_animation         -> start();
    return_text_item_animation           -> start();
    volume_slider_item_animation         -> start();
    language_switch_item_animation       -> start();
    antialiasing_checkbox_item_animation -> start();

    connect(return_text_item_animation, SIGNAL(destroyed()),           SLOT(connectSettingsMenu()));
    connect(antialiasing_checkbox_item, SIGNAL(clicked(bool)),         SLOT(setAntialiasing(bool)));
    connect(volume_slider_item,         SIGNAL(valueChanged(int)),     SLOT(setVolume(int)));
    connect(language_switch_item,       SIGNAL(valueChanged(QString)), SLOT(setLanguage(QString)));
    connect(language_switch_item,       SIGNAL(valueChanged(QString)), SLOT(reloadHint()));
    connect(return_text_item,           SIGNAL(playSound()),           SLOT(mouseHoverSound()));
    connect(return_text_item,           SIGNAL(clicked()),             SLOT(mouseClickSound()));
}

void GameArea::connectMainMenu()
{
    TextGraphicsItem *quit_item     = static_cast <TextGraphicsItem *> (scene -> items().at(0));
    TextGraphicsItem *settings_item = static_cast <TextGraphicsItem *> (scene -> items().at(1));
    TextGraphicsItem *play_item     = static_cast <TextGraphicsItem *> (scene -> items().at(2));

    connect(quit_item,     SIGNAL(clicked()), SLOT(close()));
    connect(settings_item, SIGNAL(clicked()), SLOT(createSettingsMenu()));
    connect(play_item,     SIGNAL(clicked()), SLOT(createSongSelectMenu()));
}

void GameArea::connectSettingsMenu()
{
    TextGraphicsItem *return_item = static_cast <TextGraphicsItem *> (scene -> items().at(3));
    connect(return_item, SIGNAL(clicked()), SLOT(createMainMenu()));
}

void GameArea::setAntialiasing(bool is_enable)
{
    GameSettings::setAntialiasing(is_enable);
}

void GameArea::setVolume(int value)
{
    decoder -> setVolume(value);
    GameSettings::setVolume(value);
}

void GameArea::setLanguage(QString language)
{
    GameSettings::setLanguage(language);
}

void GameArea::mouseHoverSound()
{
    AutoSound *sound = new AutoSound(QDir::currentPath() + "/resources/sounds/sys/hover.WAV");
    Q_UNUSED(sound);
}

void GameArea::mouseClickSound()
{
    AutoSound *sound = new AutoSound(QDir::currentPath() + "/resources/sounds/sys/click.WAV");
    Q_UNUSED(sound);
}

void GameArea::createSongSelectMenu()
{
    SongSelectMenu menu(this);

    menu -> show();

    createMainMenu();
}

void GameArea::reloadHint()
{
    QGraphicsSimpleTextItem *hint = new QGraphicsSimpleTextItem;
    TextLangAnimation *animation  = new TextLangAnimation(hint);

    initTextItem(hint, tr("Для смены языка требуется\nперезапуск программы"), 40, 100, false);
    hint -> setFont(QFont(hint -> font().family(), 25));

    scene -> addItem(hint);

    animation -> start();

    disconnect(static_cast <SwitchGraphicsItem *> (scene -> items().at(1)), SIGNAL(valueChanged(QString)), this, SLOT(reloadHint()));
}
