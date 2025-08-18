#include "mainwindow.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_scaleFactor(1.0)
    , m_useTransparent(false)
    , m_exportColors(false)
    , m_useColorTolerance(false)
    , m_colorTolerance(10)
    , m_translator(new QTranslator(this))
    , m_currentLanguage("ar")  // Set Arabic as default
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupLanguages();
    connectSignals();
    
    // Load translations for the default language
    loadTranslations();
    
    // Set application icon
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    
    // Set window properties
    setMinimumSize(1000, 700);
    resize(1400, 900);
    
    // Set RTL layout as default
    setLayoutDirection(Qt::RightToLeft);
    
    // Enable drag and drop
    setAcceptDrops(true);
    
    retranslateUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(m_centralWidget);
    
    // Create main splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(m_mainSplitter);
    
    createImageSections();
    createSidebar();
    
    // Add widgets to main splitter
    m_mainSplitter->addWidget(m_imageSplitter);
    m_mainSplitter->addWidget(m_sidebar);
    
    // Set splitter proportions
    m_mainSplitter->setStretchFactor(0, 3);
    m_mainSplitter->setStretchFactor(1, 1);
    m_mainSplitter->setSizes({1000, 300});
}

void MainWindow::createImageSections()
{
    // Create image splitter (horizontal for original and edited images)
    m_imageSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Original image section
    m_originalGroup = new QGroupBox("Original Image", this);
    QVBoxLayout *originalLayout = new QVBoxLayout(m_originalGroup);
    
    m_originalScrollArea = new QScrollArea(this);
    m_originalScrollArea->setWidgetResizable(false);
    m_originalScrollArea->setAlignment(Qt::AlignCenter);
    m_originalScrollArea->setStyleSheet("QScrollArea { border: 2px dashed #aaa; background-color: #f5f5f5; }");
    
    m_originalImageLabel = new ImageLabel(this);
    m_originalImageLabel->setAlignment(Qt::AlignCenter);
    m_originalImageLabel->setMinimumSize(400, 300);
    m_originalImageLabel->setStyleSheet("QLabel { border: none; background-color: transparent; }");
    m_originalImageLabel->setWordWrap(true);
    
    m_originalScrollArea->setWidget(m_originalImageLabel);
    originalLayout->addWidget(m_originalScrollArea);
    
    // Edited image section
    m_editedGroup = new QGroupBox("Edited Image", this);
    QVBoxLayout *editedLayout = new QVBoxLayout(m_editedGroup);
    
    m_editedScrollArea = new QScrollArea(this);
    m_editedScrollArea->setWidgetResizable(false);
    m_editedScrollArea->setAlignment(Qt::AlignCenter);
    m_editedScrollArea->setStyleSheet("QScrollArea { border: 2px solid #ddd; background-color: #fff; }");
    
    m_editedImageLabel = new ImageLabel(this);
    m_editedImageLabel->setAlignment(Qt::AlignCenter);
    m_editedImageLabel->setMinimumSize(400, 300);
    m_editedImageLabel->setStyleSheet("QLabel { border: none; background-color: transparent; }");
    m_editedImageLabel->setWordWrap(true);
    
    m_editedScrollArea->setWidget(m_editedImageLabel);
    editedLayout->addWidget(m_editedScrollArea);
    
    // Add to splitter
    m_imageSplitter->addWidget(m_originalGroup);
    m_imageSplitter->addWidget(m_editedGroup);
    m_imageSplitter->setSizes({500, 500});
}

void MainWindow::createSidebar()
{
    m_sidebar = new QWidget(this);
    m_sidebar->setFixedWidth(300);
    m_sidebar->setStyleSheet("QWidget { background-color: #f8f9fa; }");
    
    QVBoxLayout *sidebarLayout = new QVBoxLayout(m_sidebar);
    
    // Colors group
    m_colorsGroup = new QGroupBox("Selected Colors", this);
    QVBoxLayout *colorsLayout = new QVBoxLayout(m_colorsGroup);
    
    m_colorsList = new ColorListWidget(this);
    colorsLayout->addWidget(m_colorsList);
    
    m_clearColorsBtn = new QPushButton("Clear All Colors", this);
    m_clearColorsBtn->setEnabled(false);
    colorsLayout->addWidget(m_clearColorsBtn);
    
    sidebarLayout->addWidget(m_colorsGroup);
    
    // Options group
    m_optionsGroup = new QGroupBox("Export Options", this);
    QVBoxLayout *optionsLayout = new QVBoxLayout(m_optionsGroup);
    
    m_transparentCheckBox = new QCheckBox("Use Transparent Background", this);
    m_transparentCheckBox->setToolTip("Export as PNG with transparent background for removed colors");
    optionsLayout->addWidget(m_transparentCheckBox);
    
    m_exportColorsCheckBox = new QCheckBox("Export Color List", this);
    m_exportColorsCheckBox->setToolTip("Export selected colors to text file");
    optionsLayout->addWidget(m_exportColorsCheckBox);
    
    m_colorToleranceCheckBox = new QCheckBox("Use Color Tolerance", this);
    m_colorToleranceCheckBox->setToolTip("Remove similar colors within tolerance range");
    optionsLayout->addWidget(m_colorToleranceCheckBox);
    
    QHBoxLayout *toleranceLayout = new QHBoxLayout();
    m_colorToleranceLabel = new QLabel("Tolerance:", this);
    m_colorToleranceSpinBox = new QSpinBox(this);
    m_colorToleranceSpinBox->setRange(1, 100);
    m_colorToleranceSpinBox->setValue(10);
    m_colorToleranceSpinBox->setSuffix(" RGB");
    m_colorToleranceSpinBox->setEnabled(false);
    m_colorToleranceSpinBox->setToolTip("RGB difference tolerance (1-100)");
    
    toleranceLayout->addWidget(m_colorToleranceLabel);
    toleranceLayout->addWidget(m_colorToleranceSpinBox);
    optionsLayout->addLayout(toleranceLayout);
    
    sidebarLayout->addWidget(m_optionsGroup);
    
    // Zoom group
    m_zoomGroup = new QGroupBox("Zoom Controls", this);
    QVBoxLayout *zoomLayout = new QVBoxLayout(m_zoomGroup);
    
    QHBoxLayout *zoomSliderLayout = new QHBoxLayout();
    QLabel *zoomLabel = new QLabel("Zoom:", this);
    m_zoomSlider = new QSlider(Qt::Horizontal, this);
    m_zoomSlider->setRange(10, 2000); // 10% to 2000%
    m_zoomSlider->setValue(100);
    m_zoomSlider->setToolTip("Zoom level (10% - 2000%)");
    
    m_zoomSpinBox = new QSpinBox(this);
    m_zoomSpinBox->setRange(10, 2000);
    m_zoomSpinBox->setValue(100);
    m_zoomSpinBox->setSuffix("%");
    
    zoomSliderLayout->addWidget(zoomLabel);
    zoomSliderLayout->addWidget(m_zoomSlider);
    zoomSliderLayout->addWidget(m_zoomSpinBox);
    zoomLayout->addLayout(zoomSliderLayout);
    
    QHBoxLayout *zoomButtonsLayout = new QHBoxLayout();
    m_resetZoomBtn = new QPushButton("Reset (100%)", this);
    m_fitWindowBtn = new QPushButton("Fit to Window", this);
    zoomButtonsLayout->addWidget(m_resetZoomBtn);
    zoomButtonsLayout->addWidget(m_fitWindowBtn);
    zoomLayout->addLayout(zoomButtonsLayout);
    
    sidebarLayout->addWidget(m_zoomGroup);
    
    // Action buttons
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    
    m_openBtn = new QPushButton("Open Image", this);
    m_openBtn->setStyleSheet("QPushButton { background-color: #007bff; color: white; font-weight: bold; padding: 8px; }");
    buttonsLayout->addWidget(m_openBtn);
    
    m_saveBtn = new QPushButton("Save Edited Image", this);
    m_saveBtn->setEnabled(false);
    m_saveBtn->setStyleSheet("QPushButton { background-color: #28a745; color: white; font-weight: bold; padding: 8px; }");
    buttonsLayout->addWidget(m_saveBtn);
    
    sidebarLayout->addLayout(buttonsLayout);
    sidebarLayout->addStretch();
}

void MainWindow::setupMenuBar()
{
    m_menuBar = menuBar();
    
    // File menu
    m_fileMenu = m_menuBar->addMenu("&File");
    
    m_openAction = new QAction("&Open Image...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip("Open an image file");
    m_fileMenu->addAction(m_openAction);
    
    m_saveAction = new QAction("&Save Edited Image...", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip("Save the edited image");
    m_saveAction->setEnabled(false);
    m_fileMenu->addAction(m_saveAction);
    
    m_fileMenu->addSeparator();
    
    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("Exit the application");
    m_fileMenu->addAction(m_exitAction);
    
    // Language menu
    m_languageMenu = m_menuBar->addMenu("&Language");
    m_languageGroup = new QActionGroup(this);
    
    m_englishAction = new QAction("English", this);
    m_englishAction->setCheckable(true);
    m_englishAction->setChecked(false);  // English not checked by default
    m_englishAction->setData("en");
    m_languageGroup->addAction(m_englishAction);
    m_languageMenu->addAction(m_englishAction);
    
    m_arabicAction = new QAction("العربية", this);
    m_arabicAction->setCheckable(true);
    m_arabicAction->setChecked(true);  // Arabic checked by default
    m_arabicAction->setData("ar");
    m_languageGroup->addAction(m_arabicAction);
    m_languageMenu->addAction(m_arabicAction);
    
    // Help menu
    m_helpMenu = m_menuBar->addMenu("&Help");
    
    m_aboutAction = new QAction("&About...", this);
    m_aboutAction->setStatusTip("Show application information");
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupStatusBar()
{
    m_statusBar = statusBar();
    
    m_statusLabel = new QLabel(this);
    m_statusBar->addWidget(m_statusLabel);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    m_statusBar->addPermanentWidget(m_progressBar);
}

void MainWindow::setupLanguages()
{
    // Load translations if available
    loadTranslations();
}

void MainWindow::connectSignals()
{
    // Menu actions
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openImage);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveImage);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    connect(m_languageGroup, &QActionGroup::triggered, this, &MainWindow::changeLanguage);
    
    // Buttons
    connect(m_openBtn, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(m_saveBtn, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(m_clearColorsBtn, &QPushButton::clicked, this, &MainWindow::clearAllColors);
    connect(m_resetZoomBtn, &QPushButton::clicked, this, &MainWindow::resetZoom);
    connect(m_fitWindowBtn, &QPushButton::clicked, this, &MainWindow::fitToWindow);
    
    // Image labels
    connect(m_originalImageLabel, &ImageLabel::pixelClicked, this, &MainWindow::onPixelClicked);
    connect(m_originalImageLabel, &ImageLabel::zoomRequested, this, &MainWindow::onZoomRequested);
    connect(m_editedImageLabel, &ImageLabel::zoomRequested, this, &MainWindow::onZoomRequested);
    
    // Color list
    connect(m_colorsList, &ColorListWidget::colorRemoved, this, &MainWindow::removeSelectedColor);
    
    // Zoom controls
    connect(m_zoomSlider, &QSlider::valueChanged, this, &MainWindow::onZoomChanged);
    connect(m_zoomSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onZoomChanged);
    
    // Options
    connect(m_transparentCheckBox, &QCheckBox::toggled, this, &MainWindow::onTransparentToggled);
    connect(m_exportColorsCheckBox, &QCheckBox::toggled, this, &MainWindow::onExportColorsToggled);
    connect(m_colorToleranceCheckBox, &QCheckBox::toggled, this, &MainWindow::onColorToleranceToggled);
    connect(m_colorToleranceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        m_colorTolerance = value;
        if (m_useColorTolerance) {
            updateEditedImage();
        }
    });
    
    // Sync scroll areas
    connect(m_originalScrollArea->horizontalScrollBar(), &QScrollBar::valueChanged,
            m_editedScrollArea->horizontalScrollBar(), &QScrollBar::setValue);
    connect(m_originalScrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
            m_editedScrollArea->verticalScrollBar(), &QScrollBar::setValue);
    connect(m_editedScrollArea->horizontalScrollBar(), &QScrollBar::valueChanged,
            m_originalScrollArea->horizontalScrollBar(), &QScrollBar::setValue);
    connect(m_editedScrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
            m_originalScrollArea->verticalScrollBar(), &QScrollBar::setValue);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Image", "",
        "Image Files (*.png *.jpg *.jpeg *.bmp *.gif *.tiff);;All Files (*)");
    
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QString errorTitle = getTranslatedMessage("messages.error", "خطأ", "Error");
            QString errorMessage = getTranslatedMessage("messages.errorLoadingImage", 
                "لا يمكن تحميل ملف الصورة.", "Cannot load image file.");
            QMessageBox::warning(this, errorTitle, errorMessage);
            return;
        }
        
        m_originalImage = image;
        m_editedImage = image.copy();
        m_currentImagePath = fileName;
        
        // Update UI
        QPixmap pixmap = QPixmap::fromImage(m_originalImage);
        m_originalImageLabel->setOriginalPixmap(pixmap);
        m_editedImageLabel->setOriginalPixmap(QPixmap::fromImage(m_editedImage));
        
        updateZoom();
        updateEditedImage();
        
        m_saveAction->setEnabled(true);
        m_saveBtn->setEnabled(true);
        
        QString loadedMessage = getTranslatedMessage("messages.imageLoaded",
            "تم التحميل: %1 (%2x%3)", "Loaded: %1 (%2x%3)");
        m_statusLabel->setText(loadedMessage
                              .arg(QFileInfo(fileName).fileName())
                              .arg(image.width())
                              .arg(image.height()));
    }
}

void MainWindow::saveImage()
{
    if (m_editedImage.isNull() || m_currentImagePath.isEmpty()) {
        return;
    }
    
    QFileInfo fileInfo(m_currentImagePath);
    QString baseName = fileInfo.baseName();
    QString dirPath = fileInfo.absolutePath();
    
    QString outputFileName;
    if (m_useTransparent) {
        outputFileName = QString("%1/%2_removed_color.png").arg(dirPath, baseName);
    } else {
        QString suffix = fileInfo.suffix().toLower();
        if (suffix == "png") {
            outputFileName = QString("%1/%2_removed_color.png").arg(dirPath, baseName);
        } else {
            outputFileName = QString("%1/%2_removed_color.jpg").arg(dirPath, baseName);
        }
    }
    
    // Save image
    bool saved = false;
    if (m_useTransparent) {
        // Ensure the image is in the correct format for transparency
        QImage transparentImage = m_editedImage;
        if (transparentImage.format() != QImage::Format_ARGB32) {
            transparentImage = transparentImage.convertToFormat(QImage::Format_ARGB32);
        }
        saved = transparentImage.save(outputFileName, "PNG");
    } else {
        saved = m_editedImage.save(outputFileName);
    }
    
    if (saved) {
        QString savedMessage = getTranslatedMessage("messages.imageSaved",
            "تم الحفظ: %1", "Saved: %1");
        m_statusLabel->setText(savedMessage.arg(QFileInfo(outputFileName).fileName()));
        
        // Export colors list if requested
        if (m_exportColors && !m_actualRemovedColors.isEmpty()) {
            QString colorListFileName = QString("%1/%2_removed_color_list.txt").arg(dirPath, baseName);
            QFile colorFile(colorListFileName);
            if (colorFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&colorFile);
                out << "Removed Colors List for: " << fileInfo.fileName() << "\n";
                out << "Generated on: " << QDateTime::currentDateTime().toString() << "\n";
                out << "Total colors removed: " << m_actualRemovedColors.size() << "\n\n";
                
                // Convert to sorted list for consistent output
                QList<QRgb> sortedColors = m_actualRemovedColors.values();
                std::sort(sortedColors.begin(), sortedColors.end());
                
                for (int i = 0; i < sortedColors.size(); ++i) {
                    QColor color = QColor::fromRgb(sortedColors[i]);
                    out << QString("Color %1:\n").arg(i + 1);
                    out << QString("  RGB: (%1, %2, %3)\n").arg(color.red()).arg(color.green()).arg(color.blue());
                    out << QString("  Hex: %1\n").arg(color.name().toUpper());
                    out << "\n";
                }
                colorFile.close();
                QString savedMessage = getTranslatedMessage("messages.savedImageAndColorList",
                    "حُفظت الصورة وقائمة الألوان", "Saved image and color list");
                m_statusLabel->setText(savedMessage);
            }
        }
        
        QString successTitle = getTranslatedMessage("messages.success", "نجح", "Success");
        QString successMessage = getTranslatedMessage("messages.imageSavedSuccessfully",
            "لقد حُفظت الصورة :", "Image saved successfully as:");
        QMessageBox::information(this, successTitle, QString("%1\n%2").arg(successMessage, outputFileName));
    } else {
        QString errorTitle = getTranslatedMessage("messages.error", "خطأ", "Error");
        QString errorMessage = getTranslatedMessage("messages.failedToSaveImage",
            "فشل في حفظ الصورة.", "Failed to save image.");
        QMessageBox::warning(this, errorTitle, errorMessage);
    }
}

void MainWindow::onPixelClicked(QPoint /*position*/, QColor color)
{
    if (m_originalImage.isNull()) {
        return;
    }
    
    // Add color to removed colors list
    if (!m_removedColors.contains(color)) {
        m_removedColors.append(color);
        m_colorsList->addColor(color);
        m_clearColorsBtn->setEnabled(true);
        
        updateEditedImage();
        
        QString colorMessage = getTranslatedMessage("messages.colorSelected",
            "اللون المحدد: RGB(%1, %2, %3) - %4", "Selected color: RGB(%1, %2, %3) - %4");
        m_statusLabel->setText(colorMessage
                              .arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.name().toUpper()));
    }
}

void MainWindow::removeSelectedColor()
{
    updateEditedImage();
}

void MainWindow::clearAllColors()
{
    m_removedColors.clear();
    m_actualRemovedColors.clear();
    m_colorsList->clearColors();
    m_clearColorsBtn->setEnabled(false);
    updateEditedImage();
    QString clearedMessage = getTranslatedMessage("messages.allColorsCleared",
        "مُسحت الألوان جميعها", "All colors cleared");
    m_statusLabel->setText(clearedMessage);
}

void MainWindow::onZoomChanged(int value)
{
    if (sender() == m_zoomSlider) {
        m_zoomSpinBox->blockSignals(true);
        m_zoomSpinBox->setValue(value);
        m_zoomSpinBox->blockSignals(false);
    } else if (sender() == m_zoomSpinBox) {
        m_zoomSlider->blockSignals(true);
        m_zoomSlider->setValue(value);
        m_zoomSlider->blockSignals(false);
    }
    
    m_scaleFactor = value / 100.0;
    updateZoom();
}

void MainWindow::onTransparentToggled(bool enabled)
{
    m_useTransparent = enabled;
    updateEditedImage();
}

void MainWindow::onExportColorsToggled(bool enabled)
{
    m_exportColors = enabled;
}

void MainWindow::onColorToleranceToggled(bool enabled)
{
    m_useColorTolerance = enabled;
    m_colorToleranceSpinBox->setEnabled(enabled);
    m_colorToleranceLabel->setEnabled(enabled);
    updateEditedImage();
}

void MainWindow::onZoomRequested(int delta, QPoint /*position*/)
{
    int currentZoom = m_zoomSlider->value();
    int zoomStep = 10;
    
    if (delta > 0) {
        // Zoom in
        currentZoom += zoomStep;
    } else {
        // Zoom out
        currentZoom -= zoomStep;
    }
    
    currentZoom = qBound(10, currentZoom, 2000);
    m_zoomSlider->setValue(currentZoom);
}

void MainWindow::changeLanguage(QAction *action)
{
    if (action) {
        QString newLanguage = action->data().toString();
        m_currentLanguage = newLanguage;
        loadTranslations(); // Load translations when language changes
        retranslateUI();
    }
}

void MainWindow::showAbout()
{
    // Load translations if not already loaded
    if (m_translations.isEmpty()) {
        loadTranslations();
    }
    
    // Get translated text with fallbacks for current language
    QString appName = getTranslation("about.appName");
    QString version = getTranslation("about.version");
    QString developer = getTranslation("about.developer");
    QString developerName = getTranslation("about.developerName");
    QString description = getTranslation("about.description");
    QString descriptionText = getTranslation("about.descriptionText");
    QString features = getTranslation("about.features");
    QString license = getTranslation("about.license");
    QString licenseText = getTranslation("about.licenseText");
    QString title = getTranslation("about.title");
    
    // Fallback to hardcoded values if translation is empty
    if (appName.isEmpty()) {
        if (m_currentLanguage == "ar") {
            appName = "مزيل الألوان - محرر الصور";
            version = "الإصدار";
            developer = "المطور";
            developerName = "عبد الرحمن بن ناصر السعيد";
            description = "الوصف";
            descriptionText = "تطبيق لإزالة ألوان محددة من الصور مع دعم الشفافية والتصدير.";
            features = "المميزات";
            license = "الترخيص";
            licenseText = "مجاني للاستخدام الشخصي";
            title = "حول التطبيق";
        } else {
            appName = "Color Remover - Image Editor";
            version = "Version";
            developer = "Developed by";
            developerName = "Abdurrhman Saeed";
            description = "Description";
            descriptionText = "Application for removing specific colors from images with transparency and export support.";
            features = "Features";
            license = "License";
            licenseText = "Free for personal use";
            title = "About";
        }
    }
    
    // Get features list from JSON array
    QString featuresList;
    QJsonValue featuresValue = m_translations["about"].toObject()["featuresList"];
    if (featuresValue.isArray()) {
        QJsonArray featuresArray = featuresValue.toArray();
        for (const QJsonValue &feature : featuresArray) {
            featuresList += QString("<li>%1</li>").arg(feature.toString());
        }
    } else {
        // Fallback features list
        if (m_currentLanguage == "ar") {
            featuresList = "<li>إزالة ألوان محددة من الصور</li>"
                          "<li>دعم تسامح الألوان</li>"
                          "<li>التكبير والتصغير بالفأرة</li>"
                          "<li>دعم خلفية شفافة</li>"
                          "<li>واجهة متعددة اللغات (عربي/إنجليزي)</li>"
                          "<li>تصدير قائمة الألوان المحذوفة</li>";
        } else {
            featuresList = "<li>Remove specific colors from images</li>"
                          "<li>Color tolerance support</li>"
                          "<li>Mouse wheel zooming and panning</li>"
                          "<li>Transparent background support</li>"
                          "<li>Multi-language interface (Arabic/English)</li>"
                          "<li>Export removed colors list</li>";
        }
    }
    
    QString aboutText = QString(
        "<h2>%1</h2>"
        "<p><b>%2:</b> 1.0.0</p>"
        "<p><b>%3:</b> %4</p>"
        "<p><b>%5:</b> %6</p>"
        "<p><b>%7:</b></p>"
        "<ul>%8</ul>"
        "<p><b>%9:</b> %10</p>"
    ).arg(appName)
     .arg(version)
     .arg(developer)
     .arg(developerName)
     .arg(description)
     .arg(descriptionText)
     .arg(features)
     .arg(featuresList)
     .arg(license)
     .arg(licenseText);
    
    QMessageBox::about(this, title, aboutText);
}

void MainWindow::resetZoom()
{
    m_zoomSlider->setValue(100);
}

void MainWindow::fitToWindow()
{
    if (m_originalImage.isNull()) {
        return;
    }
    
    QSize imageSize = m_originalImage.size();
    QSize availableSize = m_originalScrollArea->viewport()->size();
    
    qreal scaleX = (qreal)availableSize.width() / imageSize.width();
    qreal scaleY = (qreal)availableSize.height() / imageSize.height();
    qreal scale = qMin(scaleX, scaleY);
    
    int zoomPercent = qRound(scale * 100);
    zoomPercent = qBound(10, zoomPercent, 2000);
    
    m_zoomSlider->setValue(zoomPercent);
}

void MainWindow::updateEditedImage()
{
    if (m_originalImage.isNull()) {
        return;
    }
    
    m_editedImage = m_originalImage.copy();
    
    // If transparency is enabled, ensure the image has an alpha channel
    if (m_useTransparent && m_editedImage.format() != QImage::Format_ARGB32) {
        m_editedImage = m_editedImage.convertToFormat(QImage::Format_ARGB32);
    }
    
    // Clear previous actual removed colors
    m_actualRemovedColors.clear();
    
    // Remove selected colors
    for (const QColor &colorToRemove : m_removedColors) {
        for (int y = 0; y < m_editedImage.height(); ++y) {
            for (int x = 0; x < m_editedImage.width(); ++x) {
                QColor pixelColor = m_editedImage.pixelColor(x, y);
                
                bool shouldRemove = false;
                if (m_useColorTolerance) {
                    shouldRemove = colorsAreSimilar(pixelColor, colorToRemove, m_colorTolerance);
                } else {
                    shouldRemove = (pixelColor.rgb() == colorToRemove.rgb());
                }
                
                if (shouldRemove) {
                    // Track the actual color being removed
                    m_actualRemovedColors.insert(pixelColor.rgb());
                    
                    if (m_useTransparent) {
                        m_editedImage.setPixelColor(x, y, QColor(0, 0, 0, 0)); // Transparent
                    } else {
                        m_editedImage.setPixelColor(x, y, QColor(255, 255, 255)); // White
                    }
                }
            }
        }
    }
    
    // Update the color list to show all actually removed colors
    updateColorsList();
    
    QPixmap editedPixmap = QPixmap::fromImage(m_editedImage);
    m_editedImageLabel->setOriginalPixmap(editedPixmap);
    m_editedImageLabel->updatePixmap(m_scaleFactor);
}

void MainWindow::updateZoom()
{
    if (!m_originalImage.isNull()) {
        m_originalImageLabel->updatePixmap(m_scaleFactor);
        m_editedImageLabel->updatePixmap(m_scaleFactor);
    }
}

void MainWindow::updateColorsList()
{
    // Clear the visual color list
    m_colorsList->clearColors();
    
    // Convert the set of removed colors to a sorted list for consistent display
    QList<QRgb> sortedColors = m_actualRemovedColors.values();
    std::sort(sortedColors.begin(), sortedColors.end());
    
    // Add all actually removed colors to the visual list
    for (QRgb rgb : sortedColors) {
        QColor color = QColor::fromRgb(rgb);
        m_colorsList->addColor(color);
    }
    
    // Update the clear button state
    m_clearColorsBtn->setEnabled(!m_actualRemovedColors.isEmpty());
}

void MainWindow::retranslateUI()
{
    // Load translations if not already loaded
    if (m_translations.isEmpty()) {
        loadTranslations();
    }
    
    // Get translated text using JSON keys with fallbacks
    QString windowTitle = getTranslation("window.title");
    QString originalImage = getTranslation("groups.originalImage");
    QString editedImage = getTranslation("groups.editedImage");
    QString selectedColors = getTranslation("groups.selectedColors");
    QString exportOptions = getTranslation("groups.exportOptions");
    QString zoomControls = getTranslation("groups.zoomControls");
    
    QString openImage = getTranslation("buttons.openImage");
    QString saveEditedImage = getTranslation("buttons.saveEditedImage");
    QString clearAllColors = getTranslation("buttons.clearAllColors");
    QString resetZoom = getTranslation("buttons.resetZoom");
    QString fitToWindow = getTranslation("buttons.fitToWindow");
    
    QString transparentBackground = getTranslation("checkboxes.transparentBackground");
    QString exportColorList = getTranslation("checkboxes.exportColorList");
    QString useColorTolerance = getTranslation("checkboxes.useColorTolerance");
    QString tolerance = getTranslation("labels.tolerance");
    
    QString originalImageText = getTranslation("placeholders.originalImageText");
    QString editedImageText = getTranslation("placeholders.editedImageText");
    
    QString fileMenu = getTranslation("menus.file");
    QString languageMenu = getTranslation("menus.language");
    QString helpMenu = getTranslation("menus.help");
    QString aboutAction = getTranslation("actions.about");
    
    QString ready = getTranslation("messages.ready");
    
    // Apply translations with fallbacks
    setWindowTitle(!windowTitle.isEmpty() ? windowTitle : 
        (m_currentLanguage == "ar" ? "مزيل الألوان - محرر الصور" : "Color Remover - Image Editor"));
    
    // Force window to update
    update();
    
    m_originalGroup->setTitle(!originalImage.isEmpty() ? originalImage :
        (m_currentLanguage == "ar" ? "الصورة الأصلية" : "Original Image"));
    m_editedGroup->setTitle(!editedImage.isEmpty() ? editedImage :
        (m_currentLanguage == "ar" ? "الصورة المحررة" : "Edited Image"));
    m_colorsGroup->setTitle(!selectedColors.isEmpty() ? selectedColors :
        (m_currentLanguage == "ar" ? "الألوان المحددة" : "Selected Colors"));
    m_optionsGroup->setTitle(!exportOptions.isEmpty() ? exportOptions :
        (m_currentLanguage == "ar" ? "خيارات التصدير" : "Export Options"));
    m_zoomGroup->setTitle(!zoomControls.isEmpty() ? zoomControls :
        (m_currentLanguage == "ar" ? "عناصر التحكم في التكبير" : "Zoom Controls"));
    
    m_openBtn->setText(!openImage.isEmpty() ? openImage :
        (m_currentLanguage == "ar" ? "فتح صورة" : "Open Image"));
    m_saveBtn->setText(!saveEditedImage.isEmpty() ? saveEditedImage :
        (m_currentLanguage == "ar" ? "حفظ الصورة المحررة" : "Save Edited Image"));
    m_clearColorsBtn->setText(!clearAllColors.isEmpty() ? clearAllColors :
        (m_currentLanguage == "ar" ? "مسح جميع الألوان" : "Clear All Colors"));
    m_resetZoomBtn->setText(!resetZoom.isEmpty() ? resetZoom :
        (m_currentLanguage == "ar" ? "إعادة تعيين (100%)" : "Reset (100%)"));
    m_fitWindowBtn->setText(!fitToWindow.isEmpty() ? fitToWindow :
        (m_currentLanguage == "ar" ? "ملائمة النافذة" : "Fit to Window"));
    
    m_transparentCheckBox->setText(!transparentBackground.isEmpty() ? transparentBackground :
        (m_currentLanguage == "ar" ? "استخدام خلفية شفافة" : "Use Transparent Background"));
    m_exportColorsCheckBox->setText(!exportColorList.isEmpty() ? exportColorList :
        (m_currentLanguage == "ar" ? "تصدير قائمة الألوان" : "Export Color List"));
    m_colorToleranceCheckBox->setText(!useColorTolerance.isEmpty() ? useColorTolerance :
        (m_currentLanguage == "ar" ? "تحديد الألوان القريبة" : "Use Color Tolerance"));
    m_colorToleranceLabel->setText(!tolerance.isEmpty() ? tolerance :
        (m_currentLanguage == "ar" ? "الألوان القريبة:" : "Tolerance:"));
    
    // Set placeholder text for image labels
    m_originalImageLabel->setText(!originalImageText.isEmpty() ? originalImageText :
        (m_currentLanguage == "ar" ? "أسقط الصورة هنا أو انقر فتح صورة" : "Drop image here or click Open Image"));
    m_editedImageLabel->setText(!editedImageText.isEmpty() ? editedImageText :
        (m_currentLanguage == "ar" ? "ستظهر الصورة المُحررة هنا" : "Edited image will appear here"));
    
    m_fileMenu->setTitle(!fileMenu.isEmpty() ? fileMenu :
        (m_currentLanguage == "ar" ? "&ملف" : "&File"));
    m_languageMenu->setTitle(!languageMenu.isEmpty() ? languageMenu :
        (m_currentLanguage == "ar" ? "&اللغة" : "&Language"));
    m_helpMenu->setTitle(!helpMenu.isEmpty() ? helpMenu :
        (m_currentLanguage == "ar" ? "&مساعدة" : "&Help"));
    
    m_aboutAction->setText(!aboutAction.isEmpty() ? aboutAction :
        (m_currentLanguage == "ar" ? "&حول..." : "&About..."));
    
    // Update menu action texts
    QString openImageAction = getTranslation("actions.openImage");
    QString saveEditedImageAction = getTranslation("actions.saveEditedImage");
    QString exitAction = getTranslation("actions.exit");
    
    m_openAction->setText(!openImageAction.isEmpty() ? openImageAction :
        (m_currentLanguage == "ar" ? "&فتح صورة..." : "&Open Image..."));
    m_saveAction->setText(!saveEditedImageAction.isEmpty() ? saveEditedImageAction :
        (m_currentLanguage == "ar" ? "&حفظ الصورة المحررة..." : "&Save Edited Image..."));
    m_exitAction->setText(!exitAction.isEmpty() ? exitAction :
        (m_currentLanguage == "ar" ? "&خروج" : "E&xit"));
    
    // Status bar
    QString statusText = (!ready.isEmpty() ? ready :
        (m_currentLanguage == "ar" ? "جاهز" : "Ready")) + QString(" [%1]").arg(m_currentLanguage.toUpper());
    m_statusLabel->setText(statusText);
    
    // Set layout direction
    if (m_currentLanguage == "ar") {
        setLayoutDirection(Qt::RightToLeft);
    } else {
        setLayoutDirection(Qt::LeftToRight);
    }
}

void MainWindow::loadTranslations()
{
    // Try multiple paths for translation files
    QStringList translationPaths = {
        QString("translations/%1.json").arg(m_currentLanguage),      // Relative path
        QString(":/translations/%1.json").arg(m_currentLanguage),    // Resource path
        QString("./%1.json").arg(m_currentLanguage),                 // Current directory
        QString("../translations/%1.json").arg(m_currentLanguage)    // Parent directory
    };
    
    bool loaded = false;
    for (const QString &translationFile : translationPaths) {
        QFile file(translationFile);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (!doc.isNull() && doc.isObject()) {
                m_translations = doc.object();
                loaded = true;
                break;
            }
        }
    }
    
    if (!loaded) {
        // Clear translations object so fallbacks will be used
        m_translations = QJsonObject();
    }
}

bool MainWindow::colorsAreSimilar(const QColor &color1, const QColor &color2, int tolerance)
{
    int rDiff = qAbs(color1.red() - color2.red());
    int gDiff = qAbs(color1.green() - color2.green());
    int bDiff = qAbs(color1.blue() - color2.blue());
    
    return (rDiff <= tolerance && gDiff <= tolerance && bDiff <= tolerance);
}

QString MainWindow::getTranslation(const QString &path) const
{
    QStringList keys = path.split('.');
    QJsonValue value = m_translations;
    
    for (const QString &key : keys) {
        if (value.isObject()) {
            value = value.toObject()[key];
        } else {
            return QString(); // Path not found
        }
    }
    
    return value.toString();
}

QString MainWindow::getTranslatedMessage(const QString &key, const QString &fallbackAr, const QString &fallbackEn) const
{
    QString translated = getTranslation(key);
    if (!translated.isEmpty()) {
        return translated;
    }
    return (m_currentLanguage == "ar") ? fallbackAr : fallbackEn;
}

// Drag and drop support
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        if (!urls.isEmpty()) {
            QString fileName = urls.first().toLocalFile();
            if (!fileName.isEmpty()) {
                QImage image(fileName);
                if (!image.isNull()) {
                    m_originalImage = image;
                    m_editedImage = image.copy();
                    m_currentImagePath = fileName;
                    
                    QPixmap pixmap = QPixmap::fromImage(m_originalImage);
                    m_originalImageLabel->setOriginalPixmap(pixmap);
                    m_editedImageLabel->setOriginalPixmap(QPixmap::fromImage(m_editedImage));
                    
                    updateZoom();
                    updateEditedImage();
                    
                    m_saveAction->setEnabled(true);
                    m_saveBtn->setEnabled(true);
                    
                    QString loadedMessage = getTranslatedMessage("messages.imageLoaded",
                        "تم التحميل: %1 (%2x%3)", "Loaded: %1 (%2x%3)");
                    m_statusLabel->setText(loadedMessage
                                          .arg(QFileInfo(fileName).fileName())
                                          .arg(image.width())
                                          .arg(image.height()));
                }
            }
        }
    }
    event->acceptProposedAction();
}

// ImageLabel Implementation
ImageLabel::ImageLabel(QWidget *parent)
    : QLabel(parent)
    , m_scaleFactor(1.0)
    , m_panning(false)
{
    setMinimumSize(400, 300);
    setAlignment(Qt::AlignCenter);
    setStyleSheet("QLabel { border: 1px solid #ddd; }");
    setCursor(Qt::CrossCursor);
}

void ImageLabel::setOriginalPixmap(const QPixmap &pixmap)
{
    m_originalPixmap = pixmap;
    updatePixmap(m_scaleFactor);
}

void ImageLabel::updatePixmap(qreal scaleFactor)
{
    m_scaleFactor = scaleFactor;
    if (!m_originalPixmap.isNull()) {
        QPixmap scaledPixmap = m_originalPixmap.scaled(
            m_originalPixmap.size() * m_scaleFactor,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        setPixmap(scaledPixmap);
        resize(scaledPixmap.size());
    }
}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_originalPixmap.isNull()) {
        QPoint clickPos = event->pos();
        
        // Convert click position to original image coordinates
        QSize scaledSize = m_originalPixmap.size() * m_scaleFactor;
        QSize labelSize = size();
        
        // Calculate offset for centering
        int offsetX = (labelSize.width() - scaledSize.width()) / 2;
        int offsetY = (labelSize.height() - scaledSize.height()) / 2;
        
        // Adjust click position
        QPoint adjustedPos = clickPos - QPoint(offsetX, offsetY);
        
        // Scale back to original image coordinates
        QPoint originalPos(
            adjustedPos.x() / m_scaleFactor,
            adjustedPos.y() / m_scaleFactor
        );
        
        // Check if click is within image bounds
        if (originalPos.x() >= 0 && originalPos.y() >= 0 &&
            originalPos.x() < m_originalPixmap.width() &&
            originalPos.y() < m_originalPixmap.height()) {
            
            // Get pixel color from original image
            QImage image = m_originalPixmap.toImage();
            QColor color = image.pixelColor(originalPos);
            
            emit pixelClicked(originalPos, color);
        }
    } else if (event->button() == Qt::MiddleButton || 
               (event->button() == Qt::LeftButton && event->modifiers() & Qt::AltModifier)) {
        // Start panning
        m_panning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    
    QLabel::mousePressEvent(event);
}

void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning && !m_originalPixmap.isNull()) {
        // Handle panning
        QPoint delta = event->pos() - m_lastPanPoint;
        m_lastPanPoint = event->pos();
        
        // Get the parent scroll area
        QScrollArea *scrollArea = qobject_cast<QScrollArea*>(parent()->parent());
        if (scrollArea) {
            QScrollBar *hScrollBar = scrollArea->horizontalScrollBar();
            QScrollBar *vScrollBar = scrollArea->verticalScrollBar();
            
            hScrollBar->setValue(hScrollBar->value() - delta.x());
            vScrollBar->setValue(vScrollBar->value() - delta.y());
        }
    }
    
    QLabel::mouseMoveEvent(event);
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton || 
        (event->button() == Qt::LeftButton && m_panning)) {
        // Stop panning
        m_panning = false;
        setCursor(Qt::CrossCursor);
    }
    
    QLabel::mouseReleaseEvent(event);
}

void ImageLabel::wheelEvent(QWheelEvent *event)
{
    if (!m_originalPixmap.isNull()) {
        emit zoomRequested(event->angleDelta().y(), event->position().toPoint());
        event->accept();
    } else {
        QLabel::wheelEvent(event);
    }
}

void ImageLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}

// ColorListWidget Implementation
ColorListWidget::ColorListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setMaximumHeight(200);
    setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(this, &QListWidget::itemClicked, this, &ColorListWidget::onItemClicked);
    connect(this, &QListWidget::customContextMenuRequested, [this](const QPoint &pos) {
        QListWidgetItem *item = itemAt(pos);
        if (item) {
            QMenu menu(this);
            QAction *removeAction = menu.addAction("Remove Color");
            connect(removeAction, &QAction::triggered, this, &ColorListWidget::removeCurrentColor);
            menu.exec(mapToGlobal(pos));
        }
    });
}

void ColorListWidget::addColor(const QColor &color)
{
    // Check if color already exists (avoid duplicates)
    for (const QColor &existingColor : m_colors) {
        if (existingColor.rgb() == color.rgb()) {
            return; // Color already exists, don't add duplicate
        }
    }
    
    m_colors.append(color);
    
    QListWidgetItem *item = new QListWidgetItem(this);
    updateItem(item, color);
    addItem(item);
}

void ColorListWidget::removeColor(const QColor &color)
{
    int index = m_colors.indexOf(color);
    if (index >= 0) {
        m_colors.removeAt(index);
        delete takeItem(index);
        emit colorRemoved(color);
    }
}

void ColorListWidget::clearColors()
{
    m_colors.clear();
    clear();
}

QList<QColor> ColorListWidget::getColors() const
{
    return m_colors;
}

void ColorListWidget::onItemClicked(QListWidgetItem * /*item*/)
{
    // Optional: Show color details or allow editing
}

void ColorListWidget::removeCurrentColor()
{
    int row = currentRow();
    if (row >= 0 && row < m_colors.size()) {
        QColor color = m_colors[row];
        removeColor(color);
    }
}

void ColorListWidget::updateItem(QListWidgetItem *item, const QColor &color)
{
    // Create color swatch
    QPixmap colorSwatch(50, 20);
    colorSwatch.fill(color);
    
    QString text = QString("RGB(%1, %2, %3)\n%4")
                   .arg(color.red())
                   .arg(color.green())
                   .arg(color.blue())
                   .arg(color.name().toUpper());
    
    item->setText(text);
    item->setIcon(QIcon(colorSwatch));
    item->setData(Qt::UserRole, color);
}
