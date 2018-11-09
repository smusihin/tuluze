#include "clipscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>


ClipScene::ClipScene(QObject* parent) : QGraphicsScene(parent)
{
    connect(this, &ClipScene::clippedImage, this, &ClipScene::setPixmap);
}

void ClipScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());


        m_selection = new QGraphicsRectItem();
        m_selection->setBrush(QBrush(QColor(158,182,255,96)));
        m_selection->setPen(QPen(QColor(158,182,255,200),1));
        addItem(m_selection);
    }

    QGraphicsScene::mousePressEvent(event);
}

void ClipScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_leftMouseButtonPressed)
    {
        auto dx = event->scenePos().x() - m_previousPosition.x();
        auto dy = event->scenePos().y() - m_previousPosition.y();
        m_selection->setRect((dx > 0) ? m_previousPosition.x() : m_previousPosition.x() - dx,
                           (dy > 0) ? m_previousPosition.y() : m_previousPosition.y() - dy,
                           dx,
                           dy);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void ClipScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        m_leftMouseButtonPressed = false;

        QRect selectionRect = m_selection->boundingRect().toRect();
        clippedRect(selectionRect.left(),selectionRect.top(),
                    selectionRect.width(),selectionRect.height());
       // clippedImage(m_currentImageItem->pixmap().copy(selectionRect));
        delete m_selection;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void ClipScene::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(m_previousPosition);
}

QPointF ClipScene::previousPosition() const
{
    return m_previousPosition;
}

void ClipScene::setImage(const QString& filePath)
{
    m_filePixmap = QPixmap(filePath);
    setPixmap(m_filePixmap);
}

void ClipScene::setPixmap(const QPixmap &pixmap)
{
    if (m_currentImageItem)
    {
        this->removeItem(m_currentImageItem);
    }
    m_currentImageItem = new QGraphicsPixmapItem(pixmap);
    addItem(m_currentImageItem);
}
