#ifndef UBNXTBOTTOMBAR_H
#define UBNXTBOTTOMBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QList>
#include <QAction>

class UBNxtBottomBar : public QWidget
{
    Q_OBJECT

public:
    explicit UBNxtBottomBar(QWidget *parent = nullptr);
    ~UBNxtBottomBar();

    void updatePosition();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onPenStyleClicked();
    void onMarkerStyleClicked();
    void onShapesClicked();

private:
    void setupUi();
    QWidget* createPart(const QList<QAction*>& actions, const QString& objectName);
    QToolButton* createToolButton(QAction* action);

    QWidget* mPartTools;
    QWidget* mPartStyle;
    QWidget* mPartActions;

    QHBoxLayout* mMainLayout;
};

#endif // UBNXTBOTTOMBAR_H
