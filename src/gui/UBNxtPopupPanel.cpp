#include "UBNxtPopupPanel.h"
#include "core/UBSettings.h"
#include "core/UBApplication.h"
#include "board/UBDrawingController.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QLabel>
#include <QVariant>

UBNxtPopupPanel::UBNxtPopupPanel(PopupType type, QWidget *parent)
    : QWidget(parent), mType(type)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setupUi();
}

UBNxtPopupPanel::~UBNxtPopupPanel()
{
}

void UBNxtPopupPanel::setupUi()
{
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(10, 10, 10, 10);

    QWidget* container = new QWidget(this);
    container->setObjectName("PopupContainer");
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(15, 15, 15, 15);
    containerLayout->setSpacing(15);

    if (mType == PenStyle) {
        setupStyleUi(false);
    } else if (mType == MarkerStyle) {
        setupStyleUi(true);
    } else if (mType == Shapes) {
        setupShapesUi();
    }

    mMainLayout->addWidget(container);

    container->setStyleSheet(
        "QWidget#PopupContainer {"
        "  background-color: white;"
        "  border-radius: 12px;"
        "  border: 1px solid #ddd;"
        "}"
    );

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 5);
    shadow->setColor(QColor(0, 0, 0, 60));
    container->setGraphicsEffect(shadow);
}

void UBNxtPopupPanel::setupStyleUi(bool isMarker)
{
    QWidget* container = findChild<QWidget*>("PopupContainer");
    QVBoxLayout* containerLayout = qobject_cast<QVBoxLayout*>(container->layout());

    // --- Color Section ---
    QLabel* colorTitle = new QLabel(isMarker ? "Marker Colors" : "Pen Colors");
    colorTitle->setObjectName("PopupTitle");
    containerLayout->addWidget(colorTitle);
    
    QGridLayout* colorGrid = new QGridLayout();
    colorGrid->setSpacing(8);
    
    bool isDark = UBSettings::settings()->isDarkBackground();
    QList<QColor> colors = isMarker ? 
        UBSettings::settings()->markerColors(isDark) : 
        UBSettings::settings()->penColors(isDark);

    for (int i = 0; i < colors.size(); ++i) {
        QPushButton* btn = new QPushButton();
        btn->setObjectName("ColorSwatch");
        btn->setFixedSize(30, 30);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setProperty("colorIndex", i);
        btn->setProperty("isMarker", isMarker);
        
        QString colorStr = colors[i].name();
        btn->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(colorStr));
        
        connect(btn, &QPushButton::clicked, this, &UBNxtPopupPanel::colorSelected);
        colorGrid->addWidget(btn, i / 5, i % 5);
    }
    containerLayout->addLayout(colorGrid);

    // --- Width Section ---
    QLabel* widthTitle = new QLabel("Stroke Width");
    widthTitle->setObjectName("PopupTitle");
    containerLayout->addWidget(widthTitle);
    
    QHBoxLayout* widthLayout = new QHBoxLayout();
    QStringList labels = {"Small", "Medium", "Large"};
    for (int i = 0; i < 3; ++i) {
        QPushButton* btn = new QPushButton(labels[i]);
        btn->setObjectName("WidthButton");
        btn->setCheckable(true);
        btn->setProperty("widthIndex", i);
        btn->setProperty("isMarker", isMarker);
        btn->setCursor(Qt::PointingHandCursor);
        
        int currentIndex = isMarker ? UBSettings::settings()->markerWidthIndex() : UBSettings::settings()->penWidthIndex();
        if (i == currentIndex) btn->setChecked(true);

        connect(btn, &QPushButton::clicked, this, &UBNxtPopupPanel::widthSelected);
        widthLayout->addWidget(btn);
    }
    containerLayout->addLayout(widthLayout);
}

void UBNxtPopupPanel::setupShapesUi()
{
    QWidget* container = findChild<QWidget*>("PopupContainer");
    QVBoxLayout* containerLayout = qobject_cast<QVBoxLayout*>(container->layout());
    
    QLabel* title = new QLabel("Shapes");
    title->setObjectName("PopupTitle");
    containerLayout->addWidget(title);
    
    containerLayout->addWidget(new QLabel("Coming Soon..."));
}

void UBNxtPopupPanel::colorSelected()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        int index = btn->property("colorIndex").toInt();
        bool isMarker = btn->property("isMarker").toBool();
        if (isMarker) {
            UBSettings::settings()->setMarkerColorIndex(index);
        } else {
            UBSettings::settings()->setPenColorIndex(index);
        }
        close();
    }
}

void UBNxtPopupPanel::widthSelected()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        int index = btn->property("widthIndex").toInt();
        bool isMarker = btn->property("isMarker").toBool();
        if (isMarker) {
            UBSettings::settings()->setMarkerWidthIndex(index);
        } else {
            UBSettings::settings()->setPenWidthIndex(index);
        }
        close();
    }
}

void UBNxtPopupPanel::showAbove(QWidget* target)
{
    if (!target) return;
    
    adjustSize();
    QPoint globalPos = target->mapToGlobal(QPoint(0, 0));
    // Center above the target
    int x = globalPos.x() + (target->width() - width()) / 2;
    int y = globalPos.y() - height() - 10;
    
    move(x, y);
    show();
    setFocus();
}

void UBNxtPopupPanel::paintEvent(QPaintEvent *)
{
    // Painting handled by children and stylesheets
}

bool UBNxtPopupPanel::event(QEvent *event)
{
    if (event->type() == QEvent::WindowDeactivate) {
        close();
        return true;
    }
    return QWidget::event(event);
}
