#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QTransform>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QSplashScreen>
#include <QProgressBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QTranslator>
#include <QLocale>
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>
#include <QColorDialog>
#include <QToolButton>
#include <QFrame>
#include <QScrollBar>
#include <QWheelEvent>
#include <QSet>
#include <QJsonDocument>
#include <QJsonObject>

class ImageLabel;
class ColorListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void saveImage();
    void onPixelClicked(QPoint position, QColor color);
    void removeSelectedColor();
    void clearAllColors();
    void onZoomChanged(int value);
    void onTransparentToggled(bool enabled);
    void onExportColorsToggled(bool enabled);
    void onColorToleranceToggled(bool enabled);
    void onZoomRequested(int delta, QPoint position);
    void changeLanguage(QAction *action);
    void resetZoom();
    void fitToWindow();
    void showAbout();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupLanguages();
    void createImageSections();
    void createSidebar();
    void connectSignals();
    
    void updateEditedImage();
    void updateZoom();
    void updateColorsList();
    void syncScrollPositions();
    void retranslateUI();
    void loadTranslations();
    bool colorsAreSimilar(const QColor &color1, const QColor &color2, int tolerance);
    QString getTranslation(const QString &path) const;
    QString getTranslatedMessage(const QString &key, const QString &fallbackAr, const QString &fallbackEn) const;
    
    // UI Components
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    QSplitter *m_imageSplitter;
    
    // Image sections
    QGroupBox *m_originalGroup;
    QGroupBox *m_editedGroup;
    QScrollArea *m_originalScrollArea;
    QScrollArea *m_editedScrollArea;
    ImageLabel *m_originalImageLabel;
    ImageLabel *m_editedImageLabel;
    
    // Sidebar
    QWidget *m_sidebar;
    QGroupBox *m_colorsGroup;
    ColorListWidget *m_colorsList;
    QPushButton *m_clearColorsBtn;
    
    QGroupBox *m_optionsGroup;
    QCheckBox *m_transparentCheckBox;
    QCheckBox *m_exportColorsCheckBox;
    QCheckBox *m_colorToleranceCheckBox;
    QSpinBox *m_colorToleranceSpinBox;
    QLabel *m_colorToleranceLabel;
    
    QGroupBox *m_zoomGroup;
    QSlider *m_zoomSlider;
    QSpinBox *m_zoomSpinBox;
    QPushButton *m_resetZoomBtn;
    QPushButton *m_fitWindowBtn;
    
    QPushButton *m_openBtn;
    QPushButton *m_saveBtn;
    
    // Menu and toolbar
    QMenuBar *m_menuBar;
    QMenu *m_fileMenu;
    QMenu *m_languageMenu;
    QMenu *m_helpMenu;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
    QActionGroup *m_languageGroup;
    QAction *m_englishAction;
    QAction *m_arabicAction;
    
    QStatusBar *m_statusBar;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    
    // Data
    QImage m_originalImage;
    QImage m_editedImage;
    QList<QColor> m_removedColors;
    QSet<QRgb> m_actualRemovedColors;  // Track all actually removed colors
    QString m_currentImagePath;
    qreal m_scaleFactor;
    bool m_useTransparent;
    bool m_exportColors;
    bool m_useColorTolerance;
    int m_colorTolerance;
    
    // Internationalization
    QTranslator *m_translator;
    QString m_currentLanguage;
    QJsonObject m_translations;
};

// Custom Image Label with click detection and zoom support
class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ImageLabel(QWidget *parent = nullptr);
    void setOriginalPixmap(const QPixmap &pixmap);
    void updatePixmap(qreal scaleFactor);

signals:
    void pixelClicked(QPoint position, QColor color);
    void zoomRequested(int delta, QPoint position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_originalPixmap;
    qreal m_scaleFactor;
    QPoint m_offset;
    QPoint m_lastPanPoint;
    bool m_panning;
};

// Custom Color List Widget
class ColorListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit ColorListWidget(QWidget *parent = nullptr);
    void addColor(const QColor &color);
    void removeColor(const QColor &color);
    void clearColors();
    QList<QColor> getColors() const;

signals:
    void colorRemoved(QColor color);

private slots:
    void onItemClicked(QListWidgetItem *item);
    void removeCurrentColor();

private:
    void updateItem(QListWidgetItem *item, const QColor &color);
    QList<QColor> m_colors;
};

#endif // MAINWINDOW_H
