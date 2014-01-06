#ifndef FORMMAIN_H
#define FORMMAIN_H

#include "ui_formmain.h"

#include <QMainWindow>
#include <QUrl>


class StatusBar;

class FormMain : public QMainWindow {
    Q_OBJECT

    friend class TabWidget;
    friend class FeedMessageViewer;
    friend class MessagesView;
    friend class FeedsView;

  public:
    // Constructors and destructors.
    explicit FormMain(QWidget *parent = 0);
    virtual ~FormMain();

    // Returns menu for the tray icon.
    QMenu *getTrayMenu();

    // Returns global tab widget.
    TabWidget *getTabWidget();

    // Returns list of all globally available actions.
    // NOTE: This is used for setting dynamic shortcuts for given actions.
    QList<QAction*> getActions();

    // Access to statusbar.
    StatusBar *statusBar();

    // Singleton accessor.
    static FormMain *getInstance();

  protected:
    // Creates all needed menus and sets them up.
    void prepareMenus();

    // Creates needed connections for this window.
    void createConnections();

    // Event handler reimplementations.
    void closeEvent(QCloseEvent *event);
    bool event(QEvent *event);

    // Sets up proper icons for this widget.
    void setupIcons();

    // Loads/saves visual state of the application.
    void loadSize();
    void saveSize();

  public slots:
    // Processes incoming message from another RSS Guard instance.
    void processExecutionMessage(const QString &message);

    // Quits the application.
    void quit();

    // Displays window on top or switches its visibility.
    void display();

    // Switches visibility of main window.
    void switchVisibility();

    // Turns on/off fullscreen mode
    void switchFullscreenMode(bool turn_fullscreen_on);

  protected slots:
    void onCommitData(QSessionManager &manager);
    void onSaveState(QSessionManager &manager);

    // Used for last-minute actions.
    void onAboutToQuit();

    // Loads web browser menu if user selects to change tabs.
    void loadWebBrowserMenu(int index);

    // Displays various dialogs.
    void showSettings();
    void showAbout();

  private:
    Ui::FormMain *m_ui;
    QMenu *m_trayMenu;
    StatusBar *m_statusBar;

    static FormMain *s_instance;
};

#endif // FORMMAIN_H
