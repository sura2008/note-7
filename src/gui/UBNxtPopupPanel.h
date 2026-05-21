#ifndef UBNXTPOPUPPANEL_H
#define UBNXTPOPUPPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QColor>

class UBNxtPopupPanel : public QWidget
{
    Q_OBJECT

public:
    enum PopupType {
        PenStyle,
        MarkerStyle,
        Shapes
    };

    explicit UBNxtPopupPanel(PopupType type, QWidget *parent = nullptr);
    ~UBNxtPopupPanel();

    void showAbove(QWidget* target);

protected:
    void paintEvent(QPaintEvent *event) override;
    void blurEvent(QFocusEvent *event);
    bool event(QEvent *event) override;

private slots:
    void colorSelected();
    void widthSelected();

private:
    void setupUi();
    void setupStyleUi(bool isMarker);
    void setupShapesUi();

    PopupType mType;
    QVBoxLayout* mMainLayout;
};

#endif // UBNXTPOPUPPANEL_H
