#include "UBNxtBottomBar.h"
#include "UBNxtPopupPanel.h"
#include "UBMainWindow.h"
#include "core/UBApplication.h"
#include <QPainter>
#include <QResizeEvent>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>

UBNxtBottomBar::UBNxtBottomBar(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setAttribute(Qt::WA_TranslucentBackground);
}

UBNxtBottomBar::~UBNxtBottomBar()
{
}

void UBNxtBottomBar::setupUi()
{
    mMainLayout = new QHBoxLayout(this);
    mMainLayout->setContentsMargins(20, 0, 20, 20);
    mMainLayout->setSpacing(20);

    // --- Part 1: Creation Tools ---
    QList<QAction*> toolActions;
    toolActions << UBApplication::mainWindow->actionPen;
    toolActions << UBApplication::mainWindow->actionEraser;
    toolActions << UBApplication::mainWindow->actionMarker;
    toolActions << UBApplication::mainWindow->actionSelector;
    toolActions << UBApplication::mainWindow->actionPlay; 
    toolActions << UBApplication::mainWindow->actionHand;
    toolActions << UBApplication::mainWindow->actionLine;
    toolActions << UBApplication::mainWindow->actionText;

    mPartTools = createPart(toolActions, "NxtPartTools");
    mMainLayout->addWidget(mPartTools);

    // --- Part 2: Style & Environment ---
    mPartStyle = new QWidget(this);
    mPartStyle->setObjectName("NxtPartStyle");
    QHBoxLayout* styleLayout = new QHBoxLayout(mPartStyle);
    styleLayout->setContentsMargins(10, 5, 10, 5);
    styleLayout->setSpacing(8);

    QPushButton* penStyleBtn = new QPushButton("Pen");
    penStyleBtn->setObjectName("StyleButton");
    penStyleBtn->setFixedSize(50, 48);
    penStyleBtn->setCursor(Qt::PointingHandCursor);
    connect(penStyleBtn, &QPushButton::clicked, this, &UBNxtBottomBar::onPenStyleClicked);
    styleLayout->addWidget(penStyleBtn);

    QPushButton* markerStyleBtn = new QPushButton("Marker");
    markerStyleBtn->setObjectName("StyleButton");
    markerStyleBtn->setFixedSize(60, 48);
    markerStyleBtn->setCursor(Qt::PointingHandCursor);
    connect(markerStyleBtn, &QPushButton::clicked, this, &UBNxtBottomBar::onMarkerStyleClicked);
    styleLayout->addWidget(markerStyleBtn);

    QPushButton* shapesBtn = new QPushButton("Shapes");
    shapesBtn->setObjectName("StyleButton");
    shapesBtn->setFixedSize(60, 48);
    shapesBtn->setCursor(Qt::PointingHandCursor);
    connect(shapesBtn, &QPushButton::clicked, this, &UBNxtBottomBar::onShapesClicked);
    styleLayout->addWidget(shapesBtn);

    // Add Backgrounds Action here
    styleLayout->addWidget(createToolButton(UBApplication::mainWindow->actionBackgrounds));

    QGraphicsDropShadowEffect* shadowStyle = new QGraphicsDropShadowEffect(mPartStyle);
    shadowStyle->setBlurRadius(15); shadowStyle->setOffset(0, 5); shadowStyle->setColor(QColor(0, 0, 0, 50));
    mPartStyle->setGraphicsEffect(shadowStyle);

    mMainLayout->addWidget(mPartStyle);

    // --- Part 3: Navigation, Modes & System ---
    QList<QAction*> sysActions;
    sysActions << UBApplication::mainWindow->actionUndo;
    sysActions << UBApplication::mainWindow->actionRedo;
    sysActions << UBApplication::mainWindow->actionBack;
    sysActions << UBApplication::mainWindow->actionForward;
    sysActions << UBApplication::mainWindow->actionPages;
    sysActions << UBApplication::mainWindow->actionBoard;
    sysActions << UBApplication::mainWindow->actionWeb;
    sysActions << UBApplication::mainWindow->actionDocument;
    sysActions << UBApplication::mainWindow->actionDesktop;
    sysActions << UBApplication::mainWindow->actionMenu;

    mPartActions = createPart(sysActions, "NxtPartActions");
    mMainLayout->addWidget(mPartActions);

    mMainLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
}

void UBNxtBottomBar::onPenStyleClicked()
{
    UBNxtPopupPanel* popup = new UBNxtPopupPanel(UBNxtPopupPanel::PenStyle, nullptr);
    popup->showAbove(qobject_cast<QWidget*>(sender()));
}

void UBNxtBottomBar::onMarkerStyleClicked()
{
    UBNxtPopupPanel* popup = new UBNxtPopupPanel(UBNxtPopupPanel::MarkerStyle, nullptr);
    popup->showAbove(qobject_cast<QWidget*>(sender()));
}

void UBNxtBottomBar::onShapesClicked()
{
    UBNxtPopupPanel* popup = new UBNxtPopupPanel(UBNxtPopupPanel::Shapes, nullptr);
    popup->showAbove(qobject_cast<QWidget*>(sender()));
}

QWidget* UBNxtBottomBar::createPart(const QList<QAction*>& actions, const QString& objectName)
{
    QWidget* part = new QWidget(this);
    part->setObjectName(objectName);
    QHBoxLayout* layout = new QHBoxLayout(part);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(5);

    for (QAction* action : actions) {
        layout->addWidget(createToolButton(action));
    }

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(part);
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 5);
    shadow->setColor(QColor(0, 0, 0, 50));
    part->setGraphicsEffect(shadow);

    return part;
}

QToolButton* UBNxtBottomBar::createToolButton(QAction* action)
{
    QToolButton* button = new QToolButton();
    button->setDefaultAction(action);
    button->setIconSize(QSize(32, 32));
    button->setAutoRaise(true);
    button->setFixedSize(48, 48);
    
    // Ensure menu actions show their menu
    if (action->menu()) {
        button->setMenu(action->menu());
        button->setPopupMode(QToolButton::InstantPopup);
    }
    
    return button;
}

void UBNxtBottomBar::updatePosition()
{
    if (parentWidget()) {
        int w = parentWidget()->width();
        int h = parentWidget()->height();
        
        int barWidth = mPartTools->sizeHint().width() + mPartStyle->sizeHint().width() + mPartActions->sizeHint().width() + (mMainLayout->spacing() * 4);
        int barHeight = 85;
        
        // Center the entire bar
        setGeometry((w - barWidth) / 2, h - barHeight, barWidth, barHeight);
    }
}

void UBNxtBottomBar::paintEvent(QPaintEvent *)
{
    // Translucent container
}

void UBNxtBottomBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Position is managed by the parent or updatePosition
}
